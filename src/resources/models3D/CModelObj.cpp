//
//  CModelObj.cpp
//  OpenJam
//
//  Created by yev on 10/6/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CModelObj.h"
#include "CStringUtils.h"
#include "RenderGlobal.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

std::set<std::string> textures;

CModelObj::CModelObj(const std::string& filename)
: IModel3D(filename)
{
    
}

CModelObj::~CModelObj()
{
    
}

bool CModelObj::Load()
{
    if (IResource::Load())
    {
        std::vector<glm::vec3> srcVertices;
        std::vector<glm::vec3> srcNormals;
        std::vector<glm::vec2> srcUVs;
        std::string currentGroup = "default";
        
        const TResourceData& data = RawData();
        std::string line;
        std::for_each(data.begin(), data.end(), [&](char ch)
        {
            line += ch;
            if (ch == '\n')
            {
                line = ReplaceString(line, " \r", "");
                line = ReplaceString(line, " \n", "");
                line = ReplaceString(line, "\r", "");
                line = ReplaceString(line, "\n", "");
                
                bool result = ParseLine(line, srcVertices, srcNormals, srcUVs,
                                        m_Vertices[currentGroup], m_Normals[currentGroup], m_UVs[currentGroup],
                                        m_TextureNames[currentGroup], currentGroup);
                if (result)
                {
                    m_Groups.insert(currentGroup);
                }
                    
                line = "";
            }
        });
        
        return true;
    }
    
    return false;
}

const std::vector<glm::vec3>& CModelObj::Vertices(const std::string& group)
{
    return m_Vertices[group];
}

const std::vector<glm::vec3>& CModelObj::Normals(const std::string& group)
{
    return m_Normals[group];
}

const std::vector<glm::vec2>& CModelObj::UVs(const std::string& group)
{
    return m_UVs[group];
}

const std::vector<unsigned int>& CModelObj::Indices(const std::string& group)
{
    return m_Indices[group];
}

const std::string& CModelObj::TextureName(const std::string& group)
{
    return m_TextureNames[group];
}

const std::set<std::string>& CModelObj::Groups() const
{
    return m_Groups;
}

bool CModelObj::ParseLine(const std::string& line,
                          std::vector<glm::vec3>& srcVertices,
                          std::vector<glm::vec3>& srcNormals,
                          std::vector<glm::vec2>& srcUVs,
                          std::vector<glm::vec3>& dstVertices,
                          std::vector<glm::vec3>& dstNormals,
                          std::vector<glm::vec2>& dstUVs,
                          std::string& dstTexture,
                          std::string& group)
{
    bool result = false;
    std::vector<std::string> components;
    SplitString(components, line, ' ');
    
    if (components.empty())
    {
        return result;
    }
    
    if (components[0] == "v")
    {
        // TODO: glm
        //glm::vec3 vertex(ReplaceString(line, "v ", ""), ' ');
        //srcVertices.push_back(vertex);
        //result = true;
    }
    else if (components[0] == "vt")
    {
        // TODO: glm
        /*glm::vec2 uv(ReplaceString(line, "vt ", ""), ' ');
        srcUVs.push_back(uv);
        result = true;*/
    }
    else if (components[0] == "vn")
    {
        // TODO: glm
        /*glm::vec3 normal(ReplaceString(line, "vn ", ""), ' ');
        srcNormals.push_back(normal);
        result = true;*/
    }
    else if (components[0] == "f")
    {
        int64_t size = components.size();
        
        std::vector<std::string> faces;
        if (size == 4)
        {
            faces.push_back(components[1]);
            faces.push_back(components[2]);
            faces.push_back(components[3]);
        }
        else
        {
            for (long i = 1; i < size - 2; ++i)
            {
                faces.push_back(components[1]);
                faces.push_back(components[i + 2]);
                faces.push_back(components[i + 1]);
            }
        }
        
        for (unsigned long i = 0; i < faces.size(); ++i)
        {
            std::vector<std::string> face;
            SplitString(face, faces[i], '/');

            if (face.size() > 0)
            {
                unsigned int vertexIndex = FromString<unsigned int>(face[0]);
                dstVertices.push_back(srcVertices[vertexIndex - 1]);
            }
            if (face.size() > 1)
            {
                unsigned int uvIndex = FromString<unsigned int>(face[1]);
                dstUVs.push_back(srcUVs[uvIndex - 1]);
            }
            if (face.size() > 2)
            {
                unsigned int normalIndex = FromString<unsigned int>(face[2]);
                dstNormals.push_back(srcNormals[normalIndex - 1]);
            }
        }
        result = true;
    }
    else if (components[0] == "usemtl")
    {
        if (components.size() > 1)
        {
            dstTexture = components[1] + ".pvr"; // TODO: remove test extansion
            textures.insert(dstTexture);
        }
        result = true;
    }
    else if (components[0] == "g")
    {
        group = ReplaceString(line, "g ", "");
        result = true;
    }
    
    return result;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
