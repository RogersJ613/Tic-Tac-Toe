/*
Jacob Rogers
CS470 Lab 2 - GameLogic cpp
*/

// Imports
#include "GameLogic.h"
#include <iostream>

using namespace std;

// Game Logic definitions
// Constructor
GameLogic::GameLogic(int* ttt_array, int boardDimension) {
	board = ttt_array;
	currentPlayer = board[0];
	this->boardDimension = boardDimension;
}

// Print the tic-tac-toe board
void GameLogic::printBoard() {
	// Ignore the first index as that is for the current player
	for(int i = 1; i <= boardDimension; ++i) {
		for(int j = 1; j <= boardDimension; ++j) {
			// Take the value of the index in board and convert it to X and O to print
			int indexValue = board[(i - 1) * boardDimension + j];
			char boardValue = ' ';

			// Player 1 is X; Player 2 is O; No input is " "
			if(indexValue == 1) {
				boardValue = 'X';
			} else
			if (indexValue == 2) {
				boardValue = 'O';
			} else if (indexValue == -1) {boardValue = 'F';}
			cout << boardValue;

			// Add in vertical bars between each column
			if(j < boardDimension) {
				cout << " | ";
			}
		}// End of Inner ForLoop
		cout << endl;

		if (i < boardDimension) {
			for (int k = 1; k <= boardDimension; ++k) {
				cout << "- ";
				if (k < boardDimension) {
					cout << "+ ";
				}
			}
			cout << endl;
		}


	} // End of Outer ForLoop
} // End of Print Board

// Check for Valid Entry
bool GameLogic::validEntry(int row, int col) {
	// First check to see if the row and col are within the board dimensions
	if (row <= boardDimension && row > 0 && col <= boardDimension && col > 0) {
		// Then check to see if that index has been seleced yet (col + 1 is to account for the current player being in the first index)
		if(board[(row - 1) * boardDimension + col] == 0) {
			return true;
		}
	}
	return false;
}

// Enter player selection into board at given index
void GameLogic::makeEntry(int row, int col) {
	board[(row - 1) * boardDimension + col] = currentPlayer;
}

// Determine Victor
// -1 for in progress ; 0 for tie ; 1 for Player 1 ; 2 for Player 2
int GameLogic::determineVictor() {
	// RowWin
        for(int i = 0; i < boardDimension; ++i) { // i is row index
                bool RowWin = true;
                int firstValue = board[i * boardDimension + 1];
                for(int j = 1; j < boardDimension + 1; ++j) { // j is the col index
                        int currentValue = board[i * boardDimension + j];
                        if(firstValue != currentValue || currentValue == 0){
                                RowWin = false;
                                break;
                        }
                }
                if(RowWin) {
                        return firstValue;
                }
        } // End of Row Win

	// colWin
        for(int i = 1; i < boardDimension + 1; ++i) { // i is col index
                bool colWin = true;
                int firstValue = board[i];
                for(int j = 1; j < boardDimension; ++j) { // j is the row index
                        int currentValue = board[j * boardDimension + i];
                        if(firstValue != currentValue || currentValue == 0){
                                colWin = false;
                                break;
                        }
                }
                if(colWin) {
                        return firstValue;
                }
        }

	// Main Diaganol
        bool dig1 = true;
        int dig1FirstValue = board[1];
        for(int i = 1; i < boardDimension; ++i) {
                int dig1CurrentValue = board[i * boardDimension + i];
                if(dig1FirstValue != dig1CurrentValue || dig1CurrentValue == 0) {
                        dig1 = false;
                        break;
                }
        }
        if(dig1) {
                return dig1FirstValue;
        }

	// Anti Diagonal
	bool dig2 = true;
	int dig2FirstValue = board[boardDimension];

	for(int i = 1; i < boardDimension; ++i) {
		int dig2CurrentValue = board[i * boardDimension + (boardDimension - i)]; // Anti-diagonal element
		if(dig2FirstValue != dig2CurrentValue || dig2CurrentValue == 0) {
			dig2 = false;
			break;
		}
	}

	if(dig2) {
		return dig2FirstValue;
	}


    // Check for if the game is still in progress
    for (int i = 1; i <= boardDimension * boardDimension; ++i) {
        if (board[i] == 0) {
            return -1;
        }
    }

    // If none of these criteria were met, then it was a tie
    return 0;



} // End of determineVictor

