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
    // re-play background music

    // set sprite movestates to true
   
    // re-set sprite and text positions 

    // clear respawn time vectors or any other unecessary vectors 

    // re-set flagEvents
    sceneEvents.resetFlags(); 
}

// Handles events from flagEvents
void Scene::handleGameFlags(){
    // if flagEvents.gameEnd is true or some event ... do somthing 
    if(FlagSystem::flagEvents.gameEnd){
       
    }
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

        // button1 = std::make_unique<Button>(Constants::BUTTON1_POSITION, Constants::BUTTON1_SCALE, Constants::BUTTON1_TEXTURE, 
        //                            Constants::BUTTON1_ANIMATIONRECTS, Constants::BUTTON1_INDEXMAX, utils::convertToWeakPtrVector(Constants::BUTTON1_BITMASK));
        // button1->setRects(0); 
        // button1->setVisibleState(false); 

        // Tiles and tilemap
        for (int i = 0; i < Constants::TILES_NUMBER; ++i) {
            tiles1.at(i) = std::make_shared<Tile>(Constants::TILES_SCALE, Constants::TILES_TEXTURE, Constants::TILES_SINGLE_RECTS[i], Constants::TILES_BITMASKS[i], Constants::TILES_BOOLS[i]); 
        }
        for (auto tile : Constants::TILES_BOOLS){
            std::cout << tile << "\n"; 
        }
        tileMap1 = std::make_unique<TileMap>(tiles1.data(), Constants::TILES_NUMBER, Constants::TILEMAP_WIDTH, Constants::TILEMAP_HEIGHT, Constants::TILE_WIDTH, Constants::TILE_HEIGHT, Constants::TILEMAP_FILEPATH, Constants::TILEMAP_POSITION); 
        rays = sf::VertexArray(sf::Lines, Constants::FOV * 2);

        // // Music
        // backgroundMusic = std::make_unique<MusicClass>(std::move(Constants::BACKGROUNDMUSIC_MUSIC), Constants::BACKGROUNDMUSIC_VOLUME);
        // if(backgroundMusic) backgroundMusic->returnMusic().play(); 
        // if(backgroundMusic) backgroundMusic->returnMusic().setLoop(Constants::BACKGROUNDMUSIC_LOOP);

        // // Sound
        // playerJumpSound = std::make_unique<SoundClass>(Constants::PLAYERJUMP_SOUNDBUFF, Constants::PLAYERJUMPSOUND_VOLUME); 
        // coinHitSound = std::make_unique<SoundClass>(Constants::COINHIT_SOUNDBUFF, Constants::COINHITSOUND_VOLUME); 
        // buttonClickSound = std::make_unique<SoundClass>(Constants::BUTTONCLICK_SOUNDBUFF, Constants::BUTTONCLICKSOUND_VOLUME);

        // // Text
        // introText = std::make_unique<TextClass>(Constants::TEXT_POSITION, Constants::TEXT_SIZE, Constants::TEXT_COLOR, Constants::TEXT_FONT, Constants::TEXT_MESSAGE);
        // scoreText = std::make_unique<TextClass>(Constants::SCORETEXT_POSITION, Constants::SCORETEXT_SIZE, Constants::SCORETEXT_COLOR, Constants::TEXT_FONT, Constants::SCORETEXT_MESSAGE);
        // endingText = std::make_unique<TextClass>(Constants::ENDINGTEXT_POSITION, Constants::ENDINGTEXT_SIZE, Constants::ENDINGTEXT_COLOR, Constants::TEXT_FONT, Constants::ENDINGTEXT_MESSAGE);
        // endingText->setVisibleState(false);

        // insertItemsInQuadtree(); 
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
    if (FlagSystem::flagEvents.mouseClicked) {
       
    }
}

void gamePlayScene::handleSpaceKey() {
    if (MetaComponents::spacePressedElapsedTime) {
       
    }
}

