//
//  CMatrix4x4.cpp
//  OpenJam
//
//  Created by yev on 3/28/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CMatrix4x4.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

template <class T>
const unsigned short CMatrix4x4<T>::s_Dimension = 4;

template <class T>
T CMatrix4x4<T>::s_nullValue = static_cast<T>(0.0);

template <class T>
CMatrix4x4<T> CMatrix4x4<T>::s_Identity = \
    CMatrix4x4<T>(std::vector<T>(
                                 {
                                     static_cast<T>(1.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0),
                                     static_cast<T>(0.0), static_cast<T>(1.0), static_cast<T>(0.0), static_cast<T>(0.0),
                                     static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(1.0), static_cast<T>(0.0),
                                     static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(1.0)
                                 }
                                )
                  );

template <class T>
CMatrix4x4<T> CMatrix4x4<T>::s_Zero = \
    CMatrix4x4<T>(std::vector<T>(
                                 {
                                     static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0),
                                     static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0),
                                     static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0),
                                     static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0), static_cast<T>(0.0)
                                 }
                                )
                  );

// *****************************************************************************
// Public Methods
// *****************************************************************************

template <class T>
CMatrix4x4<T>::CMatrix4x4()
{
    m_Matrix.resize(s_Dimension * s_Dimension);
    memset(&m_Matrix[0], 0, s_Dimension * s_Dimension);
}

template <class T>
CMatrix4x4<T>::CMatrix4x4(const std::vector<T>& matrix)
{
    *this = matrix;
}

template <class T>
CMatrix4x4<T>::~CMatrix4x4()
{
    
}

template <class T>
const std::vector<T>& CMatrix4x4<T>::operator()()const
{
    return m_Matrix;
}

template <class T>
void CMatrix4x4<T>::Element(unsigned short x, unsigned short y, T element)
{
    if ((size_t)(y * s_Dimension + x) < m_Matrix.size())
    {
        m_Matrix[y * s_Dimension + x] = element;
    }
}

template <class T>
const T& CMatrix4x4<T>::Element(unsigned short x, unsigned short y) const
{
    if ((size_t)(y * s_Dimension + x) < m_Matrix.size())
    {
        return m_Matrix[y * s_Dimension + x];
    }
    
    return s_nullValue;
}

template <class T>
void CMatrix4x4<T>::SwapElement(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    if ((size_t)(y1 * s_Dimension + x1) < m_Matrix.size() &&
        (size_t)(y2 * s_Dimension + x2) < m_Matrix.size())
    {
        std::swap(m_Matrix[y1 * s_Dimension + x1], m_Matrix[y2 * s_Dimension + x2]);
    }
}

template <class T>
CMatrix4x4<T> CMatrix4x4<T>::Clone() const
{
    CMatrix4x4 cloned = CMatrix4x4(*this);
    return cloned;
}

template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::Transpose()
{
    for (unsigned int i = 0; i < s_Dimension; ++i)
    {
        for (unsigned int j = i + 1; j < s_Dimension; ++j)
        {
            std::swap(m_Matrix[j * s_Dimension + i], m_Matrix[i * s_Dimension + j]);
        }
    }
    
    return *this;
}

template <class T>
bool CMatrix4x4<T>::IsEqual(const CMatrix4x4<T>& matrix)
{
    return (m_Matrix == matrix());
}

template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::Rotate(const CVector<T, 3>& rotation)
{
    T A      = static_cast<T>(cos(static_cast<double>(rotation.X())));
    T B      = static_cast<T>(sin(static_cast<double>(rotation.X())));
    T C      = static_cast<T>(cos(static_cast<double>(rotation.Y())));
    T D      = static_cast<T>(sin(static_cast<double>(rotation.Y())));
    T E      = static_cast<T>(cos(static_cast<double>(rotation.Z())));
    T F      = static_cast<T>(sin(static_cast<double>(rotation.Z())));
    
    T AD     =   A * D;
    T BD     =   B * D;
    
    m_Matrix[0] =   C * E;
    m_Matrix[4] =  -C * F;
    m_Matrix[8] =  -D;
    m_Matrix[1] = -BD * E + A * F;
    m_Matrix[5] =  BD * F + A * E;
    m_Matrix[9] =  -B * C;
    m_Matrix[2] =  AD * E + B * F;
    m_Matrix[6] = -AD * F + B * E;
    m_Matrix[10]=   A * C;
    
    return *this;
}

template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::Scale(const CVector<T, 3>& scale)
{
    m_Matrix[0] = scale.X();
    m_Matrix[5] = scale.Y();
    m_Matrix[10] = scale.Z();
    
    return *this;
}

template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::Translate(const CVector<T, 3>& translation)
{
    m_Matrix[3] = translation.X();
    m_Matrix[7] = translation.Y();
    m_Matrix[11] = translation.Z();
    
    return *this;
}

template <class T>
CVector<T, 3> CMatrix4x4<T>::Up() const
{
    CVector<T, 3> upVector;
    
    upVector.X(m_Matrix[1]);
	upVector.X(m_Matrix[5]);
    upVector.X(m_Matrix[9]);
    
    upVector.Normalize();
    
	return upVector;
}

