#include "smatrix.hpp"
#include "vmatrix.hpp"
#include "iostream"
#include "cassert"
#include "chrono"

#define TEST_LABEL_VAR_NAME __test_label__

#define TEST(__label) \
    { const char *TEST_LABEL_VAR_NAME = #__label;

#define END_TEST \
    std::cout << "Test " << TEST_LABEL_VAR_NAME << " OK!" << std::endl; }

void SmatrixTest() {
    TEST(smatrix equality) {
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

    TEST(smatrix sum) {
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

    TEST(smatrix diff) {
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

    TEST(smatrix multiplication) {
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

    TEST(smatrix-vector multiplication) {
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

    TEST(smatrix determinant) {
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

    TEST(smatrix inverse) {
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

    TEST(smatrix transpose) {
        SparseMatrix mat = {
            { 1, 2, 3 },
            { 2, 3, 4 }
        };

        SparseMatrix result = {
            { 1, 2 },
            { 2, 3 },
            { 3, 4 }
        };

        assert(SparseMatrix(mat.Transpose()) == result);

        END_TEST;
    }


    TEST(smatrix scalar operations) {
        SparseMatrix mat({
            { 1, 0, 3 },
            { 0, 5, 0 },
            { 7, 0, 9 }
        });

        SparseMatrix expected_add({
            { 3, 2, 5  },
            { 2, 7, 2  },
            { 9, 2, 11 }
        });
        assert((mat + 2) == expected_add);

        SparseMatrix expected_sub({
            {  0, -1,  2 },
            { -1,  4, -1 },
            {  6, -1,  8 }
        });
        assert((mat - 1) == expected_sub);

        SparseMatrix expected_mul({
            { 2,  0, 6  },
            { 0, 10, 0  },
            { 14, 0, 18 }
        });
        assert((mat * 2) == expected_mul);

        SparseMatrix expected_div({
            { 0.5, 0, 1.5 },
            { 0, 2.5, 0   },
            { 3.5, 0, 4.5 }
        });
        assert((mat / 2) == expected_div);

        SparseMatrix expected_pow({
            { 1,  0,  9 },
            { 0,  25, 0 },
            { 49, 0, 81 }
        });

        assert(mat.PowElements(2) == expected_pow);

        END_TEST;
    }
}

void VmatrixTest() {
    TEST(vmatrix equality) {
        VectorMatrix<double> mat1 = {
            { 1, 2, 3 },
            { 2, 3, 4 }
        };

        VectorMatrix<double> mat2 = {
            { 1, 2, 3 },
            { 2, 3, 5 }
        };

        assert(mat1 != mat2);

        VectorMatrix<double> mat3 = {
            { 1, 2, 3 },
            { 2, 3, 4 }
        };

        VectorMatrix<double> mat4 = {
            { 1, 2, 3 },
            { 2, 3, 4 }
        };

        assert(mat3 == mat4);

        END_TEST;
    }

    TEST(vmatrix sum) {
        VectorMatrix<double> mat1 = {
            { 1, 2, 3 },
            { 3, 4, 5 },
        };

        VectorMatrix<double> mat2 = {
            { 2, 3, 4 },
            { 4, 5, 6 },
        };

        VectorMatrix<double> sum = {
            { 3, 5, 7  },
            { 7, 9, 11 }
        };

        assert((mat1 + mat2) == sum);

        END_TEST;
    }

    TEST(vmatrix diff) {
        VectorMatrix<double> mat1 = {
            { 1, 2, 3 },
            { 3, 4, 5 },
        };

        VectorMatrix<double> mat2 = {
            { 2, 3, 4 },
            { 4, 5, 6 },
        };

        VectorMatrix<double> diff = {
            { -1, -1, -1 },
            { -1, -1, -1 }
        };

        assert((mat1 - mat2) == diff);

        END_TEST;
    }

    TEST(vmatrix multiplication) {
        VectorMatrix<double> mat1 = {
            { 3, 2 },
            { 1, 4 },
            { 5, 3 }
        };

        VectorMatrix<double> mat2 = {
            { 3, 2 },
            { 1, 4 }
        };

        VectorMatrix<double> result = {
            { 11, 14 },
            { 7,  18 },
            { 18, 22 }
        };

        assert((mat1 * mat2) == result);

        END_TEST;
    }
}

void TestSpeed() {
    TEST(test vmatrix vs smatrix) {
        VectorMatrix<double> vmat1(100, 100);
        SparseMatrix         smat1(100, 100);

        double init1 = 244;
        double step1 = 40;

        for (int i = 0; i < vmat1.Rows(); i++) {
            for (int j = 0; j < vmat1.Cols(); j++, init1 += step1) {
                if (i % 3 == 0) {
                    vmat1.Set(i, j, 0);
                    smat1.Set(i, j, 0);
                    continue;
                }

                vmat1.Set(i, j, init1);
                smat1.Set(i, j, init1);
            }
        }

        std::cout << "vmat1 real size is " << vmat1.Cols() * vmat1.Rows() << std::endl;
        std::cout << "smat1 real size is " << smat1.RealSize() << std::endl;

        VectorMatrix<double> vmat2(100, 100);
        SparseMatrix         smat2(100, 100);

        double init2 = 349;
        double step2 = 21;

        for (int i = 0; i < vmat2.Rows(); i++) {
            for (int j = 0; j < vmat2.Cols(); j++, init2 += step2) {
                if ((i + j) % 3 == 0) {
                    vmat2.Set(i, j, 0);
                    smat2.Set(i, j, 0);
                    continue;
                }

                vmat2.Set(i, j, init2);
                smat2.Set(i, j, init2);
            }
        }

        std::cout << "vmat2 real size is " << vmat2.Cols() * vmat2.Rows() << std::endl;
        std::cout << "smat2 real size is " << smat2.RealSize() << std::endl;

        VectorMatrix<double> vresult(100, 100);
        SparseMatrix         sresult(100, 100);

        // TEST VMAT

        std::cout << "VectorMatrix test start" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        vresult = vmat1 * vmat2;

        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << "VectorMatrix multiplication took " << duration.count() << " nanoseconds." << std::endl;

        // TEST SMAT

        std::cout << "SparseMatrix test start" << std::endl;

        start = std::chrono::high_resolution_clock::now();

        sresult = smat1 * smat2;

        end = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        std::cout << "SparseMatrix multiplication took " << duration.count() << " nanoseconds." << std::endl;

        bool equal = true;
        for (int i = 0; i < sresult.Rows(); i++) {
            for (int j = 0; j < sresult.Cols(); j++) {
                if (vresult.Get(i, j) != sresult.Get(i, j)) {
                    equal = false;
                    break;
                }
            }
        }

        assert(equal);

        std::cout << "vresult real size is " << vresult.Cols() * vresult.Rows() << std::endl;
        std::cout << "sresult real size is " << sresult.RealSize() << std::endl;

        END_TEST;
    }
}

int main(int argc, char **argv) {
    SmatrixTest();
    VmatrixTest();
    TestSpeed();
    return 0;
}