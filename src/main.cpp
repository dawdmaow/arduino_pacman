#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include "game.h"
#include "utils.h"
#include <Adafruit_FT6206.h>

constexpr byte CLK = 13;
constexpr byte DIN = 11;
constexpr byte CS = 10;
constexpr byte DC = 9;

auto render = Adafruit_ILI9341(CS, DC);
auto touch = Adafruit_FT6206();

constexpr uint16_t distance = 8;

int16_t toRenderCoord(byte x)
{
  return x * distance;
}

// void rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color)
// {
//   x = toRenderCoord(x);
//   y = toRenderCoord(y);
//   auto w = distance;
//   auto h = distance;
//   render.fillRect(x, y, w, h, color);
//   yield();
// }

void circle(byte x, byte y, uint16_t color, uint16_t factor)
{
  auto r = distance / factor;
  render.fillCircle(toRenderCoord(x) + distance / 2 - r, toRenderCoord(y) + distance / 2 - r, r, color);
  // yield();
}

constexpr uint16_t fontW = 18;
constexpr uint16_t fontH = 24;

void text(int16_t x, int16_t y, char *s)
{
  render.fillRect(x, y, fontW * 5, fontH, ILI9341_BLACK);
  render.setCursor(x, y);
  render.setTextColor(ILI9341_WHITE);
  render.setTextSize(3);
  render.println(s);
}

void renderFg()
{
  {
    uint16_t x = 100;
    uint16_t y = 300;

    render.fillRect(x, y, fontW * 5, fontH, ILI9341_BLACK);
    render.setCursor(x, y);
    render.setTextColor(ILI9341_WHITE);
    render.setTextSize(3);
    render.println(score);

    x = 80;
    y = 265;

    if (direction == LEFT)
    {
      text(x, y, "LEFT");
    }
    else if (direction == RIGHT)
    {
      text(x, y, "RIGHT");
    }
    else if (direction == UP)
    {
      text(x, y, "UP");
    }
    else if (direction == DOWN)
    {
      text(x, y, "DOWN");
    }
  }

  for (byte x = 0; x < width; ++x)
  {
    for (byte y = 0; y < height; ++y)
    {
      auto entity = getEntity({x, y});
      uint16_t renderX = toRenderCoord(x);
      uint16_t renderY = toRenderCoord(y);

      switch (entity)
      {
      case PLAYER:
        // render.fillCircle(renderX, renderY, distance / 2, ILI9341_YELLOW);
        circle(x, y, ILI9341_YELLOW, 3);
        break;
      case GHOST:
        // render.fillCircle(renderX, renderY, distance / 2, ILI9341_RED);
        circle(x, y, ILI9341_RED, 3);
        break;
      case COIN:
        // r = distance / 5;
        // render.fillCircle(renderX + distance / 2 - r, renderY + distance / 2 - r, r, ILI9341_YELLOW);
        // break;
        render.fillRect(renderX, renderY, distance, distance, ILI9341_BLACK);
        circle(x, y, ILI9341_GREEN, 6);
        break;
      case SUPER_COIN:
        // r = distance / 3;
        // render.fillCircle(renderX + distance / 2, renderY + distance / 2, r, ILI9341_PINK);
        // break;
        circle(x, y, ILI9341_GREEN, 3);
        break;
      case GATE:
        render.fillRect(renderX, renderY, distance - 2, distance - 2, ILI9341_ORANGE);
        break;
      case EMPTY:
        render.fillRect(renderX, renderY, distance, distance, ILI9341_BLACK);
        break;
      }
      yield();
    }
  }
}

void renderBg()
{
  for (byte x = 0; x < width; ++x)
  {
    for (byte y = 0; y < height; ++y)
    {
      auto entity = getEntity({x, y});
      auto renderX = toRenderCoord(x);
      auto renderY = toRenderCoord(y);
      auto r = distance;
      auto w = distance;
      auto h = distance;
      switch (entity)
      {
      case WALL:
        render.fillRect(renderX, renderY, w - 2, h - 2, ILI9341_BLUE);
        break;
        // case COIN:
        //   // r = distance / 5;
        //   // render.fillCircle(renderX + distance / 2 - r, renderY + distance / 2 - r, r, ILI9341_YELLOW);
        //   // break;
        //   circle(x, y, ILI9341_GREEN, 5);
        //   break;
        // case SUPER_COIN:
        //   // r = distance / 3;
        //   // render.fillCircle(renderX + distance / 2, renderY + distance / 2, r, ILI9341_PINK);
        //   // break;
        //   circle(x, y, ILI9341_GREEN, 3);
        //   break;
        // case EMPTY:
        //   render.fillRect(renderX, renderY, w, h, ILI9341_BLACK);
        //   break;
      }
      yield();
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World!");
  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);
  resetGame();

  // if (!touch.begin(40))
  // {
  //   assert(false, 4000);
  // }

  render.begin();

  // render.setCursor(26, 120);
  // render.setTextColor(ILI9341_WHITE);
  // render.setTextSize(3);
  // render.println("Hello World!");
  // yield();
  // render.fillScreen(ILI9341_RED);
  // yield();
  // render.fillScreen(ILI9341_GREEN);
  // yield();
  // render.fillScreen(ILI9341_BLUE);
  // yield();
  // render.fillScreen(ILI9341_BLACK);
  // yield();
  renderBg();
}

constexpr uint16_t displayWidth = 240;
constexpr uint16_t displayHeight = 320;

void loop()
{
  if (touch.touched())
  {
    auto p = touch.getPoint();
    auto factor = 3;

    auto h0 = displayHeight / factor;
    auto h1 = displayHeight - h0;
    auto w0 = displayWidth / factor;
    auto w1 = displayWidth - w0;

    echo("POINT:");
    echo(p.x);
    echo(p.y);
    echo(" ");

    if (p.y < h0)
    {
      nextDirection = DOWN;
      // echo("-> DOWN");
    }
    else if (p.y > h1)
    {
      nextDirection = UP;
      // echo("-> UP");
    }
    else if (p.x < w0)
    {
      nextDirection = RIGHT;
      // echo("-> RIGHT");
    }
    else if (p.x > w1)
    {
      nextDirection = LEFT;
      // echo("-> LEFT");
    }
    else
    {
      // echo("DIDNT CHANGE DIRECTION");
    }

    // echo("NEW DIRECTION:");
    // echo(direction.x);
    // echo(direction.y);
  }

  movePlayer();
  renderFg();
  // delay(200);
  tick();
}