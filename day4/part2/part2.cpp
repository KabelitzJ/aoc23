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
  auto input = std::ifstream{"day4/part2/input.txt"};

  auto line = std::string{};

  auto sum = std::uint32_t{0u};

  auto copies = std::vector<std::uint32_t>{};
  auto card_count = 0u;
  auto scores = std::unordered_map<std::uint32_t, std::uint32_t>{};

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
          score++;
        }
      }
    }

    scores[card_count] = score;

    for (auto i = 1u; i <= score; ++i) {
      copies.push_back(card_count + i);
    }

    sum++;
    card_count++;
  }

  while (!copies.empty()) {
    auto copy = copies.back();
    copies.pop_back();

    if (copy >= card_count) {
      continue;
    }

    sum++;

    for (auto i = 1u; i <= scores[copy]; ++i) {
      copies.push_back(copy + i);
    }
  }

  fmt::print("Sum: {}\n", sum);

  return 0;
}
                                 