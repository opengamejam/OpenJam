//
//  CShaderSourceSprite.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "CShaderSourceSprite.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

const std::string CShaderSourceSprite::s_GeomentryShader;
const std::string CShaderSourceSprite::s_VertexShader = MULTI_LINE_STRING(
\n #ifdef OGL2_0\n

    void main(void) {
        mat4 MVP = MainProjectionMatrix * MainModelMatrix;
        gl_Position = MVP * MainVertexPosition;

        VaryingTextureCoord = MainVertexUV;
        VaryingMainColor = MainVertexColor;
    }
\n #endif \n

\n #ifdef OGLES3\n

\n #endif \n);

// *****************************************************************************
// Public Methods
// *****************************************************************************

const std::string& CShaderSourceSprite::Vertex() const
{
    return s_VertexShader;
}

const std::string& CShaderSourceSprite::Geometry() const
{
    return s_GeomentryShader;
}

bool CShaderSourceSprite::Load(const std::string& filename)
{
    return true;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
