#pragma once

#include <cstdint>
#include <iostream>

class BitManipulator {
public:
    static bool IsOn(const uint8_t byte, const uint8_t index);

    static void TurnOnBit(uint8_t& byte, const uint8_t index);

    static void TurnOffBit(uint8_t& byte, const uint8_t index);

private:
    constexpr static const uint8_t kMinterms_[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    constexpr static const uint8_t kMaxterms_[8] = {255 - 128, 255 - 64, 255 - 32, 255 - 16, 255 - 8, 255 - 4, 255 - 2, 255 - 1};
};

class uint17_t {
public:
    uint17_t(uint8_t* last_bits, uint64_t bit_index, uint16_t* full_part);

    uint17_t& operator=(const uint32_t right);

    uint17_t& operator=(const uint17_t& right);

    uint32_t ToInt() const;

private:
    uint8_t* last_bits_;
    uint64_t bit_index_;
    uint16_t* full_part_;
    static BitManipulator bit_manipulator;
};

uint32_t ToSum(const uint17_t& lhs, const uint17_t& rhs);

uint32_t ToDif(const uint17_t& lhs, const uint17_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint17_t& value);

std::istream& operator>>(std::istream& stream, uint17_t r);
