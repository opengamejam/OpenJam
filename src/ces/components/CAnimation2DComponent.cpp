//
//  CAnimation2DComponent.cpp
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 2/6/15.
//
//

#include "CAnimation2DComponent.h"
#include "CMathFunc.hpp"
#include "ISprite.h"
#include "CImagePVR.h"
#include "CImageSegaPVR.h"
#include "RenderGlobal.h"
#include "CResourceCache.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

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
            TSequence& sequence = m_Sequences[sequenceDef.name];

            m_SequenceDurations[sequenceDef.name] = 0;
            unsigned long& duration = m_SequenceDurations[sequenceDef.name];

            const ISprite::TFramesList& frames = sequenceDef.frames;
            std::for_each(frames.begin(), frames.end(), [&](const CFrameDef& frameDef)
            {
                SFrame frame;
                // Object transform
                frame.transform.Position(CVector3Df(frameDef.offsetX, frameDef.offsetY, 0.0f));
                frame.transform.Scale(CVector3Df(frameDef.originalWidth, frameDef.originalHeight, 1.0f));

                // Texture frame
                frame.textureFrame.push_back(frameDef.textureFrame.u); // (0, 0)
                frame.textureFrame.push_back(CVector2Df(frameDef.textureFrame.v.X(),
                                                        frameDef.textureFrame.u.Y())); // (1, 0)
                frame.textureFrame.push_back(CVector2Df(frameDef.textureFrame.u.X(),
                                                        frameDef.textureFrame.v.Y())); // (0, 1)
                frame.textureFrame.push_back(frameDef.textureFrame.v); // (1, 1)

                frame.textureName = textures[frameDef.textureFrame.textureIdx];
                frame.duration = frameDef.animDuration;
                duration += frameDef.animDuration;

                sequence.push_back(frame);
            });
        });

        // Load texture atlases
        LoadTextures(textures);
    }
    else
    {
        m_SequencesNames.push_back("null");
        TSequence& sequence = m_Sequences["null"];
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
        m_FullTime = m_SequenceDurations[m_AnimationName];
    }
}

uint32_t CAnimation2DComponent::AnimationIndex() const
{
    uint32_t index = 0;
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

void CAnimation2DComponent::AnimationIndex(uint32_t index)
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
    m_Time = static_cast<unsigned long>(clamp<float>(percent, 0.0f, 1.0f) * m_FullTime);
}

uint32_t CAnimation2DComponent::FrameId()
{
    const TSequence& sequence = Sequence();
    return static_cast<unsigned int>(Percentage() * sequence.size());
}

bool CAnimation2DComponent::FrameId(uint32_t frameIndex)
{
    if (FrameId() != frameIndex && frameIndex < Sequence().size())
    {
        m_Time = static_cast<unsigned long>(((float)frameIndex / Sequence().size()) * FullTime());

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

bool CAnimation2DComponent::IsPlay() const
{
    return m_IsPlay;
}

void CAnimation2DComponent::Play(bool value)
{
    m_IsPlay = value;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

const CAnimation2DComponent::TSequence& CAnimation2DComponent::Sequence()
{
    return m_Sequences[m_AnimationName];
}

void CAnimation2DComponent::Cache()
{
    const TSequence& sequence = Sequence();
    unsigned int frameId    = FrameId();
    m_CachedFrameTransform  = sequence[frameId].transform;
    m_CachedTextureFrame    = sequence[frameId].textureFrame;
    m_CachedTextureName     = sequence[frameId].textureName;
    m_CachedTexture         = m_Textures[m_CachedTextureName];
    m_CachedIsStatic        = (sequence.size() <= 1);
}

void CAnimation2DComponent::LoadTextures(const std::vector<std::string>& textureNames)
{
    CResourceCache<ITexture> textureCache;

    std::for_each(textureNames.begin(), textureNames.end(), [&](const std::string& textureName)
    {
        ITexturePtr texture = textureCache.AcquireResource(textureName, false,
                                                       [](const std::string& filename) -> ITexturePtr
        {
            CResourceCache<IImage> imageCache;
            IImagePtr image = imageCache.AcquireResource(filename, false,
                                                        [](const std::string& filename) -> IImagePtr
            {
#ifdef OS_KOS
                IImagePtr resultImage(new CImageDreamPVR(filename));
#else
                IImagePtr resultImage(new CImagePVR(filename));
#endif
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

        m_Textures[textureName] = texture;
    });
}
