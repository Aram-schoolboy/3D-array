#include "uint17.h"

bool BitManipulator::IsOn(const uint8_t byte, const uint8_t index) {
    return (kMinterms_[index] & byte) != 0;
}

void BitManipulator::TurnOnBit(uint8_t& byte, const uint8_t index) {
    byte |= kMinterms_[index];
}

void BitManipulator::TurnOffBit(uint8_t& byte, const uint8_t index) {
    byte &= kMaxterms_[index];
}

uint17_t::uint17_t(uint8_t* last_bits, uint64_t bit_index, uint16_t* full_part):
                    last_bits_(last_bits), bit_index_(bit_index), full_part_(full_part) {}

uint17_t& uint17_t::operator=(const uint32_t right) {
    *full_part_ = right / 2;
    uint8_t& byte = last_bits_[bit_index_ / 8];
    uint8_t remain = bit_index_ % 8;

    if (right % 2 == 0) {
        bit_manipulator.TurnOffBit(byte, remain);
    } else {
        bit_manipulator.TurnOnBit(byte, remain);
    }
    return *this;
}

uint17_t& uint17_t::operator=(const uint17_t& right) {
    *this = right.ToInt();
    return *this;
}

uint32_t uint17_t::ToInt() const {
    uint32_t result = *full_part_ * 2;
    return bit_manipulator.IsOn(last_bits_[bit_index_ / 8], bit_index_ % 8) ? ++result: result;
}

uint32_t ToSum(const uint17_t& lhs, const uint17_t& rhs) {
    return lhs.ToInt() + rhs.ToInt();
}

uint32_t ToDif(const uint17_t& lhs, const uint17_t& rhs) {
    return lhs.ToInt() - rhs.ToInt();
}

std::ostream& operator<<(std::ostream& stream, const uint17_t& value) {
    stream << value.ToInt();
    return stream;
}

std::istream& operator>>(std::istream& stream, uint17_t r) {
    uint32_t input;
    stream >> input;
    r = input;
    return stream;
}
