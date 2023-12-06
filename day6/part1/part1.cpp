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

class mapping {

public:

  mapping(std::uint32_t source_start, std::uint32_t destination_start, std::uint32_t range)
  : _source_start{source_start},
    _destination_start{destination_start},
    _range{range} { }

  auto operator()(std::uint32_t value) const -> std::uint32_t {
    return _destination_start + (value - _source_start);
  }

  auto lower_bound() const -> std::uint32_t {
    return _source_start;
  }

  auto upper_bound() const -> std::uint32_t {
    return _source_start + _range - 1u;
  }

  auto operator<(const mapping& other) const -> bool {
    return _source_start < other._source_start;
  }

private:

  std::uint32_t _source_start;
  std::uint32_t _destination_start;
  std::uint32_t _range;

}; // class mapping

class map {

public:

  auto add_mapping(std::uint32_t source_start, std::uint32_t destination_start, std::uint32_t range) -> void {
    _mappings.emplace(source_start, destination_start, range);
  }

  auto operator()(std::uint32_t value) -> std::uint32_t {
    for (const auto& mapping : _mappings) {
      if (value > mapping.upper_bound()) {
        continue;
      } else if (value < mapping.lower_bound()) {
        return value;
      } else {
        return mapping(value);
      }
    }

    return value;
  }

private:

  std::set<mapping> _mappings;

}; // class map

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
  auto input = std::ifstream{"day5/part1/input.txt"};

  auto line = std::string{};

  auto result = std::numeric_limits<std::uint32_t>::max();

  auto seeds = std::vector<std::uint32_t>{};
  auto maps = std::unordered_map<std::string, map>{};

  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }

    auto colon_position = line.find_first_of(':');

    if (colon_position == std::string::npos) {
      continue;
    }
    
    auto type = line.substr(0u, colon_position);

    if (type == "seeds") {
      auto values = split(line.substr(colon_position + 1u), " ");

      for (const auto& value : values) {
        if (value.empty()) {
          continue;
        }

        auto val = std::uint32_t{};
        auto result = std::from_chars(value.data(), value.data() + value.size(), val);

        if (result.ec != std::errc::invalid_argument && result.ec != std::errc::result_out_of_range) {
          seeds.push_back(val);
        }
      }
    } else {
      auto map_type = type.substr(0u, type.find_first_of(' '));

      while (std::getline(input, line)) {
        if (line.empty()) {
          break;
        }

        auto dest_start = std::uint32_t{};
        auto src_start = std::uint32_t{};
        auto range = std::uint32_t{};
        auto stream = std::stringstream{line};

        stream >> dest_start >> src_start >> range;

        maps[map_type].add_mapping(src_start, dest_start, range);
      }
    }
  }

  for (const auto& seed : seeds) {
    auto value = seed;

    value = maps["seed-to-soil"].operator()(value);
    value = maps["soil-to-fertilizer"].operator()(value);
    value = maps["fertilizer-to-water"].operator()(value);
    value = maps["water-to-light"].operator()(value);
    value = maps["light-to-temperature"].operator()(value);
    value = maps["temperature-to-humidity"].operator()(value);
    value = maps["humidity-to-location"].operator()(value);

    result = std::min(result, value);
  }

  fmt::print("Result: {}\n", result);

  return 0;
}
                                 