/*
 ============================================================================
 Name        : Social Network Assignment - Software Engineering
 Author      : Ben Reynolds - 13309656 
 Version     : 1.0
 Copyright   : 
 Date        : 27th April 2014
 Description : 
 
  This Prograne, opens two files and the text file of the names of characters and the text file of the text itself, it manipulates the content of these files and 
  creates a third file, a csv file that contains a list of character co-occurances. 
  The programe deems a co-occurance being charavcter names appearinf being within 5 lines of eachother in the text. 
  I used a linked list for each character name that contains a list of each line that that name appears on in the text. I also used these lists to find the co-occuraces.
  I used a normal struct containing an array for the names of each character in the text file.
  I never store the text file of the play but simply scan threw it to find where each name is mentioned.

 ============================================================================

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_LINES 10000
#define MAX_CHAR 10000
const int nlists = 64;

int charcount = 0; // count number of characters in the book

//structure that is used to build linked lists of integers
 struct node
 {
  int val;
  struct node *next;
} ;

 typedef struct {    

                char Names[200][100] ;     // struct used to return string to main.
                
              } NAMES;

 struct node* heads[64]; // array with the roots of the lists
 struct node* currs[64]; // array holding pointers to current positions in list



 //initializing a list once it needs to be used.
 struct node* create_list(int val, int listNo){
     struct node *ptr = (struct node*)malloc(sizeof(struct node));

     ptr->val = val;
     ptr->next = NULL;

     heads[listNo] = currs[listNo] = ptr;
     return ptr;
 }

 //pushing an entry onto one of the lists in the array.
 struct node* add_to_list_in_array(int val, int listNo){
     if(NULL == heads[listNo]){
         return (create_list(val, listNo));
     }

     struct node *ptr = (struct node*)malloc(sizeof(struct node));

     ptr->val = val;
     ptr->next = NULL;
     currs[listNo]->next = ptr;
     currs[listNo] = ptr;

    return ptr;
 }


 // FUNCTION PROTOTYPES

                NAMES Input (NAMES characters);       // function that reads in file into an array, and returns a random member of the array to main.
                void setup_list_array(int n);
                NAMES print_list(int listNo, NAMES characters);
                NAMES LinebyLine(NAMES characters);
                NAMES scanlist(int n, NAMES characters);
                

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////                







  


int main(void) {

	int i = 0;
    NAMES characters;

            characters =  Input(characters);

            	setup_list_array( nlists);
                LinebyLine(characters);


// print the contents of all the lists to the console
	for (i = 0; i< nlists; i++){

			print_list(i, characters);
		}

            scanlist(nlists, characters);

	return EXIT_SUCCESS;
}

// this is afunction that opens the list of character names, and stores them in an array in a struct. 

NAMES  Input (NAMES characters) {

   static const char filename[] = "Les-Mis-Names.txt";      

    FILE *file = fopen( filename, "r" );        // open for reading
    int i, j;

    

    char arra[300][300];        // array 
    char line[300]; // max size

    for(i=0; i<300; i++)    
        for(j=0; j<300; j++)
            arra[i][j] = '\0';

    for(i=0; i<300; i++)

        line[i] = '\0';

    if ( file != NULL )
    {
        i=0;

            while ( fgets ( line, sizeof line, file ) != NULL ) // read a line
                {
                    strtok(line, "\n");                 // remove newline character from the input so can be compared
                    strcpy(characters.Names[i], line);          // copy from line to struct
                    charcount++;                        // count no. of characters 
                    i++;                                // new line

                }
       
        fclose ( file );        // close file
    }

    else
        {
            perror ( filename ); /* why didn't the file open? */
        }
    
    return characters ;     // return struct

}


     //linking the head and current pointers for n lists in an array.
 void setup_list_array(int n){
    int i;

    for(i=0; i<n; i++)
        {
            heads[i] = NULL;
            currs[i] = heads[i];
        }
 }

  // a function that will print one of the lists.  for checking that each instance character appears in the text is recorded properly 
 NAMES print_list(int listNo, NAMES characters){
     struct node *ptr = heads[listNo];


   //  printf("\n -------Printing list %d for %s Start------- \n",listNo, characters.Names[listNo]);
     
     while(ptr != NULL)
         {
           //  printf("[%d] ",ptr->val);   testing
             ptr = ptr->next;
         }

     return;
 }


 // this is a function that opens the full text scans threw the text line by line, when it finds an occurance a character name it adds that name to its linked list
 // it scans threw one name at a time.

 NAMES LinebyLine(NAMES characters)
 {

     int count = 1, k = 0;
    for(k=0; k<charcount; k++)          // for loop for each character 
            {
                count = 1;

                        static const char filename[] = "Les-Mis-full-text.txt";
                        FILE *file = fopen(filename, "r");
        
                if(file == NULL)
                    {
                        printf("Error Opening The text file!! \n");

                    }
                
                else if ( file != NULL )            // if file opens
                 {
                    char line[10000]; // max line size

                    
                    while (fgets(line, sizeof line, file) != NULL) // read line
                            {
                                char *pcr = strstr(line, characters.Names[k] );         // search thre line for the name of character
                                
                                    if (strstr(line, characters.Names[k] ) != NULL)
                                        {
                                           /* printf("on line %d ----  %s  \n",count, characters.Names[k]); */ // testing
                                            add_to_list_in_array(count, k);             // add the line they appear on to there list
                                            count++;                                    // increment line count
                                        }
                                    
                                    else if (pcr == NULL)               // else increment line count
                                        {
                                            count++;
                                        } 

                            }

              
            }


            fclose(file);
        }
        
        return;
 }

 // this is a function that scans threw each node in each list and if it finds any other node with the same value it adds the 
 // two names of the characters to a file called matches.csv

