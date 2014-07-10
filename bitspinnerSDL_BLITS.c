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
 *      DESCRIPTION OF MODULE - bitspinnerSDL_BLITS.c
 *    
 *      This module contains all the functions which are responsible for getting the correct SDL_Surface's
 *  	and then to blit all of the correct SDL_surfaces into the correct place on the SDL_Window at the right time.
 * 		This is achieved by using the game data to inform the choices of which SDL_Surface's to blit.
 *
 * 		The function 'blit_image' then blits the image once the correct SDL_Surface and coordinates are found.
 * 		
 * ----------------------------------------------------------------------------------------------------------------
 */


#include "bitspinner.h"

// This function is responsible for blitting the correct images when the game is on the home screen
void home_mode_bmps(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char playstr[MAX_STRING];
	char highstr[MAX_STRING];
	char tutstr[MAX_STRING];

	//initiate base surface for blitting onto. Without this surface, no bmps can be blitted to screen
	if((base_surface = init_base_surface(sw)) == NULL){
		fprintf(stderr, "\nUnable to initialize base surface:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
	}

	// blit all of the images relevant to the home screen
	blit_image(current, sw, base_surface, 0,5*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, "quit_orange.bmp");
	blit_image(current, sw, base_surface, WWIDTH/4, 0, WWIDTH/2, WHEIGHT/5, "full_logo.bmp");
	
	//animate the play button
	current->home_play == 0 ? strcpy(playstr,"play_game_green.bmp"): strcpy(playstr,"play_game_grey.bmp");
	if(current->home_play > 0){
		current->home_play = current->home_play + 1;
	}
	if(current->home_play == ANIM_LOOP){
		current->screen = GAME;
	}
	blit_image(current, sw, base_surface, WWIDTH/3, WHEIGHT/4, WWIDTH/3, WHEIGHT/5, playstr);

	//animate the high score button
	current->home_high == 0 ? strcpy(highstr,"high_scores_green.bmp"): strcpy(highstr,"high_scores_grey.bmp");
	if(current->home_high > 0){
		current->home_high = current->home_high + 1;
	}
	if(current->home_high == ANIM_LOOP){
		current->screen = HIGH;
	}
	blit_image(current, sw, base_surface, WWIDTH/3, 2*WHEIGHT/4, WWIDTH/3, WHEIGHT/5, highstr);

	//animate the tutorial button
	current->home_tut == 0 ? strcpy(tutstr,"tutorial_green.bmp"): strcpy(tutstr,"tutorial_grey.bmp");
	if(current->home_tut > 0){
		current->home_tut = current->home_tut + 1;
	}
	if(current->home_tut == ANIM_LOOP){
		current->screen = TUT;
		current->home_tut = 0;
	}
	blit_image(current, sw, base_surface, WWIDTH/3, 3*WHEIGHT/4, WWIDTH/3, WHEIGHT/5, tutstr);	
}



// This function is responsible for translating the game data into the relevant images for the screen.
void game_mode_bmps(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	//initiate base surface for blitting onto. Without this surface, no bmps can be blitted to screen
	if((base_surface = init_base_surface(sw)) == NULL){
		fprintf(stderr, "\nUnable to initialize base surface:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
	}
	//if the check button has been pressed, check the answer and respond accordingly
	if(current->check == 1){
		SDL_check_response(sw, current, base_surface);
	}

	//if time has run out
	if(current->timer == 0){
		blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "out_of_time.bmp");
		SDL_UpdateWindowSurface(sw->win);
		SDL_Delay(2000);

		// dont end the game if user is in the bonus levels, simply go to level 25.
		if(BONUS_START < current->level && current->level < BONUS_END){
			current->level = BONUS_END;
			current->spin = 0;
			current->check = 0;
			current->hint = 0;
			current->correct = 1;
			blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT, "end_of_bonus.bmp");
			SDL_UpdateWindowSurface(sw->win);
			SDL_Delay(2000);
		}
		else{
			// end the game by refering to the high score
			current->screen = HIGHSCORE;
			return;
		}
	}

	//blit all of the various components onto the screen
	blit_statics(sw, current, base_surface);

	blit_quit_check_hint(sw, current, base_surface);

	blit_the_spin(sw, current, base_surface);

	blit_quest_hintline(sw, current, base_surface);

	blit_the_display(sw, current, base_surface);
}

