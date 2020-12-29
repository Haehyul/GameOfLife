#include <fstream>
#include <string>
#include <iostream>
using namespace std;

const int number = 480;
const int half = number / 2;

int main()
{
    ofstream File("480x480.txt");
    if (!File.is_open()) {
        cout << "file not opend\n";
        return 0;
    }
    int idx = 0;
    for (int i = 0; i < half; ++i) { 
        string s(number, '0');

        s[idx] = '1';
        s[number - idx - 1] = '1';

        File << s <<'\n';
        
        ++idx;
    }
    idx = half;
    for (int i = 0; i < half; ++i) { 
        --idx;
        string s(number, '0');

        s[idx] = '1';
        s[number - idx - 1] = '1';

        File << s <<'\n';
    }
}