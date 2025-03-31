#include "base64.h"

#include <string>
#include <vector>

namespace base64 {

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string encode(const std::vector<uint8_t> &data) {
  std::string encoded;
  size_t len = data.size();
  encoded.reserve(((len + 2) / 3) * 4);  // 预分配空间以提高效率

  for (size_t i = 0; i < len; i += 3) {
    uint32_t triple = 0;
    size_t remaining = len - i;

    // 读取三个字节（不足部分补0）
    uint8_t byte1 = static_cast<uint8_t>(data[i]);
    uint8_t byte2 = (i + 1 < len) ? static_cast<uint8_t>(data[i + 1]) : 0;
    uint8_t byte3 = (i + 2 < len) ? static_cast<uint8_t>(data[i + 2]) : 0;

    triple = (byte1 << 16) | (byte2 << 8) | byte3;

    // 将24位数据分割为四个6位的索引
    encoded += base64_chars[(triple >> 18) & 0x3F];
    encoded += base64_chars[(triple >> 12) & 0x3F];

    // 处理填充
    if (remaining >= 2) {
      encoded += base64_chars[(triple >> 6) & 0x3F];
    } else {
      encoded += '=';
    }

    if (remaining >= 3) {
      encoded += base64_chars[triple & 0x3F];
    } else {
      encoded += '=';
    }
  }

  return encoded;
}

std::optional<std::vector<uint8_t>> decode(const std::string &data) {
  // 构建反向查找表（字符 -> 6位值）
  std::vector<uint8_t> lookup(256, 0xFF);  // 初始化为无效值
  for (size_t i = 0; i < base64_chars.size(); ++i) {
    lookup[static_cast<uint8_t>(base64_chars[i])] = static_cast<uint8_t>(i);
  }

  std::vector<uint8_t> decoded;
  size_t len = data.size();
  if (len % 4 != 0) {
    return std::nullopt;  // 输入长度必须是4的倍数
  }

  size_t padding = 0;
  if (len > 0 && data[len - 1] == '=') padding++;
  if (len > 1 && data[len - 2] == '=') padding++;

  decoded.reserve((len / 4) * 3 - padding);  // 预分配空间

  for (size_t i = 0; i < len; i += 4) {
    // 将4个字符转换为4个6位值
    uint32_t quadruple = 0;
    for (int j = 0; j < 4; ++j) {
      uint8_t c = static_cast<uint8_t>(data[i + j]);
      uint8_t value = lookup[c];
      if (value == 0xFF) {
        return std::nullopt;  // 非法字符
      }
      quadruple = (quadruple << 6) | value;
    }

    // 提取3个字节
    decoded.push_back(static_cast<char>((quadruple >> 16) & 0xFF));
    decoded.push_back(static_cast<char>((quadruple >> 8) & 0xFF));
    decoded.push_back(static_cast<char>(quadruple & 0xFF));
  }

  // 根据填充移除多余字节
  if (padding > 0) {
    decoded.resize(decoded.size() - padding);
  }

  return decoded;
}

}  // namespace base64