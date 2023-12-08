#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <charconv>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <set>
#include <algorithm>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

struct rule {
  std::string left;
  std::string right;
}; // struct rule

auto main() -> int {
  auto input = std::ifstream{"day8/part1/input.txt"};

  auto line = std::string{};

  std::getline(input, line);

  auto instruction = line;

  std::getline(input, line);

  auto rules = std::unordered_map<std::string, rule>{};

  while (std::getline(input, line)) {
    auto start = line.substr(0u, 3u);
    auto left = line.substr(7u, 3u);
    auto right = line.substr(12u, 3u);

    rules.insert({start, rule{left, right}});
  }

  auto current = std::string{"AAA"};
  auto steps = std::uint32_t{0u};

  while (current != std::string{"ZZZ"}) {
    current = (instruction[steps % instruction.size()] == 'L') ? rules[current].left : rules[current].right;
    ++steps;
  }

  fmt::print("Steps: {}\n", steps);

  return 0;
}
                                 