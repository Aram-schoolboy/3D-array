#include <lib/array3d.h>
#include <gtest/gtest.h>
#include <cmath>

uint32_t static const kMaxValue = pow(2, 17) - 1;

bool IsEqual(const Array3D& array3d, const std::vector<std::vector<std::vector<uint32_t>>>& array, size_t x, size_t y, size_t z) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                if (array3d[i][j][k].ToInt() != array[i][j][k]) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::vector<std::vector<std::vector<uint32_t>>> Make3DVector(size_t x, size_t y, size_t z) {
    std::vector<std::vector<std::vector<uint32_t>>> array;
    array.resize(x);
    for (size_t i = 0; i < x; ++i) {
        array[i].resize(y);
        for (size_t j = 0; j < y; ++j) {
            array[i][j].resize(z);
        }
    }
    return array;
}

void Fill(const Array3D& array3d, size_t x, size_t y, size_t z, size_t max_value) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                array3d[i][j][k] = std::min(i * j * k + i + j + k, max_value);
            }
        }
    }
}

void Fill(std::vector<std::vector<std::vector<uint32_t>>>& array, size_t x, size_t y, size_t z, size_t max_value) {
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                array[i][j][k] = std::min(i * j * k + i + j + k, max_value);
            }
        }
    }
}

TEST(Array3DTestSuite, DefaultValuesTest) {
    size_t x = 100;
    size_t y = 200;
    size_t z = 300;
    Array3D array3d(x, y, z);
    long long sum = 0;
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                sum += array3d[i][j][k].ToInt();
            }
        }
    }

    ASSERT_EQ(sum, 0);
}

TEST(Array3DTestSuite, SizeTest) {
    size_t x = 100;
    size_t y = 200;
    size_t z = 300;
    size_t size = x * y * z;
    const int kConstantSpace = 40;
    Array3D array3d(x, y, z);
    size = sizeof(uint16_t) * size + sizeof(uint8_t) * size / 8 + 1 + sizeof(array3d);

    ASSERT_TRUE(size <= x * y * z * 17 / 8 + 1 + kConstantSpace);
}

TEST(Array3DTestSuite, AssignmentTest) {
    size_t x = 100;
    size_t y = 200;
    size_t z = 300;
    std::vector<std::vector<std::vector<uint32_t>>> array = Make3DVector(x, y, z);
    Array3D array3d(x, y, z);

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                array3d[i][j][k] = i + j + k;
                array[i][j][k] = i + j + k;
                if (i * j * k % 152 == 0) {
                    array3d[i][j][k] = kMaxValue;
                    array[i][j][k] = kMaxValue;
                }
            }
        }
    }

    ASSERT_TRUE(IsEqual(array3d, array, x, y, z));
}

TEST(Array3DTestSuite, SelfAssignmentTest) {
    size_t x = 10;
    size_t y = 20;
    size_t z = 30;
    Array3D array3d(x, y, z);
    array3d[5][10][15] = array3d[5][10][15] = array3d[5][10][15] = array3d[5][10][15] = 10;
    array3d[5][10][15] = array3d[5][10][15];
    ASSERT_EQ(array3d[5][10][15].ToInt(), 10);
}

TEST(Array3DTestSuite, SumTest) {
    size_t x = 100;
    size_t y = 99;
    size_t z = 100;
    std::vector<std::vector<std::vector<uint32_t>>> array1 = Make3DVector(x, y, z);
    std::vector<std::vector<std::vector<uint32_t>>> array2 = Make3DVector(x, y, z);
    Array3D array3d1(x, y, z);

    Fill(array1, x, y, z, kMaxValue / 2);
    Fill(array3d1, x, y, z, kMaxValue / 2);

    Array3D array3d2 = array3d1 + array3d1;
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                array2[i][j][k] = array1[i][j][k] + array1[i][j][k];
            }
        }
    }

    ASSERT_TRUE(IsEqual(array3d2, array2, x, y, z));
}

