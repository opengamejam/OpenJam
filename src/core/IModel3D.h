//
//  IModel3D.h
//  OpenJam
//
//  Created by yev on 10/6/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef IMODEL3D_H
#define IMODEL3D_H

#include "Global.h"
#include "IResource.h"
#include "CVector3D.h"

namespace jam
{

class IModel3D : public IResource
{
public:
    IModel3D(const std::string& filename)
        : IResource(filename, "")
    {}
    virtual ~IModel3D() = default;
    
    virtual const std::vector<CVector3Df>& Vertices(const std::string& group = "default") = 0;
    virtual const std::vector<CVector3Df>& Normals(const std::string& group = "default") = 0;
    virtual const std::vector<CVector2Df>& UVs(const std::string& group = "default") = 0;
    virtual const std::vector<unsigned int>& Indices(const std::string& group = "default") = 0;
    virtual const std::string& TextureName(const std::string& group = "default") = 0;
    virtual const std::set<std::string>& Groups() const = 0;
};

}; // namespace jam

#endif /* defined(IMODEL3D_H) */
