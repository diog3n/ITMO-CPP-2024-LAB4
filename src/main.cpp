#include "smatrix.hpp"
#include "iostream"
#include "cassert"

#define TEST_LABEL_VAR_NAME __test_label__

#define TEST(__label) \
        { const char *TEST_LABEL_VAR_NAME = #__label;

#define END_TEST \
        std::cout << "Test " << TEST_LABEL_VAR_NAME << " OK!" << std::endl; }

int main(int argc, char **argv) {
        TEST(matrix equality) {
                SparseMatrix mat1(3, 5);
                SparseMatrix mat2(3, 5);

                mat1.Set(0, 1, 3);
                mat1.Set(0, 2, 3);

                mat2.Set(0, 1, 2);
                mat2.Set(0, 3, 1);

                assert(mat1 != mat2);

                SparseMatrix mat3(3, 5);
                SparseMatrix mat4(3, 5);

                mat3.Set(0, 1, 3);
                mat3.Set(0, 2, 3);

                mat4.Set(0, 1, 3);
                mat4.Set(0, 2, 3);

                assert(mat3 == mat4);

                END_TEST;
        }

        TEST(matrix sum) {
                SparseMatrix mat1 = {
                        { 1, 2, 3 },
                        { 3, 4, 5 },
                };

                SparseMatrix mat2 = {
                        { 2, 3, 4 },
                        { 4, 5, 6 },
                };

                SparseMatrix sum = {
                        { 3, 5, 7  },
                        { 7, 9, 11 }
                };

                assert((mat1 + mat2) == sum);

                END_TEST;
        }

        TEST(matrix diff) {
                SparseMatrix mat1 = {
                        { 1, 2, 3 },
                        { 3, 4, 5 },
                };

                SparseMatrix mat2 = {
                        { 2, 3, 4 },
                        { 4, 5, 6 },
                };

                SparseMatrix diff = {
                        { -1, -1, -1 },
                        { -1, -1, -1 }
                };

                assert((mat1 - mat2) == diff);

                END_TEST;
        }

        TEST(matrix multiplication) {
                SparseMatrix mat1 = {
                        { 3, 2 },
                        { 1, 4 },
                        { 5, 3 }
                };

                SparseMatrix mat2 = {
                        { 3, 2 },
                        { 1, 4 }
                };

                SparseMatrix result = {
                        { 11, 14 },
                        { 7,  18 },
                        { 18, 22 }
                };

                assert((mat1 * mat2) == result);

                END_TEST;
        }

        TEST(matrix-vector multiplication) {
                SparseMatrix mat = {
                        { 1, -1, 2 },
                        { 0, -3, 1 }
                };

                SparseVector<double> vec = { 2, 1, 0 };

                SparseMatrix result = {
                        { 1  },
                        { -3 }
                };

                assert((mat * vec) == result);

                END_TEST;
        }

        TEST(matrix determinant) {
                SparseMatrix mat1 = {
                        { 1, 2, 3 },
                        { 4, 5, 6 },
                        { 7, 8, 9 },
                };

                assert(mat1.Determinant() == 0);

                SparseMatrix mat2 = {
                        { 4,   3,   2,  2 },
                        { 0,   1,  -3,  3 },
                        { 0,  -1,   3,  3 },
                        { 0,   3,   1,  1 }
                };

                assert(mat2.Determinant() == -240);

                SparseMatrix mat3 = {
                        { 4, 5, 2, 5, 1 },
                        { 1, 4, 3, 1, 0 },
                        { 2, 1, 1, 3, 5 },
                        { 2, 3, 1, 4, 5 },
                        { 12, 1, 3, 4, 2}
                };

                assert(mat3.Determinant() == 503);

                END_TEST;
        }

        TEST(matrix inverse) {
                SparseMatrix mat1 = {
                        { 2, 1 },
                        { 7, 4 },
                };

                SparseMatrix result = {
                        { 4, -1 },
                        { -7, 2 }
                };

                assert(result == mat1.Inverse());

                SparseMatrix mat2 = {
                        { 4, 5, 2, 5, 1 },
                        { 1, 4, 3, 1, 0 },
                        { 2, 1, 1, 3, 5 },
                        { 2, 3, 1, 4, 5 },
                        { 12, 1, 3, 4, 2}
                };

                SparseMatrix identity = {
                        { 1, 0, 0, 0, 0 },
                        { 0, 1, 0, 0, 0 },
                        { 0, 0, 1, 0, 0 },
                        { 0, 0, 0, 1, 0 },
                        { 0, 0, 0, 0, 1 },
                };

                auto inv = mat2.Inverse();

                assert(SparseMatrix(mat2 * inv) == identity);
                END_TEST;
        }

        return 0;
}