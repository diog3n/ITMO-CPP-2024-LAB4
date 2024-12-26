#ifndef _SMATRIX_H_
#define _SMATRIX_H_

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <ostream>
#include <pthread.h>
#include <stdexcept>
#include <map>
#include <iostream>
#include <type_traits>
#include <cmath>

constexpr char INDEX_OOB[] = "Index out of bounds.";
constexpr char ITER_OOB [] = "Dereferencing an out of bounds iterator.";
constexpr char ROW_OOB  [] = "Row out of bounds.";
constexpr char COL_OOB  [] = "Col out of bounds.";

template <typename T>
class SparseVector {
public:
    using index_type = std::size_t;
    using size_type  = std::size_t;
    using map_type   = std::map<index_type, T>;
    using value_type = T;

    class Iter {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = SparseVector<T>::value_type;
        using difference_type   = std::ptrdiff_t;
        using pointer           = value_type*;
        using reference         = value_type&;

        index_type index_;
        const map_type *data_;
        const size_type size_;

    public:
        explicit Iter(index_type index, const map_type *data, const size_type size)
            : index_(index)
            , data_(data)
            , size_(size) {}

        Iter& operator++() {
            index_++;
            return *this;
        }

        Iter operator++(int) {
            auto old = *this;
            ++(*this);
            return old;
        }

        Iter operator--() {
            index_--;
            return *this;
        }

        Iter operator--(int) {
            auto old = *this;
            --(*this);
            return old;
        }

        Iter operator+(index_type step) const {
            return Iter(index_ + step, data_, size_);
        }

        Iter operator-(index_type step) const {
            return Iter(index_ - step, data_, size_);
        }

        Iter operator-(const Iter& other) const {
            return Iter(index_ - other.index_, data_, size_);
        }

        Iter operator+=(index_type step) {
            index_ += step;
            return *this;
        }

        Iter operator-=(index_type step) {
            index_ -= step;
            return *this;
        }

        Iter operator-=(const Iter& other) {
            index_ -= other.index_;
            return *this;
        }

        bool operator!=(const Iter& other) const {
            return index_ != other.index_;
        }

        value_type operator*() const {
            if (index_ >= size_)
                throw std::invalid_argument(ITER_OOB);

            if (data_->find(index_) == data_->end()) {
                return value_type();
            } else {
                return data_->at(index_);
            }

            return value_type();
        }
    };

public:
    SparseVector(): size_(0) {}
    explicit SparseVector(size_type size): size_(size) {}
    SparseVector(size_type size, const value_type& value): size_(size) {
        for (size_type i = 0; i < size_; i++) {
            Set(i, value);
        }
    }

    SparseVector(const std::initializer_list<value_type>& values): size_(values.size()) {
        index_type i = 0;
        for (const auto& el : values) {
            Set(i, el);
            i++;
        }
    }

    void Set(index_type index, const value_type& value) {
        if (index >= size_) {
            size_ = index;
        }

        // If default value - do nothing;
        if (value == value_type()) {
            if (Has(index)) {
                data_.erase(data_.find(index));
            }

            return;
        }

        data_[index] = value;
    }

    value_type Get(index_type index) const {
        if (index >= size_) {
            throw std::invalid_argument(INDEX_OOB);
        }

        if (data_.find(index) == data_.end())
            return value_type();

        return data_.at(index);
    }

    const value_type& GetRef(index_type index) const {
        return data_.at(index);
    }

    value_type& operator[](index_type index) {
        return data_[index];
    }

    const value_type& operator[](index_type index) const {
        return data_[index];
    }

    bool Has(index_type index) const {
        return data_.find(index) != data_.end();
    }

    bool operator==(const SparseVector<value_type>& other) const {
        if (size_ != other.size_) return false;

        for (index_type i = 0; i < size_; i++) {
            if (Get(i) != other.Get(i))
                return false;
        }

        return true;
    }

    bool operator!=(const SparseVector<value_type>& other) const {
        return !(*this == other);
    }

    size_type size() const {
        return size_;
    }

    size_type RealSize() const {
        return data_.size();
    }

    Iter begin() {
        return Iter(0, &data_, size_);
    }

    Iter end() {
        return Iter(size_, &data_, size_);
    }

    Iter begin() const {
        return Iter(0, &data_, size_);
    }

    Iter end() const {
        return Iter(size_, &data_, size_);
    }

    Iter cbegin() const {
        return begin();
    }

    Iter cend() const {
        return end();
    }

private:
    size_type size_;
    map_type data_;
};

constexpr char MATRIX_SIZE_DIFFER        [] = "Matricies are of different sizes";
constexpr char MATRIX_INVALID_SIZES      [] = "Matricies have invalid sizes for multiplication";
constexpr char MATRIX_INVALID_INITIALIZER[] = "Invalid initializer list for a matrix";
constexpr char MATRIX_MUST_BE_SQUARE     [] = "Mastrix must be square to perform this operation";