// This function is responsible for blitting the question image and the hint image
void blit_quest_hintline(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char hintstr[MAX_STRING];
	char queststr[MAX_STRING];

	//question if spin has been pressed
	if(current->spin != 0){
		// string copy the correct question image
		strcpy(queststr, current->question[current->q_lookup].question);
		blit_image(current, sw, base_surface, WWIDTH/6, 0, 4*WWIDTH/6, WHEIGHT/6, queststr);
	}
	
	
	if(current->hint == 1){
		// Binary or Hex
		if(current->question[current->q_lookup].bit_type == BINARY){
			//4 bit or 8 bit hint
			current->num_bits == 4? strcpy(hintstr, "hint_4bit.bmp"): strcpy(hintstr, "hint_8bit.bmp");
		}
		else{
			current->num_bits == 4? strcpy(hintstr, "hint_4bit_hex.bmp"): strcpy(hintstr, "hint_8bit_hex.bmp");
		}
		blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6,4*WWIDTH/6, WHEIGHT/12, hintstr);
	}
}

// This function is responsible for blitting the quit, check and hint buttons
void blit_quit_check_hint(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char checkstr[MAX_STRING];
	char hintstr[MAX_STRING];
	char quitstr[MAX_STRING];

	// blit the quit animation
	current->quit == 0 ? strcpy(quitstr,"quit_orange.bmp"): strcpy(quitstr,"grey_quit.bmp");
	if(current->quit > 0){
		current->quit = current->quit + 1;
	}
	if(current->quit == ANIM_LOOP){
		current->quit = 0;
		current->screen = 0;
	}
	blit_image(current, sw, base_surface, 0, 5*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, quitstr);

	// blit the hint animation
	current->hint > 1 ? strcpy(hintstr,"grey_hint.bmp"): strcpy(hintstr,"hint_orange.bmp");
	if(current->hint > 1){
		current->hint = current->hint + 1;
	}
	if(current->hint == ANIM_LOOP){
		current->hint = 1;
	}
	blit_image(current, sw, base_surface, 0,4*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, hintstr);

	//blit the check button
	strcpy(checkstr,"green_check.bmp");
	blit_image(current, sw, base_surface, 5*WWIDTH/6,4*WHEIGHT/6,WWIDTH/6, WHEIGHT/3, checkstr);
}

