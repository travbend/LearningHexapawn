//
//  Hexapawn.cpp
//  LearningHexapawn
//
//  Created by Travis  Bender on 7/14/20.
//  Copyright © 2020 Travis Bender. All rights reserved.
//

#include <iostream>
#include "Hexapawn.h"
#include <cstdlib>
#include <math.h>
using namespace std;

//Constructor
Hexapawn::Hexapawn() {
    root = newNode();
    root->code = 222333111;
    translateCode(root->code);
    player = 'O';
    oppPlayer = 'X';
    srand((unsigned int) time(0));
}

//Destructor
Hexapawn::~Hexapawn() {
    //Deletes the entire tree
    root = deleteTree(root);
}

//Function to facilitate the game
void Hexapawn::playGame() {
    cout << "Welcome to Hexapawn!" << endl;
    cout << "The pawns play like normal chess pawns." << endl;
    cout << "They can move forward to an open space or " << endl;
    cout << "diagonally to capture." << endl << endl;
    
    cout << "The game ends when a player reaches the other side" << endl;
    cout << "or a stalemate is reached and the player who " << endl;
    cout << "caused the stalemate wins." << endl << endl;
    
    //Variables to keep track of the modes
    bool trained = false;
    bool quit = false;
    
    //The game loop
    while (!quit) {
        int choice = 0;
        
        cout << endl << "Menu: " << endl;
        
        //If the computer has not already been trained
        if (!trained) {
            cout << "1. Train the computer" << endl;
            cout << "2. Play against the computer" << endl;
            cout << "3. Quit" << endl << endl;
            
            //Gather an validate input
            do {
                cout << "Please enter your choice (e.g. \'1\'): ";
                cin >> choice;
            } while (choice < 1 || choice > 3);
            
            //Choice to train the computer
            if (choice == 1) {
                //Resets the gameboard and players
                resetGame();
                //Train the computer
                root = trainComputer();
                trained = true;
                
                cout << endl << "Computer just played 100 games and is now (reasonably) trained." << endl << endl;
                
            }
            //Choice to play against the computer
            else if (choice == 2) {
                //Resets the gameboard
                resetGame();
                //Mode to play against human opponent
                mode = 1;
                root = createTree(root);
            }
            //Choice to quit
            else {
                quit = true;
                cout << "Quitting..." << endl;
                return;
            }
            
        }
        // If the computer has already been trained
        else {
            cout << "1. Play against the computer" << endl;
            cout << "2. Quit" << endl << endl;
            
            //Gather and validate input
            do {
                cout << "Please enter your choice (e.g. \'1\'): ";
                cin >> choice;
            } while (choice < 1 || choice > 2);
            
            //Choice to play against computer
            if (choice == 1) {
                resetGame();
                //Mode to play against human opponent;
                mode = 1;
                root = createTree(root);
            }
            //Choice to quit
            else {
                quit = true;
                cout << "Quitting..." << endl;
                return;
            }
        }
    }
}

//The computer plays 100 games to learn winning strategies
HEXNODE * Hexapawn::trainComputer() {
    mode = 0; // mode for training computer
    for (int i = 0; i < 100; i++) {
        //reset the gameboard and players
        resetGame();
        //play one game
        root = createTree(root);
    }
    return root;
}

// a 1 in the code represents an X, a 2 represents an O, and a 3 represents a numbered space
long Hexapawn::createCode() {
    long code = 0;
    for (int i = 0; i < 9; i++) {
        //X is a 1
        if (board[i] == 'X') {
            code += 1;
        } else if (board[i] == 'O') { // O is a 2
            code += 2;
        } else { // a number is a 3
            code += 3;
        }
        // shift the number to the left
        if (i < 8) {
            code *= 10;
        }
    }
    return code;
}

