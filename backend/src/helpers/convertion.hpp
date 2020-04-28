#pragma once

#include <charconv>
#include <optional>
#include <string_view>

template <typename T>
std::optional<T> convert(const std::string_view &sv) {
  int key;
  auto result = std::from_chars(sv.data(), sv.data() + sv.size(), key);
  if (result.ec == std::errc::invalid_argument) {
    return {};
  }
  return key;
}
