 
//
//  CShaderSourceCommon.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef CShaderSourceCommon_H
#define CShaderSourceCommon_H

#include "Global.h"
#include "IShaderSource.h"

namespace jam {

class CShaderSourceCommon : public IShaderSource {
    JAM_OBJECT
public:
    virtual const std::string& Vertex() const override;
    virtual const std::string& Fragment() const override;
    virtual const std::string& Geometry() const override;

    virtual bool Load(const std::string& filename) override;
    
    const std::string& VertexAttributeName() const;
    const std::string& UVAttributeName() const;
    const std::string& NormalAttributeName() const;
    const std::string& ColorAttributeName() const;
    
    const std::string& ProjectionMatrixUniformName() const;
    const std::string& ViewMatrixUniformName() const;
    const std::string& ModelMatrixUniformName() const;
    const std::string& NormalMatrixUniformName() const;

private:
    static const std::string s_VertexShader;
    static const std::string s_FragmentShader;
    static const std::string s_GeomentryShader;
};

}; // namespace jam

#endif /* defined(CShaderSourceCommon_H) */
    