//Checks the gameboard to see if the current game state has a winner
bool Hexapawn::checkWinner() {
    // If an X made it to the opposite side
    if (board[0] == 'X' || (board[1] == 'X' || board[2] == 'X')) {
        return true;
    }
      
    //If an O made it to the opposite side
    if (board[6] == 'O' || (board[7] == 'O' || board[8] == 'O')) {
        return true;
    }
    
    int mul = 0;
    
    //Set the multiplier for the direction of the play
    if (player == 'X') {
        mul = -1;
    } else {
        mul = 1;
    }
    
    //Get the moves available
    int * moves = getAllMoves(mul);
    
    //If there are no moves, then there is a stalemate
    //and therefore must have a winner
    if (moves == nullptr) {
        delete [] moves;
        return true;
    }
    delete [] moves;
    
    //If none of the above conditions are true, then must not have a winner
    return false;
}


//Function to create and traverse the tree
HEXNODE * Hexapawn::createTree(HEXNODE * node) {
    //Switches the player for each turn
    //As such, player must be O and oppPlayer must be X when first entering the loop for each game
    char temp = player;
    player = oppPlayer;
    oppPlayer = temp;
    
    //If the mode is to play against a human player
    if (mode == 1) {
        if (player == 'X') {
            cout << endl << "Your turn (Player \'X\'): " << endl << endl;
        } else {
            cout << endl << "Computer's turn: " << endl << endl;
        }
        displayBoard();
        cout << endl;
    }
    
    int * moves = nullptr;
    
    //If there is a winner
    if (checkWinner()) {
        //If playing against a human, then output the winner
        if (mode == 1) {
            cout << "Player " << oppPlayer << " wins!" << endl << endl;
        }
        
        //If the player equals X, then O must have won (The player switches at the beginning of the turn)
        //Therefore return the node without deleting anything
        if (player == 'X') {
            return node;
        } else {  //If X won then delete the node and return null
            delete node;
            node = nullptr;
            return nullptr;
        }
    } else {  // If there is not a winner
        // The computer's turn
        if (player == 'O') {
            
            //Get all the available moves
            moves = getAllMoves(1);
            
            //The index for the move
            int begin = 0;
            
            bool badCode = true;
            long code = 0;
            bool matchesUsed = false;
            
            do {
                //Reset the variables for each iteration of the loop
                badCode = true;
                code = 0;
                matchesUsed = false;
                
                //Get a random move
                begin = ((rand() % moves[0]) * 2) + 1;
                
                //Get the existing spaces on the board in case the move is bad
                char start = board[moves[begin] - 1];
                board[moves[begin] - 1] = moves[begin] + '0';
                char end = board[moves[begin + 1] - 1];
                board[moves[begin + 1] - 1] = 'O';
                
                //Create the code
                code = createCode();

                //Check to see if the code matches a code already seen
                for(int i = 1; i <= node->usedCodes[0]; i++) {
                    if (code == node->usedCodes[i]) {
                        matchesUsed = true;
                    }
                }
                
                
                //If have already seen this code
                if (matchesUsed) {
                    //Check to see if a node exists with this code
                    //If so, then it is a good code
                    if (node->left != nullptr && code == node->left->code) {
                        badCode = false;
                    } else if (node->midLeft != nullptr && code == node->midLeft->code) {
                        badCode = false;
                    } else if (node->midRight != nullptr && code == node->midRight->code) {
                        badCode = false;
                    } else if (node->right != nullptr && code == node->right->code) {
                        badCode = false;
                    } else {
                        badCode = true;
                    }
                } else {
                    badCode = false;
                }
                
                //If bad, reset the board to before a move was chosen
                if (badCode) {
                
                    board[moves[begin] - 1] = start;
                    board[moves[begin + 1] - 1] = end;
                }
            }
            while (badCode); //Repeat while the code is bad
            
            //If playing against a human output the move the computer decided
            if (mode == 1) {
                cout << "Computer decided to move the \'O\' at space " << moves[begin] << endl;
                cout << "to space " << moves[begin + 1] << "." << endl << endl;
            }
            
            //If the code has not been seen, add it to usedCodes
            if (node->usedCodes[0] < 4 && !matchesUsed) {
                node->usedCodes[node->usedCodes[0] + 1] = code;
                node->usedCodes[0]++;
            }
            
            HEXNODE * delNode = nullptr;
            
            //If the code already exists in a node, continue with that node
            if (node->left != nullptr && code == node->left->code) {
                node->left = createTree(node->left);
                delNode = node->left;
            } else if (node->midLeft != nullptr && code == node->midLeft->code) {
                node->midLeft = createTree(node->midLeft);
                delNode = node->midLeft;
            } else if (node->midRight != nullptr && code == node->midRight->code) {
                node->midRight = createTree(node->midRight);
                delNode = node->midRight;
            } else if (node->right != nullptr && code == node->right->code) {
                node->right = createTree(node->right);
                delNode = node->right;
            } else { // if not, create a new node with that code
                HEXNODE * tempNode = newNode();
                tempNode->code = code;
                if (node->left == nullptr) {
                    tempNode = createTree(tempNode);
                    node->left = tempNode;
                    delNode = node->left;
                } else if (node->midLeft == nullptr) {
                    tempNode = createTree(tempNode);
                    node->midLeft = tempNode;
                    delNode = node->midLeft;
                } else if (node->midRight == nullptr) {
                    tempNode = createTree(tempNode);
                    node->midRight = tempNode;
                    delNode = node->midRight;
                } else if (node->right == nullptr) {
                    tempNode = createTree(tempNode);
                    node->right = tempNode;
                    delNode = node->right;
                } else {
                    delete tempNode;
                    tempNode = nullptr;
                }
            }
            
            //If the returned node is null
            if (delNode == nullptr) {
                // don't need to delete tempNode
                //because already deleted if delNode == nullptr

                //if all the child nodes are null
                bool allNull = (node->left == nullptr && (node->midLeft == nullptr && (node->midRight == nullptr && node->right == nullptr)));
                
                //If all of the nodes are null and all of the moves have been explored
                //Delete the node
                if (allNull && node->usedCodes[0] >= moves[0]) {
                    delete [] moves;
                    delete node;
                    node = nullptr;
                    return nullptr;
                } else { // if not, then there are still paths to explore
                    delete [] moves;
                    return node;
                }
            }

            delete [] moves;
            return node;
        } else { // Player X's turn
            
            //Function to handle the human player's turn
            playersTurn();
            
            HEXNODE * tempNode = nullptr;
            HEXNODE * delNode = nullptr;
            
            //Create the code
            long code = createCode();
            bool matchesUsed = false;

            //If the code has already been seen
            for(int i = 1; i <= node->usedCodes[0]; i++) {
                if (code == node->usedCodes[i]) {
                    matchesUsed = true;
                }
            }
            
            //If the code has not already been seen, add it to used codes
            if (node->usedCodes[0] < 4 && !matchesUsed) {
                node->usedCodes[node->usedCodes[0] + 1] = code;
                node->usedCodes[0]++;
            }
            
            //If a child node already has that code, continue with that node
            if (node->left != nullptr && code == node->left->code) {
                node->left = createTree(node->left);
                delNode = node->left;
            } else if (node->midLeft != nullptr && code == node->midLeft->code) {
                node->midLeft = createTree(node->midLeft);
                delNode = node->midLeft;
            } else if (node->midRight != nullptr && code == node->midRight->code) {
                node->midRight = createTree(node->midRight);
                delNode = node->midRight;
            } else if (node->right != nullptr && code == node->right->code) {
                node->right = createTree(node->right);
                delNode = node->right;
            } else { // If not, create a new node with that code
                tempNode = newNode();
                tempNode->code = code;
                if (node->left == nullptr) {
                    tempNode = createTree(tempNode);
                    node->left = tempNode;
                    delNode = node->left;
                } else if (node->midLeft == nullptr) {
                    tempNode = createTree(tempNode);
                    node->midLeft = tempNode;
                    delNode = node->midLeft;
                } else if (node->midRight == nullptr) {
                    tempNode = createTree(tempNode);
                    node->midRight = tempNode;
                    delNode = node->midRight;
                } else if (node->right == nullptr) {
                    tempNode = createTree(tempNode);
                    node->right = tempNode;
                    delNode = node->right;
                } else {
                    delete tempNode;
                    tempNode = nullptr;
                }
            }
            
            //If the returned node is null delete this node altogether.
            //We don't want any nodes that could possibly lead to a win depending
            //on what the human picks.
            if (delNode == nullptr) {
                //don't need to delete tempNode here because should
                // already be deleted if delNode == nullptr
                
                delete node;
                node = nullptr;
                return nullptr;
            } else {
                return node;
            }
        }
    }
}

