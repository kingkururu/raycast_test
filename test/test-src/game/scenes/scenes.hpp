//
//  scenes.hpp
//
//

#pragma once

#include <iostream>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <array>

#include "../test-assets/sprites/sprites.hpp"  
#include "../test-assets/sound/sound.hpp"      
#include "../test-assets/fonts/fonts.hpp"      
#include "../test-assets/tiles/tiles.hpp"      

#include "../globals/globals.hpp"              
#include "../physics/physics.hpp"             
#include "../utils/utils.hpp"                 

#include "../test-logging/log.hpp"              

// Base scene class 
class Scene {
 public:
  Scene( sf::RenderWindow& gameWindow );
  virtual ~Scene() = default; 

  // base functions inside scene
  void runScene();  
  virtual void createAssets(){}; 

 protected:
  sf::RenderWindow& window; // from game.hpp
  FlagSystem::SceneEvents sceneEvents; // scene's own flag events

  // blank templates here
  virtual void setInitialTimes(){};
  virtual void insertItemsInQuadtree(){}; 
  virtual void handleInvisibleSprites(){};  

  virtual void setTime(){}; 

  virtual void handleInput(){};

  virtual void respawnAssets(){}; 

  virtual void handleGameEvents(){};
  virtual void handleSceneFlags(){}; 
  virtual void updateDrawablesVisibility(){}; 

  virtual void update(){};
  virtual void draw(); 
  virtual void moveViewPortWASD();

  void restartScene();
  void handleGameFlags(); 

  physics::Quadtree quadtree; 
};

// not in use
class introScene : public virtual Scene{
 public:
   using Scene::Scene; 
  ~introScene() override = default; 
    
 private:

};

// in use (the main scene in test game)
class gamePlayScene : public virtual Scene{
 public:
  using Scene::Scene; 
  ~gamePlayScene() override = default; 
 
  void createAssets() override; 

 private:
  void setInitialTimes() override;
  void insertItemsInQuadtree() override; 

  void handleInput() override; 
  void handleMouseClick(); 
  void handleSpaceKey();
  void handleMovementKeys(); 

  void respawnAssets() override; 
  void handleInvisibleSprites() override;

  void setTime() override;

  void handleGameEvents() override; 
  void handleSceneFlags() override; 

  void update() override; 
  void updateDrawablesVisibility() override; 
  void updatePlayerAndView(); 
  void updateEntityStates(); 
  void changeAnimation();

  void draw() override; 

  std::unique_ptr<Player> player; 

  std::array<std::shared_ptr<Tile>, Constants::TILES_NUMBER> tiles1;   
  std::unique_ptr<TileMap> tileMap1; 
  sf::VertexArray rays;
  
  // std::unique_ptr<MusicClass> backgroundMusic;

  // std::unique_ptr<SoundClass> playerJumpSound; 
  // std::unique_ptr<SoundClass> coinHitSound; 
  // std::unique_ptr<SoundClass> playerDeadSound;
  // std::unique_ptr<SoundClass> buttonClickSound; 

  // std::unique_ptr<TextClass> introText; 
  // std::unique_ptr<TextClass> scoreText; 
  // std::unique_ptr<TextClass> endingText; 

  size_t score {};
};

// not using right now in test game
class gamePlayScene2 : public virtual Scene{
public:
  using Scene::Scene; 
  ~gamePlayScene2() override = default; 
 
  void createAssets() override; 

 private:
  void handleInput() override; 

  void draw() override; 
  void update() override; 

  //std::unique_ptr<Background> background; 
};

