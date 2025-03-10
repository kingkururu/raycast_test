//
//  sprites.cpp
//  sfml game template
//
//

#include "sprites.hpp"

// sprite class constructor; takes in position, scale, texture 
Sprite::Sprite(sf::Vector2f position, sf::Vector2f scale, std::weak_ptr<sf::Texture> texture)
    : position(position), scale(scale), texture(texture), spriteCreated(std::make_unique<sf::Sprite>()), visibleState(true) {
    try {
        if (auto tex = texture.lock()) {  
            sf::Vector2u textureSize = tex->getSize(); 
            if (!textureSize.x || !textureSize.y) {
                throw std::runtime_error("Loaded sprite has size 0");
            }

            spriteCreated->setTexture(*tex); 
            spriteCreated->setPosition(position);
            spriteCreated->setScale(scale);

            log_info("Sprite initialized successfully");

        } else {
            throw std::runtime_error("Texture is no longer available");
        }
    }
    catch (const std::exception& e) {
        log_error(e.what());
        visibleState = false;
    }
}

float Sprite::getRadius() const {
    if (!spriteCreated) {
        log_warning("\tUnable to get sprite's radius because sprite doesn't exist");
        return 0.0f; 
    }

    // Get the size of the sprite
    sf::FloatRect bounds = spriteCreated->getGlobalBounds();
    
    // Calculate the width and height of the bounds
    float width = bounds.width;
    float height = bounds.height;
    
    // Calculate the diagonal length
    float diagonal = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    
    // Radius is half the diagonal
    return diagonal / 2.0f;
}

// sets cut-out rect for sprite animation 
void Animated::setRects(int animNum){
    try {
        if (animNum < 0 || animNum > indexMax) {
            throw std::out_of_range("Animation index out of range.");
        }
        spriteCreated->setTextureRect(animationRects[animNum]);    
    }
    catch (const std::exception& e) {
        log_error("Error in setting texture: " + std::string(e.what()) + " | Index Max: " + std::to_string(indexMax) + " | Current Index: " + std::to_string(animNum));
    }
}

void Animated::changeAnimation() {
    try {
        if (animChangeState) {
            elapsedTime += MetaComponents::deltaTime;
            if (elapsedTime > Constants::ANIMATION_CHANGE_TIME) {
                ++currentIndex;
                if (currentIndex >= indexMax) {
                    currentIndex = 0;
                }
                setRects(currentIndex);
                elapsedTime = 0.0f;
            }
        }
    }
    catch (const std::exception& e) {
        log_error("Error in changing animation: " + std::string(e.what()) + " | Current Index: " + std::to_string(currentIndex));
    }
}

float Animated::getRadius() const {
    if (!spriteCreated) {
        return 0.0f;  
        log_warning("\tUnable to get sprite's radius because sprite doesn't exist"); 
    }
    sf::IntRect rect = getRects();  // Retrieve the sf::IntRect for the sprite
    
    // Calculate the width and height of the rect
    float width = static_cast<float>(rect.width);
    float height = static_cast<float>(rect.height);
    
    // Calculate the diagonal length
    float diagonal = std::sqrt(std::pow(width, 2) + std::pow(height, 2));
    
    // Radius is half the diagonal
    return diagonal / 2.0f;
}

void Sprite::updateVisibility() {
    try {
        if (position.y > Constants::WORLD_HEIGHT + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.x > Constants::WORLD_WIDTH + Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.y < 0 - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET ||
            position.x < 0 - Constants::SPRITE_OUT_OF_BOUNDS_OFFSET) {
            setVisibleState(false);
            log_info("Sprite moved out of bounds and is no longer visible.");
        }
        log_info("Sprite position updated to (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
    }
    catch (const std::exception& e) {
        log_error("Error in updating position: " + std::string(e.what()));
    }
}

sf::IntRect Animated::getRects() const {
    try {
        if (animationRects.empty()) {
            throw std::runtime_error("Animation rects are empty.");
        }
        // log_info("Returning animation rect for index " + std::to_string(currentIndex % animationRects.size()));
        return animationRects[currentIndex % animationRects.size()];
    } 
    catch (const std::exception& e) {
        log_error("Error in getRects: " + std::string(e.what()));
        throw;
    }
}

// returns bitmask for a sprite 
std::shared_ptr<sf::Uint8[]> const Animated::getBitmask(size_t index) const {
    try {
        if (index >= bitMask.size()) {
            throw std::out_of_range("Index out of range.");
        }
        // log_info("Returning bitmask for index " + std::to_string(index));
        return bitMask[index].lock();
    } 
    catch (const std::exception& e) {
        log_error("Error in getBitmask: " + std::string(e.what()) + " | Requested index: " + std::to_string(index));
        throw;
    }
}

// specialized player position update method 
void Player::updatePlayer(sf::Vector2f newPos) {
    changePosition(newPos); 
    updatePos();
    log_info("Player position updated to (" + std::to_string(newPos.x) + ", " + std::to_string(newPos.y) + ")");
}

void Player::changeAnimation() {
    try {
        // Toggle firstTurnInstance based on previous turn
        firstTurnInstance = (prevTurnBool == firstTurnInstance) ? false : true;

        if (animChangeState) {
            elapsedTime += MetaComponents::deltaTime;

            // Change animation only if elapsed time exceeds threshold
            if (elapsedTime > Constants::ANIMATION_CHANGE_TIME) {
                // Update animation index based on 'A' key press
                if (FlagSystem::flagEvents.aPressed) {
                    prevTurnBool = false;
                    currentIndex = 6 + (currentIndex - 6 + 1) % 6; // Range 6 to 11
                } else {
                    prevTurnBool = true;
                    currentIndex = (currentIndex + 1) % 6; // Range 0 to 5
                }

                // Apply new animation frame
                setRects(currentIndex);

                // Reset elapsed time after animation change
                elapsedTime = 0.0f;
            }
        }
    } catch (const std::exception& e) {
        log_error("Error in changing animation: " + std::string(e.what()) + " | Current Index: " + std::to_string(currentIndex));
    }
}

void Player::setHeadingAngle(float headingAngle){
    this->headingAngle = headingAngle;
    float angleRad = headingAngle * (3.14f / 180.f);
    directionVector.x = std::cos(angleRad);
    directionVector.y = std::sin(angleRad);
}

// calculates obstacle's direction vector when bullet is made 
void Obstacle::setDirectionVector(float angle) {
    float angleRad = angle * (3.14f / 180.f);
    directionVector.x = std::cos(angleRad);
    directionVector.y = std::sin(angleRad);
    log_info("Obstacle direction vector set based on angle " + std::to_string(angle));
}

// sets bullet's direction vector 
void Bullet::setDirectionVector(sf::Vector2i projectionPos) {
    directionVector = static_cast<sf::Vector2f>(projectionPos) - position;
    
    // Calculate the length of the direction vector (distance to the target)
    float length = std::sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);

    if (length != 0) {
        directionVector.x /= length;
        directionVector.y /= length;
    }
    log_info("Bullet direction vector calculated.");
}
