#include <iostream>
#include <fstream>
#include <string>
#include <ranges>
#include <optional>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

struct digit {
  std::uint32_t value;
  std::string string;
}; // struct digit

static auto digits = std::array<digit, 9u>{
  digit{ 1u, "one" },
  digit{ 2u, "two" },
  digit{ 3u, "three" },
  digit{ 4u, "four" },
  digit{ 5u, "five" },
  digit{ 6u, "six" },
  digit{ 7u, "seven" },
  digit{ 8u, "eight" },
  digit{ 9u, "nine" }
};

static auto max_digit_string_length = std::ranges::max(digits | std::views::transform([](auto digit) { return static_cast<std::uint32_t>(digit.string.size()); }));

static auto matches_digit_string(std::string string) -> std::optional<std::uint32_t> {
  if (string.empty()) {
    return std::nullopt;
  }

  for (auto digit : digits | std::views::filter([string](auto digit) { return digit.string.size() <= string.size(); })) {
    if (string.starts_with(digit.string)) {
      return digit.value;
    }
  }

  return std::nullopt;
}

auto char_to_value(char c) -> std::uint32_t {
  return static_cast<std::uint32_t>(c - '0');
}

auto main() -> int {
  auto input = std::ifstream{"day1/part2/input.txt"};

  auto line = std::string{};

  auto sum = std::uint32_t{0u};

  while (std::getline(input, line)) {
    auto head = std::uint32_t{0u};
    auto numbers = std::vector<std::uint32_t>{};

    while (head < line.size()) {
      if (std::isdigit(line[head])) {
        numbers.push_back(char_to_value(line[head]));
      } else {
        auto length = std::min(max_digit_string_length, static_cast<std::uint32_t>(line.size() - head));

        for (auto i : std::views::iota(1u, length + 1u)) {
          if (auto digit = matches_digit_string(line.substr(head, i)); digit) {
            numbers.push_back(*digit);
            break;
          }
        }
      }
      
      head++;
    }

    fmt::print("{}\n", numbers);

    sum += (numbers.front() * 10u + numbers.back());
  }

  fmt::print("Sum: {}\n", sum);
  
  input.close();

  return 0;
}
