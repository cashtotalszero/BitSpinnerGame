/* ----------------------------------------------------------------------------------------------------------------
 *      BITSPINNER - Software Engineering project                                                               
 * ----------------------------------------------------------------------------------------------------------------
 *      Course: Computer Science Conversion MS50
 *      Group: 1   
 *      Hand-in deadline: 24 january 2014
 *      Group members: Alex Parrott, William Woodhead, Sandeep Sandhu, Ryan Johnson, Jose Rodriguez, Bryce Hsaio.
 * ----------------------------------------------------------------------------------------------------------------
 *      AUTHOR(s) OF MODULE: William Woodhead
 * ----------------------------------------------------------------------------------------------------------------
 *      DESCRIPTION OF MODULE - bitspinnerSDL_MAIN.c
 *    
 *      This module contains all the functions which are responsible for initialising the SDL_Window.
 * 		This module contains all the functions which are responsible for loading the BMP images.
 *  	This module also contains functions concerned with changing image addresses when the nudge button is used.
 *
 * 		
 * ----------------------------------------------------------------------------------------------------------------
 */

 #include "bitspinner.h"

//Function responsible for initialising the SDL window
void SDL_initialise(SDL_Simplewin *sw ,Game *current){

	//Initialise everything
  	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
  		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   	}
   	//initialise sw->finished
   	sw->finished = 0;
  	//initialise sw->win
	sw->win = SDL_CreateWindow("BITSPINNER", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WWIDTH, WHEIGHT, SDL_WINDOW_SHOWN);
	if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   	}
   	//load bmp names
   	load_bmps(current);
}

