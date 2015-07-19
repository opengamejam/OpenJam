//
//  CVertex.cpp
//  OpenJam
//
//  Created by yev on 3/28/14.
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//

#include "CVertex.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::StrideVertex() const
{
    return sizeof(Position) / sizeof(T); 
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::OffsetVertex() const
{
    return offsetof(CVertex, Position);
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::StrideBytesVertex() const
{
    return sizeof(Position);
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::StrideTexcoords() const
{
    return sizeof(Texcoord) / sizeof(T);
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::OffsetTexcoords() const
{
    return offsetof(CVertex, Texcoord);
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::StrideBytesTexcoords() const
{
    return sizeof(Texcoord);
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::StrideAll() const
{
    return (StrideVertex() + StrideTexcoords());
}

template< class T, template <class X> class Pos, template<class Y> class Tex >
unsigned int CVertex<T, Pos, Tex>::StrideBytesAll() const
{
    return (StrideBytesVertex() + StrideBytesTexcoords());
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

template class jam::CVertex<float, CVector3D, CVector2D>;
template class jam::CVertex<double, CVector3D, CVector2D>;
template class jam::CVertex<int, CVector3D, CVector2D>;