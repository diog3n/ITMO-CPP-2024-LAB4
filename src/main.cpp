#include "smatrix.hpp"
#include "iostream"

int main(int argc, char **argv) {
	SparseVector<int> svec1(5);

	for (int i = 0; i < svec1.size(); i++) {
		std::cout << svec1.Get(i) << std::endl;
	}

	std::cout << std::endl;

	for (int val : svec1) {
		std::cout << val << std::endl;
	}

	std::cout << std::endl;

	SparseVector<int> svec2(5);

	for (int i = 0; i < svec2.size(); i++) {
		std::cout << svec2.Get(i) << std::endl;
	}

	std::cout << std::endl;

	{
		SparseMatrix mat1(3, 5);
		SparseMatrix mat2(3, 5);

		mat1.Set(0, 1, 3);
		mat1.Set(0, 2, 3);

		mat2.Set(0, 1, 2);
		mat2.Set(0, 3, 1);

		if (mat1 == mat2) std::cout << "equal" << std::endl;
		else std::cout << "not equal" << std::endl;

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		for (const SparseMatrix::row_type& row : mat2) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		SparseMatrix mat3 = mat1 + mat2;

		for (const SparseMatrix::row_type& row : mat3) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1(2, 2);
		SparseMatrix mat2(2, 2);

		mat1.Set(0, 0, 1);
		mat1.Set(0, 1, 2);
		mat1.Set(1, 0, 3);
		mat1.Set(1, 1, 4);

		mat2.Set(0, 0, 1);
		mat2.Set(0, 1, 2);
		mat2.Set(1, 0, 3);
		mat2.Set(1, 1, 4);

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		for (const SparseMatrix::row_type& row : mat2) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		auto result = mat1 * mat2;

		for (const SparseMatrix::row_type& row : result) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 1, 0, 1 },
			{ 2, 1, 1 },
			{ 0, 1, 1 },
			{ 1, 1, 2 },
		};

		SparseMatrix mat2 = {
			{ 1, 2, 1 },
			{ 2, 3, 0 },
			{ 4, 2, 2 }
		};

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		for (const SparseMatrix::row_type& row : mat2) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		auto result = mat1 * mat2;

		for (const SparseMatrix::row_type& row : result) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 1, 0, 1 },
			{ 2, 1, 1 },
			{ 0, 1, 1 },
			{ 1, 1, 2 },
		};

		SparseMatrix mat2 = {
			{ 1 },
			{ 2 },
			{ 4 }
		};

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		for (const SparseMatrix::row_type& row : mat2) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		auto result = mat1 * mat2;

		for (const SparseMatrix::row_type& row : result) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 1, 0, 1 },
			{ 2, 1, 1 },
			{ 0, 1, 1 },
		};

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		SparseMatrix mat2 = mat1.SubMatrix(1, 1);

		for (const SparseMatrix::row_type& row : mat2) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 1, 2, 3 },
			{ 4, 5, 6 },
			{ 7, 8, 9 },
		};

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		std::cout << "Determinant = " << mat1.Determinant() << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 4,   3,   2,  2 },
			{ 0,   1,  -3,  3 },
			{ 0,  -1,   3,  3 },
			{ 0,   3,   1,  1 }
		};

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		std::cout << "Determinant = " << mat1.Determinant() << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 4, 5, 2, 5, 1 },
			{ 1, 4, 3, 1, 0 },
			{ 2, 1, 1, 3, 5 },
			{ 2, 3, 1, 4, 5 },
			{ 12, 1, 3, 4, 2}
		};

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;

		std::cout << "Determinant = " << mat1.Determinant() << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 4, 5, 2, 5, 1 },
			{ 1, 4, 3, 1, 0 },
			{ 2, 1, 1, 3, 5 },
			{ 2, 3, 1, 4, 5 },
			{ 12, 1, 3, 4, 2}
		};

		mat1.RowOperation(0, 2, SparseMatrix::ADD);

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 4, 5, 2, 5, 1 },
			{ 1, 4, 3, 1, 0 },
			{ 2, 1, 1, 3, 5 },
			{ 2, 3, 1, 4, 5 },
			{ 12, 1, 3, 4, 2}
		};

		mat1.ColOperation(0, 2, SparseMatrix::ADD);

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = MakeIdentityMatrix<double>(5);

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = MakeIdentityMatrix<double>(5);

		for (const SparseMatrix::row_type& row : mat1) {
			for (int elem : row) {
				std::cout << elem << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

	{
		SparseMatrix mat1 = {
			{ 2, 1 },
			{ 7, 4 },
		};

		auto inv = mat1.Inverse();

		std::cout << mat1 << std::endl;
		std::cout << inv << std::endl;

		std::cout << mat1 * inv << std::endl;

	}

	{
		SparseMatrix mat1 = {
			{ 4, 5, 2, 5, 1 },
			{ 1, 4, 3, 1, 0 },
			{ 2, 1, 1, 3, 5 },
			{ 2, 3, 1, 4, 5 },
			{ 12, 1, 3, 4, 2}
		};


		auto inv = mat1.Inverse();

		std::cout << mat1 << std::endl;
		std::cout << inv << std::endl;

		std::cout << mat1 * inv << std::endl;
	}

	return 0;
}