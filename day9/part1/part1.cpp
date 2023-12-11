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
#include <optional>

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

class sequence {

public:

  sequence(const std::vector<std::int32_t>& values)
  : _values{values} { }

  auto find_extrapolated_value() const -> std::int32_t {

    auto rows = std::vector<std::vector<std::int32_t>>{};
    rows.push_back(_values);

    auto row = _calculate_next_row(rows.back());

    while (row) {
      rows.push_back(*row);
      row = _calculate_next_row(rows.back());
    }

    auto value = std::int32_t{0u};

    for (const auto& row : rows | ranges::views::reverse) {
      value += row.back();
    }

    return value;
  }

private:

  auto _calculate_next_row(const std::vector<std::int32_t>& values) const -> std::optional<std::vector<std::int32_t>> {
    auto row = std::vector<std::int32_t>{};

    for (auto i = 1u; i < values.size(); ++i) {
      row.push_back(values[i] - values[i - 1u]);
    }

    if (ranges::all_of(row, [](const auto value){ return value == 0u; })) {
      return std::nullopt;
    }

    return row;
  }

  std::vector<std::int32_t> _values;

}; // struct sequence

auto main() -> int {
  auto input = std::ifstream{"day9/part1/input.txt"};

  auto result = std::int32_t{0u};

  auto line = std::string{};

  auto sequences = std::vector<sequence>{};

  auto to_number = [](const std::string& string) -> std::int32_t {
    auto value = std::int32_t{};
    std::from_chars(string.data(), string.data() + string.size(), value);
    return value;
  };  

  while (std::getline(input, line)) {
    auto values = split(line, " ");
    sequences.emplace_back(values | ranges::views::transform(to_number) | ranges::to<std::vector>);
  }

  for (const auto& sequence : sequences) {
    result += sequence.find_extrapolated_value();
  }

  fmt::print("Result: {}\n", result);

  return 0;
}
                                 