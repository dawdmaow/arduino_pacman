A Pacman clone made with C++, PlatformIO and Wokwi for Arduino Uno.

# 🎮 Description

This project is a functional Pacman game running on Arduino Uno with an ILI9341 TFT display and capacitive touch interface. The game features gameplay with ghosts, coins, power pellets, and maze navigation.

# ✨ Features

- **Classic Pacman Gameplay**: Navigate through a maze, collect coins, and avoid ghosts
- **Touch Controls**: Use the capacitive touchscreen for directional control
- **Real-time Graphics**: Rendering using the Adafruit GFX library
- **Score System**: Track your performance as you collect coins
- **Ghost AI**: Enemies that chase the player through the maze
- **Custom Level Layout**: Configurable maze design stored in level files

# 🔧 Hardware Requirements

## Components
- **Arduino Uno** (or compatible board)
- **ILI9341 TFT Display** (240x320 pixels) with capacitive touch
- **Connection Wires**

## Wiring Diagram
| Arduino Pin | Display Pin | Function          |
| ----------- | ----------- | ----------------- |
| 5V          | VCC         | Power             |
| GND         | GND         | Ground            |
| Pin 10      | CS          | Chip Select       |
| Pin 9       | D/C         | Data/Command      |
| Pin 11      | MOSI        | SPI Data          |
| Pin 13      | SCK         | SPI Clock         |
| A4          | SDA         | I2C Data (Touch)  |
| A5          | SCL         | I2C Clock (Touch) |

# 💻 Software Requirements

- **PlatformIO** (recommended) or Arduino IDE
- **Libraries** (automatically installed with PlatformIO):
  - Adafruit GFX Library (v1.11.11+)
  - Adafruit ILI9341 (v1.6.1+)
  - Adafruit FT6206 Library (v1.1.0+)
  - GFX Library for Arduino (v1.5.2+)

# 🚀 Setup
 ```bash
 pio run --target upload
 ```

# Using Wokwi Simulator
VSCode extension allows starting a visual simulation in a separate tab.

# 🛠️ Development

## Game Architecture

- **main.cpp**: Contains the Arduino setup() and loop() functions, handles display initialization and main game loop
- **game.h**: Implements game state, collision detection, ghost AI, and scoring system
- **utils.h**: Provides helper functions 

## Rendering System

The game uses a coordinate transformation system where:
- Game logic operates on an 8-pixel grid system
- Graphics are rendered using the Adafruit GFX library
- Different entities (player, ghosts, coins, walls) are rendered with distinct colors and shapes

## Performance Optimizations

- Efficient memory usage for Arduino's limited RAM

# 🎮 Game Entities

- **🟡 Player (Pacman)**: Yellow circle controlled by touch input
- **🔴 Ghosts**: Red circles that chase the player
- **🟢 Coins**: Small green circles to collect for points
- **🟢 Super Coins**: Large green circles with special properties
- **🔵 Walls**: Blue rectangles forming the maze structure
- **🟠 Gates**: Orange rectangles for special maze areas