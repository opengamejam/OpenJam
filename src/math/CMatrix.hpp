//
//  CMatrix.hpp
//  OpenJam
//
//  Created by yev on 3/28/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#ifndef CMATRIX_HPP
#define CMATRIX_HPP

#include "Global.h"
#include "CVector.hpp"

namespace jam
{
// *****************************************************************************
// Predeclarations
// *****************************************************************************

template<class T, uint32_t COLS, uint32_t ROWS>
class CMatrix;

typedef CMatrix<float, 2, 2> CMatrix2x2f;
typedef CMatrix<double, 2, 2> CMatrix2x2d;
typedef CMatrix<int32_t, 2, 2> CMatrix2x2i;
typedef CMatrix<uint32_t, 2, 2> CMatrix2x2u;

typedef CMatrix<float, 3, 3> CMatrix3x3f;
typedef CMatrix<double, 3, 3> CMatrix3x3d;
typedef CMatrix<int32_t, 3, 3> CMatrix3x3i;
typedef CMatrix<uint32_t, 3, 3> CMatrix3x3u;

typedef CMatrix<float, 4, 4> CMatrix4x4f;
typedef CMatrix<double, 4, 4> CMatrix4x4d;
typedef CMatrix<int32_t, 4, 4> CMatrix4x4i;
typedef CMatrix<uint32_t, 4, 4> CMatrix4x4u;

// *****************************************************************************
// CMatrix is a class-wrapper around primitive T[ROWS][COLS] 2d array,
// where T - type, ROWS & COLS - matrix dimensions. There are methods for
// manipulate with matrix operations (multiply, normalize, etc...)
// *****************************************************************************

template<class T, uint32_t COLS, uint32_t ROWS>
class CMatrix
{
public:
    typedef T TMatrix[ROWS][COLS];
    
public:
    CMatrix()
    : m_Matrix()
    {
    }
    
    CMatrix(const TMatrix matrix)
    {
        memcpy(m_Matrix, matrix, sizeof(m_Matrix));
    }
    
    ~CMatrix()
    {
    }
    
    INL const T* operator()() const
    {
        return *m_Matrix;
    }
    
    INL static const CMatrix<T, COLS, ROWS> Identity()
    {
        static_assert(COLS == ROWS, "Identity matrix must be square");
        
        CMatrix<T, COLS, ROWS> out;
        for (uint32_t i = 0; i < COLS; ++i)
        {
            out.Set(i, i, static_cast<T>(1));
        }
        
        return out;
    }
    
    INL static const CMatrix<T, COLS, ROWS> Perspective(float fov, float aspect, float zNear, float zFar)
    {
        static_assert(COLS >= 3 && ROWS >= 3, "Works only with 3D matrix");
        
        CMatrix<T, COLS, ROWS> out = CMatrix<T, COLS, ROWS>::Identity();
        
        float r = fov * (0.5f * (3.1415f / 180.0f));
        float deltaZ = zFar - zNear;
        float s = sin(r);
        if (deltaZ == 0.0 || s == 0.0 || aspect == 0.0)
        {
            // TODO: fire log
            return out;
        }
        
        float cotangent = cos(r) / s;
        
        out.Set(0, 0, static_cast<T>(cotangent / aspect));
        out.Set(1, 1, static_cast<T>(cotangent));
        out.Set(2, 2, static_cast<T>(-(zFar + zNear) / deltaZ));
        out.Set(3, 2, static_cast<T>(-1.0));
        out.Set(2, 3, static_cast<T>(-2.0 * zNear * zFar / deltaZ));
        out.Set(3, 3, static_cast<T>(0.0));
        
        return out;
    }
    
