 
//
//  CShaderSourceInsert.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef CShaderSourceInsert_H
#define CShaderSourceInsert_H

#include "Global.h"
#include "IShaderSource.h"

namespace jam {

class CShaderSourceInsert : public IShaderSource {
public:
    virtual const std::string& Vertex() const override;
    virtual const std::string& Fragment() const override;
    virtual const std::string& Geometry() const override;

    virtual bool Load(const std::string& filename) override;

private:
    static const std::string s_VertexShader;
    static const std::string s_FragmentShader;
    static const std::string s_GeomentryShader;
};

}; // namespace jam

#endif /* defined(CShaderSourceInsert_H) */
    
