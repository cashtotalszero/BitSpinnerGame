/* ----------------------------------------------------------------------------------------------------------------
 *      BITSPINNER - Software Engineering project                                                               
 * ----------------------------------------------------------------------------------------------------------------
 *      Course: Computer Science Conversion MS50
 *      Group: 1   
 *      Hand-in deadline: 24 january 2014
 *      Group members: Alex Parrott, William Woodhead, Sandeep Sandhu, Ryan Johnson, Jose Rodriguez, Bryce Hsaio.
 * ----------------------------------------------------------------------------------------------------------------
 *      AUTHOR(s) OF MODULE: Alex Parrott
 * ----------------------------------------------------------------------------------------------------------------
 *      DESCRIPTION OF MODULE - bitspinnerGI.c
 *    
 *      This module contains all the functions that deal with initialising, loading, setting and resetting the game
 *      data.
 *
 *      This includes checking if an answer is correct and subsequently updating the score, the level and the
 *      questions, as well as modifying the high scores file if a score needs to be entered.
 *    
 *  
 * ----------------------------------------------------------------------------------------------------------------
 */
#include "bitspinner.h"

/* 
Function allocates memory space for the Game struct:
@return *Game	Returns a pointer to the current Game struct.	
*/
Game *allocate_Game(void)
{
  Game *p;
  
  p = (Game *)malloc(sizeof(Game));
  
  if (p==NULL) {
    load_error(0,0);
  }
  reset_game(p);
  return p;
}

/*
Function initialises all variables in the Game to zero - function to be
called before the start of any new game:
@param Game *current	Pointer to the current Game struct
*/
void reset_game(Game *current)
{
  int i;
 
  // Resets all array chars to zero
  for(i=0;i<MAX_BITS;i++) {
    current->display[i] = 0;
    current->hold[i] = 0;
    current->nudge[i] = 0;
    current->nudge_pressed[i] = 0;
  }

  // Reset all questions to not done
  for(i=0;i<MAX_QUESTIONS;i++) {
    current->question[i].done=0;
  }

  // Bit display starts at 4 (a nibble :))
  current->num_bits = 4;

  // All other variables also set to zero
  current->score = 0;
  current->level = 0;
  current->q_lookup = 0;
  current->check = 0;
  current->spin = 0;
  current->hint = 0;
  current->quit = 0;
  current->home_play = 0;
  current->home_tut = 0;
  current->home_high = 0;
  return;
}

/*
Function loads both the question data (the question, bit number, answer, bit type
and difficulty rating) and high scores data (position, name and score).
@param	Game *current	Pointer to the current Game struct
*/
void load_game(Game *current)
{
  FILE *file_ptr = fopen("./qs.txt","r");	// File pointer to read in data
  char data[MAX_STRING] = { '\0' };		// String to copy data into
  int line=1;					// The line number in file of read data
  char* token;					// String to tokenise data
  int done=FALSE;				// Boolean flag when load is done
  int load=QUESTIONS;				// Flag for load type (QUESTIONS or SCORES)
  int position=0;				// Int position in table for hi scores

  // Iniitialise game to display the home screen.
  current->screen = HOME;
  
  while(!done) {
    // Read in data from file, one line at a time
    if(file_ptr!=NULL) {
      while(fgets(data,MAX_STRING,file_ptr)) {
        if(data[0]!=';' && !isspace(data[0])) {
          token = strtok(data, " ");
        
          // Step 1 (Questions): Check question image and add address to current Game
          if(load==QUESTIONS) {
            verify_bmp(token,line);
            strcpy(current->question[current->q_lookup].question,token);
          }
          // Step 1 (Scores): Get the position in top 10 for high score name
          else if(load==SCORES) {
            position=atoi(token);
          }
          token = strtok(NULL, " ");
          // Step 2 (Questions):  Check number of bits required to display question's answer...
          if(load==QUESTIONS) {
            current->question[current->q_lookup].bits = verify_bits(current,token,line);
            // ...and verify the correct answer to the question.
            token = strtok(NULL, " ");
            verify_answer(token,current,line);
          }
          // Step 2 (Scores): Copy name of high scorer into their place in the top 10
          else if(load==SCORES) {
            strcpy(current->hi_scores[position].name,token);
          }
          token = strtok(NULL, ";");
          // Step 3 (Questions): Get the question difficulty rating.
          if(load==QUESTIONS) {  
            current->question[current->q_lookup].difficulty = verify_difficulty(token,line);
            // Increment line counter and lookup index to load next question
            line++;
            current->q_lookup<MAX_QUESTIONS ? current->q_lookup=current->q_lookup+1:0;
          }
          // Step 3 (Scores): Get the player's high score
          else if(load==SCORES) {
            current->hi_scores[position].score=atoi(token);
          }
        }       
      }
      fclose(file_ptr);
      // Once questions are loaded, switch file pointer to hi scores file
      if(load==QUESTIONS) {
        file_ptr=fopen("./scores.txt","r");
        load=SCORES;
      }
      else if(load==SCORES) {
        done=TRUE;
      }
    }      
    else {
      load_error(1,0);			// Display error if file does not open 
    }
  }
  return;
}

