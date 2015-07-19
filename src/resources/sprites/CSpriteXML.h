/* 
 * File:   CSpriteXML.h
 * Author: yevgeniy.logachev
 */

#ifndef CSpriteXML_H
#define	CSpriteXML_H

#include "Global.h"
#include "ISprite.h"

class TiXmlElement;

namespace jam
{

class CSpriteXML : public ISprite
{
public:
    CSpriteXML(const std::string& filename, const std::string& loader = "");
    virtual ~CSpriteXML();
    
    virtual bool Load();
	virtual const TTexturesList& Textures() const;
    virtual const TTextureFramesList& Frames() const;
	virtual const TAnimationsList& Animations() const;
    
private:
	bool Parse(const TResourceData& data);
    
    bool ParseTextures(const TiXmlElement* xmlTextures);
    bool ParseFrames(const TiXmlElement* xmlFrames);
    bool ParseAnimations(const TiXmlElement* xmlAnimations);

private:
	TResourceData m_Data;
	TTexturesList m_Textures;
    TTextureFramesList m_Frames;
	TAnimationsList m_Animations;
};

}; // namespace jam

#endif	/* CSpriteXML_H */