    INL static const CMatrix<T, COLS, ROWS> Orthographic(float left, float right, float top, float bottom, float _near, float _far)
    {
        static_assert(COLS >= 3 && ROWS >= 3, "Works only with 3D matrix");
        
        float tx = -((right + left) / (right - left));
        float ty = -((top + bottom) / (top - bottom));
        float tz = -((_far + _near) / (_far - _near));
        
        CMatrix<T, COLS, ROWS> out;
        
        out.Set(0, 0, static_cast<T>(2.0 / (right - left)));
        out.Set(0, 3, static_cast<T>(tx));
        out.Set(1, 1, static_cast<T>(2.0 / (top - bottom)));
        out.Set(1, 3, static_cast<T>(ty));
        out.Set(2, 2, static_cast<T>(-2.0 / (_far - _near)));
        out.Set(2, 3, static_cast<T>(tz));
        out.Set(3, 3, static_cast<T>(1.0));
        
        return out;
    }
    
    INL CMatrix<T, COLS, ROWS>& Transpose()
    {
        for (uint32_t i = 0; i < ROWS; ++i)
        {
            for (uint32_t j = i + 1; j < COLS; ++j)
            {
                std::swap(m_Matrix[i][j], m_Matrix[j][i]);
            }
        }
        
        return *this;
    }
    
    INL bool IsEqual(const TMatrix matrix)
    {
        return memcmp(m_Matrix, matrix, sizeof(m_Matrix)) == 0;
    }
    
    template<uint32_t N>
    INL CMatrix<T, COLS, ROWS>& Translate(const CVector<T, N>& translation)
    {
        uint32_t size = std::min(ROWS, N);
        for (uint32_t i = 0; i < size; ++i)
        {
            m_Matrix[i][COLS - 1] = translation.Get(i);
        }
        
        return *this;
    }
    
    template<uint32_t N>
    INL CMatrix<T, COLS, ROWS>& Rotate(const CVector<T, N>& rotation)
    {
        if (N == 2)
        {
            // TODO: 2d rotation matrix
        }
        else if (N == 3 && ROWS >= 3 && COLS >= 3)
        {
            T A = static_cast<T>(cos(static_cast<double>(rotation.X())));
            T B = static_cast<T>(sin(static_cast<double>(rotation.X())));
            T C = static_cast<T>(cos(static_cast<double>(rotation.Y())));
            T D = static_cast<T>(sin(static_cast<double>(rotation.Y())));
            T E = static_cast<T>(cos(static_cast<double>(rotation.Z())));
            T F = static_cast<T>(sin(static_cast<double>(rotation.Z())));
            T AD = A * D;
            T BD = B * D;
            
            m_Matrix[0][0] =   C * E;
            m_Matrix[1][0] =  -C * F;
            m_Matrix[2][0] =  -D;
            m_Matrix[0][1] = -BD * E + A * F;
            m_Matrix[1][1] =  BD * F + A * E;
            m_Matrix[2][1] =  -B * C;
            m_Matrix[0][2] =  AD * E + B * F;
            m_Matrix[1][2] = -AD * F + B * E;
            m_Matrix[2][2] =   A * C;
        }
        else
        {
            // TODO: handle this
        }
        
        return *this;
    }
    
    template<uint32_t N>
    INL CMatrix<T, COLS, ROWS>& Scale(const CVector<T, N>& scale)
    {
        static_assert(COLS == ROWS, "Matrix must be square");
        
        uint32_t size = std::min(ROWS, N);
        for (uint32_t i = 0; i < size; ++i)
        {
            m_Matrix[i][i] = scale.Get(i);
        }
        
        return *this;
    }
    
    template<uint32_t N>
    INL CVector<T, N> Up() const
    {
        CVector<T, N> upVector;
        
        upVector.X(Get(0, 1));
        upVector.Y(Get(1, 1));
        upVector.Z(Get(2, 1));
        
        upVector.Normalize();
        
        return upVector;
    }
    
    template<uint32_t N>
    INL CVector<T, N> Forward() const
    {
        CVector<T, N> upVector;
        
        upVector.X(Get(0, 2));
        upVector.Y(Get(1, 2));
        upVector.Z(Get(2, 2));
        
        upVector.Normalize();
        
        return upVector;
    }
    
