#include "window.hpp"

GameWindow::GameWindow(unsigned int screenWidth, unsigned int screenHeight, std::string gameTitle, unsigned int frameRate ) : window(sf::VideoMode(screenWidth, screenHeight, sf::Style::Titlebar), gameTitle) {
    window.setFramerateLimit(frameRate); 
}

GameView::GameView(sf::FloatRect viewRect) : view(sf::View(viewRect)){}