inline int minus_one_pow(int pow) {
    return (pow % 2 == 0) ? 1 : -1;
}

template<typename T>
class SparseMatrixBase;

template<typename T>
SparseMatrixBase<T> MakeIdentityMatrix(std::size_t size);

template<typename T>
std::ostream& operator<<(std::ostream& out, SparseMatrixBase<T> matrix);

bool IsInsignificant(double num, double precision) {
    return num < precision;
}

bool IsEqual(double lhs, double rhs, double precision) {
    return std::abs(lhs - rhs) <= precision;
}

template<typename T>
class SparseMatrixBase {
public:
    using value_type = T;
    using index_type = std::size_t;
    using size_type = std::size_t;
    using row_type = SparseVector<value_type>;
    using container_type = SparseVector<row_type>;

    enum Operation {
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE
    };

    SparseMatrixBase() = delete;
    SparseMatrixBase(size_type rows, size_type cols)
        : rows_(rows)
        , cols_(cols)
        , data_(rows, row_type(cols)) {}

    SparseMatrixBase(std::initializer_list<std::initializer_list<value_type>> values)
        : rows_(values.size())
        , cols_(values.begin()->size())
        , data_(rows_, row_type(cols_)) {

        if (rows_ == 0 || cols_ == 0)
            throw std::invalid_argument(MATRIX_INVALID_INITIALIZER);

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

    SparseMatrixBase&
    operator=(std::initializer_list<std::initializer_list<value_type>> values) {
        *this = SparseMatrixBase(values);
        return *this;
    }

    void Set(index_type row, index_type col, const value_type& value) {
        if (row >= rows_) {
            throw std::invalid_argument(ROW_OOB);
        }

        if(col >= cols_) {
            throw std::invalid_argument(COL_OOB);
        }

        data_[row].Set(col, value);
    }

    value_type Get(index_type row, index_type col) const {
        if (row >= rows_) {
            throw std::invalid_argument(ROW_OOB);
        }

        if(col >= cols_) {
            throw std::invalid_argument(COL_OOB);
        }

        if (!data_.Has(row)) {
            return value_type();
        }

        if (!data_.GetRef(row).Has(col)) {
            return value_type();
        }

        return data_.Get(row).Get(col);
    }

    bool operator==(const SparseMatrixBase& other) const {
        if ((cols_ != other.cols_) || (rows_ != other.rows_)) {
            return false;
        }

        return data_ == other.data_;
    }

    bool operator!=(const SparseMatrixBase& other) const {
        return !(data_ == other.data_);
    }

    bool IsSquare() const {
        return cols_ == rows_;
    }

    static bool CanMultiply(const SparseMatrixBase& lhs, const SparseMatrixBase& rhs) {
    return lhs.Cols() == rhs.Rows();
    }

    SparseMatrixBase operator+(const SparseMatrixBase& other) const {
        if ((cols_ != other.cols_) || (rows_ != other.rows_)) {
            throw std::invalid_argument(MATRIX_SIZE_DIFFER);
        }

        SparseMatrixBase result(rows_, cols_);

        for (index_type row = 0; row < rows_; row++) {
            for (index_type col = 0; col < cols_; col++) {
                result.Set(row, col, data_.Get(row).Get(col)
                         + other.data_.Get(row).Get(col));
            }
        }

        return result;
    }

    SparseMatrixBase operator-(const SparseMatrixBase& other) const {
        if ((cols_ != other.cols_) || (rows_ != other.rows_)) {
            throw std::invalid_argument(MATRIX_SIZE_DIFFER);
        }

        SparseMatrixBase result(rows_, cols_);

        for (index_type row = 0; row < rows_; row++) {
            for (index_type col = 0; col < cols_; col++) {
                result.Set(row, col, data_.Get(row).Get(col)
                         - other.data_.Get(row).Get(col));
            }
        }

        return result;
    }


    SparseMatrixBase operator*(const SparseMatrixBase& other) const {
        SparseMatrixBase result(rows_, other.cols_);

        for (index_type row = 0; row < rows_; ++row) {
            for (index_type col = 0; col < other.cols_; ++col) {
                value_type sum = value_type();
                for (index_type k = 0; k < cols_; ++k) {
                    if (data_.GetRef(row).Has(k) && other.data_.GetRef(k).Has(col)) {
                        sum += data_.Get(row).Get(k) * other.data_.Get(k).Get(col);
                    }
                }
                if (sum != value_type()) {
                    result.Set(row, col, sum);
                }
            }
            }

        return result;
    }

    SparseMatrixBase operator*(const SparseVector<value_type>& vec) const {
        SparseMatrixBase<value_type> vec_mat = FromVector(vec);
        return (*this) * vec_mat;
    }

    SparseMatrixBase SubMatrix(index_type exclusion_row, index_type exclusion_col) const {
        SparseMatrixBase result(rows_ - 1, cols_ - 1);

        int i = 0;
        for (index_type row = 0; row < rows_; row++) {
            int j = 0;
            if (row == exclusion_row)
                continue;
            for (index_type col = 0; col < cols_; col++) {
                if (col == exclusion_col)
                    continue;
                result.Set(i, j, Get(row, col));
                j++;
            }
            i++;
        }

        return result;
    }

    SparseMatrixBase GetCol(index_type col) const {
        SparseMatrixBase result(rows_, 1);
        for (index_type i = 0; i < cols_; i++) {
            result.Set(i, 0, Get(i, col));
        }

        return result;
    }

    SparseMatrixBase& RowOperation(index_type row, const value_type& value, Operation op) {
        if (row >= rows_)
            throw std::invalid_argument(INDEX_OOB);

        for (index_type i = 0; i < cols_; i++) {
            switch (op) {
                case ADD:
                    Set(row, i, Get(row, i) + value);
                    break;
                case SUBTRACT:
                    Set(row, i, Get(row, i) - value);
                    break;
                case MULTIPLY:
                    Set(row, i, Get(row, i) * value);
                    break;
                case DIVIDE:
                    Set(row, i, Get(row, i) / value);
                    break;
            }
        }

        return *this;
    }

    SparseMatrixBase& ColOperation(index_type col, const value_type& value, Operation op) {
        if (col >= cols_)
            throw std::invalid_argument(INDEX_OOB);

        for (index_type i = 0; i < rows_; i++) {
            switch (op) {
                case ADD:
                    Set(i, col, Get(i, col) + value);
                    break;
                case SUBTRACT:
                    Set(i, col, Get(i, col) - value);
                    break;
                case MULTIPLY:
                    Set(i, col, Get(i, col) * value);
                    break;
                case DIVIDE:
                    Set(i, col, Get(i, col) / value);
                    break;
            }
        }

        return *this;
    }

    SparseMatrixBase Transpose() const {
        SparseMatrixBase result(cols_, rows_);

        for (index_type row = 0; row < rows_; ++row) {
            for (index_type col = 0; col < cols_; ++col) {
                if (data_.GetRef(row).Has(col)) {
                    value_type value = data_.Get(row).Get(col);
                    result.Set(col, row, value);
                }
            }
        }

        return result;
    }

    size_type RealSize() const {
        size_type rsize = 0;
        for (const auto& row : data_) {
            rsize += row.RealSize();
        }

        return rsize;
    }

    size_type Cols() const {
        return cols_;
    }

    size_type Rows() const {
        return rows_;
    }

    typename container_type::Iter begin() {
        return data_.begin();
    }

    typename container_type::Iter end() {
        return data_.end();
    }

    typename container_type::Iter begin() const {
        return data_.begin();
    }

    typename container_type::Iter end() const {
        return data_.end();
    }

    typename container_type::Iter cbegin() const {
        return begin();
    }

    typename container_type::Iter cend() const {
        return end();
    }

    size_type size() const;

    static SparseMatrixBase<value_type> FromVector(const SparseVector<value_type>& vec) {
        SparseMatrixBase<value_type> result(vec.size(), 1);

        for (int i = 0; i < vec.size(); i++) {
            result.Set(i, 0, vec.Get(i));
        }

        return result;
    }
protected:
    size_type rows_;
    size_type cols_;
    container_type data_;
};

class SparseMatrix : public SparseMatrixBase<double> {
public:
    using value_type = double;
    using index_type = std::size_t;
    using size_type = std::size_t;
    using row_type = SparseVector<value_type>;
    using container_type = SparseVector<row_type>;

