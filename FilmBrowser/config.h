#pragma once
#include <chrono>
#include <thread>

#define ASSET_PATH "assets\\"
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define CANVAS_WIDTH 1000
#define CANVAS_HEIGHT 500





inline void sleep(int ms)// sleep function for delay when pressing buttons
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline std::string scancodes(int sc) {// a fucntion that return the character based on the arythmetic value given
    switch (sc ) {
    case 4:
        return "a";
        break;
    case 5:
        return "b";
        break;
    case 6:
        return "c";
        break;
    case 7:
        return "d";
        break;
    case 8:
        return "e";
        break;
    case 9:
        return "f";
        break;
    case 10:
        return "g";
        break;
    case 11:
        return "h";
        break;
    case 12:
        return "i";
        break;
    case 13:
        return "j";
        break;
    case 14:
        return "k";
        break;
    case 15:
        return "l";
        break;
    case 16:
        return "m";
        break;
    case 17:
        return "n";
        break;
    case 18:
        return "o";
        break;
    case 19:
        return "p";
        break;
    case 20:
        return "q";
        break;
    case 21:
        return "r";
        break;
    case 22:
        return "s";
        break;
    case 23:
        return "t";
        break;
    case 24:
        return "u";
        break;
    case 25:
        return "v";
        break;
    case 26:
        return "w";
        break;
    case 27:
        return "x";
        break;
    case 28:
        return "y";
        break;
    case 29:
        return "z";
        break;
    case 30:
        return "1";
        break;
    case 31:
        return "2";
        break;
    case 32:
        return "3";
        break;
    case 33:
        return "4";
        break;
    case 34:
        return "5";
        break;
    case 35:
        return "6";
        break;
    case 36:
        return "7";
        break;
    case 37:
        return "8";
        break;
    case 38:
        return "9";
        break;
    case 39:
        return "0";
        break;
    case 42:
        return "backspace";
        break;
    case 44:
        return " ";
        break;
    }
}



