#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <charconv>
#include <unordered_map>
#include <unordered_set>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

static auto split(std::string string, std::string delimiter) -> std::vector<std::string> {
  auto result = std::vector<std::string>{};
  auto last = std::size_t{0u};
  auto next = std::size_t{0u};
  
  while ((next = string.find(delimiter, last)) != std::string::npos) {
    result.push_back(string.substr(last, next - last));
    last = next + delimiter.size();
  } 

  result.push_back(string.substr(last));

  return result;
}

auto main() -> int {
  auto input = std::ifstream{"day4/part1/input.txt"};

  auto line = std::string{};

  auto sum = std::uint32_t{0u};


  while (std::getline(input, line)) {
    auto contents = line.substr(line.find_first_of(":") + 1u);

    auto lists = split(contents, "|");

    auto winners = std::unordered_set<std::uint32_t>{};
    auto score = 0u;

    for (const auto& winner : split(lists[0], " ")) {
      auto value = std::uint32_t{0u};

      const auto result = std::from_chars(winner.data(), winner.data() + winner.size(), value);

      if (result.ec != std::errc::invalid_argument && result.ec != std::errc::result_out_of_range) {
        winners.insert(value);
      }
    }

    for (const auto& winner : split(lists[1], " ")) {
      auto value = std::uint32_t{0u};

      const auto result = std::from_chars(winner.data(), winner.data() + winner.size(), value);

      if (result.ec != std::errc::invalid_argument && result.ec != std::errc::result_out_of_range) {
        if (winners.find(value) != winners.cend()) {
          score = (score == 0u) ? 1u : score * 2u;
        }
      }
    }

    sum += score;
  }

  fmt::print("Sum: {}\n", sum);

  return 0;
}
                                 