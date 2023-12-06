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
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/all.hpp>

class range {

public:

  class iterator {

    friend class range;

  public:

    auto operator++() -> iterator& {
      ++_current;
      return *this;
    }

    auto operator++(int) -> iterator {
      auto copy = *this;
      ++_current;
      return copy;
    }

    auto operator==(const iterator& other) const -> bool {
      return _current == other._current;
    }

    auto operator*() -> std::uint32_t {
      return _current;
    }

  private:

    iterator(std::uint32_t current)
    : _current{current}{ }

    std::uint32_t _current;

  }; // struct iterator

  range(std::uint32_t start, std::uint32_t size)
  : _start{start},
    _size{size} { }

  auto begin() const -> iterator {
    return iterator{_start};
  }

  auto end() const -> iterator {
    return iterator{_start + _size};
  }

  auto lower_bound() const -> std::uint32_t {
    return _start;
  }

  auto upper_bound() const -> std::uint32_t {
    return _start + _size - 1u;
  }

  auto operator<(const range& other) const -> bool {
    return _start < other._start;
  }

private:

  std::uint32_t _start;
  std::uint32_t _size;

}; // class range

class mapping {

public:

  mapping(std::uint32_t source_start, std::uint32_t destination_start, std::uint32_t range)
  : _source_range{source_start, range},
    _destination_start{destination_start} { }

  auto operator()(std::uint32_t value) const -> std::uint32_t {
    return _destination_start + (value - _source_range.lower_bound());
  }

  auto lower_bound() const -> std::uint32_t {
    return _source_range.lower_bound();
  }

  auto upper_bound() const -> std::uint32_t {
    return _source_range.upper_bound();
  }

  auto operator<(const mapping& other) const -> bool {
    return _source_range < other._source_range;
  }

  auto range() const -> const ::range& {
    return _source_range;
  }

private:

  ::range _source_range;
  std::uint32_t _destination_start;

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

  auto begin() -> std::set<mapping>::const_iterator {
    return _mappings.begin();
  }

  auto end() -> std::set<mapping>::const_iterator {
    return _mappings.end();
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
  auto input = std::ifstream{"day5/part2/input.txt"};

  auto line = std::string{};

  auto result = std::numeric_limits<std::uint32_t>::max();

  auto seeds = std::vector<range>{};
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

      for (auto itr = values.begin(); itr != values.end();) {
        while (itr->empty()) {
          ++itr;
          continue;
        }

        auto start = std::uint32_t{};
        auto size = std::uint32_t{};
        
        std::from_chars((*itr).data(), (*itr).data() + (*itr).size(), start);

        ++itr;

        std::from_chars((*itr).data(), (*itr).data() + (*itr).size(), size);

        ++itr;

        seeds.emplace_back(start, size);
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

  auto a = 0u;

  // value = maps["temperature-to-humidity"].operator()(value);
  // value = maps["light-to-temperature"].operator()(value);
  // value = maps["water-to-light"].operator()(value);
  // value = maps["fertilizer-to-water"].operator()(value);
  // value = maps["soil-to-fertilizer"].operator()(value);
  // value = maps["seed-to-soil"].operator()(value);

  for (const auto& mapping : maps["humidity-to-location"]) {
    for (auto i : mapping.range()) {
      auto value = i;

      value = maps["temperature-to-humidity"].operator()(value);
      value = maps["light-to-temperature"].operator()(value);
      value = maps["water-to-light"].operator()(value);
      value = maps["fertilizer-to-water"].operator()(value);
      value = maps["soil-to-fertilizer"].operator()(value);
      value = maps["seed-to-soil"].operator()(value);
    }
  }

  fmt::print("Result: {}\n", result);

  return 0;
}
                                 