    static constexpr double EPSYLON = 10e-6;
    using SparseMatrixBase<double>::operator*;
    using SparseMatrixBase<double>::operator+;
    using SparseMatrixBase<double>::operator-;

    SparseMatrix() = delete;

    SparseMatrix(size_type rows, size_type cols)
        : SparseMatrixBase<double>(rows, cols) { /* nothing */ }

    SparseMatrix(std::initializer_list<std::initializer_list<value_type>> values)
        : SparseMatrixBase<double>(values) { /* nothing */ }

    SparseMatrix(const SparseMatrixBase<double>& base)
        : SparseMatrixBase<double>(base) { /* nothing */ }

    bool IsInversable() const {
        return Determinant() != 0;
    }

    // Yes, I'm overloading it
    SparseMatrix SubMatrix(index_type exclusion_row, index_type exclusion_col) const {
        SparseMatrixBase<value_type> base(*this);
        return base.SubMatrix(exclusion_row, exclusion_col);
    }

    SparseMatrix operator+(double value) const {
        SparseMatrix result(*this);
        for (index_type row = 0; row < result.Rows(); ++row) {
            for (index_type col = 0; col < result.Cols(); ++col) {
                result.Set(row, col, result.Get(row, col) + value);
            }
        }
        return result;
    }

    SparseMatrix operator-(double value) const {
        SparseMatrix result(*this);
        for (index_type row = 0; row < result.Rows(); ++row) {
            for (index_type col = 0; col < result.Cols(); ++col) {
                result.Set(row, col, result.Get(row, col) - value);
            }
        }
        return result;
    }


