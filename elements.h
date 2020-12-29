#ifndef ELEMENTS
#define ELEMENTS
#include <array>
#include <string>
#include <SDL2/SDL.h>

constexpr int WIDTH = 480;
constexpr int HEIGHT = 480;
constexpr int mag = 1;
const std::string filePath("./480x480.txt");

constexpr char alive = '1';
constexpr char dead = '0';

std::array<std::array<char, WIDTH>, HEIGHT> frame;
std::array<std::array<SDL_Rect, WIDTH>, HEIGHT> rects;

SDL_Window* gWindow = nullptr;
SDL_Surface* gSurface = nullptr;

constexpr int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
constexpr int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

#endif