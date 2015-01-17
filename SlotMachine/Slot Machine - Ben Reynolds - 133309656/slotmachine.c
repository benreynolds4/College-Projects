/*
 ============================================================================
 Name        : slotmachine.c
 Author      : Ben Reynolds - 13309656
 Class       : Software Engineering Semester 2 2014
 Version     : Version 1.0
 Description : Slot Machine - Using functions and structs.
 ============================================================================
 */

	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>

			void random (void) ;
			float Calwin (float credits, float bet, int col1, int col2, int col3);
			float EnoughCred(float credits, char replay);
			int column1 (int col1);
			int column2 (int col2);
			int column3 (int col3);
	
		typedef struct {
			
				int col1, col2, col3 ;
			
				} slot ;
		
			slot face ;

	
	int main () {



		// Define variables
			float credits = 10, bet;
			int col1, col2, col3, x;
			char replay, buf[BUFSIZ], *p;
	
	
		printf("\n\n\t ***********Welcome To Bens Slot Machine********* \n \n\tIf you get three of a kind youll double your bet,\n\ttwo of a kind youll recieve 50 percent of your bet back,\n\twhile anything else will lose your bet! \n\n\tGood Luck!! \n\n");



		// Initial Loop which contains the whole programme
			
LOOP: while (credits >= 2)
			{

				printf("\tYou Currently have %0.02f Credits \n", credits);

	
		// Bet amount and appropriote checks that you havent bet to much or little

				printf("\tEnter the number of credits you wish to bet: \n\t"); 		// Enter Credits
			
					if (fgets(buf, sizeof(buf), stdin) != NULL)			// check if input is a character instead of int
					{
   
					bet = strtol(buf, &p, 10);
					if (buf[0] != '\n' && (*p == '\n' || *p == '\0'))
					printf ("\n"); 
					else { continue; }
					
					}  
		
	
					if( bet > credits) {			//check credits bet compared to have.

						printf("\n\tYou Cannot bet more than the Credits you have! \n\n");
						continue;
						
								}


					if( bet < 2) {			//check credits bet compared to have.

						printf("\n\tYou Cannot bet less than 2 Credits! \n\n");
						continue;
						
								}

							else {

								credits = credits - bet;
								
								}


	
		// Generate Random numbers and then use switches to give random numbers identitys

				random ();		//random numbers for column 1 2 and 3

					col1 = column1 (col1);				//facing the random numbers with fruit names
					col2 = column2 (col2);
					col3 = column3 (col3);

	
		// Checks Slot results and calculates credits accordingly

	
			credits =  Calwin(credits, bet, col1, col2, col3);

		
		// Check if enough credits to play again and ask what want

			replay = EnoughCred(credits, replay);

				if(replay == 'y')		// y returns to top of loop/game
					{
						continue;
						
					}

				else if(replay == 'n')		// n ends game/exit loop
					{
						printf("\n\tYou finished the game with %0.02f credits! \n \n\tGood Bye!!", credits);
						return 0;
						
					}
					
				else 							// this piece of code is to attempt to return to top of loop when y/n isnt entered 
												//it caused some errors in my programme but was the closest i could get.
				{
					printf ("\tYou can only enter Y/N \n\n") ;
					goto LOOP ;
				}
					
				
					

		}			// end loop.0


	} 			// end main
	
	
	
	/* Functions */
		
		void random (void)
			{
			
				srand(time(NULL));					//random numbers for column 1 2 and 3
				face.col1 = 1 + rand() % 3;
				face.col2 = 1 + rand() % 3;
				face.col3 = 1 + rand() % 3;
				
			}
	
	/* This function calculates if the person won or not and the credits they won or lost */

		float Calwin (float credits, float bet, int col1, int col2, int col3)
			{

				if((face.col1 == face.col2) && (face.col2 == face.col3) )		//if winner and award credits
					{
					
						printf("\n\t********************\n\tYou're a Winner!!\n\t******************** \n\n");
						credits = credits + (bet * 2);
						
					}

				else if ((face.col1 == face.col2) || (face.col2 == face.col3) || (face.col1 == face.col3)) {		// if half house and award credits

						printf("\n\t**** You win Half Your money!! **** \n\n");
						credits = credits + (bet * 1.5);
						
					}


				else 			//else loser, lose credits
					{
					
						printf("\n\t**** You're a Loser **** \n\n");
						
					}
					
					return credits;
		}
		
		
		
		/* This function checks if at the end of the game the player has enough credits to play again */
	
		float EnoughCred(float credits, char replay)
			{
			
				if (credits < 2)		//if less than 2 credits end game
					{
				
						printf("\tYou have too few credits to play again, hard luck \n\n\t Good Bye");
						return 0;
					
					}

				else if (credits >= 2) {		//else ask to play again

						printf("\tYou have %0.02f credits, Would You like to play again? \n\t Enter y for yes and n for no: \n\t", credits);
						fflush(stdin);
						scanf("%c", &replay);
					}
					
				return replay;
			}
			
			
			/* Next Three functions give the random generated numbers fruit faces */
			
			int column1 (int col1)
			{
			
				switch (face.col1) {						//facing the numbers with fruit names
					
					case 1:  printf("\n\tResults: |APPLE|    ");
					break;

					case 2: printf("\n\tResults: |ORANGE|    ");
					break;

					case 3:  printf("\n\tResults: |PEAR|    ");
					break;
					
									}
				return col1;
			}
					
				
			int column2 (int col2)
			{
			
				switch (face.col2) {						//facing the numbers with fruit names
				
					case 1:  printf("|APPLE|    ");
					break;

					case 2: printf("|ORANGE|    ");
					break;

					case 3:  printf("|PEAR|    ");
					break;
					
									}
									
				return col2;
			}
		
		
			int column3 (int col3)
			{
			
				switch (face.col3) {						//facing the numbers with fruit names
				
					case 1:  printf("|APPLE| \n");
					break;

					case 2: printf("|ORANGE| \n");
					break;

					case 3:  printf("|PEAR| \n");
					break;
									}
									
				return col3;
			}







