#include <algorithm>
//#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include "3DRenderer.h"
#include "MeasureBrightness.h"
#include "SDLEssentials.h"
#include "Vector.h"


const char* TITLE{ "Coolsole" };
int screen_width{ 640 };
int screen_height{ 480 };
const int MAX_FONT_SIZE{ 127 };
const int MIN_FONT_SIZE{ 1 };
char TEXT_CURSOR{ '|' };
const Uint64 TICKS_PER_SECOND{ SDL_GetPerformanceFrequency() };
const Uint64 BLINKING_TIME{ static_cast<Uint64>(TICKS_PER_SECOND * .5) };

enum KeysType
{
	CTRL,
	BACKSPACE,
	RETURN,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	MAX_KEYS
};

enum MouseButtonsType
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT,
	MAX_MOUSEBUTTONS
};

struct ButtonStates
{
	bool down{ false };
	bool held{ false };
	bool up{ false };
};

void resetButtonStates(ButtonStates* array, int length)
{
	for(int i{ 0 }; i < length; ++i)
	{
		array[i].down = false;
		array[i].up = false;
	}
}

int main(int argc, char** argv)
{
	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	if(!init(window, TITLE, screen_width, screen_height, renderer))
	{
		//std::printf("Couldn't initialize!\n");
	}
	else
	{
		std::string text_history{};
		std::string text_current{};
		char text_cursor{ TEXT_CURSOR };
		int start_time{ 0 };
		int end_time{ 0 };
		std::string font_path{ "fonts/Inconsolata-Regular.ttf" };
		int font_size{ 20 };
		int old_font_size{ font_size };

		SDL_Color text_color{ 0xff, 0xff, 0xff, 0xff };
		SDL_Color background_color{ 0x0, 0x0, 0x0, 0xff };

		Texture text_current_texture;
		Texture text_history_texture;

		int scroll_y_offset{ 0 };

		ButtonStates keyboard[MAX_KEYS];
		ButtonStates mouse[MAX_MOUSEBUTTONS];
		int mouse_x{ 0 }, mouse_y{ 0 };
		
		std::string edited_file_name{};
		std::string edited_file_text{};
		bool edit_mode{ false };



		// For rendering
		bool render_mode{ false };
		std::vector<double> screen{};
		std::vector<BrightCharacter> brightASCII{};
		getBrightASCIIRelative(renderer, font_path, font_size, brightASCII);

		std::string str_screen{ "" };

		std::vector<Sphere> spheres;
		spheres.push_back(Sphere{ Vector_3d{0, 0, 3}, 2, 1.0 });

		std::vector<LightPoint> lights;
		std::vector<LightPoint> old_lights;
		lights.push_back(LightPoint{ 1, Vector_3d{3, -3, 1} });

		int sphere_texture_width{ 0 };
		int sphere_texture_height{ 0 };


		
		bool keysPressed{ false };

		bool quit{ false };
		SDL_Event e{};

		SDL_StartTextInput();
		while(!quit)
		{
			while(SDL_PollEvent(&e))
			{
				if(SDL_QUIT == e.type)
				{
					quit = true;
				}

				if(SDL_KEYDOWN == e.type)
				{
					if(SDLK_BACKSPACE == e.key.keysym.sym && text_current.length() > 0)
					{
						text_current.pop_back();
					}
				}

				if(SDL_KEYDOWN == e.type && e.key.repeat == 0)
				{
					if(SDLK_LCTRL == e.key.keysym.sym || SDLK_RCTRL == e.key.keysym.sym)
					{
						keyboard[CTRL].down = true;
						keyboard[CTRL].held = true;
					}
					if(SDLK_BACKSPACE == e.key.keysym.sym)
					{
						keyboard[BACKSPACE].down = true;
						keyboard[BACKSPACE].held = true;
					}
					if(SDLK_RETURN == e.key.keysym.sym)
					{
						keyboard[RETURN].down = true;
						keyboard[RETURN].held = true;
					}
					if(SDLK_UP == e.key.keysym.sym)
					{
						keyboard[UP].down = true;
						keyboard[UP].held = true;
					}
					if(SDLK_DOWN == e.key.keysym.sym)
					{
						keyboard[DOWN].down = true;
						keyboard[DOWN].held = true;
					}
					if(SDLK_LEFT == e.key.keysym.sym)
					{
						keyboard[LEFT].down = true;
						keyboard[LEFT].held = true;
					}
					if(SDLK_RIGHT == e.key.keysym.sym)
					{
						keyboard[RIGHT].down = true;
						keyboard[RIGHT].held = true;
					}

					keysPressed = true;
				}

				if(SDL_KEYUP == e.type && e.key.repeat == 0)
				{
					if(SDLK_LCTRL == e.key.keysym.sym || SDLK_RCTRL == e.key.keysym.sym)
					{
						keyboard[CTRL].up = true;
						keyboard[CTRL].held = false;
					}
					if(SDLK_BACKSPACE == e.key.keysym.sym)
					{
						keyboard[BACKSPACE].up = true;
						keyboard[BACKSPACE].held = false;
					}
					if(SDLK_RETURN == e.key.keysym.sym)
					{
						keyboard[RETURN].up = true;
						keyboard[RETURN].held = false;
					}
					if(SDLK_UP == e.key.keysym.sym)
					{
						keyboard[UP].up = true;
						keyboard[UP].held = false;
					}
					if(SDLK_DOWN == e.key.keysym.sym)
					{
						keyboard[DOWN].up = true;
						keyboard[DOWN].held = false;
					}
					if(SDLK_LEFT == e.key.keysym.sym)
					{
						keyboard[LEFT].up = true;
						keyboard[LEFT].held = false;
					}
					if(SDLK_RIGHT == e.key.keysym.sym)
					{
						keyboard[RIGHT].up = true;
						keyboard[RIGHT].held = false;
					}
				}

				if(SDL_MOUSEMOTION == e.type)
				{
					SDL_GetMouseState(&mouse_x, &mouse_y);
				}

				if(SDL_MOUSEBUTTONDOWN == e.type)
				{
					if(SDL_BUTTON_LEFT == e.button.button)
					{
						//std::printf("Left mouse button down.\n");
						mouse[MOUSE_LEFT].down = true;
						mouse[MOUSE_LEFT].held = true;
					}
					if(SDL_BUTTON_RIGHT == e.button.button)
					{
						//std::printf("Right mouse button down.\n");
						mouse[MOUSE_RIGHT].down = true;
						mouse[MOUSE_RIGHT].held = true;
					}
					if(SDL_BUTTON_MIDDLE == e.button.button)
					{
						//std::printf("Middle mouse button down.\n");
						mouse[MOUSE_MIDDLE].down = true;
						mouse[MOUSE_MIDDLE].held = true;
					}
				}

				if(SDL_MOUSEBUTTONUP == e.type)
				{
					if(SDL_BUTTON_LEFT == e.button.button)
					{
						//std::printf("Left mouse button up.\n");
						mouse[MOUSE_LEFT].up = true;
						mouse[MOUSE_LEFT].held = false;
					}
					if(SDL_BUTTON_RIGHT == e.button.button)
					{
						//std::printf("Right mouse button up.\n");
						mouse[MOUSE_RIGHT].up = true;
						mouse[MOUSE_RIGHT].held = false;
					}
					if(SDL_BUTTON_MIDDLE == e.button.button)
					{
						//std::printf("Middle mouse button up.\n");
						mouse[MOUSE_MIDDLE].up = true;
						mouse[MOUSE_MIDDLE].held = false;
					}
				}

				if(e.type == SDL_MOUSEWHEEL)
				{
					if(keyboard[CTRL].held)
					{
						if(e.wheel.y > 0) // scroll up
						{
							font_size += 1;
							if(font_size > MAX_FONT_SIZE)
								font_size = MIN_FONT_SIZE;
						}
						else if(e.wheel.y < 0) // scroll down
						{
							font_size -= 1;
							if(font_size < MIN_FONT_SIZE)
								font_size = MAX_FONT_SIZE;
						}
					}
					else
					{
						if(e.wheel.y > 0) // scroll up
						{
							scroll_y_offset += (font_size * 4) / 3; // font_size in pixels
						}
						else if(e.wheel.y < 0) // scroll down
						{
							scroll_y_offset -= (font_size * 4) / 3; // font_size in pixels

						}
					}
				}


				if(e.type == SDL_TEXTINPUT)
				{
					text_current += e.text.text;
				}
			}

			if(keyboard[RETURN].up)
			{
				std::string text_current_upper{ text_current };
				std::transform(text_current_upper.begin(), text_current_upper.end(), text_current_upper.begin(), ::toupper);

				if(render_mode)
				{
					render_mode = false;
					text_history += str_screen;
					text_history += '\n';
				}

				if(!edit_mode)
				{
					text_history += text_current;
					text_history += '\n';

					if((text_current_upper.rfind("HELP", 0) == 0 && text_current_upper.length() == 4) || text_current_upper.rfind("HELP ", 0) == 0)
					{
						text_history +=
							"   HELP           - get a list of commands\n"
							"   OPEN, READ     - open a file of name\n"
							"   EDIT, CREATE   - create or edit a file of name\n"
							"   CLOSE          - close currently open files\n"
							"   RENDER         - render a sphere\n";
					}
					if(text_current_upper.rfind("OPEN ", 0) == 0)
					{
						std::string file_name{ text_current.substr(5, text_current_upper.length()) };
						std::ifstream file(file_name);

						if(file.is_open())
						{
							std::string line{};
							while(getline(file, line))
							{
								text_history += line;
								text_history += '\n';
							}

							file.close();
						}
					}
					if(text_current_upper.rfind("READ ", 0) == 0)
					{
						std::string file_name{ text_current.substr(5, text_current_upper.length()) };
						std::ifstream file(file_name);

						if(file.is_open())
						{
							std::string line{};
							while(getline(file, line))
							{
								text_history += line;
								text_history += '\n';
							}

							file.close();
						}
					}
					if(text_current_upper.rfind("EDIT ", 0) == 0 && text_current_upper.length() > 5)
					{
						edited_file_text = "";
						edited_file_name = text_current.substr(5, text_current_upper.length());
						std::ifstream file(edited_file_name);

						if(file.is_open())
						{
							std::string line{};
							while(getline(file, line))
							{
								edited_file_text += line;
								edited_file_text += '\n';
							}
							if(edited_file_text != "")
							{
								edited_file_text.pop_back(); // get rid of the extra '\n'
							}

							file.close();
						}
						edit_mode = true;
					}
					if(text_current_upper.rfind("CREATE ", 0) == 0 && text_current_upper.length() > 7)
					{
						edited_file_text = "";
						edited_file_name = text_current.substr(7, text_current_upper.length());
						std::ifstream file(edited_file_name);

						if(file.is_open())
						{
							std::string line{};
							while(getline(file, line))
							{
								edited_file_text += line;
								edited_file_text += '\n';
							}
							if(edited_file_text != "")
							{
								edited_file_text.pop_back(); // get rid of the extra '\n'
							}

							file.close();
						}
						edit_mode = true;
					}
					if(text_current_upper.rfind("RENDER", 0) == 0)
					{
						render_mode = true;
					}
					text_current = "";
				}
				else if(edit_mode)
				{
					if(text_current_upper.rfind("CLOSE", 0) == 0)
					{
						edit_mode = false;

						std::ofstream file;
						file.open(edited_file_name);
						if(edited_file_text != "") // get rid of the '\n' created when closing the file
						{
							edited_file_text.pop_back();
						}
						file << edited_file_text;

						edited_file_text += '\n'; // put it back for proper rendering

						file.close();

						text_history += edited_file_text;
						text_history += "   ";
						text_history += TITLE;
						text_history += " closed \"" + edited_file_name + "\"\n";

						edited_file_name = "";
						text_current = "";
					}

					edited_file_text += text_current;
					edited_file_text += '\n';

					text_current = "";
				}
			}


			SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);

			TTF_Font* text_font{ TTF_OpenFont(font_path.c_str(), font_size) };

			SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
			SDL_RenderClear(renderer);

			text_history_texture.renderTextWrapped(renderer, text_font, text_history, text_color, background_color, 0, 0 + scroll_y_offset);


			end_time = SDL_GetPerformanceCounter();
			if(keysPressed)
				start_time = end_time;
			int delta_time{ static_cast<int>(end_time - start_time) };
			
			// Blinking text cursor
			text_cursor = TEXT_CURSOR;
			if(delta_time > BLINKING_TIME)
			{
				text_cursor = ' ';
			}
			if(delta_time >= 2 * BLINKING_TIME)
			{
				start_time = SDL_GetPerformanceCounter();
			}

			if(edit_mode)
			{
				if(keyboard[BACKSPACE].down && text_current.length() == 0 && edited_file_text.length() > 0)
				{
					do
					{
						text_current += edited_file_text.back();
						edited_file_text.pop_back();
					} while( edited_file_text.length() > 0 && edited_file_text.back() != '\n' );

					std::reverse(text_current.begin(), text_current.end());
					text_current.pop_back();
				}

				text_current_texture.renderTextWrapped(renderer, text_font, edited_file_text + text_current + text_cursor, text_color, background_color, 0, 0 + text_history_texture.getHeight() + scroll_y_offset);
			}
			else if(render_mode)
			{
				Vector_3d v{};
				Vector_2d sphere_pos{ sphere_texture_width / 2, text_history_texture.getHeight() + scroll_y_offset + sphere_texture_height / 2 };
				Vector_2d mouse_pos{ mouse_x, mouse_y };
				mouse_pos = mouse_pos - sphere_pos;
				//mouse_pos = normalizeVector(mouse_pos);

				v.x = mouse_pos.x * .01;
				v.y = mouse_pos.y * .01;
				v.z = .5;
				LightPoint point_light{};
				point_light.intensity = 1.0;
				point_light.position = v;
				lights.back() = point_light;


				if(font_size != old_font_size)
				{
					getBrightASCIIRelative(renderer, font_path, font_size, brightASCII);
				}
				if(lights != old_lights || font_size != old_font_size)
				{
					renderScreen(screen, spheres, lights);
					str_screen = screenToASCII(screen, brightASCII);
				}


				
				text_current_texture.renderTextWrapped(renderer, text_font, str_screen, text_color, background_color, 0, 0 + text_history_texture.getHeight() + scroll_y_offset);
				sphere_texture_width = text_current_texture.getWidth();
				sphere_texture_height = text_current_texture.getHeight();
			}
			else
			{
				text_current_texture.loadFromRenderedText(renderer, text_font, text_current + text_cursor, text_color, background_color);
				text_current_texture.render(renderer, 0, 0 + text_history_texture.getHeight() + scroll_y_offset, NULL);
			}

			SDL_RenderPresent(renderer);

			resetButtonStates(mouse, MAX_MOUSEBUTTONS);
			resetButtonStates(keyboard, MAX_KEYS);
			keysPressed = false;

			old_font_size = font_size;
			old_lights = lights;

			text_history_texture.free();
			text_current_texture.free();
			TTF_CloseFont(text_font);
		}
		SDL_StopTextInput();
	}
	
	close(window, renderer);

	return 0;
}