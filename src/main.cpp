#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include "game.h"
#include "utils.h"

constexpr byte CLK = 13;
constexpr byte DIN = 11;
constexpr byte CS = 10;
constexpr byte DC = 9;

auto render = Adafruit_ILI9341(CS, DC);

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

void renderFg()
{
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
        render.fillCircle(renderX, renderY, distance / 2, ILI9341_YELLOW);
        break;
      case GHOST:
        render.fillCircle(renderX, renderY, distance / 2, ILI9341_RED);
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
      case COIN:
        r = distance / 5;
        render.fillCircle(renderX + distance / 2, renderY + distance / 2, r, ILI9341_YELLOW);
        break;
      case SUPER_COIN:
        r = distance / 3;
        render.fillCircle(renderX + distance / 2, renderY + distance / 2, r, ILI9341_PINK);
        break;
      case EMPTY:
        render.fillRect(renderX, renderY, w, h, ILI9341_BLACK);
        break;
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

void loop()
{
  renderFg();
}