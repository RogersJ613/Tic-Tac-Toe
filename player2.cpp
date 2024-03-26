/*
Jacob Rogers
CS470 Lab 2 - Player 2
*/

// Imports
#include <iostream>
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <cmath>
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
int main() {
	// Access the shared key size
	key_t sizeKey = ftok("ttt_size.txt", 42);
	int sizeid = shmget(sizeKey, 4, 0666);
	int* sharedKeySize = (int*)shmat(sizeid, NULL, 0);

	// Verify sharedkey key was accessed correctly
	if (sizeKey == -1) {
		perror("ftok - key");
		return 1;
	}

	// Verify shared memory was accessed correctly
	if (sizeid == -1) {
		perror("shmget - key");
		return 1;
	}

	// Access the shared memory that is used by both players - learned from ChatGPT
	key_t key = ftok("tic_tac_toe.txt", 314);
	int memid = shmget(key, 0, 0666);
	int* sharedBoard = (int*)shmat(memid, NULL, 0);

	// Verify shared memory was accessed correctly
	if (key == -1) {
		perror("ftok - array");
		return 1;
	}

	// Verify that the shared memory was created
	if (memid == -1) {
		perror("shmget - array");
		return 1;
	}

	// Calculate the board size based off the keySize
	int arraySize = *sharedKeySize / 4;
	int boardSize = (int)sqrt(arraySize - 1);

	// Wait for player 1 to make the starting move
	cout << "Player 1's turn" << endl;
	while(sharedBoard[0] == 1) {
		sleep(1);
	}

	// Create instance of GameLogic
        GameLogic* player2 = new GameLogic(sharedBoard, boardSize);

        // Variable to hold if there is a victor
        int victor = -1;
        while(victor == -1) {
                // Initialize variables that input will be stored in
                char rowInput[256];
                char colInput[256];

                // Print out the board
                player2->printBoard();

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
                if(!player2->validEntry(row, col)) {
                        cout << "Invalid Entry" << endl;
                        continue;
                } else {
                        player2->makeEntry(row, col);
                }

		// Print updated board
		player2->printBoard();

                // Switch current player to player 2, then wait until back to player 1
                sharedBoard[0] = 1;
		cout << "Player 1's Turn" << endl;
                while(sharedBoard[0] == 1) {
                        sleep(1);
                }

                // Determine if there is a victor
                victor = player2->determineVictor();
        } // End of inprogress gameplay while loop

	// Switch to player 1 so player 1 can provide win/loss statements
	sharedBoard[0] = 1;
	player2->printBoard();

        if(victor == 0) {
                cout << "This game ended in a tie :|" << endl;
        } else
        if(victor == 2) {
                cout << "You Won! :)" << endl;
        } else
        if(victor == 1) {
                cout << "You Lost :(" << endl;
        }

	// Detach shared memory
	shmdt(sharedBoard);
	shmdt(sharedKeySize);
} // End of main()
