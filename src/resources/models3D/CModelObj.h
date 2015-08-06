//
//  CModelObj.h
//  OpenJam
//
//  Created by yev on 10/6/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CMODELOBJ_H
#define CMODELOBJ_H

#include "Global.h"
#include "IModel3D.h"
#include "CVector3D.h"

namespace jam
{

class CModelObj : public IModel3D
{
public:
    CModelObj(const std::string& filename);
    virtual ~CModelObj();
    
    virtual bool Load();
    
    virtual const std::vector<CVector3Df>& Vertices(const std::string& group = "default");
    virtual const std::vector<CVector3Df>& Normals(const std::string& group = "default");
    virtual const std::vector<CVector2Df>& UVs(const std::string& group = "default");
    virtual const std::vector<unsigned int>& Indices(const std::string& group = "default");
    virtual const std::string& TextureName(const std::string& group = "default");
    virtual const std::set<std::string>& Groups() const;
    
private:
    bool ParseLine(const std::string& line,
                   std::vector<CVector3Df>& srcVertices,
                   std::vector<CVector3Df>& srcNormals,
                   std::vector<CVector2Df>& srcUVs,
                   std::vector<CVector3Df>& dstVertices,
                   std::vector<CVector3Df>& dstNormals,
                   std::vector<CVector2Df>& dstUVs,
                   std::string& dstTexture,
                   std::string& group);
    
private:
    std::map<std::string, std::vector<CVector3Df> > m_Vertices;
    std::map<std::string, std::vector<CVector3Df> > m_Normals;
    std::map<std::string, std::vector<CVector2Df> > m_UVs;
    std::map<std::string, std::vector<unsigned int> > m_Indices;
    std::map<std::string, std::string> m_TextureNames;
    std::set<std::string> m_Groups;
};

}; // namespace jam

#endif /* defined(CMODELOBJ_H) */
