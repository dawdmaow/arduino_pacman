A Pacman clone made with C++, PlatformIO and Wokwi for Arduino Uno.

# Description

This project is a functional Pacman game running on Arduino Uno with an ILI9341 TFT display and capacitive touch interface. The game features gameplay with ghosts, coins, power pellets, and maze navigation.

# Features

- **Classic Pacman Gameplay**: Navigate through a maze, collect coins, and avoid ghosts
- **Touch Controls**: Use the capacitive touchscreen for directional control
- **Real-time Graphics**: Rendering using the Adafruit GFX library
- **Score System**: Track your performance as you collect coins
- **Ghost AI**: Enemies that chase the player through the maze
- **Custom Level Layout**: Configurable maze design stored in level files

# Requirements

- **PlatformIO**

# Setup
 ```bash
 pio run --target upload
 ```

# Using Wokwi Simulator
VSCode extension allows starting a visual simulation in a separate tab.
