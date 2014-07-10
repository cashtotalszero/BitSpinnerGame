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
 *      DESCRIPTION OF MODULE - bitspinnerSDL_EVENTS.c
 *    
 *      This module contains all the functions that deal with SDL_Events.
 * 		The only event we are interested in, is(left)mouseclick.
 * 		When a mouse is clicked, depending on what mode the game is in, changes are made to the status of the game
 * 		and therefore the images displayed. These functions deal with making such changes.
 * 		
 * 		This module also contains a clock time check to see if the game player has run out of time.
 *	
 * ----------------------------------------------------------------------------------------------------------------
 */

#include "bitspinner.h"


//This function is called by main. It checks for an event and then reacts accordingly. ie. if there is a mouse click it updates the game status
void check_for_event(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	SDL_Event event;
	int x, y;

	/* Check for events */
	while((SDL_PollEvent(&event)) == 1){  /* Loop until there are no events left on the queue */
	    switch(event.type){ /* Process the appropriate event type */
	        case SDL_MOUSEBUTTONDOWN:
	        	y = event.button.y; //y coordinate
		    	x = event.button.x; // x coordinate
	        	if(x > 0 && x < WWIDTH && y > 0 && y < WHEIGHT && event.button.button == SDL_BUTTON_LEFT){
	        		//depending on the screen state, the mouse click changes affect the screen in different ways...
	        		if(current->screen == HOME){
		    			home_screen_changes(x, y, current, sw, base_surface);
		    		}
	        		if(current->screen == GAME){
		    			make_game_changes(x, y, current, sw, base_surface);
		    		}
		    		if(current->screen == HIGH){
		    			high_screen_changes(x, y, current, sw, base_surface);
		    		}
		    		if(current->screen == TUT){
		    			tutorial_screen_changes(x, y, current, sw, base_surface);
		    		}
		    		if(current->screen == HIGHSCORE){
		    			input_highscore_changes(x, y, current, sw, base_surface);
		    		}
			    }
	    		break;   
	    }
	} 	
}



//This function checks whether the final answer is correct or not
void SDL_check_response(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){
	
	//check if the answer if correct
	if(update_score(current) == 0){
		blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "wrong.bmp");
		SDL_UpdateWindowSurface(sw->win);
		SDL_Delay(35*DELAY);
		current->check = 0;
	}
	else{
		blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "correct.bmp");
		SDL_UpdateWindowSurface(sw->win);
		SDL_Delay(35*DELAY);
		current->spin = 0;
		current->check = 0;
		current->hint = 0;

		//move onto hex bonus round on level 16
		if(current->level == BONUS_START){
			blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "hexbonusround.bmp");
			SDL_UpdateWindowSurface(sw->win);
			SDL_Delay(100*DELAY);
			return;
		}

		if(current->level == BYTE_UP){
			blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "bytepowerup.bmp");
			SDL_UpdateWindowSurface(sw->win);
			SDL_Delay(100*DELAY);
			return;
		}

		if(current->level == BONUS_END){
			blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "end_of_bonus.bmp");
			SDL_UpdateWindowSurface(sw->win);
			SDL_Delay(100*DELAY);
			return;
		}
	}	
}

// This function check that the user hasnt run out of time
void check_for_clock(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	if(current->spin != 0){
		current->counter++;
		if(current->counter%COUNTER_NUM == 0){
			current->timer--;
		}
	}
}