//This function creates a base surface for blitting all of the bmp's onto. Without this base surface, no bmps can be displayed on screen
SDL_Surface* init_base_surface(SDL_Simplewin *sw){

	//declare a surface and the rectangle it is going to be filled with
	SDL_Surface *base_surface;
	SDL_Rect base_rectdata = {0,0, WWIDTH, WHEIGHT};
	SDL_Rect *base_rect;
	base_rect = &base_rectdata;
	//declare the black background colour
	Uint32 black;
	//base surface is the window surface. we can now blit onto this surface
	base_surface = SDL_GetWindowSurface(sw->win);
    if(base_surface == NULL) {
        fprintf(stderr, "GetWindowSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
	// make the base surface black
	black = SDL_MapRGB(base_surface->format, 0, 0, 0);
	SDL_FillRect(base_surface, NULL, black);

	return base_surface;
}



//create a linked list to store all of the images involved in the game
void load_bmps(Game *current){

	FILE *bmps;
	char in_str[MAX_STRING];
	Bmp_node *bmp_current;

	current->start = initialise_bmp_list();
	bmp_current = current->start;

	bmps = gfopen("bmp_names", "r");

	while(fgets(in_str, MAX_STRING, bmps) != NULL){
		bmp_current = create_bmp_node(in_str, bmp_current);
	}
	fclose(bmps);
}

//sub function of load_bmps - creates a node in the linked list
Bmp_node *create_bmp_node(char *p, Bmp_node *current){

	int len;

	current->next = (Bmp_node*)malloc(sizeof(Bmp_node));
	if(current->next == NULL){
		printf("NULL POINTER RETURNED CREATING BMPS - SDL MODULE");
		exit(2);
	}
	len = strlen(p);
	*(p + len - 1) = '\0';
	current = current->next;
	strcpy(current->bmp_address, p);
	current->struct_surface = SDL_LoadBMP(p);
	current->next = NULL;
	return current;
}

// sub function of load bmps - makes sure no null pointers are passed as arguments
Bmp_node *initialise_bmp_list(){

	Bmp_node *start;

	start = (Bmp_node*)malloc(sizeof(Bmp_node));
	if(start == NULL){
		printf("NULL POINTER RETURNED CREATING BMPS - SDL MODULE");
		exit(2);
	}
	strcpy(start->bmp_address, "!!START!!");
	start->next = NULL;
	return start;
}



//This function takes an integer and returns the bmp address for the respective ASCII code. These are the bmps for the 8bit display
void get_8bit_hex_bmp(int i, char* p){

	switch(i){

		case 36: strcpy(p, "space_8bit_spinner.bmp");break;
		case 10: strcpy(p, "A_8bit_spinner.bmp"); break;
		case 11: strcpy(p, "B_8bit_spinner.bmp"); break;
		case 12: strcpy(p, "C_8bit_spinner.bmp"); break;
		case 13: strcpy(p, "D_8bit_spinner.bmp"); break;
		case 14: strcpy(p, "E_8bit_spinner.bmp"); break;
		case 15: strcpy(p, "F_8bit_spinner.bmp"); break;
		case 16: strcpy(p, "G_8bit_spinner.bmp"); break;
		case 17: strcpy(p, "H_8bit_spinner.bmp"); break;
		case 18: strcpy(p, "I_8bit_spinner.bmp"); break;
		case 19: strcpy(p, "J_8bit_spinner.bmp"); break;
		case 20: strcpy(p, "K_8bit_spinner.bmp"); break;
		case 21: strcpy(p, "L_8bit_spinner.bmp"); break;
		case 22: strcpy(p, "M_8bit_spinner.bmp"); break;
		case 23: strcpy(p, "N_8bit_spinner.bmp"); break;
		case 24: strcpy(p, "O_8bit_spinner.bmp"); break;
		case 25: strcpy(p, "P_8bit_spinner.bmp"); break;
		case 26: strcpy(p, "Q_8bit_spinner.bmp"); break;
		case 27: strcpy(p, "R_8bit_spinner.bmp"); break;
		case 28: strcpy(p, "S_8bit_spinner.bmp"); break;
		case 29: strcpy(p, "T_8bit_spinner.bmp"); break;
		case 30: strcpy(p, "U_8bit_spinner.bmp"); break;
		case 31: strcpy(p, "V_8bit_spinner.bmp"); break;
		case 32: strcpy(p, "W_8bit_spinner.bmp"); break;
		case 33: strcpy(p, "X_8bit_spinner.bmp"); break;
		case 34: strcpy(p, "Y_8bit_spinner.bmp"); break;
		case 35: strcpy(p, "Z_8bit_spinner.bmp"); break;
		case 0: strcpy(p, "0x_8bit_spinner.bmp"); break;
		case 1: strcpy(p, "1x_8bit_spinner.bmp"); break;
		case 2: strcpy(p, "2_8bit_spinner.bmp"); break;
		case 3: strcpy(p, "3_8bit_spinner.bmp"); break;
		case 4: strcpy(p, "4_8bit_spinner.bmp"); break;
		case 5: strcpy(p, "5_8bit_spinner.bmp"); break;
		case 6: strcpy(p, "6_8bit_spinner.bmp"); break;
		case 7: strcpy(p, "7_8bit_spinner.bmp"); break;
		case 8: strcpy(p, "8_8bit_spinner.bmp"); break;
		case 9: strcpy(p, "9_8bit_spinner.bmp"); break;
	}
}

//This function takes an integer and returns the bmp address for the respective ASCII code. These are the bmps for the 4bit display
void get_4bit_hex_bmp(int i, char* p){


	switch(i){
		case 36: strcpy(p, "space_4bit_spinner.bmp");break;
		case 10: strcpy(p, "A_4bit_spinner.bmp"); break;
		case 11: strcpy(p, "B_4bit_spinner.bmp"); break;
		case 12: strcpy(p, "C_4bit_spinner.bmp"); break;
		case 13: strcpy(p, "D_4bit_spinner.bmp"); break;
		case 14: strcpy(p, "E_4bit_spinner.bmp"); break;
		case 15: strcpy(p, "F_4bit_spinner.bmp"); break;
		case 16: strcpy(p, "G_4bit_spinner.bmp"); break;
		case 17: strcpy(p, "H_4bit_spinner.bmp"); break;
		case 18: strcpy(p, "I_4bit_spinner.bmp"); break;
		case 19: strcpy(p, "J_4bit_spinner.bmp"); break;
		case 20: strcpy(p, "K_4bit_spinner.bmp"); break;
		case 21: strcpy(p, "L_4bit_spinner.bmp"); break;
		case 22: strcpy(p, "M_4bit_spinner.bmp"); break;
		case 23: strcpy(p, "N_4bit_spinner.bmp"); break;
		case 24: strcpy(p, "O_4bit_spinner.bmp"); break;
		case 25: strcpy(p, "P_4bit_spinner.bmp"); break;
		case 26: strcpy(p, "Q_4bit_spinner.bmp"); break;
		case 27: strcpy(p, "R_4bit_spinner.bmp"); break;
		case 28: strcpy(p, "S_4bit_spinner.bmp"); break;
		case 29: strcpy(p, "T_4bit_spinner.bmp"); break;
		case 30: strcpy(p, "U_4bit_spinner.bmp"); break;
		case 31: strcpy(p, "V_4bit_spinner.bmp"); break;
		case 32: strcpy(p, "W_4bit_spinner.bmp"); break;
		case 33: strcpy(p, "X_4bit_spinner.bmp"); break;
		case 34: strcpy(p, "Y_4bit_spinner.bmp"); break;
		case 35: strcpy(p, "Z_4bit_spinner.bmp"); break;
		case 0: strcpy(p, "0x_4bit_spinner.bmp"); break;
		case 1: strcpy(p, "1x_4bit_spinner.bmp"); break;
		case 2: strcpy(p, "2_4bit_spinner.bmp"); break;
		case 3: strcpy(p, "3_4bit_spinner.bmp"); break;
		case 4: strcpy(p, "4_4bit_spinner.bmp"); break;
		case 5: strcpy(p, "5_4bit_spinner.bmp"); break;
		case 6: strcpy(p, "6_4bit_spinner.bmp"); break;
		case 7: strcpy(p, "7_4bit_spinner.bmp"); break;
		case 8: strcpy(p, "8_4bit_spinner.bmp"); break;
		case 9: strcpy(p, "9_4bit_spinner.bmp"); break;
	}
}

// this function takes an integer and returns the correct bmp address for the numbers image
void get_digit_str(int n, char *p){

	switch(n){
		case 0: strcpy(p, "0_num.bmp"); break;
		case 1: strcpy(p, "1_num.bmp"); break;
		case 2: strcpy(p, "2_num.bmp"); break;
		case 3: strcpy(p, "3_num.bmp"); break;
		case 4: strcpy(p, "4_num.bmp"); break;
		case 5: strcpy(p, "5_num.bmp"); break;
		case 6: strcpy(p, "6_num.bmp"); break;
		case 7: strcpy(p, "7_num.bmp"); break;
		case 8: strcpy(p, "8_num.bmp"); break;
		case 9: strcpy(p, "9_num.bmp"); break;
	}
}

// this function takes a character and returns the correct bmp address for the characters image
void get_letter_str(char c, char *p){

	if(isalpha(c)){
		toupper(c);
	}

	//only acceptable non alphanumeric characters
	if(c != ' ' && c != '_'){
		if(!isalnum(c)){
			printf("Must be an alpha character\n");
			printf("letter_str function: character is %c\n\n", c);
			exit(2);
		}
	}

	switch(c){
		case ' ': strcpy(p, "space_let.bmp");break;
		case '_': strcpy(p, "space_let.bmp");break;
		case 'A': strcpy(p, "A_let.bmp"); break;
		case 'B': strcpy(p, "B_let.bmp"); break;
		case 'C': strcpy(p, "C_let.bmp"); break;
		case 'D': strcpy(p, "D_let.bmp"); break;
		case 'E': strcpy(p, "E_let.bmp"); break;
		case 'F': strcpy(p, "F_let.bmp"); break;
		case 'G': strcpy(p, "G_let.bmp"); break;
		case 'H': strcpy(p, "H_let.bmp"); break;
		case 'I': strcpy(p, "I_let.bmp"); break;
		case 'J': strcpy(p, "J_let.bmp"); break;
		case 'K': strcpy(p, "K_let.bmp"); break;
		case 'L': strcpy(p, "L_let.bmp"); break;
		case 'M': strcpy(p, "M_let.bmp"); break;
		case 'N': strcpy(p, "N_let.bmp"); break;
		case 'O': strcpy(p, "O_let.bmp"); break;
		case 'P': strcpy(p, "P_let.bmp"); break;
		case 'Q': strcpy(p, "Q_let.bmp"); break;
		case 'R': strcpy(p, "R_let.bmp"); break;
		case 'S': strcpy(p, "S_let.bmp"); break;
		case 'T': strcpy(p, "T_let.bmp"); break;
		case 'U': strcpy(p, "U_let.bmp"); break;
		case 'V': strcpy(p, "V_let.bmp"); break;
		case 'W': strcpy(p, "W_let.bmp"); break;
		case 'X': strcpy(p, "X_let.bmp"); break;
		case 'Y': strcpy(p, "Y_let.bmp"); break;
		case 'Z': strcpy(p, "Z_let.bmp"); break;
		case '0': strcpy(p, "0_num.bmp"); break;
		case '1': strcpy(p, "1_num.bmp"); break;
		case '2': strcpy(p, "2_num.bmp"); break;
		case '3': strcpy(p, "3_num.bmp"); break;
		case '4': strcpy(p, "4_num.bmp"); break;
		case '5': strcpy(p, "5_num.bmp"); break;
		case '6': strcpy(p, "6_num.bmp"); break;
		case '7': strcpy(p, "7_num.bmp"); break;
		case '8': strcpy(p, "8_num.bmp"); break;
		case '9': strcpy(p, "9_num.bmp"); break;
	}
}

//This function takes a number and returns the next number in a 16 part cycle
int get_next_hex_num(int i){

	if(i != 15){
		return i + 1;
	}
	else{
		return 0;
	}
	return i + 1;
}

//This function takes a number and returns the next number in a alpha numberic cycle
int get_next_input_char(int i){

	if(i == 36){
		return 10;
	}
	else if(i == 9){
		return 36;
	}
	else if(i == 35){
		return 0;
	}
	else{	
		return (i + 1);
	}
	return i + 1;
}
