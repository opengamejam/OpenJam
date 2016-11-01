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

namespace jam {

class CModelObj : public IModel3D {
public:
    CModelObj(const std::string& filename);
    virtual ~CModelObj();

    virtual bool Load() override;

    virtual const std::vector<glm::vec3>& Vertices(const std::string& group = "default") override;
    virtual const std::vector<glm::vec3>& Normals(const std::string& group = "default") override;
    virtual const std::vector<glm::vec2>& UVs(const std::string& group = "default") override;
    virtual const std::vector<unsigned int>& Indices(const std::string& group = "default") override;
    virtual const std::string& TextureName(const std::string& group = "default") override;
    virtual const std::set<std::string>& Groups() const override;

private:
    bool ParseLine(const std::string& line,
        std::vector<glm::vec3>& srcVertices,
        std::vector<glm::vec3>& srcNormals,
        std::vector<glm::vec2>& srcUVs,
        std::vector<glm::vec3>& dstVertices,
        std::vector<glm::vec3>& dstNormals,
        std::vector<glm::vec2>& dstUVs,
        std::string& dstTexture,
        std::string& group);

private:
    std::unordered_map<std::string, std::vector<glm::vec3> > m_Vertices;
    std::unordered_map<std::string, std::vector<glm::vec3> > m_Normals;
    std::unordered_map<std::string, std::vector<glm::vec2> > m_UVs;
    std::unordered_map<std::string, std::vector<unsigned int> > m_Indices;
    std::unordered_map<std::string, std::string> m_TextureNames;
    std::set<std::string> m_Groups;
};

}; // namespace jam

#endif /* defined(CMODELOBJ_H) */