// this functon hold all the images that are not affected by mouse clicks
void blit_statics(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char levelstr[MAX_STRING];
	char timerstr[MAX_STRING];
	char pointstr[MAX_STRING];
	
	//blit all of the static images onto the base_surface - ie. the images which aren'g going to change by mouse clicks
	if(current->spin == 0){
		blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT/5, "full_logo.bmp");
	}
	else{
		blit_image(current, sw, base_surface, 0, 0, WWIDTH/6, WHEIGHT/3, "Logo.bmp");
		blit_image(current, sw, base_surface, 5*WWIDTH/6, 0, WWIDTH/6, WHEIGHT/3, "Logo.bmp");
	}

	//blit the level
	blit_image(current, sw, base_surface, WWIDTH/6,11*WHEIGHT/12, 4*WWIDTH/6, WHEIGHT/12, "level_number.bmp");
	//blit the 10's column of the level
	get_digit_str(current->level/10, levelstr);
	blit_image(current, sw, base_surface, 55*WWIDTH/97, 45*WHEIGHT/48, WWIDTH/35, WHEIGHT/24, levelstr);
	//blit the 1's column of the level
	get_digit_str(current->level%10, levelstr);
	blit_image(current, sw, base_surface, 58*WWIDTH/97, 45*WHEIGHT/48, WWIDTH/35, WHEIGHT/24, levelstr);

	// blit the clock
	blit_image(current, sw, base_surface, 0,2*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, "timer_blue.bmp");
	//blit the 10's column of the timer
	get_digit_str(current->timer/10, timerstr);
	blit_image(current, sw, base_surface, 2*WWIDTH/36, 19*WHEIGHT/48, WWIDTH/36, WHEIGHT/12, timerstr);
	//blit the 1's column of the timer
	get_digit_str(current->timer%10, timerstr);
	blit_image(current, sw, base_surface, 3*WWIDTH/36, 19*WHEIGHT/48, WWIDTH/36, WHEIGHT/12, timerstr);
	

	//blit the score
	blit_image(current, sw, base_surface, 0, 3*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, "points_blue.bmp");
	//blit the 1000's column of the points
	get_digit_str(((current->score/1000)%10), pointstr);
	blit_image(current, sw, base_surface, 2*WWIDTH/48, 28*WHEIGHT/48, WWIDTH/48, WHEIGHT/24, pointstr);
	//blit the 100's column of the points
	get_digit_str(((current->score/100)%10), pointstr);
	blit_image(current, sw, base_surface, 3*WWIDTH/48, 28*WHEIGHT/48, WWIDTH/48, WHEIGHT/24, pointstr);
	//blit the 10's column of the points
	get_digit_str(((current->score/10)%10), pointstr);
	blit_image(current, sw, base_surface, 4*WWIDTH/48, 28*WHEIGHT/48, WWIDTH/48, WHEIGHT/24, pointstr);
	//blit the 1's column of the points
	get_digit_str(current->score%10, pointstr);
	blit_image(current, sw, base_surface, 5*WWIDTH/48, 28*WHEIGHT/48, WWIDTH/48, WHEIGHT/24, pointstr);	
}

//animate the spin button
void blit_the_spin(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char spinstr[MAX_STRING];
	//if spin hasnt been pressed once, blit green button flashing grey
	if(current->spin == 0){
		if((current->counter/10)%2 == 0){
			strcpy(spinstr,"grey_spin.bmp");
		}
		else{
			strcpy(spinstr,"green_spin.bmp");
		}
		current->counter ++;
	}
	else{//once spin has been pressed once, defualt g->spin = 1. if it is pressed it circles up to 5 as a grey button before coming back to being a green button
		current->spin > 1 ? strcpy(spinstr,"grey_spin.bmp"): strcpy(spinstr,"green_spin.bmp");
		//if g->spin is > 1 circle up until 5
		if(current->spin > 1){
			current->spin = current->spin + 1;
		}
		if(current->spin == ANIM_LOOP){
			current->spin = 1;
		}
	}
	blit_image(current, sw, base_surface, 5*WWIDTH/6,2*WHEIGHT/6,WWIDTH/6, WHEIGHT/3, spinstr);
}

