#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include <cstddef>

#include "genelife_ca.h"

const std::string title = "Genelife";
constexpr int pixel_size = 4;
// constexpr std::size_t board_width = 8;
// constexpr std::size_t board_height = 8;
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
          int r, g, b;
          if (cell->rule.gene == 0) {
            r = 255;
            g = 0;
          } else {
            r = 0;
            g = 255;
          }
          b = cell->state * 60;
          if (cell->state == 0) {
            r = g = b = 0;
          }
          renderer.SetDrawColor(r, g, b, 255);
          renderer.FillRect(x * pixel_size, y * pixel_size,
                            x * pixel_size + pixel_size - 1,
                            y * pixel_size + pixel_size - 1);
        }
      }
    }
    renderer.Present();

    ca.step();
  }

  return 0;
}
