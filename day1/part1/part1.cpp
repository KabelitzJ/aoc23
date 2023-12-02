#include <iostream>
#include <fstream>
#include <string>
#include <ranges>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

auto main() -> int {
  auto input = std::ifstream{"day1/part1/input.txt"};

  if (!input.is_open()) {
    fmt::print(stderr, "Failed to open input file\n");
    return 1;
  }

  auto line = std::string{};

  auto sum = std::uint32_t{};

  while (std::getline(input, line)) {
    auto numbers = line | std::views::filter([](char c) { return std::isdigit(c); }) | std::views::transform([](char c) { return static_cast<std::uint32_t>(c - '0'); }) | ranges::to<std::vector<std::uint32_t>>;

    if (numbers.empty()) {
      fmt::print(stderr, "Invalid input: {}\n", line);
      return 1;
    }

    sum += (numbers.front() * 10u + numbers.back());
  }

  fmt::print("Sum: {}\n", sum);
  
  input.close();

  return 0;
}
