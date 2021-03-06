#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include <cstddef>

#include "genelife_ca.h"

const std::string title = "Genelife";
constexpr int pixel_size = 3;
constexpr std::size_t board_width = 64 * 8;
constexpr std::size_t board_height = 64 * 8;

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
          auto r = (cell->rule.gene & 0x0000'0000'000f'ffff);
          auto g = (cell->rule.gene & 0x0000'00ff'fff0'0000) >> 4 * 5;
          auto b = (cell->rule.gene & 0x0fff'ff00'0000'0000) >> 4 * 10;
          // r = g = b = 255;
          auto a = cell->state / (float)cell->rule.max_state() * 10;
          r *= a;
          g *= a;
          b *= a;
          if (cell->state != 0) {
            renderer.SetDrawColor(r, g, b, 255);
            renderer.FillRect(x * pixel_size, y * pixel_size,
                              x * pixel_size + pixel_size - 1,
                              y * pixel_size + pixel_size - 1);
          }
        }
      }
    }
    renderer.Present();

    for (int i = 0; i < 1; i++)
      ca.step();
  }

  return 0;
}
