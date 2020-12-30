#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <SDL2/SDL.h>
using namespace std;

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

int readFile()
{
    ifstream file(filePath.c_str());
    if (!file.is_open()) 
        return -1;
    for (int y = 0; y < HEIGHT; ++y) {
        char c;
        for (int x = 0; x < WIDTH; ++x) {
            file.get(c);
            // frame[y][x] = c - '0';
            frame[y][x] = c == '1' ? alive : dead;
        }
        file.get(c); // ignore '\n'
    }
    file.close();

    return 1;
}

int initialize()
{
    if (readFile() < 0) return -1;

    gWindow = SDL_CreateWindow(
        "SDL2 Test", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        WIDTH * mag + 50,
        HEIGHT * mag + 50,
        SDL_WINDOW_SHOWN
        );

    if (!gWindow) return -2;
    gSurface = SDL_GetWindowSurface(gWindow);
    if (!gSurface) return -3;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            rects[y][x].y = y * mag + 25;
            rects[y][x].x = x * mag + 25;
            rects[y][x].w = mag;
            rects[y][x].h = mag;
        }
    }

    return 1;
}

void update()
{
    array<array<char, WIDTH>, HEIGHT> temp(frame);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int ct = 0;
            for (int k = 0; k < 8; ++k) {
                int newy = y + dy[k];
                int newx = x + dx[k];
                if (newy >= 0 && newy < HEIGHT && newx >= 0 && newx < WIDTH)
                    if (frame[newy][newx] == alive)
                        ct++;
            }
            if (frame[y][x] == dead)
                if (ct == 3)
                    temp[y][x] = alive;
            if (frame[y][x] == alive)
                if (ct != 2 && ct != 3)
                    temp[y][x] = dead;
        }
    }
    frame = temp; 
}

void Draw()
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (frame[y][x] == alive) {
                SDL_FillRect(gSurface, &rects[y][x], SDL_MapRGB(gSurface->format, 0xff, 0xff, 0xff));
            }
            else
                //SDL_FillRect(gSurface, &rects[y][x], SDL_MapRGB(gSurface->format, 10, 200, 120));
                SDL_FillRect(gSurface, &rects[y][x], SDL_MapRGB(gSurface->format, 0, 0, 0));
        }
    }
}

void Close()
{
    SDL_FreeSurface(gSurface);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    if (initialize() < 0) {
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                quit = true;
        }

        Draw();

        SDL_UpdateWindowSurface(gWindow);
        //SDL_Delay((1.0 / 60) * 1000);
        update();
    }

    return 0;
}