//Function that gets all moves on the board depending on the player
int * Hexapawn::getAllMoves(int mul) {
    //Initialize the array
    int * moves = new int[13];
    for (int i = 0; i < 13; i++) {
        moves[i] = -1;
    }
    
    //Arrays for the players' places
    int xSpots[] = {-1, -1, -1};
    int oSpots[] = {-1, -1, -1};
    int xIterator = 0;
    int oIterator = 0;
    
    //Fill the arrays with the players spots
    for(int i = 0; i < 9; i++) {
        if (board[i] == 'X') {
            xSpots[xIterator] = i + 1;
            xIterator++;
        } else if (board[i] == 'O') {
            oSpots[oIterator] = i + 1;
            oIterator++;
        }
    }
    
    //An iterator to insert elements into the array
    int moveIterator = 1;

    //Determine whether the player is X or O
    for (int i = 0; i < 3; i++) {
        int begin = 0;
        
        if (mul == -1) {
            begin = xSpots[i];
        } else {
            begin = oSpots[i];
        }
        
        if (begin == -1) {
            continue;
        }
        
        moves[moveIterator] = begin;
        moveIterator++;
        
        
        //might have to put a test here to verify x > 3, but if
        //that was the case x would win, so not necessary.
        
        //The space directly in front of the player
        if (board[begin + (mul * 3) - 1] != player && board[begin + (mul * 3) - 1] != oppPlayer) {
            moves[moveIterator] = begin + (mul * 3);
            moveIterator++;
            moves[moveIterator] = begin;
            moveIterator++;
        }
        
        //Checks to see if a player can capture an opponent
        if ((begin == 1 || begin == 4) || begin == 7) {
            if (mul == -1 && board[begin - 2 - 1] == oppPlayer) {
                moves[moveIterator] = begin - 2;
                moveIterator++;
                moves[moveIterator] = begin;
                moveIterator++;
            }
            if (mul == 1 && board[begin + 4 - 1] == oppPlayer) {
                moves[moveIterator] = begin + 4;
                moveIterator++;
                moves[moveIterator] = begin;
                moveIterator++;
            }
        }
        //Checks to see if a player can capture an opponent
        if ((begin == 3 || begin == 6) || begin == 9) {
            if (mul == -1 && board[begin - 4 - 1] == oppPlayer) {
                moves[moveIterator] = begin - 4;
                moveIterator++;
                moves[moveIterator] = begin;
                moveIterator++;
            }
            if (mul == 1 && board[begin + 2 - 1] == oppPlayer) {
                moves[moveIterator] = begin + 2;
                moveIterator++;
                moves[moveIterator] = begin;
                moveIterator++;
            }
        }
        //Checks to see if a player can capture an opponent
        if ((begin == 2 || begin == 5) || begin == 8) {
            if (mul == -1) {
                if (board[begin - 4 - 1] == oppPlayer) {
                    moves[moveIterator] = begin - 4;
                    moveIterator++;
                    moves[moveIterator] = begin;
                    moveIterator++;
                }
                
                if (board[begin - 2 - 1] == oppPlayer) {
                    moves[moveIterator] = begin - 2;
                    moveIterator++;
                    moves[moveIterator] = begin;
                    moveIterator++;
                }
            }
            if (mul == 1) {
                if (board[begin + 4 - 1] == oppPlayer) {
                    moves[moveIterator] = begin + 4;
                    moveIterator++;
                    moves[moveIterator] = begin;
                    moveIterator++;
                }
                
                if (board[begin + 2 - 1] == oppPlayer) {
                    moves[moveIterator] = begin + 2;
                    moveIterator++;
                    moves[moveIterator] = begin;
                    moveIterator++;
                }
            }
        }
        //The moveIterator is always incremented in case there are multiple moves on the same piece.
        //This decrements moveIterator if necessary.
        if (moves[moveIterator - 1] == begin) {
            moves[moveIterator - 1] = -1;
            moveIterator--;
        }
    }
    
    //Set the first value to be the number of moves
    moves[0] = (moveIterator - 1) / 2;
    
    //If the array is empty
    if (moves[1] == -1) {
        delete [] moves;
        return nullptr;
    } else {
        return moves;
    }
}

