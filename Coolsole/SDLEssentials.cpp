#include "SDLEssentials.h"

bool init(SDL_Window*& window, const char* title, int screen_width, int screen_height, SDL_Renderer*& renderer)
{
	bool success{ true };

	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		//std::printf("Couldn't initialize SDL! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_RESIZABLE);
		if(!window)
		{
			//std::printf("Couldn't create window! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
		}
		if(!renderer)
		{
			//printf("Couldn't create renderer! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags))
			{
				//std::printf("Couldn't initialize SDL_image! IMG Error: %s\n", IMG_GetError());
				success = false;
			}
			if(TTF_Init() == -1)
			{
				//std::printf("Couldn't initialize SDL_ttf! TTF Error: %s\n", TTF_GetError());
				success = false;
			}
		}
	}

	return success;
}

void close(SDL_Window*& window, SDL_Renderer*& renderer)
{
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}











void Texture::free()
{
	SDL_DestroyTexture(m_texture);
	m_texture = nullptr;
}

Texture::Texture()
{
	m_texture = NULL;
	m_w = 0;
	m_h = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path)
{
	free();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
	{
		;//std::printf("Couldn't load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xff, 0xff));
		m_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if(m_texture == NULL)
		{
			//std::printf("Couldn't create texture from surface %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			m_w = loadedSurface->w;
			m_h = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}

	return m_texture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool Texture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color textColor)
{
	free();

	SDL_Surface* loadedText = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
	if(loadedText == NULL)
	{
		if(text != "")
			;//std::printf("Couldn't create surface from text! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		m_texture = SDL_CreateTextureFromSurface(renderer, loadedText);
		if(m_texture == NULL)
		{
			;//std::printf("Couldn't create texture from surface text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			m_w = loadedText->w;
			m_h = loadedText->h;
		}

		SDL_FreeSurface(loadedText);
	}

	return m_texture != NULL;
}

bool Texture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color textColor, SDL_Color backgroundColor)
{
	free();

	SDL_Surface* loadedText = TTF_RenderUTF8_LCD(font, text.c_str(), textColor, backgroundColor);
	//SDL_Surface* loadedText = TTF_RenderUTF8_Shaded(font, text.c_str(), textColor, backgroundColor);
	//SDL_Surface* loadedText = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
	if(loadedText == NULL)
	{
		if(text != "")
			;//std::printf("Couldn't create surface from text! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		m_texture = SDL_CreateTextureFromSurface(renderer, loadedText);
		if(m_texture == NULL)
		{
			;//std::printf("Couldn't create texture from surface text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			m_w = loadedText->w;
			m_h = loadedText->h;
		}

		SDL_FreeSurface(loadedText);
	}

	return m_texture != NULL;
}

bool Texture::renderTextWrapped(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color text_color, SDL_Color background_color, int x, int y)
{
	free();

	bool success{ true };

	std::string rendered_string{ "" };
	int overall_height{ 0 };

	int i{ 0 };
	while(i < text.size())
	{
		rendered_string += text[i];
		if(text[i] == '\n' || text[i] == '\0' || i == text.size() - 1)
		{
			success = success && loadFromRenderedText(renderer, font, rendered_string, text_color, background_color);

			render(renderer, x, y + overall_height, NULL);

			overall_height += m_h;
			rendered_string = "";
		}

		++i;
	}

	m_h = overall_height;

	return success;
}
#endif

Texture& Texture::operator=(Texture& other)
{
	//self guard
	if(this == &other)
		return *this;

	m_w = other.m_w;
	m_h = other.m_h;
	m_texture = other.m_texture;

	return other;
}

bool Texture::createFromSurface(SDL_Renderer* renderer, SDL_Surface* surface)
{
	free();

	m_texture = SDL_CreateTextureFromSurface(renderer, surface);
	if(m_texture == NULL)
	{
		;//std::printf("Couldn't create texture from surface text! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		m_w = surface->w;
		m_h = surface->h;
	}


	return m_texture != NULL;
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect RenderedQuad = { x, y, m_w, m_h };
	if(clip != NULL)
	{
		RenderedQuad.w = clip->w;
		RenderedQuad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, m_texture, clip, &RenderedQuad, angle, center, flip);
}

//For scaling you need to provide a clip rect
void Texture::renderScaled(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double scale, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect RenderedQuad = { x, y, m_w, m_h };
	if(clip != NULL)
	{
		RenderedQuad.w = clip->w * scale;
		RenderedQuad.h = clip->h * scale;
	}
	else
	{
		;//std::printf("Warning: Used Texture::renderScale(), without providing a clip rect.\n");
	}

	SDL_RenderCopyEx(renderer, m_texture, clip, &RenderedQuad, angle, center, flip);
}

void Texture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(m_texture, r, g, b);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_texture, blending);
}

void Texture::setAlpha(Uint8 a)
{
	SDL_SetTextureAlphaMod(m_texture, a);
}
