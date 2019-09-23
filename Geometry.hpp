#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>

template <class T, size_t N>
class Vec
{
    public:
        
        Vec() = default;
        Vec(const Vec& rhs) = default;
        
        Vec(const std::array<T,N>& arr);
        
        template<typename ...Args>
        Vec(Args ...args) : _data{args...}
        {
        }

        template<typename T1>
        Vec(const Vec<T1,N>& rhs)
        {
            for(size_t i =0; i < N; ++i)
                _data[i] = static_cast<T>(rhs[i]);
        }
        
        Vec& operator = (Vec rhs);
        // Vec& operator = (Vec&& rhs);

        Vec operator + (const Vec& rhs) const;
        Vec operator - (const Vec& rhs) const;
        Vec operator * (const Vec& rhs) const;

        Vec& operator += (const Vec& rhs);
        Vec& operator -= (const Vec& rhs);
        Vec& operator *= (const Vec& rhs);

        Vec& operator += (const double rhs);
        Vec& operator -= (const double rhs);
        Vec& operator *= (const double rhs);
        Vec& operator /= (const double rhs);

        Vec operator + (const double rhs) const;
        Vec operator - (const double rhs) const;
        Vec operator * (const double rhs) const;
        Vec operator / (const double rhs) const;

        T& operator[](const unsigned index);
        const T& operator[](const unsigned index) const;

        double abs2() const;
        double abs() const;
        double dot(const Vec& rhs) const;
        Vec& normalize();

        template<typename U, size_t N1>
        friend std::ostream& operator <<(std::ostream& os, const Vec<U,N1>& rhs);

    private:
        std::array<T,N> _data;
};

template<typename T, size_t N>
inline Vec<T,N>::Vec(const std::array<T,N>& arr) : _data(arr)
{
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator = (Vec<T,N> rhs)
{
    std::swap(this->_data, rhs._data);
    return *this;
}

template <typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator + (const Vec<T,N>& rhs) const
{
    Vec<T,N> temp = *this;
    temp += rhs;
    return temp;
}

template <typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator - (const Vec<T,N>& rhs) const
{
    Vec<T,N> temp = *this;
    temp -= rhs;
    return temp;
}

template <typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator * (const Vec<T,N>& rhs) const
{
    Vec<T,N> temp = *this;
    temp *= rhs;
    return temp;
}

template <typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator += (const Vec<T,N>& rhs)
{
    for(size_t i = 0; i < N; ++i)
        this->_data[i] += rhs._data[i];
    return *this;
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator -= (const Vec<T,N>& rhs)
{
    for(size_t i = 0; i < N; ++i)
        this->_data[i] -= rhs._data[i];
    return *this;
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator *= (const Vec<T,N>& rhs)
{
    for(size_t i = 0; i < N; ++i)
        this->_data[i] *= rhs._data[i];
    return *this;
}


template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator += (const double rhs)
{
    std::for_each(this->_data.begin(), this->_data.end(), [&](T& d){d+=rhs;});
    return *this;
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator -= (const double rhs)
{
    std::for_each(this->_data.begin(), this->_data.end(), [&](T& d){d-=rhs;});
    return *this;
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator *= (const double rhs)
{
    std::for_each(this->_data.begin(), this->_data.end(), [&](T& d){d*=rhs;});
    return *this;
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::operator /= (const double rhs)
{
    std::for_each(this->_data.begin(), this->_data.end(), [&](T& d){d*=(1.0)/rhs;});
    return *this;
}

template<typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator + (const double rhs) const
{
    Vec<T,N> temp = *this;
    temp += rhs;
    return temp;
}

template<typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator - (const double rhs) const
{
    Vec<T,N> temp = *this;
    temp -= rhs;
    return temp;
}

template<typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator * (const double rhs) const
{
    Vec<T,N> temp = *this;
    temp *= rhs;
    return temp;
}

template<typename T, size_t N>
inline Vec<T,N> Vec<T,N>::operator / (const double rhs) const
{
    Vec<T,N> temp = *this;
    temp /= rhs;
    return temp;
}

template<typename T, size_t N>
T& Vec<T,N>::operator[](const unsigned index)
{
    return this->_data[index];
}

template<typename T, size_t N>
const T& Vec<T,N>::operator[](const unsigned index) const
{
    return this->_data[index];
}

template<typename T, size_t N>
double Vec<T,N>::abs2() const
{
    return std::inner_product(this->_data.begin(), this->_data.end(), this->_data.begin(), 0.0f);
}

template<typename T, size_t N>
double Vec<T,N>::abs() const
{
    return std::sqrt(Vec<T,N>::abs2());
}

template<typename T, size_t N>
double Vec<T,N>::dot(const Vec<T,N>& rhs) const
{
    return std::inner_product(this->_data.begin(), this->_data.end(), rhs._data.begin(), 0.0f);
}

template<typename T, size_t N>
inline Vec<T,N>& Vec<T,N>::normalize()
{
    *this /= this->abs();
    return *this;
}

template<typename T, size_t N>
std::ostream& operator <<(std::ostream& os, const Vec<T,N>& rhs)
{
    os << "[";
    for(size_t i = 0; i < N - 1; ++i)
        os << rhs._data[i] << ", ";
    os << rhs._data[N-1] << "]";
    return os;
}

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

using Vec2i = Vec<int, 2>;
using Vec3i = Vec<int, 3>;
using Vec4i = Vec<int, 4>;

#endif