//This function is a subfunction of game_mode_bmps. it blit the spin wheels, the hold buttons and the nudge buttons
void blit_the_display(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char spinner[MAX_STRING];
	char tmpspin[MAX_STRING];
	char nudgestr[MAX_STRING];
	char holdstr[MAX_STRING];
	int i;

	tmpspin[0] = '\0';
	//depending on 4bit or 8 bit display, the wheel numbers, the hold and the nudges are blitted.
	if(current->num_bits == 4){
		for(i = 1; i <= 4; i++){
			if(current->question[current->q_lookup].bit_type == BINARY){
				current->display[i-1] == 0?strcpy(spinner, "spinner_0_4bit.bmp"): strcpy(spinner, "spinner_1_4bit.bmp");
			}
			else{
				get_4bit_hex_bmp(current->display[i-1], tmpspin);
				strcpy(spinner, tmpspin);
			}
			blit_image(current, sw, base_surface, i*WWIDTH/6,WHEIGHT/6,WWIDTH/6, WHEIGHT/3, spinner);


			current->hold[i-1] == 0?strcpy(holdstr, "green_hold_4bit.bmp"):strcpy(holdstr, "red_hold_4bit.bmp");
			blit_image(current, sw, base_surface, i*WWIDTH/6,3*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, holdstr);

			//animate the nudge button
			current->nudge_pressed[i] == 0? strcpy(nudgestr, "green_nudge_4bit.bmp"): strcpy(nudgestr, "grey_nudge_4bit.bmp");
			if(current->nudge_pressed[i] > 0){
				current->nudge_pressed[i] = current->nudge_pressed[i] + 1;
			}
			if(current->nudge_pressed[i] == ANIM_LOOP){
				current->nudge_pressed[i] = 0;
			}
			blit_image(current, sw, base_surface, i*WWIDTH/6,4*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, nudgestr);
		}
	}

	else{
		for(i = 2; i <= 9; i++){
			if(current->question[current->q_lookup].bit_type == BINARY){
				current->display[i - 2] == 0? strcpy(spinner, "spinner_0_8bit.bmp"): strcpy(spinner, "spinner_1_8bit.bmp");
			}
			else{
				get_8bit_hex_bmp(current->display[i - 2], spinner);
			}
			blit_image(current, sw, base_surface, i*WWIDTH/12,WHEIGHT/6, WWIDTH/12, WHEIGHT/3, spinner);


			current->hold[i - 2] == 0? strcpy(holdstr, "hold_green_8bit.bmp"): strcpy(holdstr, "hold_red_8bit.bmp");
			blit_image(current, sw, base_surface, i*WWIDTH/12,3*WHEIGHT/6,WWIDTH/12, WHEIGHT/6, holdstr);

			//animate the nudge button
			current->nudge_pressed[i - 2] == 0? strcpy(nudgestr, "nudge_green_8bit.bmp"): strcpy(nudgestr, "nudge_grey_8bit.bmp");
			if(current->nudge_pressed[i - 2] > 0){
				current->nudge_pressed[i - 2] = current->nudge_pressed[i - 2] + 1;
			}
			if(current->nudge_pressed[i - 2] == ANIM_LOOP){
				current->nudge_pressed[i - 2] = 0;
			}
			blit_image(current, sw, base_surface, i*WWIDTH/12,4*WHEIGHT/6,WWIDTH/12, WHEIGHT/6, nudgestr);
		}
	}
}

//This function is the code which actually blits the relevant bmp's in the correct place on the screen
void blit_image(Game *current, SDL_Simplewin *sw, SDL_Surface *base_surface, int x, int y, int w, int h, char str[MAX_STRING]){

	SDL_Surface *surface;
	//delcare the placements of the logos
	SDL_Rect rectdata = {x,y,w, h};
	SDL_Rect *rect;
	Bmp_node *bmp_current;

	bmp_current = current->start;

	//search through the linked list of bmp addresses until find the string match
	while(strcmp(bmp_current->bmp_address, str) != 0){
		if(bmp_current->next != NULL){
			bmp_current = bmp_current->next;
		}
		else{
			printf("NOT A VALID BMP ADDRESS- SDL MODULE: BLIT IMAGE\n");
			printf("%s\n", str);
			exit(2);
		}
	}
	// string match found. so take the surface from the same node nd blit
	surface = bmp_current->struct_surface;
	rect = &rectdata;
	SDL_BlitScaled(surface, NULL, base_surface, rect);	
}




