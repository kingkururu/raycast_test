#include "scenes.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Base scene functions  
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

// Scene constructure sets up window and sprite respawn times 
Scene::Scene( sf::RenderWindow& gameWindow ) : window(gameWindow), quadtree(0.0f, 0.0f, Constants::WORLD_WIDTH, Constants::WORLD_HEIGHT){ 
    MetaComponents::smallView = sf::View(Constants::VIEW_RECT); 
    MetaComponents::smallView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));

    MetaComponents::bigView = sf::View(sf::FloatRect(0, 0, Constants::WORLD_WIDTH, Constants::WORLD_HEIGHT)); 
    MetaComponents::bigView.setViewport(sf::FloatRect(0.0f, 0.f, 1.0f, 1.0f)); 

    log_info("scene made"); 
}

void Scene::runScene() {
    if (FlagSystem::flagEvents.gameEnd) return; // Early exit if game ended
    
    setTime();

    handleInput();

    respawnAssets();

    handleGameEvents();
    handleGameFlags();
    handleSceneFlags();

    update();
    draw();
}

void Scene::draw(){
    window.clear(sf::Color::Black);
    window.display(); 
 }

void Scene::moveViewPortWASD(){
    // move view port 
    if(FlagSystem::flagEvents.aPressed){
        MetaComponents::smallView.move(sf::Vector2f(-10, 0)); 
    }
    if(FlagSystem::flagEvents.dPressed){
        MetaComponents::smallView.move(sf::Vector2f(10, 0)); 
    }
    if(FlagSystem::flagEvents.sPressed){
        MetaComponents::smallView.move(sf::Vector2f(0, 10)); 
    }
    if(FlagSystem::flagEvents.wPressed){
        MetaComponents::smallView.move(sf::Vector2f(0, -10)); 
    }
}

// Resets everything for scene to start again. The position, moveState, flagEvents, etc are all reset 
void Scene::restartScene() {
  
    sceneEvents.resetFlags(); 
}

