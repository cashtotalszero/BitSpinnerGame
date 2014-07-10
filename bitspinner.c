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
 *      DESCRIPTION OF MODULE - bitspinner.c
 *    
 *      Main function 
 * 		
 * 		- declares and initiates all structs. We have attempted to code in an OO manner by using struct
 * 		to represent different object involved in the program. eg. a question. 
 *
 * 		- The game is in a constant animation loop, constantly updating new images to the screen
 *      
 * ----------------------------------------------------------------------------------------------------------------
 */

#include "bitspinner.h"

int main(void){	

	// declare a window to place surfaces into
	SDL_Simplewin sw;
	SDL_Surface *base_surface = NULL;

	// Seed the random number generator with the current time
  	srand((unsigned)time(NULL));
	Game *current = allocate_Game(); // Declares Game struct

    	load_game(current);

	//this function is of our design. 
	SDL_initialise(&sw, current);

	
	do{
		//choose which screen mode to display
		switch(current->screen){
			case HOME: home_mode(&sw, current, base_surface);
					break;
			case GAME: game_mode(&sw, current, base_surface);
					break;
			case HIGH: high_mode(&sw, current, base_surface);
					break;
			case TUT: tutorial_mode(&sw, current, base_surface);
					break;
			case HIGHSCORE: input_score_mode(&sw, current, base_surface);
					break;
		}

	}while(sw.finished == 0);

	SDL_DestroyWindow(sw.win);
	SDL_Quit();


    return 0;
}
