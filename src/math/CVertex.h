//
//  CVertex.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/20/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CVERTEX_H
#define CVERTEX_H

#include "CVector2D.h"
#include "CVector3D.h"

namespace jam
{

template< class T, template <class X> class Pos, template<class Y> class Tex >
class CVertex
{
public:
    Pos<T> Position;
    Tex<T> Texcoord;
    
    unsigned int StrideVertex() const;
    unsigned int OffsetVertex() const;
    unsigned int StrideBytesVertex() const;
    
    unsigned int StrideTexcoords() const;
    unsigned int OffsetTexcoords() const;
    unsigned int StrideBytesTexcoords() const;
    
    unsigned int StrideAll() const;
    unsigned int StrideBytesAll() const;
};

typedef CVertex<float, CVector3D, CVector2D> CVertex3Df;
typedef CVertex<double, CVector3D, CVector2D> CVertex3Dd;
typedef CVertex<int, CVector3D, CVector2D> CVertex3Di;

}; // namespace jam

#endif /* defined(CVERTEX_H) */
