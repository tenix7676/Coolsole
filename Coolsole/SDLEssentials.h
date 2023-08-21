#pragma once
#include <string>
//#include <cstdio>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

bool init(SDL_Window*& window, const char* title, int screen_width, int screen_height, SDL_Renderer*& renderer);
void close(SDL_Window*& window, SDL_Renderer*& renderer);

class Texture
{
public:
	Texture();
	~Texture();

	bool loadFromFile(SDL_Renderer* renderer, std::string path);
#if defined(SDL_TTF_MAJOR_VERSION)
	bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color textColor);
	bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color textColor, SDL_Color backgroundColor);
	bool renderTextWrapped(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color text_color, SDL_Color background_color, int x=0, int y=0);
#endif
	bool createFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

	void free();

	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void renderScaled(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double scale = 1.0, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 a);

	int getWidth() { return m_w; }
	int getHeight() { return m_h; }

	Texture& operator=(Texture& other);
private:
	SDL_Texture* m_texture;
	int m_w;
	int m_h;
};