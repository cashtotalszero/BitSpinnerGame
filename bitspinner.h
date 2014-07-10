/* ----------------------------------------------------------------------------------------------------------------
 *      BITSPINNER - Software Engineering project                                                               
 * ----------------------------------------------------------------------------------------------------------------
 *      Course: Computer Science Conversion MS50
 *      Group: 1   
 *      Hand-in deadline: 24 january 2014
 *      Group members: Alex Parrott, William Woodhead, Sandeep Sandhu, Ryan Johnson, Jose Rodriguez, Bryce Hsaio.
 * ----------------------------------------------------------------------------------------------------------------
 *      AUTHOR(s) OF MODULE: Alex Parrott, William Woodhead
 * ----------------------------------------------------------------------------------------------------------------
 *      DESCRIPTION OF MODULE - bitspinner.h
 *    
 *      Header file - #includes, #defines, and all function prototypes separated into their modules
 *      
 * ----------------------------------------------------------------------------------------------------------------
 */

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define WWIDTH 1200
#define WHEIGHT 750


#define MAX_QUESTIONS 146
#define MAX_LEVEL 33
#define MAX_BITS 8
#define MAX_STRING 200
#define MAX_NAME 9
#define BINARY 2
#define HEX 16
#define MAXTIME 60
#define ANIM_LOOP 6

#define TOPTEN 10
#define QUESTIONS 1
#define SCORES 2
#define HOME 0
#define GAME 1
#define HIGH 2
#define TUT 3
#define HIGHSCORE 4

#define BONUS_START 15
#define BONUS_END 20
#define BYTE_UP 20
#define COUNTER_NUM 20

#define TRUE 1
#define FALSE 0
#define UNSET -1

#define SDL_8BITCOLOUR 256
#define DELAY 20

// All info required for windows / renderer & event loop
struct SDL_Simplewin{
   SDL_bool finished;
   SDL_Window *win;
};
typedef struct SDL_Simplewin SDL_Simplewin;

// Custom QA struct to hold all question and answer data
struct qa {
  char question[MAX_STRING];  	// String for every question bmp that we create.
  int bits;			// ...and no. of bit required for the display
  int bit_type;			
  int answer[MAX_BITS];	 	// There is a corresponding answer... 
  int difficulty;		// ...difficulty rating...
  int done;			// Flag for when the question has been asked
};
typedef struct qa QA;

// Custom Scores struct to hold top 10 hi scores information
struct scores {
  char name[MAX_NAME];		// String holding name of hi score player
  int score;			// Their score...
};
typedef struct scores Scores;

//linked list set up to store bmp file names
struct bmp_node{
  char bmp_address[MAX_STRING];
  SDL_Surface *struct_surface;
  struct bmp_node *next;
};
typedef struct bmp_node Bmp_node;

// Custom Game struct to hold all current game data
struct game {
  int screen;     		// The screen mode eg. home, game, high scores etc.
  int score;	 		// The player's current score
  int level;	 		// ...and current level
  int check;	 		// Status of check button (1 = on, 0 = off)...
  int spin;	 		// ...likewise for spin button...
  int hint;			// ...the show hint button..
  int quit;	 		// ...and of quit button
  int correct;			// Boolean flag if player's answer is correct
  int timer;			// The question countdown clock
  int counter;			// Counter used to decrement clock
  int home_play;    		// This is used to enable an animation of the play button
  int home_tut;     		// This is used to run through the tutorial screens
  int home_high;    		// This is used to animate the high score button
  int num_bits;			// Number of bits to display answer (4 or 8)			
  int hold[MAX_BITS];	 	// Array holding status of all hold buttons...
  int nudge[MAX_BITS];	 	// ...likewise for nudge buttons
  int nudge_pressed[MAX_BITS]; 	// This allows for the animation of the nudge buttons
  int display[MAX_BITS];	// Array holding the current display on BitSpinner
  int q_lookup;	 		// Number of current question
  QA question[MAX_QUESTIONS];	// Array of all the questions & answers
  Scores hi_scores[TOPTEN];	// Array of the current hi scores
  Bmp_node *start;      	// This is the starting node of all the bmp images
};
typedef struct game Game;

//SDL_MODES module function prototypes...
void game_mode(SDL_Simplewin *, Game *, SDL_Surface *);
void home_mode(SDL_Simplewin *, Game *, SDL_Surface *);
void high_mode(SDL_Simplewin *, Game *, SDL_Surface *);
void tutorial_mode(SDL_Simplewin *, Game *, SDL_Surface *);
void input_score_mode(SDL_Simplewin *, Game *, SDL_Surface *);

//SDL_BLITS module function prototypes...
void home_mode_bmps(SDL_Simplewin *, Game *, SDL_Surface *);
void game_mode_bmps(SDL_Simplewin*, Game*, SDL_Surface *);
void high_mode_bmps(SDL_Simplewin *, Game *, SDL_Surface *);
void blit_the_high_scores(SDL_Simplewin *, Game *, SDL_Surface *);
void tutorial_mode_bmps(SDL_Simplewin *, Game *, SDL_Surface *);
void input_score_bmps(SDL_Simplewin *, Game *, SDL_Surface *);
void blit_image(Game *, SDL_Simplewin *, SDL_Surface *, int, int, int, int, char s[MAX_STRING]);
void blit_statics(SDL_Simplewin *, Game *, SDL_Surface *);
void blit_quit_check_hint(SDL_Simplewin *, Game *, SDL_Surface *);
void blit_the_display(SDL_Simplewin *, Game *, SDL_Surface *);
void blit_quest_hintline(SDL_Simplewin *, Game *, SDL_Surface *);
void blit_the_spin(SDL_Simplewin *, Game *, SDL_Surface *);

//SDL_MAIN module function prototypes...
void SDL_initialise(SDL_Simplewin*, Game *);
SDL_Surface* init_base_surface(SDL_Simplewin *);
void load_bmps(Game *);
Bmp_node *create_bmp_node(char *, Bmp_node *);
Bmp_node *initialise_bmp_list();
void get_digit_str(int, char*);
void get_letter_str(char , char *);
void get_4bit_hex_bmp(int, char* );
void get_8bit_hex_bmp(int, char* );
int get_next_input_char(int );
int get_next_hex_num(int);

//SDL_EVENTS module function prototypes...
void check_for_event(SDL_Simplewin *, Game *, SDL_Surface*);
void make_game_changes(int , int , Game *, SDL_Simplewin *, SDL_Surface*);
void SDL_check_response(SDL_Simplewin *, Game *, SDL_Surface *);
void check_for_clock(SDL_Simplewin *, Game *, SDL_Surface *);
void home_screen_changes(int, int, Game *, SDL_Simplewin *, SDL_Surface *);
void high_screen_changes(int, int, Game *, SDL_Simplewin *, SDL_Surface *);
void tutorial_screen_changes(int, int, Game *, SDL_Simplewin *, SDL_Surface *);
void input_highscore_changes(int, int, Game *, SDL_Simplewin *, SDL_Surface *);

//GI module function prototypes...
Game *allocate_Game(void);
void reset_game(Game *current);
void load_game(Game *current);
void verify_bmp(char *address,int line);
int verify_bits(Game *current,char *num,int line);
void verify_answer(char *data,Game *current,int line);
int verify_difficulty(char *num,int line);
void load_error(int code,int line);
void randomise_display(Game *current);
int start_level(Game *current);
int get_random_question(Game *current);
int update_score(Game *current);
FILE *gfopen(char *, const char *);
void save_user_name(Game *);
char get_char_from_int(int );
