/*  Authour: Ben Reynolds
	Version: 1.0
	File Name: FilmGenie.c
	Description: Film Guessing Game
*/

	/* This piece of code has 6 functions including main and one struct, it uses the structs to pass arrays in and out of functions.
	   the first function thats implemented is Fileinput, its job is to take the filmtitles.txt and split its contents into an array of titles. 
	   the random function then generates a random numbrr to select a random title. the guess function takes users guess makes sure they comply with the
	   rules and then displays reseults of guesses, it also controls the amount of guesses the user has. finally the ast function is used to display the
	   titles in astrisk form. The main is used to drive the programme. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
	
#define MAX_LINES 100
#define MAX_CHAR 100


	// Film struct contains two arrays, these are used to return arrays from functions.

	typedef struct {	

                char filmtitle[200];			// struct used to return string to main.
                char astr[300];
                
              } FILM;


      // FUNCTION PROTOTYPES

				FILM FileInput (FILM titles); 		// function that reads in file into an array, and returns a random member of the array to main.
				int Random (int x);					// random number function
				void GameInfo(void);				// info about the game
				FILM guess (FILM titles);					// function that takes in guesses and asses the correct answers
				FILM Ast (FILM titles);				// prints the film title in asterisks


	// MAIN function used to drive the programme, tried to keep it as clean and as empty as could be.

		int main () {

			FILM titles;							// define struct
			char response;

			GameInfo();
	
		do
		{
			
					titles = FileInput(titles);				 
								// printf("%s", titles.filmtitle); //prints acc filmtitle name for testing.
					titles = Ast(titles);	
							printf("%s", titles.astr);
							
					guess(titles);

						printf("\nWould you like to play again? Enter y or n: \n");
						scanf("%c", &response);
						fflush(stdin);

				if (response == 'n')
					{
						printf("Thank you for playing. Good Bye.");
						return 0;
					}
		}while(response == 'y' );
}



// FUNCTION DEFINITIONS


// This function simply prints the rules of the game at the beginning, it has no inputs or anything to return. 

void GameInfo (void)
{
		printf("\n****Welcome to Bens FilmGenie Game****\n");
		printf("This game will show you a classic film title on the screen but its letters will be hidden by asterisks. \nYou'll"
	        " then be given the oppertunity to guess a character or the whole title. You'll have a maximum of 5 guesses. \n");
		printf("\b\bBest of Luck!!\n\n");
		printf("Film Title:");
} 


// this function FileInput takes the file filmtitles.txt and opens the file for reading, then splits all the titles in that
// file into an array called str. The function then calls the Random function to generate a random integer which is used
// to select a random member of str. this random member is then stored in filmtitle in the struct FILM.

FILM FileInput (FILM titles) {

	// Input file into an array


	FILE *fopen(), *fi;                                                                                                                                                                  

		 	char str[MAX_LINES][MAX_CHAR];
         	char buffer[MAX_CHAR];
         	char *revstr[MAX_CHAR];
         	int curr_line = 0, i = 0, j =0;
        
         
	fi = fopen("filmtitles.txt", "r");		// open file for reading

		if (fi == NULL)
		{
			printf("The file containing the filmtitles couldnt be opened.");
			exit;
		}

     	while(curr_line < MAX_LINES && ((fgets(buffer, MAX_CHAR, fi)) != NULL))
     		{

                 
                 memcpy(str[i], buffer, strlen(buffer));
                 curr_line++;
                 ++j;							// increment i and j
                 ++i;
         	}


         
      i = Random(i);			// Randomise the member of array to be used.
 
      		strcpy(titles.filmtitle, str[i]);			// copys random string to struct
      										
				return titles ;		// return struct

		fclose(fi);					// close file
	}

		// this Random function simply generates a random integer which is then used in the FileInput function to select a random member
		// of the array str. 

	int Random (int x) {

			srand(time(NULL));				// randomise the random number
        	x = rand() % 45;				// x be the random random number

        	return x;						// return x
	}

	
	/* This is function  gives the user 5 guesses, takes in the users guess each time. checks if entered c or f to see if
	   user wants guess single character or the full title. The function also displays results of the input and the game, this is something id of liked to have in a 
	   seperate function but couldnt get it to work correctly. */
	
	FILM guess (FILM titles)
		{

			char answer[10];
			char guess[200];
			int i = 0, j = 0;

		do 									// do while loop
		{
	
			LOOP:	printf("\n Please enter c or f: \n ");			// goto here if wrong input
			scanf("%c", &answer[0]);		// input c or f		
			fflush(stdin);

				if (answer[0] == 'c')			
				{
					printf("Please guess a single character: \n");
		
						gets(guess);			// get the guess
						fflush(stdin);

						int len = strlen(titles.filmtitle);			// get length of film title and guess
						int len2 = strlen(guess);					
						i = 0;
						
							while (i < len)
							{
								titles.filmtitle[i] = toupper(titles.filmtitle[i]);		// film title and guess to upper case for the comparison.
								i++;
							}

						i= 0;
							while (i < len2)
								{
									guess[i] = toupper(guess[i]);
									i++;
								}

						i = 0;
						for(i=0; titles.filmtitle[i] != '\0'; i++)		// scanning through the character array filmtitle untill end
						{
							if(guess[0] == titles.filmtitle[i])			// if the member = guess, change asterisk for that letter
								{
								titles.astr[i] = titles.filmtitle[i];
								continue;
								}
							
							else		// if it doesnt dont change asterisk
								{
									titles.astr[i] = titles.astr[i];
									continue;
								}

							
							
						}

						printf("%s", titles.astr);			// print out the new asterisk string to help the user with guess.


				} 		// close if

		
				else if (answer[0] == 'f')					// if guessing whole title
				{
				
					i = 0;
						printf("Please guess the whole film title: \n");
						gets(guess);			// get the guess
						fflush(stdin);			// clear buffer

							int len = strlen(titles.filmtitle);			// get string lengths.
							int len2 = strlen(guess);

							while (i < len)
								{
									titles.filmtitle[i] = toupper(titles.filmtitle[i]);		// get length of film title and guess
									i++;
								}

						i= 0;
							while (i < len2)
								{
									guess[i] = toupper(guess[i]);		// film title and guess to upper case for the comparison.
									i++;
								}


									if (strncmp( guess, titles.filmtitle, (len - 1)) == 0)		// compare strings
										{
											switch (j)
											{
											case 0: printf("\nYou got it right on guess number 1, Thats Amazing!!");
											break;
											case 1: printf("\nYou got it right on guess number 2, Thats Class!!");
											break;
											case 2: printf("\nYou got it right on guess number 3, Well Done!!");
											break;
											case 3: printf("\nYou got it right on guess number 4, Wow.!");
											break;
											case 4: printf("\nYou got it right on guess number 5, PHEW!!");
											break;
										}
											break;
										}

									else 			
										{
											printf("Hard Luck \n");

											if (j == 4)
											{
													printf("\nThe Right answer was: \n%s", titles.filmtitle);  
											}
										}

			
			
					}		// close else if


	
				else {
					
					printf("You must enter either c or f! \n");
					goto LOOP;			// go back LOOP if wrong input
				}

			// printf("Guess: %s \n", guess);		// prints the guess for testing


				
				j++;			


	} while (j < 5);			// do while


	}								// close function


	
	/* This function takes the randomly selected film title and prints asterisks for every letter in that film title, 
	   it then returns titles. */
	FILM Ast (FILM titles)
		{
			int i = 0 ; 	
			char string[300];
			
				while ( titles.filmtitle[i]  != '\0'  )		// scanning through film titles array
				{
		
					if ((titles.filmtitle[i]  >= 65 && titles.filmtitle[i]  <= 90)  || (titles.filmtitle[i]  >= 97 && titles.filmtitle[i]  <= 122) )
					{				// check if == a letter in alphabet


						string[i] = '*';		// assign * if it does to a temp string called string
						i++;
						continue;
					}
		
					else if ( titles.filmtitle[i] == 32	)			// if space enter space
					{ 

						string[i] = ' ';
						i++;
						continue;
					} 
			
					else {			 // else do nothing 

						string[i];
						i++;
						continue;
					} 
				}			// close while loop

				string[i-1]='\0';			// because else increments i an extra time string[i-1] needs to be assigned to '\0'

				
			


				strcpy(titles.astr, string);		// copy contents of string to astr in struct

				return titles;			// return struct

		}		// close function





