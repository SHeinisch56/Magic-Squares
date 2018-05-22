////////////////////////////////////////////////////////////////////////////////
// Main File:        verify_magic.c
// This File:        verify_magic.c
// Other Files:      N/A
// Semester:         CS 354 Spring 2017
//
// Author:           Steve Heinisch
// Email:            sheinisch@wisc.edu
// CS Login:         heinisch
//
/////////// IF PAIR PROGRAMMING IS ALLOWED, COMPLETE THIS SECTION //////////////
//
// Pair Partner:     (name of your pair programming partner)
// Email:            (email address of your programming partner)
// CS Login:         (partner's CS login name)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

Square * construct_square(char *filename);
int verify_magic(Square * square);

int main(int argc, char *argv[])
{
	// Check input arguments to get filename
	//If there aren't two arguments, print error and exit
	if (argc != 2) {
		printf("Usage: ./verify_magic <filename>\n");
		exit(1);
	}

	// Construct square
	Square *square = construct_square(argv[1]);

	//Verify if it's a magic square and print true or false
	int val = verify_magic(square);

	//if verify returns 
	if (val == 1) {
		printf("true\n");
	}
	if (val == 0) {
		printf("false\n");
	}

	//Loop through each integer pointer within **array and free 
	for (int i = 0; i < square->size; i++) {
		free(*(square->array + i));
	}

	//free the array itself
	free(square->array);

	//free the square
	free(square);

	return 0;
}

/* construct_square reads the input file to initialize a square struct
* from the contents of the file and returns the square.
* The format of the file is defined in the assignment specifications
*/
Square * construct_square(char *filename)
{
	// Open and read the file
	//Declare a file pointer
	FILE *magicSquareF;

	//Use fopen to open the file for reading
	magicSquareF = fopen(filename, "r");

	//Read the first line to get the square size
	//Read in the size of the square
	char dim[10];

	fgets(dim, 10, magicSquareF);

	//Convert these from characters to integers
	int dimension = atoi(dim);

	// Initialize a new Square struct of that size
	Square *magicSquare = malloc(sizeof(Square));

	//Assign the read dimension to be the size of the square
	magicSquare->size = dimension;

	//Allocate integer pointers on the heap (as many as the dimension of the square is), each of which will point to a row of the square
	magicSquare->array = malloc(sizeof(int*)*dimension);

	//Loop through the integer pointers and allocate memory for the integers for each row
	for (int i = 0; i < dimension; i++) {
		*(magicSquare->array + i) = malloc(sizeof(int)*dimension);
	}

	// Read the rest of the file to fill up the square
	//Create an array of characters big enough to read in each line
	char str[500];

	//Store ',' so we can parse it out in strtok
	const char s[2] = ",";
	char *token;

	//Read in the next line
	fgets(str, 500, magicSquareF);

	//Set the token to the line that was read and parse ','
	token = strtok(str, s);

	//This loop parses the string for each row, converts the characters to integers, and stores the in the 2D square array
	//For each row
	for (int i = 0; i < dimension; i++) {
		//Starting with column 0
		int j = 0;
		//While the token is not NULL, convert the token to an integer and store it in the ith row and jth column
		while (token != NULL) {
			*(*(magicSquare->array + i) + j) = atoi(token);
			//increment to the next column
			j++;
			token = strtok(NULL, s);
		}
		//Read in the next row and repeat the for loop
		if (fgets(str, 500, magicSquareF) != NULL) {
			token = strtok(str, s);
		}
		else {
			token = strtok(NULL, s);
		}
	}

	//Print the square
/*
	for (int i = 0; i < magicSquare->size; i++) {
		for (int j = 0; j < magicSquare->size; j++) {
			printf("%i", *(*(magicSquare->array + i) + j));
			if (j < magicSquare->size - 1) {
				printf(",");
			}
		}
		printf("\n");
	}
*/
	fclose(magicSquareF);

	return magicSquare;
}

/* verify_magic verifies if the square is a magic square
*
* returns 1(true) or 0(false)
*/
int verify_magic(Square * square)
{
	
	//Assume it is a magic square, change to false if necessary
	bool isMagicSquare = true;

	// Check all rows sum to same number
	//Initialize an integer at 0 to use to find the sum of the first row
	int sumOfFirstRow = 0;

	//Add each column in the first row to find the sum of the first row
	for (int j = 0; j < square->size; j++) {
		//Since the first row is i=0, we don't need to add i when indexing through 
		sumOfFirstRow += *(*(square->array) + j);
	}

	//printf("%d\n", sumOfFirstRow);

	//For the rest of the rows, find the sum and compare to the sum of the first row
	for (int i = 1; i < square->size; i++) {
		int sumOfNextRow = 0;
		for (int j = 0; j < square->size; j++) {
			sumOfNextRow += *(*(square->array + i) + j);
		}

		//If the sum of a row is not the same as the sum of the first row, the square is not a magic sqaure and we break from the loop, no further checking needed
		if (sumOfNextRow != sumOfFirstRow) {
			isMagicSquare = false;
			break;
		}
	}

	//If we find that it is not a magic square, we can return 0 (false) to from the function, no further checking is needed
	if (!isMagicSquare) {
		return 0;
	}

	// Check all cols sum to same number
	//Same with rows, sum each column, checking each against the sum of the first row
	for (int i = 0; i < square->size; i++) {
		int sumOfColJ = 0;

		for (int j = 0; j < square->size; j++) {
			sumOfColJ += *(*(square->array + i) + j);
		}
		//If any column does not equal the sum of the first row, immediately change isMagicSquare to false, break the loop, and return 0 from the function
		if (sumOfColJ != sumOfFirstRow) {
			isMagicSquare = false;
			break;
		}
	}
	//If we find that it is not a magic square, we can return 0 (false) to from the function, no further checking is needed
	if (!isMagicSquare) {
		return 0;
	}
	
	// Check main diagonal
	//Integer to track the sum of the main diagonal
	int sumMainDia = 0;
	//Each row in the main diagonal is the same index of each column, so simply looping through the ith row and column will loop through the main diagonal
	for (int i = 0; i < square->size; i++) {
		sumMainDia += *(*(square->array + i) + i);
	}
	//If the main diagonal does not equal the sum of the first row, immediately change isMagicSquare to false and return 0 from the function
	if (sumMainDia != sumOfFirstRow) {
		isMagicSquare = false;
	}
	if (!isMagicSquare) {
		return 0;
	}

	// Check secondary diagonal
	//Integer to track the sum of the secondary diagonal
	int sumSecDia = 0;
	
	//For the secondary diagonal, when we are at row i, we will be at the column j that is opposite of it, so we start at i = 0 and j as the size - 1, and increment i
	//while decrementing j, and adding to the sum
	int j = square->size - 1;

	for (int i = 0; i < square->size; i++) {
		sumSecDia += *(*(square->array + i) + j);
		j--;
	}

	//If the sum of the secondary column does not equal the sum of the first row, return 0 (false), if not then return 1 (true)
	if (sumSecDia != sumOfFirstRow) {
		return 0;
	}
	else {
		return 1;
	}
}
