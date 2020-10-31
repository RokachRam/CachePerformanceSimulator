#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define MAX_FILE_NAME 100 
#define NULL ((char *)0)
void main(int argc, char* argv[])
{
	int cacheSize = atoi(argv[1]);
	printf("cashe size int : %d\n", cacheSize);
	char* associativity = argv[2];
	int blockSize = atoi(argv[3]);
	printf("block size int : %d\n", blockSize);
	char* traceFile = argv[4];
	int* intArray;
	int numberOfLines = countlines(traceFile);
	intArray = (int*)malloc(sizeof(int) * numberOfLines);
	if (!readFromFile(intArray, numberOfLines, traceFile)) { printf("error reading file\n"); exit(0); }
	if (!strcmp(associativity, "1")) oneWay(intArray, cacheSize, blockSize, numberOfLines);
	if (!strcmp(associativity, "2")) twoWay(intArray,cacheSize,blockSize,numberOfLines);
	if (!strcmp(associativity, "F")) FA(intArray,cacheSize,blockSize,numberOfLines);
	system("pause");
}
int readFromFile(int* arr, int numoflines, const char* trace)  // a function to read the text from file and store it in an array
{
	FILE* pointer = fopen(trace, "r");
	if (pointer == NULL) {
		printf("No file found.");
		return 0;
	}
	int i = 0;
	while (i < numoflines) {
		fscanf(pointer, "%d", &arr[i]);
		i++;
	}
	return 1;
}

int countlines(char filename[MAX_FILE_NAME]) //a function to count the number of lines in the txt file
{
	FILE* fp;
	int count = 1;  // Line counter (result) 
	char c;  // To store a character read from file 
	fp = fopen(filename, "r"); // Open the file 

	// Check if file exists 
	if (fp == NULL)
	{
		printf("Could not open file %s", filename);
		return 0;
	}

	// Extract characters from file and store in character c 
	for (c = getc(fp); c != EOF; c = getc(fp))
		if (c == '\n') // Increment count if this character is newline 
			count = count + 1;

	// Close the file 
	fclose(fp);
	//printf("The file %s has %d lines\n ", filename, count);

	return count;
}
float oneWay(int* intArray,int cachesize,int blocksize,int numberOfLines) {
	int hitcounter = 0;
	int misscounter = 0;
	int lines = cachesize / blocksize;
	int BlockNumber;
	int line;
	int* cacheArray;
	cacheArray = (int*)malloc(sizeof(int) * lines);
	for (int i = 0; i < numberOfLines; i++) //loops over the size of the txt
	{
		BlockNumber = intArray[i] / blocksize;  //calculate the block number to be inserted to the cache
		line = BlockNumber % lines; //uses modulu 
		if (cacheArray[line] == BlockNumber) hitcounter++;
		else {
			misscounter++;
			cacheArray[line] = BlockNumber;
		}

	}
	float missrate = (float)misscounter / numberOfLines;
	printf("missrate: %f %\n", missrate * 100); // in percentage
	return (float)missrate;

}
int twoWay(int* intArray, int cachesize, int blocksize,int numOfLines) {
	int hitcounter = 0;
	int misscounter = 0;
	int lines = cachesize / (2*blocksize);  //this time the size is half 
	int BlockNumber;
	int line;
	int* cacheArray1 = (int*)malloc(sizeof(int) * lines); //there are 2 arrays to store
	int* cacheArray2 = (int*)malloc(sizeof(int) * lines); 
	for (int i = 0; i < numOfLines; i++)
	{
		BlockNumber = intArray[i] / blocksize;
		line = BlockNumber % lines;
		if (cacheArray1[line] == BlockNumber || cacheArray2[line] == BlockNumber) hitcounter++;//if blocknumber is in one of them
		else {
			misscounter++; 
			cacheArray2[line] = cacheArray1[line]; //the second array always stores the least used value
			cacheArray1[line]= BlockNumber; //updates by order
		}
	}
	float missrate = (float)misscounter / numOfLines;
	printf("missrate: %f %\n", missrate * 100);
	return missrate;
}
int FA(int* intArray, int cachesize, int blocksize,int numOfLines) {
	int hitcounter = 0;
	int misscounter = 0; 
	int lines = cachesize /blocksize; // number of lines in the cache memmory
	int* cacheArray = (int*)malloc(sizeof(int) * lines); // the array that "stores" the block numbers in the cache memmory
	int* LRUArray = (int*)malloc(sizeof(int) * lines); // an array that counts last recently used 
	memset(LRUArray, 0, lines*4); // initializing to 0 
	for (int i = 0; i < numOfLines; i++)
	{
		int found = 0;
		int blocknum = intArray[i] / blocksize;   //the value to be inseted to the cachearray
		for (int j = 0; j < lines; j++) //checks if the block already exist int the cache array
		{
			if (cacheArray[j] == blocknum) {
				found = 1; 
				LRUArray[j]++;
				break;
			}
		}
		if (found) {
			hitcounter++;
		}
		else { //finds the least used value
			int min = 256;
			int minindex = 0;
			for (int i = 0; i < lines; i++)
			{
				if (LRUArray[i] < min) {
					min = LRUArray[i];
					minindex = i;
				}
			}
			cacheArray[minindex] = blocknum; //stores according to the min index of used values
			LRUArray[minindex] = 0; //initializes the "use" state
			misscounter++;
		}
	}
	float missrate = (float)misscounter / numOfLines;
	printf("missrate: %f %\n", missrate * 100); // in percentage 
	return missrate;
}
