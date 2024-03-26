/*
Jacob Rogers
CS470 Lab 2 - Player 1
*/

// Imports
#include <iostream>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "GameLogic.h"

// Namespaces
using namespace std;

// Function to check to see if input contains only integers
bool isInteger(const char *input) {
	// Check if the string is empty
	if (*input == '\0') {
		return false;
	}

	// Iterate through each character in the input string to verify if it is a digit
	while (*input != '\0') {
	        if (!isdigit(*input)) {
			return false;
		}
		input++;
	}

	// If all characters are digits, return true
	return true;
}

// Main function
int main(int argc, char *argv[]) {
	// Verify that there is only one argument and that argument is an integer
	if (argc != 2) {
        	cerr << "Usage: " << argv[0] << " <positive_integer>" << endl;
        	return 1;
    	}

	// Got from ChatGPT - Checks each character in the argument to ensure it is an integer.
	char *input = argv[1];
	while (*input) {
		if (!isdigit(*input)) {
			cerr << "Error: Input must be a positive integer." << endl;
			return 1;
		}
		++input;
	}

	// Save the argument as an int value to be used for size of tic-tac-toe board
	int boardSize = stoi(argv[1]);

	// Verify boardSize is larger than 0
	if (boardSize < 1) {
		cerr << "Error: Input must be a positive integer." << endl;
			return 1;
	}

	// Value to hold how large the array needs to be: (n x n) then + 1 for determining who's turn it is
	int arraySize = boardSize * boardSize + 1;

	// Create a shared memory that will contain the size for shmget that is being used for the array
	key_t sizeKey = ftok("ttt_size.txt", 42);
	int sizeid = shmget(sizeKey, 1024, IPC_CREAT | 0666);
	size_t* sharedKeySize = (size_t*)shmat(sizeid, NULL, 0);

	// Verify that the shared key size key was created
	if (sizeKey == -1) {
		perror("ftok - key");
		return 1;
	}


	// Verify the shared memory was created
	if (sizeid == -1) {
		perror("shmget - key");
		return 1;
	}

	// Create the shared memory for the array to be accessed by both players - learned from ChatGPT
	key_t key = ftok("tic_tac_toe.txt", 314);
	size_t size = (arraySize) * sizeof(int);
	int memid = shmget(key, size, IPC_CREAT | 0666);
	int* sharedBoard = (int*)shmat(memid, NULL, 0);

	// Verify that the shared memory key was created
	if (key == -1) {
		perror("ftok - array");
		return 1;
	}


	// Verify that the shared memory was created
	if (memid == -1) {
		perror("shmget - array");
		return 1;
	}

	// Attach the size of memid to the shared memory that holds that value
	*sharedKeySize = size;

	// Populate the shared array, first index is to determine which player's turn it is
	// Player 1 : 1
	// Player 2 : 2
	// Unused : 0
	sharedBoard[0] = 1;
	for(int i = 1; i < arraySize; ++i) {
		sharedBoard[i] = 0;
	}

	// Create instance of GameLogic
	GameLogic* player1 = new GameLogic(sharedBoard, boardSize);

	// Variable to hold if there is a victor
	int victor = -1;
	while(victor == -1) {
		// Initialize variables that input will be stored in
		char rowInput[256];
		char colInput[256];

		// Print out the board
		player1->printBoard();

		cout << "Enter row number: ";
		cin.getline(rowInput, sizeof(rowInput));

		cout << "Enter column number: ";
		cin.getline(colInput, sizeof(colInput));

		if (!isInteger(rowInput) || !isInteger(colInput)) {
			cout << "Error: Input must be integers." << endl;
		}
		int row = -1;
		int col = -1;
		try {
			row = stoi(rowInput);
			col = stoi(colInput);
		} catch (const std::invalid_argument& e) {
			cout << "Error: Invalid input. Please enter valid integers for row and column." << endl;
			continue;
		}
		// If the entry is not a vaild entry, continue the loop to start again
		if(!player1->validEntry(row, col)) {
			cout << "Invalid Entry" << endl;
			continue;
		} else {
			player1->makeEntry(row, col);
		}

		// Print updated board
		player1->printBoard();

		// Switch current player to player 2, then wait until back to player 1
		sharedBoard[0] = 2;
		cout << "Player 2's Turn" << endl;
		while(sharedBoard[0] == 2) {
			sleep(1);
		}

		// Determine if there is a victor
		victor = player1->determineVictor();
	} // End of inprogress gameplay while loop

	// Change current player to 2 again to allow player 2 to calculate victor
	sharedBoard[0] = 2;
	player1->printBoard();

	if(victor == 0) {
		cout << "This game ended in a tie :|" << endl;
	} else
	if(victor == 1) {
		cout << "You Won! :)" << endl;
	} else
	if(victor == 2) {
		cout << "You Lost :(" << endl;
	}

	// Wait to ensure player2 file has deallocated memory
	sleep(1);
	// Detach shared memory
	shmdt(sharedBoard);
	shmctl(memid, IPC_RMID, nullptr);
	shmdt(sharedKeySize);
	shmctl(sizeid, IPC_RMID, nullptr);
} // End of main()