//This function is a subfunction of check_for_event. It make the appropriate changes to the game structure based on the position of any mouse click
void make_game_changes(int x, int y, Game *current, SDL_Simplewin *sw, SDL_Surface *base_surface){

	int i;

	// quit button first- this allows user to quit at any point 
	if(x > 0 && y > 5*WHEIGHT/6 && x < WWIDTH/6 && y < WHEIGHT){
		current->quit = 1;
	}

	if(current->spin == 0){
	//spin button
		if(x > 5*WWIDTH/6 && y > 2*WHEIGHT/6 && x < WWIDTH && y < 4*WHEIGHT/6){
			current->spin = 1;
			current->counter = 0;
			randomise_display(current);
		}
	}
	//make sure spin has been pressed before any other button can be pressed.
	if(current->spin == 0){
		return;
	}

	//spin button after initial spin
	if( x > 5*WWIDTH/6 && y > 2*WHEIGHT/6 && x < WWIDTH && y < 4*WHEIGHT/6){
		current->spin = 2;
		for(i = 0; i < current->num_bits; i ++){
			if(current->hold[i] != 1){
				if(current->question[current->q_lookup].bit_type == BINARY){
					current->display[i] == 0? (current->display[i] = 1): (current->display[i] = 0);
				} 
				else{
					randomise_display(current);
				}
			}
		}
	}
	
	// check button
	if(x > 5*WWIDTH/6 && y > 4*WHEIGHT/6 && x < WWIDTH && y < WHEIGHT){
		current->check = 1;
		//check answer with current display
	}
	//hint 
	if(x > 0 && y > 4*WHEIGHT/6 && x < WWIDTH/6 && y < 5*WHEIGHT/6){
		current->hint = 2;
	}

	//hold
	if(current->num_bits == 4){
		for(i = 1; i <= 4; i++){
			if( x > i*WWIDTH/6 && y > 3*WHEIGHT/6 && x < (i+1)*WWIDTH/6 && y < 4*WHEIGHT/6){
				current->hold[i-1] == 0? (current->hold[i-1] = 1): (current->hold[i-1] = 0);
			}	
		}
	}
	else{
		for(i = 2; i <= 9; i++){
			if( x > i*WWIDTH/12 && y > 3*WHEIGHT/6 && x < (i+1)*WWIDTH/12 && y < 4*WHEIGHT/6){
				current->hold[i-2] == 0? (current->hold[i-2] = 1): (current->hold[i-2] = 0);
			}	
		}
	}

	//nudge
	if(current->num_bits == 4){
		for(i = 1; i <= 4; i++){
			if( x > i*WWIDTH/6 && y > 4*WHEIGHT/6 && x < (i+1)*WWIDTH/6 && y < 5*WHEIGHT/6){
				if(current->hold[i - 1] == 0){
					if(current->question[current->q_lookup].bit_type == BINARY){
						current->display[i-1] == 0?(current->display[i-1] = 1):  (current->display[i-1] = 0);
					}
					else{
						current->display[i-1] = get_next_hex_num(current->display[i-1]);
					}
				}
				current->nudge_pressed[i] = 1;
			}	
		}
	}
	else{
		for(i = 2; i <= 9; i++){
			if( x > i*WWIDTH/12 && y > 4*WHEIGHT/6 && x < (i+1)*WWIDTH/12 && y < 5*WHEIGHT/6){
				if(current->hold[i - 2] == 0){
					if(current->question[current->q_lookup].bit_type == BINARY){
						current->display[i-2] == 0?(current->display[i-2] = 1):  (current->display[i-2] = 0);
					}
					else{
						current->display[i-2] = get_next_hex_num(current->display[i-2]);
					}
				}
				current->nudge_pressed[i-2] = 1;
			}	
		}
	}
}

//This function is a subfunction of check_for_event. This function is responsible for dealing with the mouse events on the home screen
void home_screen_changes(int x, int y, Game *current, SDL_Simplewin *sw, SDL_Surface *base_surface){

	// quit button first- this allows user to quit the game 
	if(x > 0 && y > 5*WHEIGHT/6 && x < WWIDTH/6 && y < WHEIGHT){
		exit(2);
	}

	// play the game button
	if(x > WWIDTH/3 && y > WHEIGHT/4 && x < (2*WWIDTH)/3 && y < (9*WHEIGHT)/20){
		current->home_play = 1;
	}

	// high scores button
	if(x > WWIDTH/3 && y > (2*WHEIGHT)/4 && x < (2*WWIDTH)/3 && y < (14*WHEIGHT)/20){
		current->home_high = 1;
	}

	// tutorial button
	if(x > WWIDTH/3 && y > (3*WHEIGHT)/4 && x < (2*WWIDTH)/3 && y < (19*WHEIGHT)/20){
		current->home_tut = 1;
	}
}

//This function is a subfunction of check_for_event. This function is responsible for correctly dealing with the mouse clicks when the game is on the high scores screen
void high_screen_changes(int x, int y, Game *current, SDL_Simplewin *sw, SDL_Surface *base_surface){

	// quit button first- this allows user to quit the game 
	if(x > 0 && y > 5*WHEIGHT/6 && x < WWIDTH/6 && y < WHEIGHT){
		current->quit = 1;
	}
}

//This function is a subfunction of check_for_event. This function is responsible for dealing with the mouse clicks on the tutorial screen
void tutorial_screen_changes(int x, int y, Game *current, SDL_Simplewin *sw, SDL_Surface *base_surface){

	// quit button first- this allows user to quit the game 
	if(x > 0 && y > 5*WHEIGHT/6 && x < WWIDTH/6 && y < WHEIGHT){
		current->quit = 1;
	}
	else{
		// move onto the next tutorial page
		current->home_tut++;
	}
}

//This function is a subfunction of check_for_event. This function is responsible for dealing with the mouse clicks on the input high score screen
void input_highscore_changes(int x, int y, Game *current, SDL_Simplewin *sw, SDL_Surface *base_surface){

	int i;
	// quit button first- this allows user to quit the screen 
	if(x > 0 && y > 5*WHEIGHT/6 && x < WWIDTH/6 && y < WHEIGHT){
		current->screen = HOME;
	}

	// check button
	if(x > 5*WWIDTH/6 && y > 4*WHEIGHT/6 && x < WWIDTH && y < WHEIGHT){
		current->check = 1;
		//check answer with current display
	}

	for(i = 2; i <= 9; i++){
		if( x > i*WWIDTH/12 && y > 4*WHEIGHT/6 && x < (i+1)*WWIDTH/12 && y < 5*WHEIGHT/6){
			if(current->hold[i - 2] == 0){

				current->display[i-2] = get_next_input_char(current->display[i-2]);
			}
			current->nudge_pressed[i-2] = 1;
		}	
	}
}