TEST(Array3DTestSuite, DifTest) {
    size_t x = 100;
    size_t y = 99;
    size_t z = 100;
    std::vector<std::vector<std::vector<uint32_t>>> array1 = Make3DVector(x, y, z);
    std::vector<std::vector<std::vector<uint32_t>>> array3 = Make3DVector(x, y, z);
    Array3D array3d1(x, y, z);
    Array3D array3d2(x, y, z);
    Array3D array3d3(x, y, z);

    Fill(array1, x, y, z, kMaxValue);
    Fill(array3d1, x, y, z, kMaxValue);

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                if (array3d1[i][j][k].ToInt() < i + j + k) {
                    array3d2[i][j][k] = 0;
                    array3[i][j][k] = array1[i][j][k];
                } else {
                    array3d2[i][j][k] = array3d1[i][j][k].ToInt() - i - j - k;
                    array3[i][j][k] = i + j + k;
                }
            }
        }
    }

    array3d3 = array3d1 - array3d2;

    ASSERT_TRUE(IsEqual(array3d3, array3, x, y, z));
}

TEST(Array3DTestSuite, MultTest) {
    size_t x = 100;
    size_t y = 99;
    size_t z = 100;
    std::vector<std::vector<std::vector<uint32_t>>> array1 = Make3DVector(x, y, z);
    std::vector<std::vector<std::vector<uint32_t>>> array2 = Make3DVector(x, y, z);

    Array3D array3d1(x, y, z);
    Array3D array3d2(x, y, z);

    uint16_t q = 4;
    Fill(array1, x, y, z, kMaxValue / q);
    Fill(array3d1, x, y, z, kMaxValue / q);

    array3d2 = array3d1 * q;
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                array2[i][j][k] = array1[i][j][k] * q;
            }
        }
    }

    ASSERT_TRUE(IsEqual(array3d2, array2, x, y, z));
}

TEST(Array3DTestSuite, MixAssignmentTest) {
    size_t x = 1;
    size_t y = 1;
    size_t z = 1;

    Array3D arr1 = Array3D::make_array(x, y, z);
    Array3D arr2(x, y, z);
    Array3D arr3 = arr1;

    arr2[0][0][0] = arr3[0][0][0] = arr3[0][0][0] = arr2[0][0][0] = arr1[0][0][0] = 1234;
    ASSERT_EQ(arr1[0][0][0].ToInt(), 1234);
    ASSERT_EQ(arr2[0][0][0].ToInt(), 1234);
    ASSERT_EQ(arr3[0][0][0].ToInt(), 1234);

    arr2[0][0][0] = 2;
    arr1[0][0][0] = arr1[0][0][0] = 1;
    arr3[0][0][0] = 3;

    ASSERT_EQ(arr1[0][0][0].ToInt(), 1);
    ASSERT_EQ(arr2[0][0][0].ToInt(), 2);
    ASSERT_EQ(arr3[0][0][0].ToInt(), 3);
}

TEST(Array3DTestSuite, MixOperationsTest) {
    size_t x = 1;
    size_t y = 1;
    size_t z = 1;

    Array3D arr1 = Array3D::make_array(x, y, z);
    Array3D arr2(x, y, z);
    arr1[0][0][0] = 1;
    arr2[0][0][0] = 2;

    Array3D arr3 = (arr1 + arr2) * 10 - arr2 - arr2 * 1 + arr1 * 0; // (1+2) * 10 - 2 - 2*1 + 1*0 = 26
    ASSERT_EQ(arr3[0][0][0].ToInt(), 26);
}

TEST(Array3DTestSuite, InputTest) {
    size_t x = 100;
    size_t y = 99;
    size_t z = 100;
    Array3D arr(x, y, z);
    std::vector<std::vector<std::vector<uint32_t>>> vector = Make3DVector(x, y, z);
    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                uint32_t num = i + j + k;
                std::stringstream ss;
                if (num % 152 == 0) {
                    num = kMaxValue;
                }
                ss << num;
                ss >> arr[i][j][k];
                vector[i][j][k] = num;
            }
        }
    }

    ASSERT_TRUE(IsEqual(arr, vector, x, y, z));
}

TEST(Array3DTestSuite, OutputTest) {
    size_t x = 100;
    size_t y = 99;
    size_t z = 100;
    Array3D arr = Array3D::make_array(x, y, z);
    std::vector<std::vector<std::vector<uint32_t>>> vector = Make3DVector(x, y, z);

    Fill(arr, x, y, z, kMaxValue);
    Fill(vector, x, y, z, kMaxValue);

    for (size_t i = 0; i < x; ++i) {
        for (size_t j = 0; j < y; ++j) {
            for (size_t k = 0; k < z; ++k) {
                std::stringstream ss;
                uint32_t number;
                ss << arr[i][j][k];
                ss >> number;
                arr[i][j][k] = number;
            }
        }
    }

    ASSERT_TRUE(IsEqual(arr, vector, x, y, z));
}
