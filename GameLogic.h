#pragma once

/*
Jacob Rogers
CS470 Lab 2 - GameLogic header
*/


// Game Logic class declarations
struct GameLogic {

public:
	// Constructor and deconstructor
	GameLogic(int* ttt_array, int boardDimension);

	// Print Tic-tac-toe board
	void printBoard();

	// Check for valid entry
	bool validEntry(int row, int col);

	// Enter player selection into board
	void makeEntry(int row, int col);

	// Determine Victor
	int determineVictor();

private:
	// Variable to hold ttt_array reference
	int* board;
	int currentPlayer;
	int boardDimension;

};
