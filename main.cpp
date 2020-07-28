//
//  main.cpp
//  LearningHexapawn
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
    
    return 0;
}
