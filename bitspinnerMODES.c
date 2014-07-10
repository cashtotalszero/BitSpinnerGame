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
 *      DESCRIPTION OF MODULE - bitspinnerMODES.c
 *    
 *      This module contains all the functions which represent the different modes of the game 
 *    	These modes are in a ongoing animation loop until the mode is changed.
 * 		
 * ----------------------------------------------------------------------------------------------------------------
 */

#include "bitspinner.h"

// these functions reperesent the different screen modes ie. home screen, tutorial screen etc.
void home_mode(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){


	reset_game(current);

	do{
		//upload the BMP's relevant for the home screen
		home_mode_bmps(sw, current, base_surface);

		SDL_UpdateWindowSurface(sw->win);
		//check for events relevant to the home screen
		check_for_event(sw, current, base_surface);

		SDL_Delay(DELAY);

	}while(current->screen == HOME && sw->finished == 0);
}

void game_mode(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	int level_return;

	do{
		level_return = start_level(current);

		if(level_return == 0){
			do{
				//this function is of our design. 
				game_mode_bmps(sw, current, base_surface);

				check_for_clock(sw, current, base_surface);

				// This is a standard SDL function
				SDL_UpdateWindowSurface(sw->win);

				//this function is of our design. 
				check_for_event(sw, current, base_surface);

				// This is a standard SDL function
				SDL_Delay(DELAY);
				
			}while(current->correct == 0 && current->screen == GAME);
		}
		else if(level_return == 2){
			current->screen = HIGHSCORE;
		}
		else{
			current->screen = HOME;
		}
	}while(current->screen == GAME && sw->finished == 0);
}

void high_mode(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	do{
		//upload the BMP's relevant for the home screen
		high_mode_bmps(sw, current, base_surface);

		SDL_UpdateWindowSurface(sw->win);
		//check for events relevant to the home screen
		check_for_event(sw, current, base_surface);

		SDL_Delay(DELAY);

	}while(current->screen == HIGH && sw->finished == 0);
}

void tutorial_mode(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	do{
		//upload the BMP's relevant for the home screen
		tutorial_mode_bmps(sw, current, base_surface);

		SDL_UpdateWindowSurface(sw->win);
		//check for events relevant to the home screen
		check_for_event(sw, current, base_surface);

		SDL_Delay(DELAY);

	}while(current->screen == TUT && sw->finished == 0);
}

void input_score_mode(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	int i;

	//initiate base surface for blitting onto. Without this surface, no bmps can be blitted to screen
	if((base_surface = init_base_surface(sw)) == NULL){
		fprintf(stderr, "\nUnable to initialize base surface:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
	}

	if(current->score <= current->hi_scores[9].score){
		blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "justmissed.bmp");
		SDL_UpdateWindowSurface(sw->win);
		SDL_Delay(4500);
		current->screen = HOME;
	}

	else{
		blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "highscore.bmp");
		SDL_UpdateWindowSurface(sw->win);
		SDL_Delay(1000);

		// reset numbers
		for(i = 0; i < 8; i++){
			current->nudge[i] = 0;
			current->nudge_pressed[i] = 0;
			current->display[i] = 0;
		}
		//reset other settings.
		current->check = 0;
		current->num_bits = 8;

		do{
			input_score_bmps(sw, current, base_surface);

			SDL_UpdateWindowSurface(sw->win);
			//check for events relevant to the home screen
			check_for_event(sw, current, base_surface);

			SDL_Delay(DELAY);

		}while(current->screen == HIGHSCORE && sw->finished == 0);
	}
}
