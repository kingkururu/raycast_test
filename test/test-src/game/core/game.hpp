//
//  game.hpp
//  sfml game template 
//


#pragma once

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "../scenes/scenes.hpp"

class GameManager {
public:
    GameManager();
    void loadScenes(); 
    void runGame();
    void runScenesFlags();
    void resetFlags(); 
    
private:
    void countTime(); // countTime counts time regardless of the scene 
    void handleEventInput(); // handleEventInput taks input from device, such as keyboard, mouse, etc 

    GameWindow mainWindow;

    std::unique_ptr<gamePlayScene> gameScene;
};

