#pragma once

#include "uint17.h"
#include <cstdint>
#include <iostream>

class Array1D {
public:
    Array1D(uint16_t* full_parts, uint8_t* last_bits, uint64_t full_part_index);

    uint17_t operator[](uint64_t z);

private:
    uint16_t* full_parts_;
    uint8_t* last_bits_;
    uint64_t index;
};

class Array2D {
public:
    Array2D(uint16_t* full_parts, uint8_t* last_bits, uint64_t full_part_index, size_t z);

    Array1D operator[](uint64_t y) const;

private:
    uint16_t* full_parts_;
    uint8_t* last_bits_;
    uint64_t index;
    size_t z_;
};

class Array3D {
public:
    Array3D(size_t x, size_t y, size_t z);

    ~Array3D();

    Array3D(const Array3D& other);

    Array3D& operator=(Array3D other);

    size_t GetX() const;

    size_t GetY() const;

    size_t GetZ() const;

    static Array3D make_array(size_t x, size_t y, size_t z);

    Array2D operator[](uint64_t x) const;

private:
    size_t x_;
    size_t y_;
    size_t z_;
    uint16_t* full_parts_;
    uint8_t* last_bits_;

    void swap(Array3D& other);
};

Array3D CombineArrays(const Array3D& lhs, const Array3D& rhs, uint32_t(*function)(const uint17_t&, const uint17_t&));

Array3D operator+(const Array3D& lhs, const Array3D& rhs);

Array3D operator-(const Array3D& lhs, const Array3D& rhs);

Array3D operator*(const Array3D& lhs, const uint32_t rhs);

static void swap(size_t& lhs, size_t& rhs);

static void swap(uint16_t*& lhs, uint16_t*& rhs);

static void swap(uint8_t*& lhs, uint8_t*& rhs);

std::ostream& operator<<(std::ostream& stream, const Array3D& arr);

std::istream& operator>>(std::istream& stream, const Array3D& arr);
