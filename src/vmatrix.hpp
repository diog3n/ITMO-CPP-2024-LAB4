#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <vector>

template<typename T>
class VectorMatrix {
public:
    using size_type = size_t;
    using index_type = size_t;
    using value_type = T;
    using row_type = std::vector<value_type>;
    using container_type = std::vector<row_type>;

private:

public:
    VectorMatrix()
        : rows_(0)
        , cols_(0)
    { /* nothing */ }

    VectorMatrix(size_type rows, size_type cols)
        : rows_(rows)
        , cols_(cols)
    {
            data_.resize(rows_);
            for (row_type& row : data_) {
                row.resize(cols_);
            }
    }

    VectorMatrix(const VectorMatrix& other) = default;
    VectorMatrix(const std::initializer_list<std::initializer_list<value_type>>& values) {
        rows_ = values.size();
        cols_ = values.begin()->size();

        data_.resize(rows_);
        for (row_type& row : data_) {
            row.resize(cols_);
        }

        index_type i = 0;
        for (const auto& row : values ) {
                index_type j = 0;
                if (i >= rows_) {
                        break;
                }

                for (const auto& el : row) {
                        if (j >= cols_)
                                break;

                        Set(i, j, el);
                        j++;
                }
                i++;
        }
    }

    VectorMatrix& operator=(const VectorMatrix& other) = default;

    VectorMatrix&
    operator=(const std::initializer_list<std::initializer_list<value_type>>& values) {
        VectorMatrix<value_type> other(values);

        *this = values;
        return *this;
    }


    bool operator==(const VectorMatrix& other) const {
        if (!SameShape(*this, other))
            return false;

        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < cols_; j++) {
                if (data_[i][j] != other.data_[i][j])
                    return false;
            }
        }

        return true;
    }

    bool operator!=(const VectorMatrix& other) const {
        return !(*this == other);
    }

    static bool SameShape(const VectorMatrix& lhs, const VectorMatrix& rhs) {
        if ((lhs.cols_ != rhs.cols_) || (lhs.rows_ != rhs.rows_))
            return false;

        return true;
    }

    VectorMatrix operator+(const VectorMatrix& other) const {
        VectorMatrix<value_type> result(rows_, cols_);
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < cols_; j++) {
                result.data_[i][j] = data_[i][j] + other.data_[i][j];
            }
        }

        return result;
    }

    VectorMatrix operator-(const VectorMatrix& other) const {
        VectorMatrix<value_type> result(rows_, cols_);
        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < cols_; j++) {
                result.data_[i][j] = data_[i][j] - other.data_[i][j];
            }
        }

        return result;
    }

    VectorMatrix operator*(const VectorMatrix& other) const {
        VectorMatrix<value_type> result(rows_, cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < other.cols_; ++j) {
                for (size_t k = 0; k < cols_; ++k) {
                    result.data_[i][j] += data_[i][k] * other.data_[k][j];
                }
            }
        }

        return result;
    }

    value_type Get(index_type row, index_type col) const {
        return data_[row][col];
    }

    void Set(index_type row, index_type col, const value_type& value) {
        data_[row][col] = value;
    }

    size_type Cols() const {
        return cols_;
    }

    size_type Rows() const {
        return rows_;
    }

private:
    container_type data_;
    size_type rows_;
    size_type cols_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const VectorMatrix<T>& mat) {
    bool is_first = true;
    for (const auto row : mat) {
        bool is_first_el = true;
        for (const auto el : row) {
            if (!is_first_el)
                out << ' ';

            is_first_el = false;
            out << el;
        }
        if (!is_first)
            out << std::endl;

        is_first = false;
    }

    return out;
}