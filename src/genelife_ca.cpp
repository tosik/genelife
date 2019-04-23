#include "genelife_ca.h"

#include <iostream>
#include <memory>

namespace genelife {

GenelifeCA::GenelifeCA(const int &width, const int &height)
    : width(width), height(height), random_engine(seed_gen()) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = std::make_shared<Cell>();
      // cell->rule.gene = random_engine();
      if (random_engine() % 10 == 0) {
        cell->age = random_engine();
        cell->rule.mutate(cell->age);
        cell->state = cell->rule.max_state() - 1;
      } else {
        cell->state = 0;
      }
      cells.push_back(cell);
    }
  }

  // clone
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto cell = get_cell(x, y);
      auto clone = cell->clone();
      next_cells.push_back(clone);
    }
  }
}

int GenelifeCA::get_state(const int &x, const int &y) const {
  return get_cell(x, y)->state;
}

std::shared_ptr<Cell> GenelifeCA::get_cell(const int &x, const int &y) const {
  int wrapped_x = x;
  if (x < 0)
    wrapped_x += width;
  if (x >= width)
    wrapped_x -= width;

  int wrapped_y = y;
  if (y < 0)
    wrapped_y += height;
  if (y >= height)
    wrapped_y -= height;

  return cells[wrapped_x + wrapped_y * height];
}

void GenelifeCA::step() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cells[x + y * height] = next_cells[x + y * height];
      next_cells[x + y * height] = nullptr;
    }
  }

  /*
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (random_engine() % 100 == 0) {
        std::shared_ptr<Cell> c[9];
        c[0] = get_cell(x - 1, y - 1);
        c[1] = get_cell(x, y - 1);
        c[2] = get_cell(x + 1, y - 1);
        c[3] = get_cell(x - 1, y);
        c[4] = get_cell(x, y);
        c[5] = get_cell(x + 1, y);
        c[6] = get_cell(x - 1, y + 1);
        c[7] = get_cell(x, y + 1);
        c[8] = get_cell(x + 1, y + 1);

        c[4]->rule = c[random_engine() % 9]->rule;
        c[4]->rule.mutate(random_engine);
      }
    }
  }
  */

  /*
  // copy rules
  std::vector<int> r(width * height);
  std::iota(r.begin(), r.end(), 0);
  std::shuffle(r.begin(), r.end(), random_engine);
  for (int yy = 0; yy < height; yy++) {
    for (int xx = 0; xx < width; xx++) {
      int x = r[xx + yy * height] % width;
      int y = r[xx + yy * height] / height;
      std::shared_ptr<Cell> c[9];
      c[0] = get_cell(x - 1, y - 1);
      c[1] = get_cell(x, y - 1);
      c[2] = get_cell(x + 1, y - 1);
      c[3] = get_cell(x - 1, y);
      c[4] = get_cell(x, y);
      c[5] = get_cell(x + 1, y);
      c[6] = get_cell(x - 1, y + 1);
      c[7] = get_cell(x, y + 1);
      c[8] = get_cell(x + 1, y + 1);
      if (c[4]->is_living()) {
        std::vector<int> v(9);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), random_engine);
        for (auto i = 0; i < 9; i++) {
          if (c[v[i]]->is_dead())
            c[v[i]]->rule = c[4]->rule;
        }
      }
    }
  }
  */

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      std::shared_ptr<Cell> c[9];
      c[0] = get_cell(x - 1, y - 1);
      c[1] = get_cell(x, y - 1);
      c[2] = get_cell(x + 1, y - 1);
      c[3] = get_cell(x - 1, y);
      c[4] = get_cell(x, y);
      c[5] = get_cell(x + 1, y);
      c[6] = get_cell(x - 1, y + 1);
      c[7] = get_cell(x, y + 1);
      c[8] = get_cell(x + 1, y + 1);

      auto next_c = c[4]->clone();
      next_cells[x + y * height] = next_c;
      if (c[4]->is_dead())
        next_c->age = 0;

      auto rule = c[4]->rule;
      auto age = c[4]->age;

      // 死んでいたら周りからルールを探す
      if (c[4]->is_dead()) {
        std::vector<int> v(9);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), random_engine);
        for (auto i = 0; i < 9; i++) {
          if (c[v[i]]->is_living()) {
            rule = c[v[i]]->rule;
          }
        }
      }

      // 一番年上のセルの年齢を使う
      for (int i = 0; i < 9; i++) {
        if (c[i]->is_living())
          age = std::max<int>(c[i]->age, age);
      }

      /*
      // ルールを実行してそのまま反映
      auto result =
          rule.run(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
      if (result == c[4]->state) {
        next_c->age++;
      } else {
        next_c->age = age;
      }
      next_c->state = result;
      next_c->rule.mutate(next_c->age);
      */

      // 死んだセルが蘇るときにルールを遺伝する
      auto result =
          rule.run(c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7], c[8]);
      next_c->state = result;
      if (result == c[4]->state && !c[4]->is_dead()) {
        // next_c->age++;
      }
      if (c[4]->is_dead() && !next_c->is_dead()) {
        next_c->rule = rule;
        // next_c->age = age;
        next_c->age = age + 1;
        next_c->state = rule.max_state() - 1;
      } else if (c[4]->is_dying()) {
        next_c->age++;
      }
      if (!next_c->is_dead()) {
        if (false && steps > 300) {
          next_c->rule.gene = rule.rule_str_to_bits("1/1");
        } else {
          bool mutated = next_c->rule.mutate(next_c->age);
          if (mutated) {
            if (result == 0)
              next_c->state = 0;
            else if (result < next_c->rule.max_state() - 1)
              next_c->state = next_c->rule.max_state() - 2;
            else if (result == next_c->rule.max_state() - 1)
              next_c->state = next_c->rule.max_state() - 1;
          }
        }
      }
      if (next_c->is_dead()) {
        next_c->age = 0;
      }
    }
  }
  steps++;

  /*
  // 稀に局所ランダマイズされる
  if (steps > 100) {
    if (steps % 500 == 0) {
      printf("randomize\n");
      auto w = 50;
      auto h = 50;
      auto sx = random_engine() % (width - 50);
      auto sy = random_engine() % (height - 50);
      for (int y = sy; y < h + sy; y++) {
        for (int x = sx; x < w + sx; x++) {
          auto cell = next_cells[x + y * height];
          if (random_engine() % 5 == 0) {
            cell->age = random_engine();
            cell->rule.mutate(cell->age);
            cell->state = cell->rule.max_state() - 1;
          } else {
            cell->state = 0;
          }
        }
      }
    }
  }
  */
}

} // namespace genelife