// This function is responsible for blitting the correct images when the game is on the high scores screen
void high_mode_bmps(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char quitstr[MAX_STRING];

	//initiate base surface for blitting onto. Without this surface, no bmps can be blitted to screen
	if((base_surface = init_base_surface(sw)) == NULL){
		fprintf(stderr, "\nUnable to initialize base surface:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
	}

	// blit the quit animation
	current->quit == 0 ? strcpy(quitstr,"back_orange.bmp"): strcpy(quitstr,"back_grey.bmp");
	if(current->quit > 0){
		current->quit = current->quit + 1;
	}
	if(current->quit == ANIM_LOOP){
		current->quit = 0;
		current->screen = HOME;
	}
	blit_image(current, sw, base_surface, 0, 5*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, quitstr);

	blit_image(current, sw, base_surface, WWIDTH/4, 0, WWIDTH/2, WHEIGHT/5, "full_logo.bmp");

	// blit the actual names and scores
	blit_the_high_scores(sw, current, base_surface);	
}

// This function is responsible for blitting the names and scores themselves
void blit_the_high_scores(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char *p;
	char letstr[MAX_STRING];
	char numstr[MAX_STRING];
	//These numbers are to do with the ration across the screen
	int i = 5;
	int j = 6;

	int cnt = 0;

	//10 high scores 
	while(j < 16){

		p = current->hi_scores[j-6].name;
		cnt = 0;
		i = 5;

		// blit the characters one by one
		while(cnt < 8){
			// reinitialise letstr
			letstr[0] = '\0';
			//get new letstr
			get_letter_str(*p, letstr);
			//blit the high scores
			blit_image(current, sw, base_surface, i*WWIDTH/25, j*WHEIGHT/20,WWIDTH/25, WHEIGHT/25, letstr);
			//increment to next letter
			i++;
			p++;
			cnt++;
		}

		// blit the score
		i = i + 2;
		//blit the 1000's column of the points
		get_digit_str(((current->hi_scores[j-6].score/1000)%10), numstr);
		blit_image(current, sw, base_surface, i*WWIDTH/25, j*WHEIGHT/20, WWIDTH/25, WHEIGHT/25, numstr);
		i++;
		//blit the 100's column of the points
		get_digit_str(((current->hi_scores[j-6].score/100)%10), numstr);
		blit_image(current, sw, base_surface, i*WWIDTH/25, j*WHEIGHT/20, WWIDTH/25, WHEIGHT/25, numstr);
		i++;
		//blit the 10's column of the points
		get_digit_str(((current->hi_scores[j-6].score/10)%10), numstr);
		blit_image(current, sw, base_surface, i*WWIDTH/25, j*WHEIGHT/20, WWIDTH/25, WHEIGHT/25, numstr);
		//blit the 1's column of the points
		i++;
		get_digit_str(current->hi_scores[j-6].score%10, numstr);
		blit_image(current, sw, base_surface, i*WWIDTH/25, j*WHEIGHT/20, WWIDTH/25, WHEIGHT/25, numstr);

		j++;
	}
}



// This function is responsible for blitting the correct images when the game is in the tutorial screen
void tutorial_mode_bmps(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){

	char quitstr[MAX_STRING];

	//initiate base surface for blitting onto. Without this surface, no bmps can be blitted to screen
	if((base_surface = init_base_surface(sw)) == NULL){
		fprintf(stderr, "\nUnable to initialize base surface:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
	}

	//make bottom bar blue
	blit_image(current, sw, base_surface, 0, 0, WWIDTH, 4*WHEIGHT/5, "space_let.bmp");

	// blit the back animation
	current->quit == 0 ? strcpy(quitstr,"back_orange.bmp"): strcpy(quitstr,"back_grey.bmp");
	if(current->quit > 0){
		current->quit = current->quit + 1;
	}
	if(current->quit == ANIM_LOOP){
		current->quit = 0;
		current->screen = HOME;
	}
	blit_image(current, sw, base_surface, 0, 5*WHEIGHT/6,WWIDTH/6, WHEIGHT/6, quitstr);

	// use a switch statement to run the tutorial though when the mouse is clicked
	switch(current->home_tut){
		case 0: blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/10, 2*WWIDTH/3, 2*WHEIGHT/3, "tutorialscreen1.bmp");
				blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6, 2*WWIDTH/3, WHEIGHT/8, "tutorial1.bmp");
				break;
		case 1: blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/10, 2*WWIDTH/3, 2*WHEIGHT/3, "tutorialscreen2.bmp");
				blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6, 2*WWIDTH/3, WHEIGHT/8, "tutorial2.bmp");
				break;
		case 2: blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/10, 2*WWIDTH/3, 2*WHEIGHT/3, "tutorialscreen2.bmp");
				blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6, 2*WWIDTH/3, WHEIGHT/8, "tutorial2b.bmp");
				break;
		case 3: blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/10, 2*WWIDTH/3, 2*WHEIGHT/3, "tutorialscreen3.bmp");
				blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6, 2*WWIDTH/3, WHEIGHT/8, "tutorial3.bmp");
				break;
		case 4: blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/10, 2*WWIDTH/3, 2*WHEIGHT/3, "tutorialscreen3.bmp");
				blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6, 2*WWIDTH/3, WHEIGHT/8, "tutorial3b.bmp");
				break;
		case 5: blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/10, 2*WWIDTH/3, 2*WHEIGHT/3, "tutorialscreen4.bmp");
				blit_image(current, sw, base_surface, WWIDTH/6, 5*WHEIGHT/6, 2*WWIDTH/3, WHEIGHT/8, "tutorial4.bmp");
				break;
		case 6: blit_image(current, sw, base_surface, WWIDTH/6, 2*WHEIGHT/6, 2*WWIDTH/3, 2*WHEIGHT/8, "good_luck.bmp");
				break;
		case 7: current->screen = HOME;
				break;

	}
}



