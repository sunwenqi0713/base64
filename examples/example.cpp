#include "base64.h"
#include <iostream>

int main() {
    // 基本编码解码
    std::vector<uint8_t> data = { 'H', 'e', 'l', 'l', 'o', 0xDE, 0xAD, 0xBE, 0xEF };
    std::string encoded = base64::encode(data);
    std::cout << "Encoded: " << encoded << "\n";

    if (auto decoded = base64::decode(encoded)) {
        std::cout << "Decoded success! Size: " << decoded->size() << " bytes\n";
    } else {
        std::cerr << "Decode failed!\n";
    }
    return 0;
}