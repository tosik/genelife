#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include <cstddef>
#include <future>
#include <sstream>

#include "genelife_ca.h"

const std::string title = "Genelife";

int main(int argc, char *argv[]) {
  bool headless = false;
  if (!headless) {
    constexpr int pixel_size = 2;
    constexpr std::size_t board_width = 64 * 10;
    constexpr std::size_t board_height = 64 * 10;
    SDL2pp::SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
                    SDL_INIT_EVENTS);
    SDL2pp::Window window(title, SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED, board_width * pixel_size,
                          board_height * pixel_size, 0);
    SDL2pp::Renderer renderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // std::vector<int> rule_pattern = {9, 8, 7};
    std::vector<int> rule_pattern = {15, 6};
    genelife::GenelifeCA ca(board_width, board_height, rule_pattern);

    while (true) {
      SDL_Event event;
      SDL_PollEvent(&event);
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return 0;
        }
      }

      renderer.SetDrawColor(0, 0, 0, 255);
      renderer.Clear();

      for (std::size_t y = 0; y < board_height; y++) {
        for (std::size_t x = 0; x < board_width; x++) {
          {
            auto cell = ca.get_cell(x, y);
            /*
            auto r = ((cell->rule.gene & 0x0000'0000'000f'ffff) % 255);
            auto g = ((cell->rule.gene & 0x0000'00ff'fff0'0000) >> 4 * 5) %
            255; auto b = ((cell->rule.gene & 0x0fff'ff00'0000'0000) >> 4 *
            10) % 255;
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

            /*
            int r, g, b;
            r = (cell->age / 10) % 255;
            g = (cell->rule.gene) % 255;
            b = (cell->state == 0) ? 0 : 255;
            if (cell->state == 0)
              r = g = b = 0;
            */

            /*
            int r, g, b;
            r = cell->state == 1 ? 255 : 0;
            g = cell->state == 2 ? 255 : 0;
            b = cell->state == 3 ? 255 : 0;
            */

            static int colors[] = {
                0xffffff, 0xff66ff, 0xffff66, 0x6666ff, 0x66ffb2, 0xcccc00,
                0x0000cc, 0x336600, 0x330033, 0x990000, 0x808080,
            };
            auto color =
                colors[cell->rule.type % (sizeof(colors) / sizeof(*colors))];
            int r = (color & 0xff0000) >> (4 * 4);
            int g = (color & 0x00ff00) >> (4 * 2);
            int b = (color & 0x0000ff) >> (4 * 0);

            if (cell->state > 0) {
              renderer.SetDrawColor(r, g, b, 255);
              renderer.FillRect(x * pixel_size, y * pixel_size,
                                x * pixel_size + pixel_size - 1,
                                y * pixel_size + pixel_size - 1);
            }
          }
        }
      }
      renderer.Present();

      // SDL_Delay(100);

      std::stringstream ss;
      ss << "Genelife steps=" << ca.steps;

      window.SetTitle(ss.str());

      for (int i = 0; i < 2; i++)
        ca.step();
    }
  } else {
    constexpr std::size_t board_width = 64 * 2;
    constexpr std::size_t board_height = 64 * 2;
    std::cout << "Start headless mode" << std::endl;

    for (int base_mode = 0; base_mode < 29; base_mode++) {
      for (int mode = 0; mode < 29; mode++) {
        const std::vector rule_pattern = {0, base_mode, mode};

        std::cout << "Pattern : ";
        for (auto rule : rule_pattern) {
          std::cout << rule << ",";
        }
        std::cout << " : ";

        volatile int stables = 0;
        const int total = 10;
        std::vector<std::thread> threads;
        for (int s = 0; s < total; s++) {
          threads.push_back(std::thread([&] {
            genelife::GenelifeCA ca(board_width, board_height, rule_pattern);

            int num_of_living_zero = 0;
            int num_of_dying_zero = 0;
            int stable_counter = 0;

            while (true) {
              int living_counter[] = {0, 0, 0, 0, 0, 0, 0, 0};
              int dying_counter[] = {0, 0, 0, 0, 0, 0, 0, 0};
              int dead_counter[] = {0, 0, 0, 0, 0, 0, 0, 0};

              for (std::size_t y = 0; y < board_height; y++) {
                for (std::size_t x = 0; x < board_width; x++) {
                  {
                    auto cell = ca.get_cell(x, y);
                    if (cell->is_living())
                      living_counter[cell->rule.type]++;
                    if (cell->is_dying())
                      dying_counter[cell->rule.type]++;
                    if (cell->is_dead())
                      dead_counter[cell->rule.type]++;
                  }
                }
              }

              const int stable_threashold = 100;

              if (num_of_living_zero == living_counter[0] &&
                  num_of_dying_zero == dying_counter[0]) {
                stable_counter++;
              } else {
                stable_counter = 0;
              }

              if (stable_counter > stable_threashold) {
                stables++;
                break;
              }

              if (ca.steps > stable_threashold * 10) {
                break;
              }

              num_of_living_zero = living_counter[0];
              num_of_dying_zero = dying_counter[0];

              ca.step();
            }
          }));
        }

        for (auto &thread : threads) {
          thread.join();
        }

        std::cout << " stable = " << stables << "/" << total << std::endl;
      }
    }
  }

  return 0;
}
