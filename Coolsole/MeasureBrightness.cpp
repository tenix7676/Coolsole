#include "MeasureBrightness.h"

SDL_Color getPixelColor(const SDL_Surface* pSurface, const int X, const int Y)
{
    // Bytes per pixel
    const Uint8 Bpp = pSurface->format->BytesPerPixel;

    /*
    Retrieve the address to a specific pixel
    pSurface->pixels	= an array containing the SDL_Surface' pixels
    pSurface->pitch		= the length of a row of pixels (in bytes)
    X and Y				= the offset on where on the image to retrieve the pixel, (0, 0) is in the upper left corner of the image
    */
    Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;

    Uint32 PixelData = *(Uint32*)pPixel;

    SDL_Color Color = { 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE };

    // Retrieve the RGB values of the specific pixel
    SDL_GetRGB(PixelData, pSurface->format, &Color.r, &Color.g, &Color.b);

    return Color;
}

double getBrightness(SDL_Renderer* renderer, std::string font_path, char c, int font_size, int hdpi, int vdpi, SDL_Color text_color, SDL_Color background_color)
{
    char* chars = new char[2];
    chars[0] = c;
    chars[1] = '\0';
    
    TTF_Font* char_font{ TTF_OpenFontDPI(font_path.c_str(), font_size, hdpi, vdpi) }; // for some odd reason using the dpi version gives better results ::shrug
	//TTF_SetFontHinting(char_font, TTF_HINTING_NONE);
    //TTF_SetFontHinting(char_font, TTF_HINTING_LIGHT);
	SDL_Surface* char_surface{ TTF_RenderUTF8_LCD(char_font, chars, text_color, background_color)};
    //SDL_Surface* char_surface{ TTF_RenderUTF8_Solid(char_font, chars, text_color) };
    delete[] chars;
    chars = nullptr;
    TTF_CloseFont(char_font);
    
    //std::printf("\'%c\' width: %d, \theight: %d, \tbright: ", c, char_surface->w, char_surface->h);


    if(char_surface != NULL)
    {   
        double brightness = 0.0;
        for(int y{ 0 }; y < char_surface->h; ++y)
        {
            for(int x{ 0 }; x < char_surface->w; ++x)
            {
                SDL_Color rgb;
                rgb = getPixelColor(char_surface, x, y);

                brightness += (rgb.r + rgb.g + rgb.b) / 3.0;
            }
        }
        
        SDL_FreeSurface(char_surface);
        
        //std::printf("%f\n", brightness / (255.0 * (char_surface->h * char_surface->w)));
        return brightness / (255.0 * (char_surface->h * char_surface->w));
    }
    else
    {
        return 0.0;
    }
}


bool lessBrightness(BrightCharacter a, BrightCharacter b)
{
    return a.brightness < b.brightness;
}

void getBrightASCIIReal(SDL_Renderer* renderer, std::string font_path, int font_size, std::vector<BrightCharacter>& brightChars)
{
    for(char c{ 32 }; c < 127; ++c)
    {
        BrightCharacter brightChar;
        brightChar.character = c;
        brightChar.brightness = getBrightness(renderer, font_path, c, font_size);

        brightChars.push_back(brightChar);
    }

    std::sort(brightChars.begin(), brightChars.end(), lessBrightness);
}

void getBrightASCIIRelative(SDL_Renderer* renderer, std::string font_path, int font_size, std::vector<BrightCharacter>& brightChars)
{
    double maxBrightness{ 0 };
    
    brightChars.clear();

    for(char c{ 32 }; c < 127; ++c)
    {
        BrightCharacter brightChar;

        double brightness = getBrightness(renderer, font_path, c, font_size);
        brightChar.character = c;
        brightChar.brightness = brightness;

        if(brightness > maxBrightness) maxBrightness = brightness;

        brightChars.push_back(brightChar);
    }

    std::sort(brightChars.begin(), brightChars.end(), lessBrightness);
        
    for(auto& brightChar : brightChars)
    {
        brightChar.brightness /= maxBrightness;
    }
}

BrightCharacter getBrightestUpTo(const std::vector<BrightCharacter>& brightASCII, double max)
{
    if(!brightASCII.empty())
    {
        for(int i{ 1 }; i < brightASCII.size(); ++i)
        {
            if(brightASCII[i].brightness >= max)
            {
                double currentBrightness{ brightASCII[i - 1].brightness };

                std::vector<BrightCharacter> sameBrightness{};
                int j{ 0 };
                while(i + j < brightASCII.size() && brightASCII[i - 1 + j].brightness == currentBrightness)
                {
                    sameBrightness.push_back(brightASCII[i - 1 + j]);
                    ++j;
                }

                if(j - 1 < 0)
                    j = 1;

                return sameBrightness[Random::get(0, j - 1)];
            }
        }

        return brightASCII.back();
    }
    else
    {
        BrightCharacter nullbc;
        nullbc.brightness = 0;
        nullbc.character = ' ';
        return nullbc;
    }
}