/*
 ============================================================================
 Name        : GPS.c
 Author      : P�draig Cunningham + Ben Reynolds
 Version     : 3.0 - Elevation and Adding Splits
  ============================================================================

	==================================================================================================================================================
	|  We were given the basic gps file and we had to read in elevation and print out the splits. With the elevation i used the same                  |
    |  system Padraig did when inputiing the time lat and lon from the gpx file.                                                                      |
    |  When splitting the files into kilometre splita i did this in the calc_tot_dist function, i included an if statement to split the file          |
    |  when pathlen was divisble by 1000, unfortunately i couldnt find a way to print out the remaining metre after the last kilometre, i tried many  |
    |  methods but i just couldnt get it to work.                                                                                                     |
	|  I included a function so that you could choose what gpx file to analyse.                                                                       |
	==================================================================================================================================================
  */


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>



#define RUN 0
#define HOWTH 1
#define ZELL 2
#define d2r (M_PI / 180.0)

// The node structure for storing paths
struct node {
	double lat;
	double lon;
	double elevation;
	char timeString[22];
	struct node *next;
	struct node *previous;
};

// The structure for storing times containing hr, min and sec fields
struct timeStr {
	double hr;
	double min;
	double sec;
};

// Node Structure for storing Splits Data, each member is an array of 100 so that the file can have a max of 100 kilometres.
 struct SplitData {
	double selevation[100];
	double pace[100];
	int splitnumber[100];
	int distance[100];
	double speed[100];
}data;




int choosefile(void);
int type = 0;
double haversine_m(double lat1, double long1, double lat2, double long2);
int openFileAndLoadData();
char *readStringAfterToken(char *txtstr, char *tkn, char *res, int len, int steps);
double readDoubleAfterToken(char *txtstr, char *tkn, int steps);
double calculate_tot_dist(struct node *lh);
struct node* create_list(double lat, double lon, double elevation, char *timeStr);
struct node* add_to_list(double lat, double lon, double elevation, char *timeStr);
struct timeStr *timeStrFromString(char *tstring, struct timeStr *tstruct);
int timeDiffV2(struct timeStr *t1, struct timeStr *t2);

struct node *head = NULL;
struct node *curr = NULL;



char startTimeStr[25];		//Hold the start and end time strings as global vars.
char finishTimeStr[25];

char splitstarttime[25];
char splitendtime[25];

char *GPX_FILE_PATH = "./inputFiles/Howth-Cross.gpx";





int main(void) {

	choosefile();
	openFileAndLoadData();



	//print_list(head);
	calculate_tot_dist(head);
	return EXIT_SUCCESS;
}

 