/*
Function verifies the bmp address of the image which displays the question in 
graphical form. This function generates an error if the address cannot be found 
or the file will not open:
@param	char *address	String holding the address of the bmp file to lookup
@param	int line	The line number of the file where address was taken from
*/	
void verify_bmp(char *address,int line) 
{
  FILE *bmp_ptr = fopen(address,"r");
  
  // Display error if bmp file does not open 
  if(bmp_ptr==NULL) {
    load_error(2,line);
  }
  fclose(bmp_ptr);
  return;
}

/*
Function verifies the number of bits needed to correctly display the answer to
the corresponding question. All questions require either 4 or 8 bits. This 
function also sets what type of answer is needed for the question (binary or hex).
@param	char *num	String holding the number of bits (and type of display)
@param	int line	The line number of the file where num was taken from
@return int bits	The parsed bit number converted to type int
*/
int verify_bits(Game *current,char *num,int line)
{
  int bits=0;			// The number of bits needed to display answer
  int error=FALSE;		// Boolean flag for any load errors

  // Step 1: Display type needs to be set to either binary or hex
  if(num[1]=='b' && num[2]=='\0') {
    current->question[current->q_lookup].bit_type=BINARY; 
  }
  else if(num[1]=='x' && num[2]=='\0') { 
    current->question[current->q_lookup].bit_type=HEX; 
  }
  else { 
    error=TRUE; 
  }
  num[1]='\0';

  // Step 2: Bit number should be a single digit...
  if(strlen(num)>1) {
    error=TRUE;
  }
  else { bits = atoi(num); }

  // ...which is either 4 or 8
  if((bits!=4 && bits!=8)) {
    error=TRUE;
  }
   
  // End the program if any errors
  if(error) {
    load_error(3,line);
  }
  return bits;
}

/*
Function verifies the char string holding the correct question answer. Each digit
is then converted to type into and loaded into the QA struct. The string size 
is variable and should match the number of bits for the question (i.e. either 4 
or 8 bits/cells). All chars in the string should be either a binary or hex digit:
@param	char *data	String holding the correct answer for the question
@param Game *current	Pointer to the current Game struct
@param int line		The line number of the file where num was taken from
*/
void verify_answer(char *data,Game *current,int line)
{
  int i;
  
  // The size of the display should be the same as the bit number
  if((int)strlen(data)>current->question[current->q_lookup].bits) {
    load_error(4,line);
  }

  // Display error is any char in the display is not binary or hex digit
  for(i=0;i<current->question[current->q_lookup].bits;i++) {
    if(!isxdigit(data[i])) {
      load_error(5,line);
    }
  // Else, convert char to corresponding int
    else {
      switch(data[i]) {
        case '0': current->question[current->q_lookup].answer[i]=0; break;
        case '1': current->question[current->q_lookup].answer[i]=1; break; 
        case '2': current->question[current->q_lookup].answer[i]=2; break;
        case '3': current->question[current->q_lookup].answer[i]=3; break; 
        case '4': current->question[current->q_lookup].answer[i]=4; break;
        case '5': current->question[current->q_lookup].answer[i]=5; break; 
        case '6': current->question[current->q_lookup].answer[i]=6; break;
        case '7': current->question[current->q_lookup].answer[i]=7; break; 
        case '8': current->question[current->q_lookup].answer[i]=8; break;
        case '9': current->question[current->q_lookup].answer[i]=9; break; 
        case 'a': current->question[current->q_lookup].answer[i]=10; break;
        case 'b': current->question[current->q_lookup].answer[i]=11; break; 
        case 'c': current->question[current->q_lookup].answer[i]=12; break;
        case 'd': current->question[current->q_lookup].answer[i]=13; break;
        case 'e': current->question[current->q_lookup].answer[i]=14; break;
        case 'f': current->question[current->q_lookup].answer[i]=15; break; 
      }
    }
  } 
}

/*
Function verifies the difficulty level for the corresponding question. Levels 
range from 0-9. The difficulty rating char is taken from the string, verified and 
then converted into an int:
@param	char *num	String holding the difficulty level
@param	int line	The line number of the file where num was taken from
@return int diff	The verified difficulty level converted to type int
*/
int verify_difficulty(char *num,int line)
{
  char difficulty = '\0';	// Char to hold difficulty rating taken from string
  int diff = UNSET;		// Int to hold difficulty rating converted to int
  int i = 0;			// A counter
  int found = FALSE;		// Boolean flag states when char has been found in string
  
  // Ensure difficulty is only a single char...
  while(num[i] != '\0') {
    if(!isspace(num[i]) && !found) {
      difficulty = num[i];
      found=TRUE;
    }
    else if(!isspace(num[i]) && found) {
      load_error(6,line);
    }
    i++;
  }
  // ...which is a valid digit. Then convert it to type int.
  if(found && isdigit(difficulty)) {
    diff = atoi(&difficulty);
  }
  else {
    load_error(6,line);
  }
  return diff;
}

