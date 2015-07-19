//
//  CGameObjectDesc.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/16/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CGAMEOBJECTDESC_H
#define CGAMEOBJECTDESC_H

#include "Global.h"
#include "CVector3D.h"

namespace jam
{

class CGameObjectDesc
{
public:
    
    std::string filename;
    std::string name;
    CVector3Df position;
    CVector3Df rotation;
    CVector3Df scale;
    CVector3Df size;
    
    
    CGameObjectDesc(const std::string& _filename = "",
                    const std::string& _name = "",
                    const CVector3Df& _position = CVector3Df(),
                    const CVector3Df& _rotation = CVector3Df(),
                    const CVector3Df& _scale = CVector3Df(1.0f, 1.0f, 1.0f),
                    const CVector3Df& _size = CVector3Df(1.0f, 1.0f, 1.0f))
    : filename(_filename)
    , name(_name)
    , position(_position)
    , rotation(_rotation)
    , scale(_scale)
    , size(_size)
    {}
};

}; // namespace jam

#endif // CGAMEOBJECTDESC_H
