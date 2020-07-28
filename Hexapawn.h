//
//  Hexapawn.h
//  LearningHexapawn
//
//  Created by Travis  Bender on 7/14/20.
//  Copyright © 2020 Travis Bender. All rights reserved.
//

#ifndef Hexapawn_h
#define Hexapawn_h

struct HEXNODE {
    //A unique code that identifies each gameboard state. 1 represents an X, 2 represents
    //an O, and 3 represents a numerical (empty) spot.
    //The leftmost digit of code corresponds to board[0], the rightmost digit to board[8].
    long code;
    //Four branches from each node (max number of moves is 4 for each node)
    HEXNODE * left;
    HEXNODE * midLeft;
    HEXNODE * midRight;
    HEXNODE * right;
    //An array that stores the gameboard states (codes) already processed.
    //usedCodes[0] is the number of codes stored.  The codes start at usedCodes[1].
    long usedCodes[5];
};

class Hexapawn {
private:
    //The root of the game tree
    HEXNODE * root;
    //The game board
    char board[9];
    //Identifies the current player (X or O) for a given turn
    char player;
    //Identifies the opposite player (X or O) for a given turn
    char oppPlayer;
    //Variable to change the mode (training the computer vs. human playing)
    int mode;
    
    
public:
    //Constructor
    Hexapawn();
    //Destructor
    ~Hexapawn();
    //Returns a unique code for each state of the gameboard
    long createCode();
    //Checks the gameboard for a winner
    bool checkWinner();
    //Creates and traverses the game tree.  The computer learns which moves lead to a loss.
    HEXNODE * createTree(HEXNODE * node);
    //Returns all possible moves at a game state for a given player.  Mul is the multipler.
    //It is -1 for X and 1 for O.
    int * getAllMoves(int mul);
    //The driver function
    void playGame();
    //Displays the board
    void displayBoard();
    //Function to facilitate the human's turn depending on the mode.
    void playersTurn();
    //Translates the code
    void translateCode(long code);
    //Deletes the tree starting at node
    HEXNODE * deleteTree(HEXNODE * node);
    //Creates and returns a new node
    HEXNODE * newNode();
    //Resets the gameboard and the players (but leaves the computer's memory alone)
    void resetGame();
    //Makes the computer play 100 games to learn winning strategies
    HEXNODE * trainComputer();
};

#endif /* Hexapawn_h */
