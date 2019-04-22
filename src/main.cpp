#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include <cstddef>

#include "genelife_ca.h"

const std::string title = "Genelife";
constexpr int pixel_size = 2;
constexpr std::size_t board_width = 64 * 4;
constexpr std::size_t board_height = 64 * 4;

int main(int argc, char *argv[]) {
  SDL2pp::SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
                  SDL_INIT_EVENTS);
  SDL2pp::Window window(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                        board_width * pixel_size, board_height * pixel_size, 0);
  SDL2pp::Renderer renderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  genelife::GenelifeCA ca(board_width, board_height);

  while (true) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
      return 0;
    if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_ESCAPE)
        return 0;
    }

    renderer.SetDrawColor(0, 0, 0, 255);
    renderer.Clear();
    for (std::size_t y = 0; y < board_height; y++) {
      for (std::size_t x = 0; x < board_width; x++) {
        {
          auto cell = ca.get_cell(x, y);
          /*
          auto r = ((cell->rule.gene & 0x0000'0000'000f'ffff) % 255);
          auto g = ((cell->rule.gene & 0x0000'00ff'fff0'0000) >> 4 * 5) % 255;
          auto b = ((cell->rule.gene & 0x0fff'ff00'0000'0000) >> 4 * 10) % 255;
          */
          /*
          auto r = ((cell->rule.gene & 0x0000'0fff) % 255);
          auto g = ((cell->rule.gene & 0x00ff'f000) >> 4 * 3) % 255;
          auto b = ((cell->rule.gene & 0xff00'0000) >> 4 * 6) % 255;
          auto a = (cell->state + 1) / (float)cell->rule.max_state() * 5;
          r = std::min<int>((int)r * a, 255);
          g = std::min<int>((int)g * a, 255);
          b = std::min<int>((int)b * a, 255);
          */
          /*
          int r, g, b;
          r = g = b = 255;
          if (cell->state == 0)
            r = g = b = 0;
          */

          int r, g, b;
          r = cell->age % 255;
          g = (cell->state) * 50;
          b = (cell->state == 0) ? 0 : 255;
          renderer.SetDrawColor(r, g, b, 255);
          renderer.FillRect(x * pixel_size, y * pixel_size,
                            x * pixel_size + pixel_size - 1,
                            y * pixel_size + pixel_size - 1);
        }
      }
    }
    renderer.Present();

    for (int i = 0; i < 10; i++)
      ca.step();
  }

  return 0;
}
