#pragma once

#include <string>
#include <vector>
#include <optional>
#
namespace base64 {

std::string encode(const std::vector<uint8_t> &data);
std::optional<std::vector<uint8_t>> decode(const std::string &data);

}  // namespace base64
