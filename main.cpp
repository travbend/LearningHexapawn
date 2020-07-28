//
//  main.cpp
//  LearningHexapawn2
//
//  Created by Travis  Bender on 7/14/20.
//  Copyright © 2020 Travis Bender. All rights reserved.
//

#include <iostream>
#include "Hexapawn.h"
using namespace std;

int main() {

    //Create an instance
    Hexapawn hexapawn = Hexapawn();
    
    //Play the game
    hexapawn.playGame();
    
    //hexapawn.trainComputer();
    
    //hexapawn.test(10);
    
    return 0;
}

//Have a problem with verifying moves;