//This function is responsible for blitting the correct images when the game is asking the user to put in their name for a high score
void input_score_bmps(SDL_Simplewin *sw, Game *current, SDL_Surface *base_surface){


	char spinner[MAX_STRING];
	char nudgestr[MAX_STRING];
	char checkstr[MAX_STRING];
	int i;

	//initiate base surface for blitting onto. Without this surface, no bmps can be blitted to screen
	if((base_surface = init_base_surface(sw)) == NULL){
		fprintf(stderr, "\nUnable to initialize base surface:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
	}

	if(current->check == 1){
		save_user_name(current);
		current->screen = HOME;
		return;
	}

	// blit logo
	blit_image(current, sw, base_surface, 0, 0, WWIDTH, WHEIGHT/5, "full_logo.bmp");

	blit_image(current, sw, base_surface, WWIDTH/6, WHEIGHT/2, 2*WWIDTH/3, WHEIGHT/6, "name_please.bmp");

	// blit the display
	for(i = 2; i <= 9; i++){
		
		get_8bit_hex_bmp(current->display[i-2], spinner);
		blit_image(current, sw, base_surface, i*WWIDTH/12, WHEIGHT/6, WWIDTH/12, WHEIGHT/3, spinner);

		//animate the nudge button
		current->nudge_pressed[i-2] == 0? strcpy(nudgestr, "nudge_green_8bit.bmp"): strcpy(nudgestr, "nudge_grey_8bit.bmp");
		if(current->nudge_pressed[i-2] > 0){
			current->nudge_pressed[i-2] = current->nudge_pressed[i-2] + 1;
		}
		if(current->nudge_pressed[i-2] == 6){
			current->nudge_pressed[i-2] = 0;
		}
		blit_image(current, sw, base_surface, i*WWIDTH/12,4*WHEIGHT/6,WWIDTH/12, WHEIGHT/6, nudgestr);
	}

	//blit the save button
	strcpy(checkstr,"save_green.bmp");
	blit_image(current, sw, base_surface, 5*WWIDTH/6,4*WHEIGHT/6,WWIDTH/6, WHEIGHT/3, checkstr);
}



