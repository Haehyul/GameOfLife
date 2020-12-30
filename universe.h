#ifndef UNIVERSE
#define UNIVERSE
#include <vector>
#include <string>
#include <SDL2/SDL.h>

class Universe
{
    Universe() = delete;
public:
    Universe(const size_t width, const size_t height, const size_t mag, const char* filePath):
        _width(width), _height(height), _mag(mag), _filePath(filePath), 
        _grid(width * height), _rects(width * height), gWindow(nullptr), gSurface(nullptr)
        { 
            Initialize();
        }

    ~Universe() { Close(); }

    // Run program
    void Run();

private:
    // Inits everything
    int Initialize();

    // Read file
    int LoadFile();

    // Creates next generation
    void NextGeneration();

    // Draw to window
    void Draw();

    // Close SDL 
    void Close();

    int count_alive() { return 0; }

    template <typename T1, typename... T>
    auto count_alive(T1 s, T... ts) { return (s - '0') + count_alive(ts...); }

    unsigned char& cell(const size_t row, const size_t col)
    {
        return _grid[row * _width + col];
    }

    int count_neighbors(const size_t row, const size_t col);
private:
    const size_t _width;
    const size_t _height;
    const size_t _mag;
    const std::string _filePath;

    SDL_Window *gWindow;
    SDL_Surface *gSurface;

    std::vector<unsigned char> _grid;
    std::vector<SDL_Rect> _rects;

    static const unsigned char alive = '1';
    static const unsigned char dead = '0';
};

#endif