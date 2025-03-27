#include <iostream>

#include "base64.h"

int main() {
  std::string original = "Hello World!";
  std::string encoded = base64::encode(original);
  std::string decoded = base64::decode(encoded);

  std::cout << "Original: " << original << "\n"
            << "Encoded : " << encoded << "\n"
            << "Decoded : " << decoded << std::endl;

  return 0;
}