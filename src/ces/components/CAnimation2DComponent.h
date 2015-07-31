//
//  CAnimation2DComponent.h
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 2/6/15.
//
//

#ifndef __Arkanoid__CAnimation2DComponent__
#define __Arkanoid__CAnimation2DComponent__

#include "IComponent.h"
#include "CTransform.h"
#include "CMatrix4x4.h"

namespace jam
{
CLASS_PTR(CAnimation2DComponent);
CLASS_PTR(ISprite);
CLASS_PTR(ITexture);

class CAnimation2DComponent final : public IComponent
{
public:
    struct SFrame
    {
        std::string textureName;
        CMatrix4x4f textureMatrix;
        std::vector<CVector2Df> textureFrame;
        CTransform3Df transform;
        unsigned long duration;
    };
    typedef std::vector<SFrame> TSequence;
    
    enum AnimationFlags
    {
        FrameChanged = 0x01
    };
    
public:
    CAnimation2DComponent();
    ~CAnimation2DComponent();
    
    void Sprite(ISpritePtr sprite);
    ISpritePtr Sprite() const;
    
    const CTransform3Df& FrameTransform();
    const std::vector<CVector2Df>& TextureFrame();
    const std::string& TextureName();
    ITexturePtr Texture();
    bool IsStatic();
    
    const std::string& AnimationName() const;
    void AnimationName(const std::string& name);
    
    unsigned int AnimationIndex() const;
    void AnimationIndex(unsigned index);
    
    unsigned long Time() const;
    bool Time(unsigned long ms);
    
    float Percentage() const;
    void Percentage(float percent);
    
    unsigned int FrameId();
    bool FrameId(unsigned int frameIndex);
    
    unsigned long FullTime() const;
    void FullTime(unsigned long ms);
    
    bool IsPlay() const;
    void Play(bool value);
    
private:
    const TSequence& Sequence();
    void Cache();
    void LoadTextures(const std::vector<std::string>& textureNames);
    
private:
    ISpritePtr m_Sprite;
    
    std::map<std::string, TSequence> m_Sequences;
    std::vector<std::string> m_SequencesNames;
    std::map<std::string, unsigned long> m_SequenceDurations;
    std::map<std::string, ITexturePtr> m_Textures;
    
    CTransform3Df m_CachedFrameTransform;
    std::vector<CVector2Df> m_CachedTextureFrame;
    std::string m_CachedTextureName;
    ITexturePtr m_CachedTexture;
    bool m_CachedIsStatic;
    
    unsigned long m_FullTime;
    unsigned long m_Time;
    
    std::string m_AnimationName;
    bool m_IsPlay;
};
    
}; // namespace jam

#endif /* defined(__Arkanoid__CAnimation2DComponent__) */