// Calculate distance between two points expressed as lat and long using
// Haversine formula.
double haversine_m(double lat1, double long1, double lat2, double long2) {
	double dlong = (long2 - long1) * d2r;
	double dlat = (lat2 - lat1) * d2r;
	double a = pow(sin(dlat / 2.0), 2)
			+ cos(lat1 * d2r) * cos(lat2 * d2r) * pow(sin(dlong / 2.0), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double d = 6367137 * c;

	return d;
}

// Function to allow you to choose which file to open.
int choosefile(void) 
{  

	

printf ("\n Please enter a file, enter he number of the file you wish to open: ") ; 
printf (" \n 1: Howth Cross \n 2: Run 4.9km \n 3: Zell 75km ") ; 
printf ("\n If the number enter is not valid, Howth Cross will be opened: \n ") ; 
  
scanf("%d", &type); 

  
switch(type) 
{ 
  
case 1: 
{ 
    GPX_FILE_PATH = "./inputFiles/Howth-Cross.gpx"; // location of GPX file 
    break; 
} 
  
case 2: 
{  
    GPX_FILE_PATH = "./inputFiles/Run4.9k.gpx"; // location of GPX file 
    break; 
} 
  
case 3: 
{ 
    GPX_FILE_PATH = "./inputFiles/Zell75k.gpx"; // location of GPX file 
    break; 
} 
  
default: 
{ 
    GPX_FILE_PATH = "./inputFiles/Howth-Cross.gpx"; // location of GPX file 
    break; 
}  
} 
}

//Function that is called once at the start to read in the character names.
int openFileAndLoadData() {
	int lineNum = 0;
	char theLine[120];
	char endOfHdr[10] = "<trkseg>";
	char endOfData[10] = "</trkseg>";
	char *timeStrPtr;
	char tempTimeStr[30];


	FILE *fpn = fopen(GPX_FILE_PATH, "r+"); /* open for reading */
	if (fpn == NULL) { /* check does file exist etc */
		printf("Cannot open %s for reading. \n", GPX_FILE_PATH);
		printf("Set up GPX file at %s and restart. \n", GPX_FILE_PATH);
		perror("Error opening GPX file. \n");
	} else {

		// First work through the header in the GPX file.
		// Assume header finishes at "</trkseg>" stored in endOfHdr.
		while (fgets(theLine, sizeof(theLine), fpn)
				&& ((strncmp(theLine, endOfHdr,7) != 0)))
				{
					//Skip through the file until "<trkseg>" is reached (endOfHdr).
					lineNum++;
				}

		// Read the text data and store the lat, lon and time data in a linked list.
		while (fgets(theLine, sizeof(theLine), fpn)
				&& ((strncmp(theLine, endOfData,7) != 0))) //finish when "</trkseg>" reached (endOfData)
		{
			timeStrPtr = readStringAfterToken(theLine, "<time>",tempTimeStr, 20, 6);
	

			add_to_list(
					readDoubleAfterToken(theLine, "lat=",5),
					readDoubleAfterToken(theLine, "lon=",5),
					readDoubleAfterToken(theLine, "<ele>",5),
					timeStrPtr);
			lineNum++;
		}
	}
	fclose(fpn);
return lineNum;


}


// a function that will calculate the total length of the track
// printd splits and calculates data in splits

double calculate_tot_dist(struct node *lh){
	double lat1 = 0, lon1 = 0;
	double pathLen = 0;
	struct node *ptr = lh;
	double averagePace = 0;
	double elevationchange = 0;
	int kilocounter = 0;
	int i =0;


	struct timeStr tm1,tm2;
	struct timeStr spl1, spl2;


	strcpy(startTimeStr, lh->timeString);
	strcpy(splitstarttime, lh -> timeString);


			printf("  ----------------------------------------------------------------------------\n");
			printf("|| Split No:           ||");
			printf(" Elevation Change: ||");
			printf(" Time:      ||");
			printf("  Speed:         ||\n");
			printf("  ----------------------------------------------------------------------------\n");



    while(ptr != NULL){
    	if (lat1 == 0){
    		// First node
    		lat1 = ptr->lat;
    		lon1 = ptr->lon;
    	    elevationchange = ptr->elevation;
    	    lh = lh -> next;
    		 ptr = ptr->next;
    		 
    	}else
    	{
    			pathLen += haversine_m(lat1, lon1, ptr->lat, ptr->lon);
    		lh = lh -> next;


		  		if (((int)pathLen / 1000) > kilocounter )		// if total distance divedend by a 100 is greater than counter of the splits
		   			 {	  		

		
					strcpy(splitendtime, lh -> timeString);			// point to start of time

		  				spl1 = *timeStrFromString(splitstarttime, &spl1);			
		  				spl2 = *timeStrFromString(splitendtime, &spl2);

		  				data.pace[i] = timeDiffV2(&spl1, &spl2);		// calculate the tie differnce between start and end

		  				data.pace[i] /= 60;						// calculates splits time
		  				data.speed[i] = 60/data.pace[i];		// calculate av speed

				       		data.splitnumber[i] = kilocounter; 			// split number 
				    		data.selevation[i] = ptr->elevation;		// elevation
				    		data.distance[i] = pathLen;					//distance for testing



		    			double elev = data.selevation[i] - data.selevation[i-1];		// elevation = this split - the last
		    		printf("||        %4.2d         || ", (data.splitnumber[i] + 1));
		    		printf("     %4.2lf        ||", elev);
		    		printf("  %4.2lf      ||",data.pace[i]);
		    	//	printf("  %4.2d  ||", data.distance[i]);
		    		printf("      %4.2lf      || \n", data.speed[i]);

		    		kilocounter++;			// increment i and kilocounter
		    		i++;

		    		strcpy(splitstarttime, lh-> timeString);



		    	

		   			  }


		   


		   			 




    		lat1 = ptr->lat;
    		lon1 = ptr->lon;
    		elevationchange = ptr->elevation;
    		ptr = ptr->next;


    	}

    }

    /*			int x = 0;															attempt at printing the last x hundred metres
    			printf("Path Length: %5.0f m \n",pathLen);							i couldnt get this to work at all.
		   		double alpha = pathLen - 1000* (i);
		   		printf("%lf", alpha);
		   		while(x < 1)
		   		{
		   		if(alpha < 1000 && alpha > 0)
		   			{
		   					
    						printf("Path Length: %5.0f m \n",pathLen);
    						x= x +1;
		   					goto LOOP;
		   					
		   			}
		   			x++;
		   		}
	*/
		   			


   
 printf("  ----------------------------------------------------------------------------\n");

    printf("\t\t\t\tPath Length: %5.0f m \n",pathLen);

    strcpy(finishTimeStr, curr->timeString);

    	tm1 = * timeStrFromString(startTimeStr, &tm1);
    	tm2 = * timeStrFromString(finishTimeStr, &tm2);

    	printf("\t\t\t\tElapsed Time: %d sec", timeDiffV2(&tm1, &tm2));

    	averagePace = timeDiffV2(&tm1, &tm2)/pathLen*1000.0/60.0;
    	printf("\n\t\t\t\tAverage Pace: %4.2f m/km \n", averagePace);

   // 	printf("\t\t\t\televation: %lf m  \n", elevationchange); elevation for testing

    return pathLen;
}



// This function will return a  substring from string txtstr.
// The string will be searched for the first occurrence of tkn
// and then a substring on length len will be returned starting steps spaces from the
// start of tkn.
// If the token is not found in the string it returns a null pointer.

char *readStringAfterToken(char *txtstr, char *tkn, char *res, int len, int steps){
	//target string, token, length of substring and steps beyond start of token
	char *tmpstr;
	char *ret =NULL;

	tmpstr = strstr(txtstr, tkn);

	if (tmpstr ) // Checking to make sure the pointer is not NULL, i.e. strstr returned something.
	 ret = strncpy(res, (tmpstr+steps), len);

	 return ret;
}


// This function will return a double from string txtstr. The string will be searched
// for the first occurrence of tkn and the reading of the double will start steps
// places after the start of tkn.
// If the token is not found in the string it returns a value of -1.
// If there is no number after the token it returns 0.

double readDoubleAfterToken(char *txtstr, char *tkn, int steps){ //target string, token and steps beyond start of token
	char *tmpstr;
	char *rem;
	double res = -1;

	tmpstr = strstr(txtstr, tkn);

	 if (tmpstr ) // Checking to make sure the pointer is not NULL, i.e. strstr returned something.
	 res = strtod((tmpstr +steps) ,&rem);

	 return res;
}

// Create the list to be used to store the data
struct node* create_list(double lat, double lon, double elevation, char *timeStr) {
	char ts[35];
	strcpy(ts,timeStr);
	struct node *ptr = (struct node*) malloc(sizeof(struct node));
	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr->lat = lat;
	ptr->lon = lon;
	ptr->elevation = elevation;
	strcpy(ptr->timeString,timeStr);

	ptr->next = NULL;

	head = curr = ptr;
	return ptr;
}

// Add nodes to the main data list
struct node* add_to_list(double lat, double lon, double elevation, char *timeStr) {
	char ts[35];
		strcpy(ts,timeStr);
		//printf("\n adding node to list with time as %s\n", timeStr);

	if (NULL == head) {
		return (create_list(lat, lon, elevation, timeStr));
	}

	struct node *ptr = (struct node*) malloc(sizeof(struct node));
	if (NULL == ptr) {
		printf("\n Node creation failed \n");
		return NULL;
	}
	ptr->lat = lat;
	ptr->lon = lon;
	ptr->elevation = elevation;
	strcpy(ptr->timeString,timeStr);

	ptr->next = NULL;

	curr->next = ptr;
	curr = ptr;

	return ptr;
}

// A function to populate a timeStr time structure from an GPX time string.
// e.g. "2013-09-12T15:59:18Z"

struct timeStr *timeStrFromString(char *tstring, struct timeStr *tstruct){

		sscanf(tstring+11, "%lf", &tstruct->hr);
		sscanf(tstring+14, "%lf", &tstruct->min);
		sscanf(tstring+17, "%lf", &tstruct->sec);

	return tstruct;
}

// A function to return the time difference in seconds between
// two timeStr time structures.
// It assumes the two times are from the same day.

int timeDiffV2(struct timeStr *t1, struct timeStr *t2){
	int ret = 0, s1 = 0, s2 = 0;
	s1 = t1->hr * 3600 + t1->min * 60 + t1->sec;
	s2 = t2->hr * 3600 + t2->min * 60 + t2->sec;
	ret = s2 - s1;
	return ret;
}

