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
#include "CVector2D.h"

namespace jam
{

CLASS_PTR(CTextureFrame);
    
class CTextureFrame
{
public:
    CTextureFrame(const std::string& _name = "",
                  unsigned int _textureIdx = 0,
                  const CVector2Df& _u = CVector2Df(),
                  const CVector2Df& _v = CVector2Df())
        : name(_name)
        , textureIdx(_textureIdx)
        , u(_u)
        , v(_v)
    {}
    
    std::string name;
    unsigned int textureIdx;
    CVector2Df  u;
    CVector2Df  v;
};

struct CFrameDef
{
	CFrameDef(float _offsetX = 0,
              float _offsetY = 0,
              float _originalWidth = 0,
              float _originalHeight = 0,
              unsigned int _animDuration = 50)
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

	float        offsetX;		// Additional offset on X axis, used for centering frame for animation
	float        offsetY;		// Additional offset on Y axis, used for centering frame for animation
	float        originalWidth; // For future use
	float        originalHeight;// For future use
	unsigned int animDuration;	// Delay, which sprite will show, in ms
    
    CTextureFramePtr textureFrame;
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
	typedef std::vector<std::string>                TTexturesList;
	typedef std::vector<CFrameDef>                  TFramesList;
    typedef std::vector<CTextureFramePtr>           TTextureFramesList;
	typedef std::vector<CAnimationDef>              TAnimationsList;

public:
    virtual const TTexturesList& Textures() const = 0;
	virtual const TTextureFramesList& Frames() const = 0;
	virtual const TAnimationsList& Animations() const = 0;
    
protected:
    ISprite(const std::string& filename, const std::string& loader)
        : IResource(filename, loader)
    {}
    
    virtual ~ISprite() = default;
};

}; // namespace jam

#endif	/* ISPRITE_H */
