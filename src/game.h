#include <Arduino.h>
#include <avr/pgmspace.h>
#include "utils.h"

/*
def handle_pacman_death(self):
        self.lives -= 1
        if self.lives > 0:
            self.reset_positions()
        else:
            self.game_over()

    def reset_positions(self):
        self.pacman_pos = (14, 23)
        self.ghost_positions = [(14, 11), (13, 14), (14, 14), (15, 14)]
        self.ghost_states = [False] * 4

    def reset_ghost(self, ghost_index: int):
        self.ghost_positions[ghost_index] = (14, 14)  # Return to ghost house
        self.ghost_states[ghost_index] = False

    def make_ghosts_vulnerable(self):
        self.ghost_states = [True] * 4

    def is_valid_position(self, x: int, y: int) -> bool:
        return 0 <= x < self.width and 0 <= y < self.height

    def game_over(self) -> bool:
        return self.lives <= 0 or not any(
            self.board[y][x] in (EntityType.DOT, EntityType.POWER_PELLET)
            for y in range(self.height)
            for x in range(self.width)
        )

    def get_score(self) -> int:
        return self.score

    def get_lives(self) -> int:
        return self.lives
*/

constexpr byte width = 28;
constexpr byte height = 31;

struct Vec
{
    int8_t x;
    int8_t y;

    int index()
    {
        assert(x >= 0 && x < width);
        assert(y >= 0 && y < height);
        return y * width + x;
    }

    Vec operator+(Vec other)
    {
        return {x + other.x, y + other.y};
    }

    bool operator==(Vec other)
    {
        return x == other.x && y == other.y;
    }
};

struct Grid
{
    u16 grid[55]; // Enough for 868 (28*31) bits.

    Grid()
    {
        for (int i = 0; i < 55; ++i)
        {
            grid[i] = 0;
        }
    }

    void enable(Vec pos)
    {
        int index = pos.index();
        int wordIndex = index / 16;
        int bitIndex = index % 16;
        grid[wordIndex] |= (1 << bitIndex);
    }

    void disable(Vec pos)
    {
        int index = pos.index();
        int wordIndex = index / 16;
        int bitIndex = index % 16;
        grid[wordIndex] &= ~(1 << bitIndex);
    }

    bool occupied(Vec pos)
    {
        int index = pos.index();
        int wordIndex = index / 16;
        int bitIndex = index % 16;
        return (grid[wordIndex] & (1 << bitIndex)) != 0;
    }
};

PROGMEM const char layout[] = "#############################............##............##.####.#####.##.#####.####.##^####.#####.##.#####.####^##.####.#####.##.#####.####.##..........................##.####.##.########.##.####.##.####.##.########.##.####.##......##....##....##......#######.##### ## #####.############.##### ## #####.############.##          ##.############.## ###--### ##.############.## #ssssss# ##.######T     .   #ssssss#   .     t######.## #ssssss# ##.############.## ######## ##.############.##          ##.############.## ######## ##.############.## ######## ##.#######............##............##.####.#####.##.#####.####.##.####.#####.##.#####.####.##^..##.......P........##..^####.##.##.########.##.##.######.##.##.########.##.##.####......##....##....##......##.##########.##.##########.##.##########.##.##########.##..........................#############################";

enum Entity
{
    WALL,
    EMPTY,
    COIN,
    SUPER_COIN,
    GHOST,
    GATE,
    TELEPORT_A,
    TELEPORT_B,
    PLAYER
};

Entity entityFromChar(char c)
{
    switch (c)
    {
    case '#':
        return WALL;
    case ' ':
        return EMPTY;
    case '.':
        return COIN;
    case '^':
        return SUPER_COIN;
    case '-':
        return GATE;
    }
    assert(false);
}

struct Ghost
{
    Vec pos;
    bool dead;
};

enum Direction
{
    Up,
    Down,
    Left,
    Right,
};

constexpr Vec LEFT = {-1, 0};
constexpr Vec RIGHT = {1, 0};
constexpr Vec UP = {0, -1};
constexpr Vec DOWN = {0, 1};

uint16_t score;
byte lives;
Vec playerPos;
Grid collectedCoins;
Ghost ghosts[4];
Vec direction;

void resetGame()
{
    score = 0;
    lives = 3;
    playerPos = {14, 23};
    collectedCoins = Grid();
    ghosts[0] = {{14, 11}, false};
    ghosts[1] = {{14, 12}, false};
    ghosts[2] = {{14, 13}, false};
    ghosts[3] = {{14, 14}, false};
    direction = LEFT;
}

Entity getEntity(Vec pos)
{
    for (byte i = 0; i < 4; ++i)
    {
        if (ghosts[i].pos == pos)
        {
            return GHOST;
        }
    }

    if (pos == playerPos)
    {
        return PLAYER;
    }

    auto entity = entityFromChar(pgm_read_byte(layout + pos.index()));

    if (entity == COIN || entity == SUPER_COIN)
    {
        if (collectedCoins.occupied(pos))
        {
            return EMPTY;
        }
    }

    return entity;
}

bool isValidPos(Vec pos)
{
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

void resetPositions()
{
    playerPos = {14, 23};
    ghosts[0].pos = {14, 11};
    ghosts[1].pos = {13, 14};
    ghosts[2].pos = {14, 14};
    ghosts[3].pos = {15, 14};
}

void resetGhost(Ghost *ghost)
{
    ghost->pos = {14, 14};
    ghost->dead = false;
}

bool isGameOver()
{
    return lives <= 0; // TODO: Check if all coins are collected.
}

void playerDeath()
{
    lives--;
    if (lives > 0)
    {
        resetPositions();
    }
    // else
    // {
    //     gameOver();
    // }
}

void makeGhostsVulnerable()
{
    for (byte i = 0; i < 4; ++i)
    {
        ghosts[i].dead = true;
    }
}

bool movePlayer()
{
    auto newPos = playerPos + direction;
    if (!isValidPos(newPos))
    {
        return false;
    }
    auto entity = getEntity(newPos);
    if (entity == WALL)
    {
        return false;
    }
    if (entity == COIN)
    {
        score++;
        collectedCoins.enable(newPos);
    }
    if (entity == SUPER_COIN)
    {
        score += 5;
        makeGhostsVulnerable();
        collectedCoins.enable(newPos);
    }
    playerPos = newPos;
    return true;
}

void checkGhostCollision()
{
    for (byte i = 0; i < 4; ++i)
    {
        if (ghosts[i].pos == playerPos)
        {
            if (ghosts[i].dead)
            {
                score += 200;
                resetGhost(&ghosts[i]);
            }
            else
            {
                playerDeath();
            }
        }
    }
}

bool moveGhost(Ghost &ghost, Vec dir)
{
    auto newPos = ghost.pos + dir;
    if (!isValidPos(newPos))
    {
        return false;
    }
    if (getEntity(newPos) == WALL)
    {
        return false;
    }
    ghost.pos = newPos;
    checkGhostCollision();
    return true;
}