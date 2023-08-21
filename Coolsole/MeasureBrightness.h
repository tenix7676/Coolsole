#pragma once
#include "Random.h"
#include "SDLEssentials.h"
#include <algorithm>
#include <string>
#include <vector>

const int DEFAULT_HDPI{ 2048 };
const int DEFAULT_VDPI{ 1080 };
const SDL_Color WHITE{ SDL_Color(0xff, 0xff, 0xff, 0xff) };
const SDL_Color BLACK{ SDL_Color(0x00, 0x00, 0x00, 0xff) };

double getBrightness(SDL_Renderer* renderer, std::string font_path, char c, int font_size, int hdpi = DEFAULT_HDPI, int vdpi = DEFAULT_VDPI, SDL_Color text_color = WHITE, SDL_Color background_color = BLACK);

struct BrightCharacter
{
    char character;
    double brightness;
};

void getBrightASCIIReal(SDL_Renderer* renderer, std::string font_path, int font_size, std::vector<BrightCharacter>& brightChars);
void getBrightASCIIRelative(SDL_Renderer* renderer, std::string font_path, int font_size, std::vector<BrightCharacter>& brightChars);
BrightCharacter getBrightestUpTo(const std::vector<BrightCharacter>& brightASCII, double max);