    SparseMatrix operator*(double value) const {
        SparseMatrix result(*this);
        for (index_type row = 0; row < result.Rows(); ++row) {
            for (index_type col = 0; col < result.Cols(); ++col) {
                result.Set(row, col, result.Get(row, col) * value);
            }
        }
        return result;
    }

    SparseMatrix operator/(double value) const {
        if (std::abs(value) < EPSYLON) {
            throw std::invalid_argument("Division by zero");
        }
        SparseMatrix result(*this);
        for (index_type row = 0; row < result.Rows(); ++row) {
            for (index_type col = 0; col < result.Cols(); ++col) {
                result.Set(row, col, result.Get(row, col) / value);
            }
        }
        return result;
    }

    SparseMatrix PowElements(double exponent) const {
        SparseMatrix result(*this);
        for (index_type row = 0; row < result.Rows(); ++row) {
            for (index_type col = 0; col < result.Cols(); ++col) {
                result.Set(row, col, std::pow(result.Get(row, col), exponent));
            }
        }
        return result;
    }

    bool operator==(const SparseMatrix& other) const {
        if ((cols_ != other.cols_) || (rows_ != other.rows_)) {
            return false;
        }

        for (index_type i = 0; i < rows_; i++) {
            for (index_type j = 0; j < cols_; j++) {
                if (!IsEqual(Get(i, j), other.Get(i, j), EPSYLON)) {
                    return false;
                }
            }
        }

        return true;
    }

    value_type Determinant() const {
        index_type row = 0;
        if (rows_ == 1) {
            return Get(0, 0);
        }

        value_type result{};

        index_type i = row;
        for (index_type j = 0; j < cols_; j++) {
            result = result + minus_one_pow(i + j) * Get(i, j)
                * SubMatrix(i, j).Determinant();
        }

        return result;
    }

    SparseMatrix Inverse() const {
        const size_type n = cols_;
        SparseMatrix tmp = *this;
        SparseMatrix inv = MakeIdentityMatrix<value_type>(n);

        for (size_type i = 0; i < n; i++) {
            double pivot = tmp.Get(i, i);
            tmp.ScaleRow(i, 1/pivot);
            inv.ScaleRow(i, 1/pivot);

            for (size_type j = i + 1; j < n; j++) {
                double factor = tmp.Get(j, i);
                tmp.AddRowMultiple(i, j, -factor);
                inv.AddRowMultiple(i, j, -factor);
            }
        }

        for (size_type i = n - 1; i >= 1; i--) {
            for (size_type j = i - 1; j >= 0; j--) {
                double factor = tmp.Get(j, i);

                tmp.AddRowMultiple(i, j, -factor);
                inv.AddRowMultiple(i, j, -factor);

                if (j == 0) break;
            }
        }

        return inv;
    }

    SparseMatrix Power(int pow) const {
        auto result = *this;
        for (int i = 1; i < pow; i++) {
            result = result * (*this);
        }

        return result;
    }

private:
    void AddRowMultiple(index_type row_from, index_type row_to, double multiple) {
        for (int i = 0; i < cols_; i++) {
            Set(row_to, i, Get(row_to, i) + Get(row_from, i) * multiple);
        }
    }

    void ScaleRow(index_type row, double multiple) {
        for (int i = 0; i < cols_; i++) {
            Set(row, i, Get(row, i) * multiple);
        }
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& out, SparseMatrixBase<T> matrix) {
    for (const auto& row : matrix) {
        for (const auto elem : row) {
            out << elem << " ";
        }
        out << std::endl;
    }
    return out;
}

template<typename T>
SparseMatrixBase<T> MakeIdentityMatrix(std::size_t size) {
    SparseMatrixBase<T> id(size, size);
    for (typename SparseMatrixBase<T>::index_type i = 0; i < size; i++) {
        for (typename SparseMatrixBase<T>::index_type j = 0; j < size; j++) {
            if (i == j) {
                id.Set(i, j, 1);
                continue;
            }

            id.Set(i, j, 0);
        }
    }

    return id;
}

#endif