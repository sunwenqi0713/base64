#include "base64.h"

#include <string>
#include <vector>

namespace base64 {

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string encode(const std::string &input) {
  std::string encoded;
  size_t input_len = input.size();
  encoded.reserve(((input_len + 2) / 3) * 4);  // 预分配空间以提高效率

  for (size_t i = 0; i < input_len; i += 3) {
    uint32_t triple = 0;
    size_t remaining = input_len - i;

    // 读取三个字节（不足部分补0）
    uint8_t byte1 = static_cast<uint8_t>(input[i]);
    uint8_t byte2 = (i + 1 < input_len) ? static_cast<uint8_t>(input[i + 1]) : 0;
    uint8_t byte3 = (i + 2 < input_len) ? static_cast<uint8_t>(input[i + 2]) : 0;

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

std::string decode(const std::string &input) {
  // 构建反向查找表（字符 -> 6位值）
  std::vector<uint8_t> lookup(256, 0xFF);  // 初始化为无效值
  for (size_t i = 0; i < base64_chars.size(); ++i) {
    lookup[static_cast<uint8_t>(base64_chars[i])] = static_cast<uint8_t>(i);
  }

  std::string decoded;
  size_t input_len = input.size();
  if (input_len % 4 != 0) {
    return "";  // 输入长度必须是4的倍数
  }

  size_t padding = 0;
  if (input_len > 0 && input[input_len - 1] == '=') padding++;
  if (input_len > 1 && input[input_len - 2] == '=') padding++;

  decoded.reserve((input_len / 4) * 3 - padding);  // 预分配空间

  for (size_t i = 0; i < input_len; i += 4) {
    // 将4个字符转换为4个6位值
    uint32_t quadruple = 0;
    for (int j = 0; j < 4; ++j) {
      uint8_t c = static_cast<uint8_t>(input[i + j]);
      uint8_t value = lookup[c];
      if (value == 0xFF) {
        return "";  // 非法字符
      }
      quadruple = (quadruple << 6) | value;
    }

    // 提取3个字节
    decoded += static_cast<char>((quadruple >> 16) & 0xFF);
    decoded += static_cast<char>((quadruple >> 8) & 0xFF);
    decoded += static_cast<char>(quadruple & 0xFF);
  }

  // 根据填充移除多余字节
  if (padding > 0) {
    decoded.resize(decoded.size() - padding);
  }

  return decoded;
}

}  // namespace base64