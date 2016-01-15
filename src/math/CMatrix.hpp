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

template<class T, uint32_t COLS, uint32_t ROWS>
class CMatrix
{
public:
    typedef T TMatrix[ROWS][COLS];
    
public:
    CMatrix()
    {
        memset(m_Matrix, 0, sizeof(m_Matrix));
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
    
    static const CMatrix<T, COLS, ROWS> Identity()
    {
        static_assert(COLS == ROWS, "Identity matrix must be square");
        
        CMatrix<T, COLS, ROWS> out;
        for (uint32_t i = 0; i < COLS; ++i)
        {
            out.Set(i, i, static_cast<T>(1));
        }
        
        return out;
    }
    
    static const CMatrix<T, COLS, ROWS> Perspective(float fov, float aspect, float zNear, float zFar)
    {
        static_assert(COLS >= 3 && ROWS >= 3, "Works only with 3D matrix");
        
        CMatrix<T, COLS, ROWS> out = CMatrix<T, COLS, ROWS>::Identity();
        
        float r = fov * (0.5f * (3.1415f / 180.0f));
        float deltaZ = zFar - zNear;
        float s = sin(r);
        if (deltaZ == 0.0 || s == 0.0 || aspect == 0.0)
        {
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
    
    static const CMatrix<T, COLS, ROWS> Orthographic(float left, float right, float top, float bottom, float _near, float _far)
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
        for (uint64_t i = 0; i < ROWS; ++i)
        {
            for (uint64_t j = i + 1; j < COLS; ++j)
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
        else if (N == 3 && ROWS == 4 && COLS == 4)
        {
            T A      = static_cast<T>(cos(static_cast<double>(rotation.X())));
            T B      = static_cast<T>(sin(static_cast<double>(rotation.X())));
            T C      = static_cast<T>(cos(static_cast<double>(rotation.Y())));
            T D      = static_cast<T>(sin(static_cast<double>(rotation.Y())));
            T E      = static_cast<T>(cos(static_cast<double>(rotation.Z())));
            T F      = static_cast<T>(sin(static_cast<double>(rotation.Z())));
            
            T AD     =   A * D;
            T BD     =   B * D;
            
            T* ptr = *m_Matrix;            
            *(ptr + 0) =   C * E;
            *(ptr + 4) =  -C * F;
            *(ptr + 8) =  -D;
            *(ptr + 1) = -BD * E + A * F;
            *(ptr + 5) =  BD * F + A * E;
            *(ptr + 9) =  -B * C;
            *(ptr + 2) =  AD * E + B * F;
            *(ptr + 6) = -AD * F + B * E;
            *(ptr + 10)=   A * C;
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
    
    INL CVector<T, 3> Up() const
    {
        // TODO:
        CVector<T, 3> upVector;
        
        /*upVector.X(m_Matrix[1]);
        upVector.Y(m_Matrix[5]);
        upVector.Z(m_Matrix[9]);*/
        
        upVector.Normalize();
        
        return upVector;
    }
    
    INL CVector<T, 3> Forward() const
    {
        // TODO:
        CVector<T, 3> upVector;
        
        /*upVector.X(m_Matrix[2]);
        upVector.Y(m_Matrix[6]);
        upVector.Z(m_Matrix[10]);*/
        
        upVector.Normalize();
        
        return upVector;
    }
    
    INL CVector<T, 3> Right() const
    {
        CVector<T, 3> upVector;
        
        /*upVector.X(m_Matrix[0]);
        upVector.Y(m_Matrix[4]);
        upVector.Z(m_Matrix[8]);*/
        
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
    
    INL CMatrix<T, COLS, ROWS> operator*(const CMatrix<T, COLS, ROWS>& matrix)
    {
        CMatrix<T, COLS, ROWS> result = *this;
        
        result *= matrix;
        
        return result;
    }
    
    INL CMatrix<T, COLS, ROWS>& operator*=(const CMatrix<T, COLS, ROWS>& matrix)
    {
        // TODO: try constexpr
        TMatrix result = {{0}, };
        
        for (uint64_t i = 0; i < ROWS; ++i)
        {
            for (uint64_t j = 0; j < COLS; ++j)
            {
                for (uint64_t k = 0; k < COLS; ++k)
                {
                    result[i][j] += m_Matrix[i][k] * matrix.Get(k, j);
                }
            }
        }
        
        memcpy(m_Matrix, result, sizeof(m_Matrix));
        
        return *this;
    }
    
    INL CMatrix<T, COLS, ROWS> operator+(const CMatrix<T, COLS, ROWS>& matrix)
    {
        CMatrix<T, COLS, ROWS> result = *this;
        
        result += matrix;
        
        return result;
    }
    
    INL CMatrix<T, COLS, ROWS>& operator+=(const CMatrix<T, COLS, ROWS>& matrix)
    {
        for (uint64_t i = 0; i < ROWS; ++i)
        {
            for (uint64_t j = 0; j < COLS; ++j)
            {
                m_Matrix[i][j] += matrix.Get(i, j);
            }
        }
        
        return *this;
    }
    
    INL CMatrix<T, COLS, ROWS> operator-(const CMatrix<T, COLS, ROWS>& matrix)
    {
        CMatrix<T, COLS, ROWS> result = *this;
        
        result -= matrix;
        
        return result;
    }
    
    INL CMatrix<T, COLS, ROWS>& operator-=(const CMatrix<T, COLS, ROWS>& matrix)
    {
        for (uint64_t i = 0; i < ROWS; ++i)
        {
            for (uint64_t j = 0; j < COLS; ++j)
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
    
    void Dump() const
    {
        printf("{\n");
        for (uint64_t i = 0; i < ROWS; ++i)
        {
            for (uint64_t j = 0; j < COLS; ++j)
            {
                printf("\t%f\t", m_Matrix[i][j]);
            }
            printf("\n");
        }
        printf("}\n");
    }
    
private:
    static_assert(COLS > 0 && ROWS > 0, "CMatrix cannot be zero-dimensioned");
    TMatrix m_Matrix;
};

typedef CMatrix<float, 4, 4> CMatrix4x4f;
typedef CMatrix<double, 4, 4> CMatrix4x4d;
typedef CMatrix<int32_t, 4, 4> CMatrix4x4i;

}; // namespace jam

#endif /* defined(CMatrix_HPP) */
