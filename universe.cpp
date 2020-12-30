#include "./universe.h"
#include <fstream>
#include <iostream>
#include <string>

int Universe::Initialize()
{
    if (LoadFile() < 0) return -1;

    gWindow = SDL_CreateWindow(
        "Game Of Life",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _width * _mag + 20,
        _height * _mag + 20,
        SDL_WINDOW_SHOWN
    );

    if (!gWindow) return -2;
    gSurface = SDL_GetWindowSurface(gWindow);
    if (!gWindow) return -3;

    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            _rects[y * _width + x].y = y * _mag + 10;
            _rects[y * _width + x].x = x * _mag + 10;
            _rects[y * _width + x].w = _mag;
            _rects[y * _width + x].h = _mag;
        }
    }

    return 1;
}

int Universe::LoadFile()
{
    using std::string;

    std::ifstream File(_filePath.c_str()); 
    if (!File.is_open())
        return -1;

    for (int y = 0; y < _height; ++y)
    {
        string line;
        std::getline(File, line);

        for (int x = 0; x < _width; ++x)
        {
            _grid[y * _width + x] = line[x] == alive ? alive : dead;
        }
    }

    File.close();

    return 1;
}

void Universe::Run()
{
    bool isRunning = true; 
    SDL_Event e;
    
    Draw();

    SDL_UpdateWindowSurface(gWindow);
    SDL_Delay(2000);
    while (isRunning)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                isRunning = false;
        }

        NextGeneration();

        Draw();

        SDL_UpdateWindowSurface(gWindow);

        SDL_Delay((1.0 / 60) * 1000);
    }
}

void Universe::NextGeneration()
{
    std::vector<unsigned char> temp(_grid);
    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            auto count = count_neighbors(y, x); 

            if (cell(y, x) == alive)
            {
                temp[y * _width + x] = 
                    (count == 2 || count == 3) ? alive : dead;
            }
            else
            {
                temp[y * _width + x] = (count == 3) ? alive : dead;
            }
        }
    }

    _grid = temp;
}

void Universe::Draw()
{
    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            if (_grid[y * _width + x] == alive)
                SDL_FillRect(gSurface, &_rects[y * _width + x], SDL_MapRGB(gSurface->format, 0xff, 0xff, 0xff));
            else
                SDL_FillRect(gSurface, &_rects[y * _width + x], SDL_MapRGB(gSurface->format, 0, 0, 0));
        }
    }
}

int Universe::count_neighbors(const size_t row, const size_t col)
{
    if (row == 0 && col == 0) {
        return count_alive(cell(1, 0), cell(1, 1), cell(0, 1));
    }

    if (row == 0 && col == _width - 1) {
        return count_alive(cell(0, _width - 2), cell(1, _width - 2),
                            cell(1, _width - 1));    
    }

    if (row == _height - 1 && col == 0) {
        return count_alive(cell(_height - 2, 0), cell(_height - 2, 1),
                            cell(_height - 1, 1));
    }

    if (row == _height - 1 && col == _width - 1) {
        return count_alive(cell(_height - 2, _width - 1),
                            cell(_height - 2, _width - 2),
                            cell(_height - 1, _width - 2));
    }

    if (row == 0 && col > 0 && col < _width - 1) {
        return count_alive(cell(0, col - 1), cell(1, col - 1),
                            cell(1, col), cell(1, col + 1),
                            cell(0, col + 1));
    }

    if (row == _height - 1 && col > 0 && col < _width - 1) {
        return count_alive(cell(row, col - 1), cell(row - 1, col - 1),
                            cell(row - 1, col), cell(row - 1, col + 1),
                            cell(row, col + 1));
    }

    if (col == 0 && row > 0 && row < _height - 1) {
        return count_alive(cell(row - 1, 0), cell(row - 1, 1),
                            cell(row, 1), cell(row + 1, 1),
                            cell(row + 1, 0));
    }

    if (col == _width - 1 && row > 0 && row < _height -1) {
        return count_alive(cell(row - 1, col), cell(row - 1, col - 1),
                            cell(row, col - 1), cell(row + 1, col - 1),
                            cell(row + 1, col));
    }

    return count_alive(cell(row - 1, col - 1), cell(row - 1, col),
                        cell(row - 1, col + 1), cell(row, col + 1),
                        cell(row + 1, col + 1), cell(row + 1, col),
                        cell(row + 1, col - 1), cell(row, col - 1));
}

void Universe::Close()
{
    SDL_FreeSurface(gSurface);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}