//
//  ISprite.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef ISPRITE_H
#define	ISPRITE_H

#include "Global.h"
#include "IResource.h"
#include "CMath.h"

namespace jam
{
    
class CTextureFrame
{
public:
    CTextureFrame(const std::string& _name = "",
                  uint32_t _textureIdx = 0,
                  const glm::vec2& _u = glm::vec2(),
                  const glm::vec2& _v = glm::vec2())
        : name(_name)
        , textureIdx(_textureIdx)
        , u(_u)
        , v(_v)
    {}
    
    std::string name;
    uint32_t textureIdx;
    glm::vec2 u;
    glm::vec2 v;
};

struct CFrameDef
{
	CFrameDef(float _offsetX = 0,
              float _offsetY = 0,
              float _originalWidth = 0,
              float _originalHeight = 0,
              uint32_t _animDuration = 50)
        : offsetX(_offsetX)
        , offsetY(_offsetY)
        , originalWidth(_originalWidth)
        , originalHeight(_originalHeight)
        , animDuration(_animDuration)
	{}

	void Reset()
	{
		offsetX = 0;
		offsetY = 0;
		originalWidth = 0;
		originalHeight = 0;
		animDuration = 50;
	}

	float offsetX;              // Additional offset on X axis, used for centering frame for animation
	float offsetY;              // Additional offset on Y axis, used for centering frame for animation
	float originalWidth;        // For future use
	float originalHeight;       // For future use
	uint32_t animDuration;      // Delay, which sprite will show, in ms
    
    CTextureFrame textureFrame;
};
    
class CAnimationDef
{
public:
    CAnimationDef(const std::string& _name,
                  const std::vector<CFrameDef>& _frames)
        : name(_name)
        , frames(_frames)
    {}
    
    std::string name;
    std::vector<CFrameDef> frames;
};

class ISprite : public IResource
{
public:
    ISprite(const std::string& filename)
    : IResource(filename)
    {}
    
    virtual ~ISprite() = default;
    
	typedef std::vector<std::string> TTexturesList;
	typedef std::vector<CFrameDef> TFramesList;
    typedef std::vector<CTextureFrame> TTextureFramesList;
	typedef std::vector<CAnimationDef> TAnimationsList;

public:
    virtual const TTexturesList& Textures() const = 0;
	virtual const TTextureFramesList& Frames() const = 0;
	virtual const TAnimationsList& Animations() const = 0;
};

}; // namespace jam

#endif	/* ISPRITE_H */

