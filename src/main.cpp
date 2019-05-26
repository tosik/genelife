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
    constexpr int pixel_size = 3;
    constexpr std::size_t board_width = 64 * 10;
    constexpr std::size_t board_height = board_width;
    constexpr int consistency = 10;

    std::vector<int> rule_pattern = {0};
    genelife::GenelifeCA ca(board_width, board_height, rule_pattern,
                            consistency);

    SDL2pp::SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER |
                    SDL_INIT_EVENTS);
    SDL2pp::Window window(title, SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED, board_width * pixel_size,
                          board_height * pixel_size, 0);
    SDL2pp::Renderer renderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    while (true) {
      SDL_Event event;
      SDL_PollEvent(&event);
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          return 0;
        }
      }

      renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
      renderer.SetDrawColor(0, 0, 0, 255);
      renderer.Clear();

      for (std::size_t y = 0; y < board_height; y++) {
        for (std::size_t x = 0; x < board_width; x++) {
          {
            auto cell = ca.get_cell(x, y);
            int r, g, b;
            /*
            r = g = b = cell->state * 255 / cell->rule.max_state();
            */

            r = cell->state * 255 / cell->rule.max_state();
            g = abs(cell->vx) * 15;
            b = abs(cell->vy) * 15;

            if (true || cell->state > 0) {
              renderer.SetDrawColor(r, g, b, 255);
              renderer.FillRect(x * pixel_size, y * pixel_size,
                                x * pixel_size + pixel_size - 1,
                                y * pixel_size + pixel_size - 1);
            }
          }
        }
      }

      /*
      static auto func = [](int x) {
        if (x % 2 >= 1) {
          return x - 1;
        } else {
          return x + 1;
        }
      };

      renderer.SetDrawBlendMode(SDL_BLENDMODE_ADD);
      for (std::size_t y = 0; y < board_height; y++) {
        for (std::size_t x = 0; x < board_width; x++) {
          int xx = func(x);
          int yy = func(y);
          auto cell = ca.get_cell(xx % board_width, yy % board_height);
          int r, g, b;
          r = g = b = cell->state * 255 / cell->rule.max_state();

          if (cell->state > 0) {
            renderer.SetDrawColor(r, g, b, 255);
            renderer.FillRect(x * pixel_size, y * pixel_size,
                              x * pixel_size + pixel_size - 1,
                              y * pixel_size + pixel_size - 1);
          }
        }
      }
      */

      /*
      renderer.SetDrawBlendMode(SDL_BLENDMODE_ADD);
      for (std::size_t x = 0; x < board_width; x++) {
        int sum = 0;
        for (std::size_t y = 0; y < board_height; y++) {
          auto cell = ca.get_cell(x, y);
          if (cell->is_living())
            sum++;
        }

        for (std::size_t yy = 0; yy < board_height; yy++) {
          if (sum > yy)
            renderer.SetDrawColor(255, 0, 0, 255);
          else
            renderer.SetDrawColor(0, 0, 0, 255);
          int y = board_height - yy;
          renderer.FillRect(x * pixel_size, y * pixel_size,
                            x * pixel_size + pixel_size - 1,
                            y * pixel_size + pixel_size - 1);
        }
      }
      */

      renderer.Present();

      // SDL_Delay(100);

      std::stringstream ss;
      ss << "Genelife steps=" << ca.steps;

      window.SetTitle(ss.str());

      for (int i = 0; i < 1; i++)
        ca.step();
    }
  } else {
    constexpr std::size_t board_width = 64 * 2;
    constexpr std::size_t board_height = 64 * 2;
    constexpr int consistency = 10;
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
            genelife::GenelifeCA ca(board_width, board_height, rule_pattern,
                                    consistency);

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
