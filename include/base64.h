#pragma once

#include <optional>
#include <string>
#include <vector>
#
namespace base64 {
    
std::string encode(const uint8_t *data, size_t len);
std::string encode(const std::vector<uint8_t> &data);

std::optional<std::vector<uint8_t>> decode(const std::string &data);
bool decode(const std::string &data, uint8_t *output, size_t &sz);

}  // namespace base64
