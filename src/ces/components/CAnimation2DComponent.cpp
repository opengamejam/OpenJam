//
//  CAnimation2DComponent.cpp
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 2/6/15.
//
//

#include "CAnimation2DComponent.h"
#include "CMathFunc.h"
#include "ISprite.h"
#include "CImagePVR.h"
#include "RenderGlobal.h"
#include "CResourceCache.hpp"

using namespace jam;

CAnimation2DComponent::CAnimation2DComponent()
: IComponent(ComponentId<CAnimation2DComponent>())
, m_Sprite(nullptr)
, m_CachedFrameTransform()
, m_CachedTexture(nullptr)
, m_CachedIsStatic(true)
, m_FullTime(0)
, m_Time(0)
, m_IsPlay(true)
{

}

CAnimation2DComponent::~CAnimation2DComponent()
{

}

void CAnimation2DComponent::Sprite(ISpritePtr sprite)
{
    if (sprite == m_Sprite)
    {
        return;
    }
    
    m_Sprite = sprite;
    m_Sequences.clear();
    m_SequencesNames.clear();
    
    if (sprite)
    {
        // Sequences
        const ISprite::TTexturesList& textures = Sprite()->Textures();
        const ISprite::TAnimationsList& sequences = Sprite()->Animations();
        std::for_each(sequences.begin(), sequences.end(), [&](const CAnimationDef& sequenceDef)
        {
            m_SequencesNames.push_back(sequenceDef.name);
            TSequenceList& sequence = m_Sequences[sequenceDef.name];
            
            const ISprite::TFramesList& frames = sequenceDef.frames;
            std::for_each(frames.begin(), frames.end(), [&](const CFrameDef& frameDef)
            {
                SFrame frame;
                // Object transform
                frame.transform.Position(CVector3Df(frameDef.offsetX, frameDef.offsetY, 0.0f));
                frame.transform.Scale(CVector3Df(frameDef.originalWidth, frameDef.originalHeight, 1.0f));
                
                // Texture frame
                frame.textureFrame.push_back(frameDef.textureFrame->u); // (0, 0)
                frame.textureFrame.push_back(CVector2Df(frameDef.textureFrame->v.X(),
                                                        frameDef.textureFrame->u.Y())); // (1, 0)
                frame.textureFrame.push_back(CVector2Df(frameDef.textureFrame->u.X(),
                                                        frameDef.textureFrame->v.Y())); // (0, 1)
                frame.textureFrame.push_back(frameDef.textureFrame->v); // (1, 1)
                
                // Load texture atlases
                frame.textureName = textures[frameDef.textureFrame->textureIdx];
                
                CResourceCache<ITexture> textureCache;
                ITexturePtr texture = textureCache.AcquireResource(frame.textureName, false,
                                                                   [](const std::string& filename) -> ITexturePtr
                {
                    CResourceCache<IImage> imageCache;
                    IImagePtr image = imageCache.AcquireResource(filename, false,
                                                                 [](const std::string& filename) -> IImagePtr
                    {
                        IImagePtr resultImage(new CImagePVR(filename));
                        if (!resultImage->Load())
                        {
                            resultImage = nullptr;
                        }
                        
                        return resultImage;
                    });
                 
                    ITexturePtr resultTexture = GRenderer->CreateTexture();
                    if (image)
                    {
                        resultTexture->AssignImage(image);
                    }
                    
                    return resultTexture;
                });
                
                m_Textures[frame.textureName] = texture;
                
                sequence.push_back(frame);
            });
        });
    }
    else
    {
        m_SequencesNames.push_back("null");
        TSequenceList& sequence = m_Sequences["null"];
        sequence.push_back(SFrame());
    }
    
    m_AnimationName = m_SequencesNames.front();
    m_FullTime = 2000;
    m_Time = 0;
    
    Cache();
}

ISpritePtr CAnimation2DComponent::Sprite() const
{
    return m_Sprite;
}

const CTransform3Df& CAnimation2DComponent::FrameTransform()
{
    return m_CachedFrameTransform;
}

const std::vector<CVector2Df>& CAnimation2DComponent::TextureFrame()
{
    return m_CachedTextureFrame;
}

const std::string& CAnimation2DComponent::TextureName()
{
    return m_CachedTextureName;
}

ITexturePtr CAnimation2DComponent::Texture()
{
    return m_CachedTexture;
}

bool CAnimation2DComponent::IsStatic() 
{
    return m_CachedIsStatic;
}

const std::string& CAnimation2DComponent::AnimationName() const
{
    return m_AnimationName;
}

void CAnimation2DComponent::AnimationName(const std::string& name)
{
    if (m_Sequences.find(name) != m_Sequences.end())
    {
        m_AnimationName = name;
        
        m_Time = 0;
        m_FullTime = 2000; // TODO:
    }
}

unsigned int CAnimation2DComponent::AnimationIndex() const
{
    unsigned int index = 0;
    std::all_of(m_SequencesNames.begin(), m_SequencesNames.end(), [&](const std::string& name)
    {
        if (name == m_AnimationName)
        {
            return false;
        }
        ++index;
        
        return true;
    });
    
    return index;
}

void CAnimation2DComponent::AnimationIndex(unsigned index)
{
    if (index < m_SequencesNames.size())
    {
        AnimationName(m_SequencesNames[index]);
    }
}

unsigned long CAnimation2DComponent::Time() const
{
    return m_Time;
}

bool CAnimation2DComponent::Time(unsigned long ms)
{
    unsigned int frameIndex = FrameId();
    m_Time = clamp<unsigned long>(ms, 0, m_FullTime);
    if (m_Time == m_FullTime)
    {
        m_Time = 0;
    }
    
    bool wasChanged = FrameId() != frameIndex;
    if (wasChanged)
    {
        Cache();
    }
    
    return wasChanged;
}

float CAnimation2DComponent::Percentage() const
{
    return (float)m_Time / m_FullTime;
}

void CAnimation2DComponent::Percentage(float percent)
{
    m_Time = clamp<float>(percent, 0.0f, 1.0f) * m_FullTime;
}

unsigned int CAnimation2DComponent::FrameId()
{
    const TSequenceList& sequence = Sequence();
    return (Percentage() * sequence.size());
}

bool CAnimation2DComponent::FrameId(unsigned int frameIndex)
{
    if (FrameId() != frameIndex && frameIndex < Sequence().size())
    {
        m_Time = ((float)frameIndex / Sequence().size()) * FullTime();
        
        Cache();
        
        return true;
    }
    
    return false;
}

unsigned long CAnimation2DComponent::FullTime() const
{
    return m_FullTime;
}

void CAnimation2DComponent::FullTime(unsigned long ms)
{
    m_FullTime = ms;
}

const CAnimation2DComponent::TSequenceList& CAnimation2DComponent::Sequence()
{
    return m_Sequences[m_AnimationName];
}

void CAnimation2DComponent::Cache()
{
    const TSequenceList& sequence = Sequence();
    unsigned int frameId    = FrameId();
    m_CachedFrameTransform  = sequence[frameId].transform;
    m_CachedTextureFrame    = sequence[frameId].textureFrame;
    m_CachedTextureName     = sequence[frameId].textureName;
    m_CachedTexture         = m_Textures[m_CachedTextureName];
    m_CachedIsStatic        = (sequence.size() <= 1);
}

bool CAnimation2DComponent::IsPlay() const
{
    return m_IsPlay;
}

void CAnimation2DComponent::Play(bool value)
{
    m_IsPlay = value;
}