/*
Function generates a random display or either binary and hex digits (depending
on the answer required for the question) which the player can then manipulate
during gameplay:
@param Game *current	Pointer to the current Game struct
*/
void randomise_display(Game *current)
{
  int random, i, level;

  // Set to display either hex or binary display
  if(current->question[current->q_lookup].bit_type == BINARY){	//NOTE: 5 subject to change in next version!!!
    level = BINARY;
  }
  else{
    level = HEX;
  }
  // Generate random binary or hex number and add to display array in current game
  for(i = 0; i < MAX_BITS; i++) {
    random = rand()%level;
    while (random<0) {
      random = rand()%level;
    }
    if(current->hold[i] == 0){
      current->display[i]=random;
    }
  }
  return;
}

/*
Function starts each level of the game by getting a random question (ensuring
that it hasn't already been asked in the current game) and setting it as the 
current question:
@param	Game *current	Pointer to the current Game struct
@return int		Returns true if all questions in the game have already been asked
*/
int start_level(Game *current)
{
  int i=0;
  int random;  

  // Increment level number, return true if all questions already answered 
  current->level = current->level + 1;
  if(current->level >= MAX_QUESTIONS + 1){
    return 1;
  }
  if(current->level == MAX_LEVEL){
    return 2;
  }

  // Get a random question number (which hasn't already been asked)
  random = get_random_question(current);

  // Assign this question as the current question lookup and set done as true
  current->q_lookup = random;
  current->question[random].done = TRUE;
  current->num_bits=current->question[random].bits;
  current->correct = FALSE;
  current->timer = MAXTIME;
  current->counter = 0;
  
  //resets all hold buttons
  for(i = 0; i < current->num_bits; i ++){
    current->hold[i] = 0;
  }
  return 0;
}

/*
Function generates a random question number which is at the appropriate difficulty
for the current game level.
@param	Game *current	Pointer to the current Game struct
@return	int random	A random question number 
*/
int get_random_question(Game *current)
{  

  int random = UNSET;		// Random question number 
  int found = FALSE;		// Flag for when appropriate question is found		
  int loopjump = FALSE;		// Boolean flag to end loop

  // Loop random number generator until valid number is found
  while(!found){ 
    random = rand()%MAX_QUESTIONS;
    if(random >= 0){
      /* 
      Only accept questions of the appropriate difficulty grade for the level.
      Questions become increasingly difficult as the player progresses.
      */
      if((current->level < 4  && current->question[random].difficulty == 1) ||
        (current->level >= 4  && current->level < 8  && current->question[random].difficulty == 2) ||
        (current->level >= 8  && current->level < 12 && current->question[random].difficulty == 3) ||
        (current->level >= 12 && current->level < 16 && current->question[random].difficulty == 4) ||
        (current->level >= 16 && current->level < 18 && current->question[random].difficulty == 5) ||
        (current->level >= 18 && current->level < 21 && current->question[random].difficulty == 6) ||
        (current->level >= 21 && current->level < 24 && current->question[random].difficulty == 7) ||      
        (current->level >= 24 && current->level < 28 && current->question[random].difficulty == 8) ||
        (current->level >= 28 && current->level < MAX_LEVEL && current->question[random].difficulty == 9)){
        if(!current->question[random].done){
          found=TRUE;
        }
      }
    }
    // Exit the loop if it has cycled 100 times (ie. cannot find a question) - prevents crashes 
    loopjump++;
    if(loopjump == 6000){
      printf("LOOP JUMPED IN FUNCTION get_random_question\n\n");
      found=TRUE;
      reset_game(current);
      current->screen = HOME;
    }
  }
  return random;
}

/*
Function compares the current display with the correct answer to the current 
question. If correct, the player's socre is updated. The faster the player 
answers, the higher the score (incrementations of 16,8,4 or 2).
@param 	Game *current	Pointer to the current Game struct
@return	int		Returns boolean flag, correct answer is true, else false	
*/
int update_score(Game *current)
{
  int i;

  current->correct = TRUE;
  // Set correct to false if any of the digits do not match
  for(i = 0; i < current->num_bits; i++) {
    if(current->question[current->q_lookup].answer[i] != current->display[i]) {
      current->correct = FALSE;
    }
  }
  // If correct, update the score. Dyanamic scoring applies (higher score for faster answer).
  if(current->correct){
    if(current->timer>50) {
      current->score = current->score + 16;
    }
    else if(current->timer>30 && current->timer<=50) {
      current->score = current->score + 8;
    }
    else if(current->timer>10 && current->timer<=30) {
      current->score = current->score + 4;
    }
    else if(current->timer>=0 && current->timer<=10) {
      current->score = current->score + 2;
    }
  }
  return current->correct;
}

