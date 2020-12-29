#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
using namespace std;

constexpr int WIDTH = 100;
constexpr int HEIGHT = 100;
constexpr int mag = 5;
const string filePath("./100x100.txt");

array<array<int, WIDTH>, HEIGHT> frame;
array<array<SDL_Rect, WIDTH>, HEIGHT> rects;

void initialize()
{
    ifstream file(filePath.c_str());
    for (int y = 0; y < HEIGHT; ++y) {
        char c;
        for (int x = 0; x < WIDTH; ++x) {
            file.get(c);
            frame[y][x] = c - '0';
        }
        file.get(c); // ignore '\n'
    }
    file.close();
}

SDL_Window* gWindow = nullptr;
SDL_Surface* gSurface = nullptr;

constexpr int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
constexpr int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

void update()
{
    std::cout << "\n\nUPDATE" << endl;
    array<array<int, WIDTH>, HEIGHT> temp(frame);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int ct = 0;
            for (int k = 0; k < 8; ++k) {
                int newy = y + dy[k];
                int newx = x + dx[k];
                if (newy >= 0 && newy < HEIGHT && newx >= 0 && newx < WIDTH)
                    if (frame[newy][newx] == 1)
                        ct++;
            }
            if (frame[y][x] == 0)
                if (ct == 3)
                    temp[y][x] = 1;
            if (frame[y][x] == 1)
                if (ct != 2 && ct != 3)
                    temp[y][x] = 0;
        }
    }
    frame = temp; 
}

int main(int argc, char *argv[])
{
    initialize();

    gWindow = SDL_CreateWindow(
        "SDL2 Test", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        WIDTH * mag,
        HEIGHT * mag,
        SDL_WINDOW_SHOWN
        );

    if (!gWindow) return -1;
    gSurface = SDL_GetWindowSurface(gWindow);
    if (!gSurface) return -2;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            rects[y][x].y = y * mag;
            rects[y][x].x = x * mag;
            rects[y][x].w = mag;
            rects[y][x].h = mag;
        }
    }

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                quit = true;
        }

        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (frame[y][x] == 1) {
                    SDL_FillRect(gSurface, &rects[y][x], SDL_MapRGB(gSurface->format, 180, 10, 140));
                }
                else
                    SDL_FillRect(gSurface, &rects[y][x], SDL_MapRGB(gSurface->format, 10, 200, 120));
            }
        }

        SDL_UpdateWindowSurface(gWindow);
        SDL_Delay((1.0 / 10) * 1000);
        update();
    }

    SDL_FreeSurface(gSurface);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}