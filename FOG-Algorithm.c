/*
 * This program reads in a data file with movement data from a patient who is
 * suffering from Parkinson´s disease, and then an algorithm is used to detect a
 * Freeze of Gait
 *
 * To aquire data, the Totem openhealth sensor is used
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Authors:
 * Michel Verlaan <michel.verl@gmail.com>
 * Remco de Groot <renrunner@hotmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_SAMPLES 24800
#define NUMBER_OF_COLUMMS 8
#define NUMBER_OF_SAMPLES_PER_COLLUM (NUMBER_OF_SAMPLES/NUMBER_OF_COLUMMS)
#define THRESHOLD 10000
#define FREEZE_DT 600

int FogDetection(int *timearray, int *array);
void MeanFilter(int *orgArray, int *resultArray);
/* int GetOrientation(int *orgArray); */

int main()
{
	FILE *fp = fopen("/home/user/P2T2H.csv", "r"); /* change this path to your data file */

	int array[NUMBER_OF_SAMPLES] = {0};

	int time[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};
	int uselessrow[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};

	int axx_x[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};
	int axx_y[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};
	int axx_z[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};

	int giro_x[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};
	int giro_y[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};
	int giro_z[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};

	int GaussianResult[NUMBER_OF_SAMPLES_PER_COLLUM] = {0};

	int n = 0, i = 0;
	int teller = 1;
	int v = 0;
	int FOGdetected = 0;

	if(fp == NULL){
		perror("Error opening file");
		return(-1);
	}

   	printf("Opening file finished, continuing...\n");

	int collum_time = 0, collum_axx_x = 0, collum_axx_y = 0, collum_axx_z = 0, collum_giro_x = 0;
	int collum_giro_y = 0, collum_giro_z = 0, collum_useless = 0;

	/* File array with data from .csv file */
	while (fscanf(fp, "%d,", &n) > 0){

		array[i++] = n;

	}

	/* Sort the data in different array´s */
	for (v = 0; v < NUMBER_OF_SAMPLES; v++){
			 switch(teller){
				case 1:  time[collum_time++] = array[v];
						 break;
				case 2 : axx_x[collum_axx_x++] = array[v];
						 break;
				case 3 : axx_y[collum_axx_y++] = array[v];
						 break;
				case 4 : axx_z[collum_axx_z++] = array[v];
						 break;
				case 5 : giro_x[collum_giro_x++] = array[v];
						 break;
				case 6 : giro_y[collum_giro_y++] = array[v];
						 break;
				case 7 : giro_z[collum_giro_z++] = array[v];
						 break;
				case 8 : uselessrow[collum_useless++] = array[v];
						 break;
			 }

		    if (teller == 8)
		    	teller = 1;
		    else
		    	teller++;

	}


	fclose(fp);
/*
		for (int i = 0; i < NUMBER_OF_SAMPLES_PER_COLLUM; i++)
		{
			printf("Sample: %d \n", axx_y[i]);
			i++;
		}
*/

	MeanFilter(axx_y, GaussianResult);
	FOGdetected = FogDetection(time, GaussianResult);

	printf("\nThere are %d FoG measurements detected", FOGdetected);

	return(0);
}


/* To do: change factors as functioninput */
void MeanFilter(int *orgArray, int *resultArray){
	for (int i = 2; i < (NUMBER_OF_SAMPLES_PER_COLLUM-2); i++){
		*resultArray = ((orgArray[i-2]) + (orgArray[i-1]) * 3 + (orgArray[i]) * 5 + (orgArray[i+1]) * 3 + (orgArray[i+2])) /13;
		resultArray++;
	}
}

int FogDetection(int *timeArray ,int *dataArray){
	int maxAfter = 0;
	int maxBefore = 0;
	int LastPeak =0;
	int dt = 0;
	int FOGcounter = 0;

	for (int i = 5; i < (NUMBER_OF_SAMPLES_PER_COLLUM-5); i++){
		maxBefore = 0;
		maxAfter = 0;

		for (int j = (i-1); j >= (i-5); j--){
			if (maxBefore < dataArray[j])
				maxBefore = dataArray[j];
		}

		for (int n = (i+1); n <= (i+5); n++){
			if (maxAfter< dataArray[n])
				maxAfter = dataArray[n];
		}

		if(maxBefore < dataArray[i] && maxAfter < dataArray[i] && dataArray[i] > THRESHOLD){
			dt = timeArray[i]-timeArray[LastPeak];
			if(dt < FREEZE_DT){
				printf("\n%d\t@%5d ms\n",dt,timeArray[i]);
				FOGcounter++;
			}
			LastPeak = i;
			/* Todo Skip 5 measurements if peak is detected to save computation time */
		}
	}
	return FOGcounter;
}


