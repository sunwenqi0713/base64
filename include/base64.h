#pragma once

#include <string>

namespace base64 {

std::string encode(const std::string &input);
std::string decode(const std::string &input);

}  // namespace base64
