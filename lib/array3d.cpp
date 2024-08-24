#include "array3d.h"

void swap(size_t& lhs, size_t& rhs) {
    size_t temp = lhs;
    lhs = rhs;
    rhs = temp;
}

void swap(uint16_t*& lhs, uint16_t*& rhs) {
    uint16_t* temp = lhs;
    lhs = rhs;
    rhs = temp;
}

void swap(uint8_t*& lhs, uint8_t*& rhs) {
    uint8_t* temp = lhs;
    lhs = rhs;
    rhs = temp;
}

Array1D::Array1D(uint16_t* full_parts, uint8_t* last_bits, uint64_t full_part_index)
        :full_parts_(full_parts), last_bits_(last_bits), index(full_part_index) {}

uint17_t Array1D::operator[](uint64_t z) {
    return {last_bits_, index + z, &full_parts_[index + z]};
}

Array2D::Array2D(uint16_t* full_parts, uint8_t* last_bits, uint64_t full_part_index, size_t z)
        :full_parts_(full_parts), last_bits_(last_bits), index(full_part_index), z_(z) {}

Array1D Array2D::operator[](uint64_t y) const {
    return {full_parts_, last_bits_, index + y * z_};
}

Array3D::Array3D(size_t x, size_t y, size_t z): x_(x), y_(y), z_(z) {
    size_t size = x * y * z;
    full_parts_ = new uint16_t[size] {};
    last_bits_ = new uint8_t[size / 8 + 1] {};
}

Array3D::~Array3D() {
    delete[] full_parts_;
    delete[] last_bits_;
}

Array3D::Array3D(const Array3D& other) {
    x_ = other.GetX();
    y_ = other.GetY();
    z_ = other.GetZ();
    size_t size = x_ * y_ * z_;
    full_parts_ = new uint16_t[size];
    last_bits_ = new uint8_t[size / 8 + 1];

    for (uint64_t i = 0; i < x_; ++i) {
        for (uint64_t j = 0; j < y_; ++j) {
            for (uint64_t k = 0; k < z_; ++k) {
                (*this)[i][j][k] = other[i][j][k].ToInt();
            }
        }
    }
}

Array3D& Array3D::operator=(Array3D other) {
    swap(other);
    return *this;
}

size_t Array3D::GetX() const {
    return x_;
}

size_t Array3D::GetY() const {
    return y_;
}

size_t Array3D::GetZ() const {
    return z_;
}

void Array3D::swap(Array3D& other) {
    ::swap(x_, other.x_);
    ::swap(y_, other.y_);
    ::swap(z_, other.z_);
    ::swap(full_parts_, other.full_parts_);
    ::swap(last_bits_, other.last_bits_);
}

Array3D Array3D::make_array(size_t x, size_t y, size_t z) {
    return {x, y, z};
}

Array2D Array3D::operator[](uint64_t x) const {
    return {full_parts_, last_bits_, x * y_ * z_, z_};
}

Array3D CombineArrays(const Array3D& lhs, const Array3D& rhs, uint32_t(*function)(const uint17_t&, const uint17_t&)) {
    size_t x = lhs.GetX();
    size_t y = lhs.GetY();
    size_t z = lhs.GetZ();
    Array3D new_array(x, y, z);
    for (uint64_t i = 0; i < x; ++i) {
        for (uint64_t j = 0; j < y; ++j) {
            for (uint64_t k = 0; k < z; ++k) {
                new_array[i][j][k] = function(lhs[i][j][k], rhs[i][j][k]);
            }
        }
    }
    return new_array;
}

Array3D operator+(const Array3D& lhs, const Array3D& rhs) {
    return CombineArrays(lhs, rhs, ToSum);
}

Array3D operator-(const Array3D& lhs, const Array3D& rhs) {
    return CombineArrays(lhs, rhs, ToDif);
}

Array3D operator*(const Array3D& lhs, const uint32_t rhs) {
    size_t x = lhs.GetX();
    size_t y = lhs.GetY();
    size_t z = lhs.GetZ();
    Array3D new_array(x, y, z);
    for (uint64_t i = 0; i < x; ++i) {
        for (uint64_t j = 0; j < y; ++j) {
            for (uint64_t k = 0; k < z; ++k) {
                new_array[i][j][k] = lhs[i][j][k].ToInt() * rhs;
            }
        }
    }
    return new_array;
}

std::istream& operator>>(std::istream& stream, const Array3D& arr) {
    size_t x = arr.GetX();
    size_t y = arr.GetY();
    size_t z = arr.GetZ();
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                stream >> arr[i][j][k];
            }
        }
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Array3D& arr) {
    size_t x = arr.GetX();
    size_t y = arr.GetY();
    size_t z = arr.GetZ();
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                stream << arr[i][j][k] << ' ';
            }
            stream << '\n';
        }
        stream << "---------------------------------\n";
    }
    return stream;
}
