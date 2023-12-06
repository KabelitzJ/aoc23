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

static auto get_value(const std::string& line) -> std::uint64_t {
  auto stream = std::stringstream{};

  auto colon = line.find_first_of(':');

  auto last = std::size_t{0u};
  auto next = std::size_t{0u};

  auto parts = line.substr(colon + 1u);

  while ((next = parts.find_first_not_of(' ', last)) != std::string::npos) {
    last = parts.find_first_of(' ', next);
    auto substr = parts.substr(next, last - next);
    stream << substr;
  } 

  auto str = stream.str();
  auto value = std::uint64_t{0};
  std::from_chars(str.data(), str.data() + str.size(), value);
  
  return value;
}

auto main() -> int {
  auto input = std::ifstream{"day6/part2/input.txt"};

  auto result = std::uint32_t{0u}; 

  auto line = std::string{};

  std::getline(input, line);

  const auto time = get_value(line);

  std::getline(input, line);

  const auto distance = get_value(line);

  for (auto held_time = std::uint64_t{0u}; held_time <= time; ++held_time) {
    const auto time_to_race = time - held_time;
    const auto velocity = held_time;

    const auto current_distance = time_to_race * velocity;

    if (current_distance > distance) {
      ++result;
    }
  }

  fmt::print("Result: {}\n", result);

  return 0;
}
                                 