// Handles events from flagEvents
void Scene::handleGameFlags(){
    
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Game Scene #1 down below 
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

// Gets called once before the main game loop 
void gamePlayScene::createAssets() {
    try {
        globalTimer.Reset();  

        // Animated sprites
        player = std::make_unique<Player>(Constants::SPRITE1_POSITION, Constants::SPRITE1_SCALE, Constants::SPRITE1_TEXTURE, Constants::SPRITE1_SPEED, Constants::SPRITE1_ACCELERATION, 
                                          Constants::SPRITE1_ANIMATIONRECTS, Constants::SPRITE1_INDEXMAX, utils::convertToWeakPtrVector(Constants::SPRITE1_BITMASK));
        player->setRects(0); 
        
        frame = std::make_unique<Sprite>(Constants::FRAME_POSITION, Constants::FRAME_SCALE, Constants::FRAME_TEXTURE); 
        backgroundBig = std::make_unique<Sprite>(Constants::BACKGROUNDBIG_POSITION, Constants::BACKGROUNDBIG_SCALE, Constants::BACKGROUNDBIG_TEXTURE); 
         
        bullets.push_back(std::make_unique<Bullet>(Constants::BULLET_STARTINGPOS, Constants::BULLET_STARTINGSCALE, Constants::BULLET_TEXTURE, Constants::BULLET_INITIALSPEED, Constants::BULLET_ACCELERATION, 
                                                   Constants::BULLET_ANIMATIONRECTS, Constants::BULLET_INDEXMAX,  utils::convertToWeakPtrVector(Constants::BULLET_BITMASK)));
        bullets[0]->setRects(0);

        // Tiles and tilemap
        for (int i = 0; i < Constants::TILES_NUMBER; ++i) {
            tiles1.at(i) = std::make_shared<Tile>(Constants::TILES_SCALE, Constants::TILES_TEXTURE, Constants::TILES_SINGLE_RECTS[i], Constants::TILES_BITMASKS[i], Constants::TILES_BOOLS[i]); 
        }
       
        tileMap1 = std::make_unique<TileMap>(tiles1.data(), Constants::TILES_NUMBER, Constants::TILEMAP_WIDTH, Constants::TILEMAP_HEIGHT, Constants::TILE_WIDTH, Constants::TILE_HEIGHT, Constants::TILEMAP_FILEPATH, Constants::TILEMAP_POSITION); 
        rays = sf::VertexArray(sf::Lines, Constants::RAYS_NUM);
        rays = sf::VertexArray(sf::Quads, Constants::RAYS_NUM);
   
        // Music
        backgroundMusic = std::make_unique<MusicClass>(std::move(Constants::BACKGROUNDMUSIC_MUSIC), Constants::BACKGROUNDMUSIC_VOLUME);
        if(backgroundMusic) backgroundMusic->returnMusic().play(); 
        if(backgroundMusic) backgroundMusic->returnMusic().setLoop(Constants::BACKGROUNDMUSIC_LOOP);

        // Text
        introText = std::make_unique<TextClass>(Constants::TEXT_POSITION, Constants::TEXT_SIZE, Constants::TEXT_COLOR, Constants::TEXT_FONT, Constants::TEXT_MESSAGE);
        scoreText = std::make_unique<TextClass>(Constants::SCORETEXT_POSITION, Constants::SCORETEXT_SIZE, Constants::SCORETEXT_COLOR, Constants::TEXT_FONT, Constants::SCORETEXT_MESSAGE);
     
        insertItemsInQuadtree(); 
        setInitialTimes();

        globalTimer.End("initializing assets in scene 1"); // for logging purposes
    } 
    catch (const std::exception& e) {
        log_error("Exception in createAssets: " + std::string(e.what()));
    }
}

void gamePlayScene::setInitialTimes(){

}

void gamePlayScene::insertItemsInQuadtree(){
    quadtree.insert(player);  
    quadtree.insert(bullets[bullets.size() - 1]); 
}

void gamePlayScene::respawnAssets(){
   
} 

void gamePlayScene::handleInvisibleSprites() {
    
}

void gamePlayScene::setTime(){
    // count respawn time here 
    if (FlagSystem::flagEvents.spacePressed || MetaComponents::spacePressedElapsedTime) {
        MetaComponents::spacePressedElapsedTime += MetaComponents::deltaTime; 
    } else {
        MetaComponents::spacePressedElapsedTime = 0.0f; 
    }
    
} 

void gamePlayScene::handleInput() {
    handleMouseClick();
    handleSpaceKey(); 
    handleMovementKeys();
}

void gamePlayScene::handleMouseClick() {    
   
}

void gamePlayScene::handleSpaceKey() {
   
}

void gamePlayScene::handleMovementKeys() {
    if (!player->getMoveState()) return;

    int tileX = static_cast<int>((player->getSpritePos().x - Constants::TILEMAP_POSITION.x) / Constants::TILE_WIDTH);
    int tileY = static_cast<int>((player->getSpritePos().y - Constants::TILEMAP_POSITION.y) / Constants::TILE_HEIGHT);
    int tileIndexInMap = tileY * Constants::TILEMAP_WIDTH + tileX;
    bool canWalkOnTile = tileMap1->getTile(tileIndexInMap)->getWalkable(); 

    sf::FloatRect playerBounds = player->returnSpritesShape().getGlobalBounds();
    sf::Vector2f originalPlayerPos = player->getSpritePos();

    if (FlagSystem::flagEvents.aPressed){ // turn left
        player->returnSpritesShape().rotate(-1.0f); // degrees
        float newAngle = player->returnSpritesShape().getRotation();
        player->setHeadingAngle(newAngle);
    }
    if (FlagSystem::flagEvents.dPressed){ // turn right 
        player->returnSpritesShape().rotate(1.0f); // degrees
        float newAngle = player->returnSpritesShape().getRotation();
        player->setHeadingAngle(newAngle);
    }
    if (FlagSystem::flagEvents.wPressed && canWalkOnTile){ // front 
        physics::spriteMover(player, physics::followDirVec); 
    }
    if (FlagSystem::flagEvents.sPressed && canWalkOnTile){ // back
        physics::spriteMover(player, physics::followDirVecOpposite); 
    }   

    int newTileX = static_cast<int>((player->getSpritePos().x - Constants::TILEMAP_POSITION.x) / Constants::TILE_WIDTH);
    int newTileY = static_cast<int>((player->getSpritePos().y - Constants::TILEMAP_POSITION.y) / Constants::TILE_HEIGHT);
    int newTileIndexInMap = newTileY * Constants::TILEMAP_WIDTH + newTileX;
    bool canWalkOnTileAgain = tileMap1->getTile(newTileIndexInMap)->getWalkable(); 

    if(!canWalkOnTileAgain){
        player->changePosition(originalPlayerPos);
        player->updatePos(); 
    }
    sf::Vector2f playerPos = player->getSpritePos();
    float spriteWidth = playerBounds.width;
    float spriteHeight = playerBounds.height;

    float newX = std::clamp(playerPos.x, spriteWidth, Constants::VIEW_SIZE_X - spriteWidth);
    float newY = std::clamp(playerPos.y, spriteHeight, Constants::VIEW_SIZE_Y - spriteHeight);

    player->changePosition(sf::Vector2f{newX, newY});
    player->updatePos(); 
}

// Keeps sprites inside screen bounds, checks for collisions, update scores, and sets flagEvents.gameEnd to true in an event of collision 
void gamePlayScene::handleGameEvents() { 
    scoreText->getText().setString("Score: " + std::to_string(score));

    physics::calculateRayCast3d(player, tileMap1, rays, wallLine); 
  
} 

void gamePlayScene::handleSceneFlags(){
   
}

void gamePlayScene::update() {
    try {
        updateEntityStates();
        changeAnimation();
        updateDrawablesVisibility(); 
        handleInvisibleSprites();

        updatePlayerAndView(); 
        quadtree.update(); 

        // Set the view for the window
        window.setView(MetaComponents::smallView);
        
    } catch (const std::exception& e) {
        log_error("Exception in updateSprites: " + std::string(e.what()));
    }
}

void gamePlayScene::updateEntityStates(){ 
   
}

void gamePlayScene::changeAnimation(){ 
   for (const auto& bullet : bullets) if (bullet) bullet->changeAnimation();
}

void gamePlayScene::updatePlayerAndView() {

}

void gamePlayScene::updateDrawablesVisibility(){
    try{
      
    }
    catch(const std::exception & e){
        log_error("Exception in updateDrawablesVisibility: " + std::string(e.what()));
    }
}

// Draws only the visible sprite and texts
void gamePlayScene::draw() {
    try {
        window.clear(sf::Color::Black); 

        drawInBigView();
        drawInSmallView();

        window.display(); 
    } 
    catch (const std::exception& e) {
        log_error("Exception in draw: " + std::string(e.what()));
    }
}

void gamePlayScene::drawInBigView(){
    window.setView(MetaComponents::bigView);

    drawVisibleObject(backgroundBig);

    window.draw(wallLine);

    drawVisibleObject(bullets[0]); 
    drawVisibleObject(frame); 
    drawVisibleObject(introText);
}

void gamePlayScene::drawInSmallView(){
    window.setView(MetaComponents::smallView);

    // background for small view
    sf::RectangleShape mainRect(sf::Vector2f(Constants::VIEW_SIZE_X, Constants::VIEW_SIZE_Y));
    mainRect.setFillColor(sf::Color::Black);
    mainRect.setPosition(0,0);

    window.draw(mainRect);

    drawVisibleObject(tileMap1);
    drawVisibleObject(player);

    window.draw(rays); 
}
