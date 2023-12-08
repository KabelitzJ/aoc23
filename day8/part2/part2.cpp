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
#include <numeric>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

struct rule {
  std::string left;
  std::string right;
}; // struct rule

static constexpr auto gcd(int a, int b) -> std::int32_t {
  for (;;) {
    if (a == 0) {
      return b;
    }

    b %= a;

    if (b == 0) {
      return a;
    }

    a %= b;
  }
}

static constexpr auto lcm(int a, int b) -> std::int32_t {
  const auto temp = gcd(a, b);

  return temp ? (a / temp * b) : 0;
}

auto main() -> int {
  auto input = std::ifstream{"day8/part2/input.txt"};

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

  auto currents = rules 
    | ranges::views::filter([](auto entry){ return entry.first[2u] == 'A'; })
    | ranges::views::transform([](auto entry){ return entry.first; })
    | ranges::to<std::vector>;

  auto all_steps = std::vector<std::uint32_t>{};

  for (const auto& current : currents) {
    auto steps = std::uint32_t{0u};
    auto c = current;

    while (c[2u] != 'Z') {
      c = (instruction[steps % instruction.size()] == 'L') ? rules[c].left : rules[c].right;
      ++steps;
    }

    all_steps.push_back(steps);
  }

  auto result = all_steps[0u];

  for (auto i = 1u; i < all_steps.size(); ++i) {
    result = std::lcm(result, all_steps[i]);
  }

  fmt::print("Steps: {}\n", all_steps);
  fmt::print("Result: {}\n", result);

  return 0;
}
                                 