template <class T>
CVector<T, 3> CMatrix4x4<T>::Forward() const
{
    CVector<T, 3> upVector;
    
    upVector.X(m_Matrix[2]);
	upVector.X(m_Matrix[6]);
    upVector.X(m_Matrix[10]);
    
    upVector.Normalize();
    
	return upVector;
}

template <class T>
CVector<T, 3> CMatrix4x4<T>::Right() const
{
    CVector<T, 3> upVector;
    
    upVector.X(m_Matrix[0]);
	upVector.X(m_Matrix[4]);
    upVector.X(m_Matrix[8]);
    
    upVector.Normalize();
    
	return upVector;
}

template <class T>
CMatrix4x4<T> CMatrix4x4<T>::operator*(const CMatrix4x4<T>& matrix)
{
    CMatrix4x4<T> result = *this;
    
    result *= matrix;
    
    return result;
}


template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::operator*=(const CMatrix4x4<T>& matrix)
{
    // TODO: optimize me
    std::vector<T> m1 = matrix();
    std::vector<T> m2 = m_Matrix;
    
    m_Matrix[0]  = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	m_Matrix[1]  = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	m_Matrix[2]  = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	m_Matrix[3]  = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];
    
	m_Matrix[4]  = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	m_Matrix[5]  = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	m_Matrix[6]  = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	m_Matrix[7]  = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];
    
	m_Matrix[8]  = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	m_Matrix[9]  = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	m_Matrix[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	m_Matrix[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];
    
	m_Matrix[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	m_Matrix[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	m_Matrix[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	m_Matrix[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    
    return *this;
}

template <class T>
CMatrix4x4<T> CMatrix4x4<T>::operator+(const CMatrix4x4<T>& matrix)
{
    CMatrix4x4<T> result = *this;
    
    result += matrix;
    
    return result;
}

template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::operator+=(const CMatrix4x4<T>& matrix)
{
    std::vector<T> m2 = matrix();
    
    size_t size = s_Dimension * s_Dimension;
    for (size_t i = 0; i < size; ++i)
    {
        m_Matrix[i] += m2[i];
    }
    
    return *this;
}

template <class T>
CMatrix4x4<T>& CMatrix4x4<T>::operator=(const std::vector<T>& other)
{
    m_Matrix = other;
    
    return *this;
}

template <class T>
void CMatrix4x4<T>::Dump() const
{
    printf("{\n");
    for (unsigned int i = 0; i < s_Dimension; ++i)
    {
        printf("%f\t%f\t%f\t%f\n", static_cast<double>(m_Matrix[i * s_Dimension + 0]),
                                   static_cast<double>(m_Matrix[i * s_Dimension + 1]),
                                   static_cast<double>(m_Matrix[i * s_Dimension + 2]),
                                   static_cast<double>(m_Matrix[i * s_Dimension + 3]));
    }
    printf("}\n");
}

// *****************************************************************************
// Static Methods
// *****************************************************************************

template <class T>
const CMatrix4x4<T> CMatrix4x4<T>::Identity()
{
    return s_Identity;
}

template <class T>
const CMatrix4x4<T> CMatrix4x4<T>::Zero()
{
    return s_Zero;
}

template <class T>
const CMatrix4x4<T> CMatrix4x4<T>::Perspective(float fov, float aspect, float zNear, float zFar)
{
    CMatrix4x4<T> out = CMatrix4x4<T>::Identity();
    
    float r = fov * (0.5f * (3.1415f / 180.0f));
	float deltaZ = zFar - zNear;
	float s = sin(r);
	if (deltaZ == 0.0 || s == 0.0 || aspect == 0.0)
    {
		return out;
	}
    
	float cotangent = cos(r) / s;
    
    out.Element(0, 0, static_cast<T>(cotangent / aspect));
    out.Element(1, 1, static_cast<T>(cotangent));
    out.Element(2, 2, static_cast<T>(-(zFar + zNear) / deltaZ));
    out.Element(2, 3, static_cast<T>(-1.0));
    out.Element(3, 2, static_cast<T>(-2.0 * zNear * zFar / deltaZ));
    out.Element(3, 3, static_cast<T>(0.0));
    
	return out;
}

template <class T>
const CMatrix4x4<T> CMatrix4x4<T>::Orthographic(float left, float right, float top, float bottom, float _near, float _far)
{
    float tx = -((right + left) / (right - left));
	float ty = -((top + bottom) / (top - bottom));
	float tz = -((_far + _near) / (_far - _near));
    
	CMatrix4x4<T> out = CMatrix4x4<T>::Zero();
    
    out.Element(0, 0, static_cast<T>(2.0 / (right - left)));
    out.Element(3, 0, static_cast<T>(tx));
    out.Element(1, 1, static_cast<T>(2.0 / (top - bottom)));
    out.Element(3, 1, static_cast<T>(ty));
    out.Element(2, 2, static_cast<T>(-2.0 / (_far - _near)));
    out.Element(3, 2, static_cast<T>(tz));
    out.Element(3, 3, static_cast<T>(1.0));
    
	return out;
}


// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

template class jam::CMatrix4x4<float>;
template class jam::CMatrix4x4<double>;
template class jam::CMatrix4x4<int>;