void gamePlayScene::handleMovementKeys() {
    if (!player->getMoveState()) return;

    int tileX = static_cast<int>((player->getSpritePos().x - Constants::TILEMAP_POSITION.x) / Constants::TILE_WIDTH);
    int tileY = static_cast<int>((player->getSpritePos().y - Constants::TILEMAP_POSITION.y) / Constants::TILE_HEIGHT);
    int tileIndexInMap = tileY * Constants::TILEMAP_WIDTH + tileX;
    bool canWalkOnTile = tileMap1->getTile(tileIndexInMap)->getWalkable(); 

    sf::FloatRect playerBounds = player->returnSpritesShape().getGlobalBounds();
    sf::Vector2f originalPlayerPos = player->getSpritePos();

    // std::cout << "Tile X: " << tileX << ", Tile Y: " << tileY << ", inex: " << tileIndexInMap << "can walk: "<< canWalkOnTile<< std::endl;
    
    if (FlagSystem::flagEvents.wPressed && canWalkOnTile){ // front 
        physics::spriteMover(player, physics::followDirVec); 
    }
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
    // scoreText->getText().setPosition(MetaComponents::smallView.getCenter().x - 460, MetaComponents::smallView.getCenter().y - 270);
    // scoreText->getText().setString("Score: " + std::to_string(score));

    physics::calculateRayCast3d(player, tileMap1, rays); // modifies the ray 
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

void gamePlayScene::updateEntityStates(){ // manually change the sprite's state
   
}

void gamePlayScene::changeAnimation(){ // change animation for sprites. change animation for texts if necessary     if (button1 && button1->getVisibleState()) button1->changeAnimation(); 
   
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
        window.clear(sf::Color::Black); // set the base baskground color blue

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

    // temporary 
    sf::RectangleShape mainRect(sf::Vector2f(Constants::WORLD_WIDTH, Constants::WORLD_HEIGHT));
    mainRect.setFillColor(sf::Color::Black);
    mainRect.setPosition(0,0);

    sf::RectangleShape rect(sf::Vector2f(50,50));
    rect.setFillColor(sf::Color::Green);
    rect.setPosition(0,0);

    window.draw(mainRect);
    window.draw(rect);

    drawRays3D();
}

void gamePlayScene::drawInSmallView(){
    window.setView(MetaComponents::smallView);

    drawVisibleObject(tileMap1);
    drawVisibleObject(player);

    window.draw(rays); 
}

void gamePlayScene::drawRays3D() {
    // Ensure the vertex array is properly configured for line primitives
    rays.setPrimitiveType(sf::Lines);
    rays.resize(2 * Constants::FOV);  // Two vertices per ray (start and end)

    float screenWidth = static_cast<float>(MetaComponents::bigView.getSize().x);
    float screenHeight = static_cast<float>(MetaComponents::bigView.getSize().y);
    float centerY = screenHeight / 2.0f;

    // Constant to scale wall heights (adjust for better visuals)
    const float wallHeightScale = 1000.0f;

    // Draw vertical lines resembling 3D walls for each ray
    for (size_t i = 0; i < Constants::FOV; ++i) {
        sf::Vertex& startVertex = rays[2 * i];
        sf::Vertex& endVertex = rays[2 * i + 1];

        // Compute the ray distance
        float dx = endVertex.position.x - startVertex.position.x;
        float dy = endVertex.position.y - startVertex.position.y;
        float rayDistance = std::sqrt(dx * dx + dy * dy);

        // Avoid division by zero and small distances
        if (rayDistance < 1.0f) rayDistance = 1.0f;

        // Calculate the height of the wall to be drawn
        float wallHeight = wallHeightScale / rayDistance;

        // Compute the x position for this wall slice on the screen
        float screenX = (i / static_cast<float>(Constants::FOV)) * screenWidth;

        // Define the top and bottom y-coordinates for the wall segment
        float wallTopY = centerY - wallHeight / 2.0f;
        float wallBottomY = centerY + wallHeight / 2.0f;

        // Draw a vertical line representing the wall
        sf::Vertex wallLine[] = {
            sf::Vertex(sf::Vector2f(screenX, wallTopY), sf::Color::Red),
            sf::Vertex(sf::Vector2f(screenX, wallBottomY), sf::Color::Red)
        };

        // Render the wall slice
        window.draw(wallLine, 2, sf::Lines);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// Game Scene #2 from down below 
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

void gamePlayScene2::createAssets() {
 try {
       
    } 

    catch (const std::exception& e) {
        log_error("Exception in createAssets: " + std::string(e.what()));
    }
}

void gamePlayScene2::handleInput() {
    moveViewPortWASD(); // change position of the view port based on keyboard input flags
}

void gamePlayScene2::draw() {
    try {
        window.clear(); // clear elements from previous screen 
        
        window.display(); 
    } 
    
    catch (const std::exception& e) {
        log_error("Exception in gamePlayScene2 draw: " + std::string(e.what()));
    }
}

void gamePlayScene2::update() {
    try {
        handleInvisibleSprites(); // do a sprite pooling or actually delete all

        window.setView(MetaComponents::smallView); 
        
    }
    catch (const std::exception& e) {
        log_error("Exception in updateSprites: " + std::string(e.what()));
    }
}