NAMES scanlist(int n, NAMES characters)
    {

        

    int i = 0;


        for(i=0; i < (n-1); i++)
            {
                struct node *temp = heads[i];  //Declare temp &Assign Starting Address to temp
                 //   printf("Checking: %s\n", characters.Names[i]);
                        while(temp != NULL)
                        {
                            
                           int j;

                                     // compare lists
                                     for(j=i+1; j < n; ++j)
                                        {
                                        struct node *tmp2 = heads[j]; 
                                            
                                             while(tmp2 != NULL)
                                              {


                                                   
                                                    if ((temp->val >= (tmp2->val - 5)) && (temp->val <= (tmp2->val + 5)) )
                                                        {

                                                                 

                                                             FILE *fp;      // file pointer
                                                             fp=fopen("matches.csv", "a+");  // open file for appending
                                                                 if (fp == NULL)
                                                                 {
                                                                    printf("error \n");
                                                                 }
                                                            
                                                            fprintf(fp, "%s, %s \n", characters.Names[i], characters.Names[j]); // print the names of co-occurance to file
                                                                fflush(stdout);

                                                         //   printf("\n %s is on the same line as %s, %d = %d  \n",characters.Names[i], characters.Names[j], temp->val, tmp2->val);//  testing print 

                                                            tmp2 = tmp2->next;              // second list to next node
                                                                if (tmp2 == NULL)           // if tmp2 is now empty go the next list
                                                                    {
                                                                        tmp2 = heads[j];
                                                                        break;
                                                                    } 
                                                                fclose(fp);             // close file because c can only have an x amount open at a a time
                     
                                                        }

                                                  
                                                   else                         
                                                    {
                                                        tmp2 = tmp2->next;
                                                             if (tmp2 == NULL)      // if tmp2 is now empty go the next list
                                                                    {
                                                                        tmp2 = heads[j];
                                                                        break;
                                                                    }
                                                    } 

                            
                                             }
                                
                                
                                        }
                            temp = temp->next;
                
                         }

            }
}