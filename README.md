# Raycasting Demo Piece

A simulated 3D graphics demo built with C++ and SFML, showcasing raycasting techniques for creating pseudo-3D environments from 2D map data. This demo explores the fundamental graphics rendering approach used in classic games like Doom and Wolfenstein 3D.

<img width="585" height="431" alt="Image" src="https://github.com/user-attachments/assets/f9b11a2f-ff7e-4837-8a57-85018e1382d5" />

## Features

- **Raycasting Engine**: Renders 3D-looking environments from 2D tile maps
- **Player Movement**: W and S keys for forward/backward
- **Camera Rotation**: A and D keys for left/right view rotation
- **Distance-Based Rendering**: Walls appear taller when closer, shorter when farther
- **Fisheye Correction**: Geometric accuracy using trigonometric functions
- **Collision Detection**: Player restricted to walkable black tiles
- **Mini-Map Visualization**: Real-time representation of player position and rays
- **Performance Optimized**: Efficient ray casting with boundary collision

## Technical Implementation

### Core Raycasting Algorithm
The raycasting system casts rays at regular intervals across the player's field of view to calculate distances to wall surfaces. Each ray's intersection with walls determines the height of vertical slices rendered on screen, creating a 3D perspective effect.

### Key Components
- **Ray Casting**: Measures distances to walls using trigonometric calculations
- **Vertical Slice Rendering**: Draws walls as rectangles with heights based on distance
- **Fisheye Correction**: Corrects distortion using cosine of angle between ray and viewing direction
- **Collision System**: Integrated with tile-based map for movement restrictions
- **Map Visualization**: Mini-map window showing real-time ray intersections

### Graphics Rendering
- **Distance-Based Shading**: Walls become darker with distance for depth illusion
- **Geometric Accuracy**: Trigonometric functions ensure proper perspective
- **Boundary Detection**: Rays stop at wall intersections for accurate measurements
- **View Frustum**: Field of view calculations for realistic 3D projection
  
## Controls

- **W**: Move forward
- **S**: Move backward  
- **A**: Rotate camera left / turn player left
- **D**: Rotate camera right / turn player right
  
## Project Structure

```
/raycast_test
│
├── .vscode/                  # VSCode settings and configurations
│
├── src/                      # Source files
│   ├── main.cpp              # Main entry point of the game
│   └── game/                 # Core engine functionalities
│       ├── globals/          # Constants and flags
│       ├── core/             # Game loop and state management
│       ├── physics/          # Physics and collision detection
│       ├── camera/           # Window and view management
│       ├── utils/            # Utility functions
│       └── scenes/           # Scene management
│
├── assets/                   # Game assets
│   ├── fonts/                # Text files and sources
│   ├── sound/                # Sound effects
│   ├── tiles/                # Tiles and tilemaps
│   └── sprites/              # Sprite images
│
├── libs/                      # External libraries
│   └── logging/               # Logging system
│
├── Makefile                   # Build instructions
└── README.md                  # Project documentation
```
### Prerequisites
- **Compiler**: Requires clang++ (or g++) with C++17 support
- **SFML**: Simple and Fast Multimedia Library for graphics and windowing
- **Custom Game Framework**: Built on top of the SFML Game Framework

## Building & Running

### Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/kingkururu/raycast_test
   cd raycast_test (location will be different)
   ```

2. **Build the Project**:
   ```bash
   make
   ```
   This will compile the raycasting demo along with the game framework.

3. **Run the Demo**:
   ```bash
   make run
   ```

4. **Clean the Build**:
   ```bash
   make clean
   ```
### Alternative Setup (macOS with Homebrew)

1. **Install SFML**:
   ```bash
   # Install Homebrew if not already installed
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   
   # Install SFML
   brew install sfml@2
   brew install .... (other missing libraries)
   ```

2. **Configure PATH** (add to ~/.zshrc or ~/.bash_profile):
   ```bash
   export PATH="/opt/homebrew/bin:$PATH"
   export PATH="/opt/homebrew/opt/sfml@2/bin:$PATH"
   export PATH="path to other libraries if any of them is missing)
   ```

## Key Learning Outcomes

- **Raycasting Implementation**: Understanding 3D graphics simulation from 2D data
- **Trigonometric Applications**: Using sine, cosine for geometric calculations
- **Graphics Optimization**: Efficient rendering techniques for real-time performance
- **Collision Detection**: Tile-based collision system integration
- **Perspective Projection**: Creating depth illusion through distance-based rendering
- **Game Loop Integration**: Smooth real-time rendering within game framework
  
### Raycasting Algorithm
The demo implements a raycasting system that:
1. Casts rays at regular intervals across the player's field of view
2. Calculates intersection points with wall tiles
3. Measures distances to determine wall heights
4. Applies fisheye correction for geometric accuracy
5. Renders vertical slices with distance-based shading

### Performance Optimizations
- **Boundary Optimization**: Rays terminate at first wall intersection
- **Trigonometric Efficiency**: Optimized angle calculations
- **Memory Management**: Efficient tile map data structures
- **Render Culling**: Only visible portions are rendered

## Advanced Features

- **Real-time Ray Visualization**: Mini-map shows active rays during casting
- **Distance-Based Shading**: Walls darken with distance for depth perception
- **Smooth Movement**: Collision detection allows fluid player movement
- **Geometric Accuracy**: Proper trigonometric calculations for realistic perspective
- **Optimized Rendering**: Efficient algorithms for real-time performance

## 🎨 Assets Credits

- **Music**: [Best Game Console](https://pixabay.com/music/video-games-best-game-console-301284/) from Pixabay
- **Graphics**: Custom artwork created using Canva and Canva AI
- **Framework**: Built using the Custom SFML Game Framework https://github.com/kingkururu/sfml_game_template/edit/main/README.md
