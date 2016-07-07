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
#include "CTransform.hpp"

namespace jam
{
CLASS_PTR(CAnimation2DComponent)
CLASS_PTR(ISprite)

class CAnimation2DComponent final : public CComponentBase
{
    JAM_OBJECT
public:
    struct SFrame
    {
        std::string textureName;
        glm::mat4x4 textureMatrix;
        std::vector<glm::vec2> textureFrame;
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
    const std::vector<glm::vec2>& TextureFrame();
    const std::string& TextureName();
    bool IsStatic();
    
    const std::string& AnimationName() const;
    void AnimationName(const std::string& name);
    
    uint32_t AnimationIndex() const;
    void AnimationIndex(uint32_t index);
    
    unsigned long Time() const;
    bool Time(unsigned long ms);
    
    float Percentage() const;
    void Percentage(float percent);
    
    uint32_t FrameId();
    bool FrameId(uint32_t frameIndex);
    
    unsigned long FullTime() const;
    void FullTime(unsigned long ms);
    
    bool IsPlay() const;
    void Play(bool value);
    
private:
    const TSequence& Sequence();
    void Cache();
    
private:
    ISpritePtr m_Sprite;
    
    std::unordered_map<std::string, TSequence> m_Sequences;
    std::vector<std::string> m_SequencesNames;
    std::unordered_map<std::string, unsigned long> m_SequenceDurations;
    
    CTransform3Df m_CachedFrameTransform;
    std::vector<glm::vec2> m_CachedTextureFrame;
    std::string m_CachedTextureName;
    bool m_CachedIsStatic;
    
    unsigned long m_FullTime;
    unsigned long m_Time;
    
    std::string m_AnimationName;
    bool m_IsPlay;
};
    
}; // namespace jam

#endif /* defined(__Arkanoid__CAnimation2DComponent__) */
