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

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

static auto get_values(const std::string& line) -> std::vector<std::uint32_t> {
  auto result = std::vector<std::uint32_t>{};

  auto colon = line.find_first_of(':');

  auto last = std::size_t{0u};
  auto next = std::size_t{0u};

  auto parts = line.substr(colon + 1u);

  while ((next = parts.find_first_not_of(' ', last)) != std::string::npos) {
    last = parts.find_first_of(' ', next);
    auto substr = parts.substr(next, last - next);

    auto value = std::uint32_t{0};
    std::from_chars(substr.data(), substr.data() + substr.size(), value);
    result.push_back(value);
  } 

  return result;
}

auto main() -> int {
  auto input = std::ifstream{"day6/part1/input.txt"};

  auto result = std::uint32_t{1u}; 

  auto line = std::string{};

  std::getline(input, line);

  const auto times = get_values(line);

  std::getline(input, line);

  const auto distances = get_values(line);

  for (const auto& [time, distance] : ranges::views::zip(times, distances)) {
    auto sum = std::uint32_t{0u};

    for (auto held_time = 0u; held_time <= time; ++held_time) {
      const auto time_to_race = time - held_time;
      const auto velocity = held_time;

      const auto current_distance = time_to_race * velocity;

      if (current_distance > distance) {
        ++sum;
      }
    }

    result *= sum;
  }

  fmt::print("Result: {}\n", result);

  return 0;
}
                                 