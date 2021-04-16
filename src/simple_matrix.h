#pragma once
#include <fmt/format.h>
#include <algorithm>
#include <exception>
#include <numeric>
#include <vector>

template <typename DT>
struct mat2d {
    const int rows;
    const int cols;
    std::vector<DT> data;

    struct view {
        using Iterator = typename std::vector<DT>::const_iterator;
        Iterator begin;
        const int rows;
        const int cols;
        view(Iterator begin, int rows, int cols)
            : begin(begin), rows(rows), cols(cols)
        {
        }

        DT operator()(int i, int j) const { return *(begin + i * cols + j); }

        mat2d operator*(const mat2d &o)
        {
            if (cols != o.rows)
                throw std::invalid_argument(
                    fmt::format("{}!={}", cols, o.rows));
            const int nr = rows;
            const int nc = o.cols;
            auto n = mat2d::zeros(nr, nc);
            for (int i = 0; i < nr; ++i)
                for (int j = 0; j < nc; ++j)
                    for (int k = 0; k < cols; ++k)
                        n(i, j) += (*this)(i, k) * o(k, j);
            return n;
        }

        void operator/=(DT m)
        {
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    (*this)(i, j) /= m;
        }

        mat2d dot(const mat2d &o)
        {
            mat2d r(rows, cols);
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    r(i, j) = (*this)(i, j) * o(i, j);
            return r;
        }

        DT sum()
        {
            return std::accumulate(begin, begin + rows * cols,
                                   static_cast<DT>(0));
        }
    };

    struct row : mat2d::view {
        row(typename mat2d::view::Iterator begin, int cols)
            : mat2d::view(begin, 1, cols)
        {
        }
        mat2d::view T() const
        {
            return mat2d::view(this->begin, this->cols, 1);
        }
    };

    mat2d(int rows, int cols) : rows(rows), cols(cols), data(rows * cols) {}
    mat2d(int rows, int cols, std::vector<DT> &&data)
        : rows(rows), cols(cols), data(data)
    {
    }
    mat2d(const mat2d &o) : rows(o.rows), cols(o.cols), data(o.data) {}
    mat2d(const mat2d::view &o)
        : rows(o.rows), cols(o.cols), data(o.begin, o.begin + o.rows * o.cols)
    {
    }

    mat2d &operator=(const mat2d &o)
    {
        if (cols != o.cols)
            throw std::invalid_argument(
                fmt::format("cols {}!={}", cols, o.cols));
        if (rows != o.rows)
            throw std::invalid_argument(
                fmt::format("rows {}!={}", rows, o.rows));
        data = o.data;
        return *this;
    }

    static mat2d zeros(int rows, int cols)
    {
        mat2d n(rows, cols);
        std::fill(n.data.begin(), n.data.end(), 0);
        return n;  // RVO?
    }

    DT &operator()(int i, int j) { return data[i * cols + j]; }
    DT operator()(int i, int j) const { return data[i * cols + j]; }

    bool operator==(const mat2d &o) const
    {
        if (rows != o.rows | cols != o.cols)
            return false;
        return std::equal(data.begin(), data.end(), o.data.begin());
    }

    bool operator==(const mat2d::view &o) const
    {
        if (rows != o.rows | cols != o.cols)
            return false;
        return std::equal(data.begin(), data.end(), o.data.begin());
    }

    bool operator!=(const mat2d &o) const { return !((*this) == o); }
    bool operator!=(const mat2d::view &o) const { return !((*this) == o); }

    mat2d::row get_row(int i) { return row(data.begin() + i * cols, cols); }

    mat2d::view T() const
    {
        return view(data.begin(), cols, rows);
    }  // not transpose

    mat2d operator*(const mat2d &o)
    {
        if (cols != o.rows)
            throw std::invalid_argument(fmt::format("{}!={}", cols, o.rows));
        const int nr = rows;
        const int nc = o.cols;
        auto n = mat2d::zeros(nr, nc);
        for (int i = 0; i < nr; ++i)
            for (int j = 0; j < nc; ++j)
                for (int k = 0; k < cols; ++k)
                    n(i, j) += (*this)(i, k) * o(k, j);
        return n;
    }

    mat2d operator*(const mat2d::view &o)
    {
        if (cols != o.rows)
            throw std::invalid_argument(fmt::format("{}!={}", cols, o.rows));
        const int nr = rows;
        const int nc = o.cols;
        auto n = mat2d::zeros(nr, nc);
        for (int i = 0; i < nr; ++i)
            for (int j = 0; j < nc; ++j)
                for (int k = 0; k < cols; ++k)
                    n(i, j) += (*this)(i, k) * o(k, j);
        return n;
    }

    mat2d operator*(DT m)
    {
        mat2d r(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                r(i, j) = (*this)(i, j) * m;
        return r;
    }

    mat2d operator+(DT m)
    {
        mat2d r(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                r(i, j) = (*this)(i, j) + m;
        return r;
    }

    mat2d dot(const mat2d &o)
    {
        mat2d r(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                r(i, j) = (*this)(i, j) * o(i, j);
        return r;
    }

    mat2d dot(const mat2d::view &o)
    {
        mat2d r(rows, cols);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                r(i, j) = (*this)(i, j) * o(i, j);
        return r;
    }
};