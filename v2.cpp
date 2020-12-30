#include "./universe.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 5) {
        // v2.exe 480 480 1 "./480x480.txt"
        std::cout << "<width><height><magnification><filePath>\n";
        return 0;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int magnification = atoi(argv[3]);
    const char *filePath = argv[4];

    Universe universe(width, height, magnification, filePath);

    universe.Run();

    return 0;
}