    template<uint32_t N>
    INL CVector<T, N> Right() const
    {
        CVector<T, N> upVector;
        
        upVector.X(Get(0, 0));
        upVector.Y(Get(1, 0));
        upVector.Z(Get(2, 0));
        
        upVector.Normalize();
        
        return upVector;
    }
    
    INL void Set(uint32_t x, uint32_t y, T element)
    {
        if (x < COLS && y < ROWS)
        {
            m_Matrix[x][y] = element;
        }
    }
    
    INL T Get(uint32_t x, uint32_t y) const
    {
        if (x < COLS && y < ROWS)
        {
            return m_Matrix[x][y];
        }
        
        return static_cast<T>(0);
    }
    INL void SwapElement(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
    {
        if (x1 < COLS && y1 < ROWS && x2 < COLS && y2 < ROWS)
        {
            std::swap(m_Matrix[x1][y1], m_Matrix[x2][y2]);
        }
    }
    
    template<uint32_t COLS2, uint32_t ROWS2>
    INL CMatrix<T, COLS, ROWS> operator*(const CMatrix<T, COLS2, ROWS2>& matrix)
    {
        CMatrix<T, COLS, ROWS> result(*this);
        result *= matrix;
        
        return result;
    }
    
    template<uint32_t COLS2, uint32_t ROWS2>
    INL CMatrix<T, COLS, ROWS>& operator*=(const CMatrix<T, COLS2, ROWS2>& matrix)
    {
        static_assert(COLS == ROWS2, "Matrix must be consistent");
        
        TMatrix result = {{0}, };
        
        for (uint32_t i = 0; i < ROWS; ++i)
        {
            for (uint32_t j = 0; j < COLS; ++j)
            {
                for (uint32_t k = 0; k < COLS; ++k)
                {
                    result[i][j] += m_Matrix[i][k] * matrix.Get(k, j);
                }
            }
        }
        
        memcpy(m_Matrix, result, sizeof(m_Matrix));
        
        return *this;
    }
    
    template<uint32_t COLS2, uint32_t ROWS2>
    INL CMatrix<T, COLS, ROWS> operator+(const CMatrix<T, COLS2, ROWS2>& matrix)
    {
        CMatrix<T, COLS, ROWS> result(*this);
        result += matrix;
        
        return result;
    }
    
    template<uint32_t COLS2, uint32_t ROWS2>
    INL CMatrix<T, COLS, ROWS>& operator+=(const CMatrix<T, COLS2, ROWS2>& matrix)
    {
        for (uint32_t i = 0; i < ROWS; ++i)
        {
            for (uint32_t j = 0; j < COLS; ++j)
            {
                m_Matrix[i][j] += matrix.Get(i, j);
            }
        }
        
        return *this;
    }
    
    template<uint32_t COLS2, uint32_t ROWS2>
    INL CMatrix<T, COLS, ROWS> operator-(const CMatrix<T, COLS2, ROWS2>& matrix)
    {
        CMatrix<T, COLS, ROWS> result(*this);
        result -= matrix;
        
        return result;
    }
    
    template<uint32_t COLS2, uint32_t ROWS2>
    INL CMatrix<T, COLS, ROWS>& operator-=(const CMatrix<T, COLS2, ROWS2>& matrix)
    {
        for (uint32_t i = 0; i < ROWS; ++i)
        {
            for (uint32_t j = 0; j < COLS; ++j)
            {
                m_Matrix[i][j] -= matrix.Get(i, j);
            }
        }
        
        return *this;
    }
    
    INL CMatrix<T, COLS, ROWS>& operator=(const TMatrix matrix)
    {
        memcpy(m_Matrix, matrix, sizeof(m_Matrix));
    }
    
private:
    static_assert(COLS > 0 && ROWS > 0, "CMatrix cannot be zero-dimensioned");
    TMatrix m_Matrix;
};

}; // namespace jam

#endif /* defined(CMatrix_HPP) */
