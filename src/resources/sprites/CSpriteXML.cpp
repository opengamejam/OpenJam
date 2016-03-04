/* 
 * File:   CSpriteXML.cpp
 * Author: yevgeniy.logachev
 */

#include "CSpriteXML.h"
#include "Global.h"
#include "ILoader.h"
#include "tinyxml.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CSpriteXML::CSpriteXML(const std::string& filename, const std::string& loader)
: ISprite(filename, loader)
{
    
}

CSpriteXML::~CSpriteXML()
{
    
}

bool CSpriteXML::Load()
{
    bool ret = false;
    
    m_Textures.clear();
    m_Frames.clear();
    m_Animations.clear();
    
    if (IResource::Load())
    {
        ret = Parse(RawData());
		if (!ret)
		{
			m_Textures.clear();
			m_Frames.clear();
			m_Animations.clear();
		}
    }

	return ret;
}

const ISprite::TTexturesList& CSpriteXML::Textures() const
{
    return m_Textures;
}

const ISprite::TTextureFramesList& CSpriteXML::Frames() const
{
    return m_Frames;
}

const ISprite::TAnimationsList& CSpriteXML::Animations() const
{
    return m_Animations;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

bool CSpriteXML::Parse(const TResourceData& data)
{
	TiXmlDocument xmlDoc;
	if (!xmlDoc.Parse(reinterpret_cast<const char*>(data.data()), 0, TIXML_ENCODING_UTF8))
	{
		return false;
	}

	const TiXmlElement* xmlRoot = xmlDoc.FirstChildElement("MappingEntry");
    if (!xmlRoot)
    {
        return false;
    }
    
    // Textures
	const TiXmlElement* xmlTextures = xmlRoot->FirstChildElement("Textures");
    if (!ParseTextures(xmlTextures))
    {
        return false;
    }

	// Texture frames
	const TiXmlElement* xmlFrames = xmlRoot->FirstChildElement("Frames");
    if (!ParseFrames(xmlFrames))
    {
        return false;
    }

	// Load animations
	const TiXmlElement* xmlAnimations = xmlRoot->FirstChildElement("Animations");
	if (!ParseAnimations(xmlAnimations))
    {
        return false;
    }
    
    return true;
}

bool CSpriteXML::ParseTextures(const TiXmlElement* xmlTextures)
{
    if (xmlTextures == NULL)
    {
        return false;
    }
    
    const TiXmlElement* xmlTexture = xmlTextures->FirstChildElement("Texture");
    while (xmlTexture)
    {
        const char* textureName = xmlTexture->GetText();
        if (textureName != NULL)
        {
            m_Textures.push_back(textureName);
        }
        
        xmlTexture = xmlTexture->NextSiblingElement("Texture");
    }
    
    return true;
}

bool CSpriteXML::ParseFrames(const TiXmlElement* xmlFrames)
{
    if (xmlFrames == NULL)
    {
        return false;
    }
    
    const TiXmlElement* xmlFrame = xmlFrames->FirstChildElement("Frame");
    while (xmlFrame)
    {
        bool corrupted = false;
        CTextureFrame textureFrame;
        if (xmlFrame->Attribute("name") == NULL)
        {
            corrupted = true;
        }
        else
        {
            textureFrame.name = xmlFrame->Attribute("name");
        }
        
        if (xmlFrame->Attribute("u"))
        {
            std::vector<std::string> components;
            SplitString(components, xmlFrame->Attribute("u"), ',');
            float x = 0.0f;
            if (components.size() > 1)
            {
                x = FromString<float>(ReplaceString(components[0], " ", ""));
            }
            float y = 0.0f;
            if (components.size() > 1)
            {
                y = FromString<float>(ReplaceString(components[1], " ", ""));
            }
            
            textureFrame.u = glm::vec2(x, y);
        }
        if (xmlFrame->Attribute("v"))
        {
            std::vector<std::string> components;
            SplitString(components, xmlFrame->Attribute("v"), ',');
            float x = 0.0f;
            if (components.size() > 1)
            {
                x = FromString<float>(ReplaceString(components[0], " ", ""));
            }
            float y = 0.0f;
            if (components.size() > 1)
            {
                y = FromString<float>(ReplaceString(components[1], " ", ""));
            }
            
            textureFrame.v = glm::vec2(x, y);
        }
        if (xmlFrame->Attribute("textureIdx", (int *)(&textureFrame.textureIdx)) == NULL)
        {
            corrupted = true;
        }
        
        if (!corrupted)
        {
            m_Frames.push_back(textureFrame);
        }
        
        xmlFrame = xmlFrame->NextSiblingElement("Frame");
    }
    
    return true;
}

bool CSpriteXML::ParseAnimations(const TiXmlElement* xmlAnimations)
{
    if (xmlAnimations == NULL)
    {
        return false;
    }
    
    const TiXmlElement* xmlAnimation = xmlAnimations->FirstChildElement("Animation");
    while (xmlAnimation)
    {
        const TiXmlElement* xmlFrames = xmlAnimation->FirstChildElement("Frames");
        if (!xmlFrames)
        {
            continue;
        }
        
        size_t animIndx = m_Animations.size();
        std::string name;
        if (xmlAnimation->Attribute("name") == NULL)
        {
            std::stringstream ss;
            ss << animIndx;
            
            name = ss.str();
        }
        else
        {
            name = xmlAnimation->Attribute("name");
        }
        
        CAnimationDef animation(name, TFramesList());
        
        const TiXmlElement* xmlFrame = xmlFrames->FirstChildElement("Frame");
        while (xmlFrame)
        {
            unsigned int frameIndx = 0;
            if (xmlFrame->Attribute("frameNumber", (int *)(&frameIndx)) != NULL)
            {
                CFrameDef frame;
                
                xmlFrame->Attribute("offsetX", &frame.offsetX);
                xmlFrame->Attribute("offsetY", &frame.offsetY);
                xmlFrame->Attribute("originalWidth", &frame.originalWidth);
                xmlFrame->Attribute("originalHeight", &frame.originalHeight);
                xmlFrame->Attribute("animDuration", (int *)(&frame.animDuration));
                
                assert(frameIndx < m_Frames.size());
                if (frameIndx < m_Frames.size())
                {
                    frame.textureFrame = m_Frames[frameIndx];
                }
                
                animation.frames.push_back(frame);
            }
            
            xmlFrame = xmlFrame->NextSiblingElement("Frame");
        }
        
        m_Animations.push_back(animation);
        
        xmlAnimation = xmlAnimation->NextSiblingElement("Animation");
    }
    
    return true;
}