//Displays the board
void Hexapawn::displayBoard()
{
    for (int i = 0; i < 9; i++)
    {
        cout << "[ " << board[i] << " ]   ";
        if (i == 2 || i == 5)
        {
            cout << endl;
        }
    }
    cout << endl;
}

//Handles the players turn
void Hexapawn::playersTurn() {
    int * moves = nullptr;
    
    //Get the moves
    moves = getAllMoves(-1);
    int begin = 0;
    int end = 0;
    
    //If the mode is to train the computer, get the moves randomly
    if (mode == 0) {
        begin = ((rand() % moves[0]) * 2) + 1;
        board[moves[begin] - 1] = moves[begin] + '0';
        board[moves[begin + 1] - 1] = 'X';
    } else {
        bool valid = false;
        
        //Validates the input
        while (!valid) {
            begin = 0;
            end = 0;
            cout << "Enter the number of the space of the 'X' you would like to move: ";
            cin >> begin;
            cout << "Enter the number of the space you would like to move to: ";
            cin >> end;
            
            for (int i = 1; i < (moves[0] * 2); i += 2) {
                if (moves[i] == begin && moves[i + 1] == end) {
                    valid = true;
                }
            }
        }
        //Set the board with the move
        board[begin - 1] = begin + '0';
        board[end - 1] = 'X';
    }
    delete [] moves;
}