/*
Function saves the player's name into the game's high scores table by saving it
to the scores.txt file and then reloading the game.
@param Game *current	Pointer to the current Game struct
*/
void save_user_name(Game *current)
{
  char temp[MAX_BITS+1] = { '\0' };	// Temp string used to hold name data for copying
  char to_print[MAX_STRING] = { '\0' };	// String to print to scores.txt file 
  Scores new_scores[TOPTEN];		// Array to hold the new amended top ten
  int insert=UNSET;  			// Int to hold the new score's position in the top ten
  int i;				// A counter

  // Convert all chars to int
  for(i=0; i<MAX_BITS; i++){
    temp[i] = get_char_from_int(current->display[i]);
  }
  // Check if player's score has made the top 10
  for(i=TOPTEN-1;i>-1;i--) {
    if(current->score>current->hi_scores[i].score) {
      insert=i;
    }
  }
  // For new top 10 scores:
  if(insert!=UNSET) {
    FILE *file_ptr = fopen("./scores.txt","w"); 
   
    // Insert the score in the appropriate spot
    for(i=0;i<TOPTEN;i++) {
      if(i==insert) { 
        strcpy(new_scores[i].name,temp);
        new_scores[i].score = current->score;
      }
      else if(i<insert) { 
        strcpy(new_scores[i].name,current->hi_scores[i].name);
        new_scores[i].score = current->hi_scores[i].score;
      }
      else if(i>insert) {
        strcpy(new_scores[i+1].name,current->hi_scores[i].name);
        new_scores[i+1].score = current->hi_scores[i].score;
      }
    }
    strcpy(new_scores[insert+1].name,current->hi_scores[insert].name);
    new_scores[insert+1].score = current->hi_scores[insert].score;
  
    // Print the new top 10 scores into file
    for(i=0;i<TOPTEN;i++) {
      sprintf(to_print,"%d %s %04d;\n",i,new_scores[i].name,(int)new_scores[i].score);
      fputs(to_print,file_ptr);    
    }
    fclose(file_ptr);
    load_game(current);		// Reload the game so updated scores display
  }
  return;
}
/*
Function converts all the ints stored in the game display and converts them into
their corresponding ints (used to convert input player names for high scores).
This is done by using the ASCII codes to lookup the correct chars.
@param int i	The int to convert
@return char	The int converted into a char
*/
// used to get the right character from the integer stored as the display
char get_char_from_int(int i)
{
  // Letters are 10-35 but the ASCII codes are from 65-90...
  if(10 <= i && i <= 35){
    // ...therefore add 55.
    return i + 55;
  }
  // Numbers are 0 to 9 but the ASCII codes are 48-58...
  else if(0 <= i && i <= 9){
    // ...therefore add 48.
    return i + 48;
  }
  // If character is a space...
  else if(i == 36){
    // ...return the ASCII code character for an underscore
    return 95;
  }
  // Default - return underscore (ie a space)
  return 95;
}

/*
Function tests image to display in game exists before SDL blitting.
@param char *file_name		The name of the file to test
@return FILE *			A pointer to the file 	 
*/
FILE *gfopen(char *file_name, const char *mode){

  FILE *ifp;

  // Create error if file fails to open
  if((ifp = fopen(file_name, mode)) == NULL){
    fprintf(stderr, "Failed to open %s\n", file_name);
    load_error(7,0);    
  }
  // Else return pointer to file
  return ifp;
}

/*
Function prints out error description bases on given code and then ends the 
program.
@param	int code	The error code
@param	int line	The line number in the file where the error occurred
*/
void load_error(int code,int line)
{
  printf("GAME LOAD ERROR = ");

  // Print specified error description
  switch(code) {
    case 0:	printf("Cannot allocate memory\n");
		break;
    case 1:	printf("Unable to open game data file.\n");
		break;
    case 2:	printf("Invalid bmp address in question data file (line %d)\n",line);
		break;
    case 3:	printf("Invalid bit display specifications (line %d)\n",line);
		break;
    case 4:	printf("Invalid number of bits in answer file (line %d)\n",line);
		break;
    case 5:	printf("Invalid display (line %d)\n",line);
		break;
    case 6:	printf("Invalid difficulty rating (line %d)\n",line);
		break;
    case 7:	printf("Unable to load game image\n");
		break;
    default:	printf("If this has printed, someone has dropped the ball.\n");
  }
  // End the program
  exit(2);
}
