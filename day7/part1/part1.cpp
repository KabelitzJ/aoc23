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

enum class type : std::uint8_t {
  five_of_a_kind,
  four_of_a_kind,
  full_house,
  three_of_a_kind,
  two_pairs,
  one_pair,
  high_card
}; // enum class type

struct entry {
  std::string hand;
  std::uint32_t bet;
}; // struct entry

static auto determine_type(const entry& entry) -> type {
  auto map = std::unordered_map<char, std::uint32_t>{};

  for (const auto c : entry.hand) {
    map[c]++;
  }

  auto has_three_of_a_kind = false;
  auto pair_counter = std::uint32_t{0u};

  for (const auto& [key, value] : map) {
    if (value == 5u) {
      return type::five_of_a_kind;
    } else if (value == 4u) {
      return type::four_of_a_kind;
    } else if (value == 3u) {
      has_three_of_a_kind = true;
    } else if (value == 2u) {
      pair_counter++;
    }
  }

  if (has_three_of_a_kind) {
    if (pair_counter > 0) {
      return type::full_house;
    } else {
      return type::three_of_a_kind;
    }
  } else {
    if (pair_counter == 2u) {
      return type::two_pairs;
    } else if (pair_counter == 1u) {
      return type::one_pair;
    }
  }

  return type::high_card;
}

static auto strengths = std::unordered_map<char, std::uint32_t>{
  { '2',  0u },
  { '3',  1u },
  { '4',  2u },
  { '5',  3u },
  { '6',  4u },
  { '7',  5u },
  { '8',  6u },
  { '9',  7u },
  { 'T',  8u },
  { 'J',  9u },
  { 'Q',  10u },
  { 'K',  11u },
  { 'A',  12u }
};

static auto is_lhs_stronger(const entry& lhs, const entry& rhs) -> bool {
  if (lhs.hand == rhs.hand) {
    return false;
  }

  for (const auto& [l, r] : ranges::views::zip(lhs.hand, rhs.hand)) {
    if (strengths[l] < strengths[r]) {
      return true;
    } else if (strengths[l] > strengths[r]) {
      return false;
    }
  }

  return false;
}

auto operator<(const entry& lhs, const entry& rhs) -> bool {
  const auto lhs_typ = determine_type(lhs);
  const auto rhs_typ = determine_type(rhs);

  if (lhs_typ == rhs_typ) {
    return !is_lhs_stronger(lhs, rhs);
  }

  return static_cast<std::uint8_t>(lhs_typ) < static_cast<std::uint8_t>(rhs_typ);
}

auto main() -> int {
  auto input = std::ifstream{"day7/part1/input.txt"};

  auto result = std::uint32_t{0u}; 

  auto line = std::string{};

  auto entries = std::vector<entry>{};

  while (std::getline(input, line)) {
    auto space = line.find_first_of(' ');

    auto hand = line.substr(0u, space);
    auto bet = line.substr(space + 1u);

    auto bet_value = std::uint32_t{};
    std::from_chars(bet.data(), bet.data() + bet.size(), bet_value);

    entries.emplace_back(hand, bet_value);
  }

  std::sort(entries.begin(), entries.end());

  for (const auto& [index, entry] : entries | ranges::views::reverse | ranges::views::enumerate) {
    result += entry.bet * (index + 1u);
  }

  fmt::print("Result: {}\n", result);

  return 0;
}
                                 