//Translates the code to the board
void Hexapawn::translateCode(long code) {
    int x = 0;
    int space = 0;
    for (int i = 8; i >= 0; i--) {
        space = code % 10;
        code /= 10;
        
        if (space == 1) {
            board[i] = 'X';
        } else if (space == 2) {
            board[i] = 'O';
        } else {
            board[i] = i + 1 + '0';
        }
        x++;
    }
}

//Deletes the tree starting at node
HEXNODE * Hexapawn::deleteTree(HEXNODE * node) {
    // If the node is null
    if (node == nullptr) {
        return nullptr;
    } else { // If not, delete all the child nodes
        if (node->left != nullptr) {
            node->left = deleteTree(node->left);
        } else if (node->midLeft != nullptr) {
            node->midLeft = deleteTree(node->midLeft);
        } else if (node->midRight != nullptr) {
            node->midRight = deleteTree(node->midRight);
        } else if (node->right != nullptr) {
            node->right = deleteTree(node->right);
        }
        delete node;
        return nullptr;
    }
}

//Creates and initializes a new node
HEXNODE * Hexapawn::newNode() {
    HEXNODE * node = new HEXNODE;
    node->left = nullptr;
    node->midLeft = nullptr;
    node->midRight = nullptr;
    node->right = nullptr;
    node->code = -1;
    for (int i = 0; i < 5; i++) {
        node->usedCodes[i] = -1;
    }
    node->usedCodes[0] = 0;
    return node;
}

//Resets the board and the players
void Hexapawn::resetGame() {
    player = 'O';
    oppPlayer = 'X';
    
    board[0] = 'O';
    board[1] = 'O';
    board[2] = 'O';
    board[3] = '4';
    board[4] = '5';
    board[5] = '6';
    board[6] = 'X';
    board[7] = 'X';
    board[8] = 'X';
}
