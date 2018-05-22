////////////////////////////////////////////////////////////////////////////////
// Main File:        generate_magic.c
// This File:        generate_magic.c
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

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

int get_square_size();
Square * generate_magic(int size);
void write_to_file(Square * square, char *filename);

int main(int argc, char *argv[])
{
	// Check input arguments to get filename
	if (argc != 2) {
		printf("Usage: ./generate_magic <filename>\n");
		exit(1);
	}

	// Get size from user
	int size = get_square_size();

	// Generate the magic square
	Square *square;
	square = generate_magic(size);

	// Write the square to the output file
	write_to_file(square, argv[1]);

	//Loop through each integer pointer in array and free
	for (int i = 0; i < square->size; i++) {
		free(*(square->array + i));
	}
	//free the array itself
	free(square->array);
	//free the square
	free(square);

	return 0;
}

/* get_square_size prompts the user for the magic square size
* checks if it is an odd number >= 3 and returns the number
*/
int get_square_size()
{
	//Prompt user
	printf("Enter size of magic square, must be odd: ");
	
	//Delcare an array of characters large enough for the first line and get it from the user
	char sizeChar[4];
	fgets(sizeChar, sizeof sizeChar, stdin);

	//convert this character to an integer
	int size = atoi(sizeChar);
	
	//Check if number is odd and 3 or greater
	if (size < 3 || size % 2 == 0) {
		printf("Size must be an odd number >= 3.\n");
		exit(1);
	}

	return size;
}

/* generate_magic constructs a magic square of size n
* using the Siamese algorithm and returns the Square struct
*/
Square * generate_magic(int n)
{
	//Allocate heap space for the square
	Square *magicSquare = malloc(sizeof(Square));

	//Set the square size
	magicSquare->size = n;

	//Allocate integer pointers (however large the size is), each of which will point to a row of the square
	magicSquare->array = malloc(sizeof(int*)*n);

	//Have each integer pointer point to a row of the square
	for (int i = 0; i < n; i++) {
		*(magicSquare->array + i) = malloc(sizeof(int)*n);
	}

	//According to the method, the first row and middle column = 1
	//First row
	int i = 0;

	//Middle column
	int j = (magicSquare->size) / 2;
	
	//Set to 1
	*(*(magicSquare->array + i) + j) = 1;

	//Input simply increments, since we assigned 1 already we start at 2
	int input = 2;

	//We do not fill in a square with each pass of the loop since there will be collisions, so we run the loop n squared times to ensure the entire square gets filled 
	for (int k = 1; k < magicSquare->size*magicSquare->size; k++) {
		//The algortihm says to move up 1 and right 1
		//To move up a row we decrement i
		i--;

		//If we are in the top row and decrement i, we need to move to the bottom row
		if (i < 0) {
			i = magicSquare->size - 1;
		}

		//To move right a column we increment j
		j++;

		//If we are in the rightmost column, we need to move to the leftmost column
		if (j == magicSquare->size) {
			j = 0;
		}
		//Check to see if there is a number in the square you moved to
		if (*(*(magicSquare->array + i) + j) > 0 &&
			*(*(magicSquare->array + i) + j) < (*(*(magicSquare->array + i) + j)*magicSquare->size*magicSquare->size) + 1) {

			//Since there is a number in this square, we need to reverse the move we just did
			//Move i back down a row
			i++;
			if (i == magicSquare->size) {
				i = 0;
			}

			//Move j back left a column
			j--;
			if (j < 0) {
				j = magicSquare->size - 1;
			}

			//Now the number goes in the box below current position, so move down a row
			i++;
			if (i == magicSquare->size) {
				i = 0;
			}

			//Put the number in the box and increment it
			*(*(magicSquare->array + i) + j) = input;
			input++;
		}
		//After moving up and right, if there is no number in the box then put the number in the box and increment it
		else {
			*(*(magicSquare->array + i) + j) = input;
			input++;
		}
	}

	//Print the size of the square and a new line
	//printf("%d", magicSquare->size);
	//printf("%c", '\n');

	//Print the square
	/*for (int i = 0; i < magicSquare->size; i++) {
		for (int j = 0; j < magicSquare->size; j++) {
			printf("%i", *(*(magicSquare->array + i) + j));
			if (j < magicSquare->size - 1)
				printf(",");
		}
		printf("\n");
	}
	*/
	return magicSquare;
}

/* write_to_file opens up a new file(or overwrites the existing file)
* and writes out the square in the format expected by verify_magic.c
*/
void write_to_file(Square * square, char *filename)
{
	//Declare a file pointer and open the file for writing
	FILE *magicSquareF;
	magicSquareF = fopen(filename, "w");

	//write the size of the square to the file, then a new line
	fprintf(magicSquareF, "%d", square->size);
	fprintf(magicSquareF, "%c", '\n');

	//Loop through each row of the square
	for (int i = 0; i < square->size; i++) {
		//For each row, loop through each column
		for (int j = 0; j < square->size; j++) {

			//Write the value at the position to the file, and if it isn't the last column also write a comma to the file
			fprintf(magicSquareF, "%d", *(*(square->array + i) + j));
			if (j < square->size - 1) {
				fprintf(magicSquareF, "%c", ',');
			}
		}
		//Write a new line after the last column in each row
		fprintf(magicSquareF, "%c", '\n');
	}
	//Don't forget to close the file!
	fclose(magicSquareF);
}
