#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ranges>
#include <charconv>
#include <unordered_map>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

auto split(std::string_view string, std::string_view delimiter) -> std::vector<std::string_view> {
  auto result = std::vector<std::string_view>{};
  auto last = std::size_t{0u};
  auto next = std::size_t{0u};
  
  while ((next = string.find(delimiter, last)) != std::string_view::npos) {
    result.push_back(string.substr(last, next - last));
    last = next + delimiter.size();
  } 

  result.push_back(string.substr(last));

  return result;
}

auto main() -> int {
  auto input = std::ifstream{"day2/part1/input.txt"};

  auto line = std::string{};

  auto sum = std::uint32_t{0u};

  while (std::getline(input, line)) {
    auto game_id = std::uint32_t{};
    auto sets = std::unordered_map<std::string_view, std::uint32_t>{};
    
    auto game_id_substring = line.substr(5u, line.find(':') - 5u);
    std::from_chars(game_id_substring.data(), game_id_substring.data() + game_id_substring.size(), game_id);

    auto sets_substring = line.substr(line.find(':') + 2u);
    auto sets_list = split(sets_substring, "; ");

    for (auto set_string : sets_list) {
      auto set_list = split(set_string, ", ");

      for (auto color_string : set_list) {
        auto color_value_pair = split(color_string, " ");

        auto value = std::uint32_t{};
        std::from_chars(color_value_pair[0].data(), color_value_pair[0].data() + color_value_pair[0].size(), value);

        sets[color_value_pair[1]] = std::max(sets[color_value_pair[1]], value);
      }
    }

    if (sets["red"] <= 12u && sets["green"] <= 13u && sets["blue"] <= 14u) {
      sum += game_id;
    }
  }

  fmt::print("Sum: {}\n", sum);

  return 0;
}
                                 