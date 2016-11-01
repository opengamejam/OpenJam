//
//  CTouchComponent.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/31/16.
//
//

#ifndef CTOUCHCOMPONENT_H
#define CTOUCHCOMPONENT_H

#include "Global.h"
#include "IComponent.h"

namespace jam {
CLASS_PTR(CUpdateComponent)

class CTouchComponent final : public CComponentBase {
    JAM_OBJECT
public:
    typedef std::function<void(const glm::vec2&)> TTouchFunc;

public:
    CTouchComponent();
    virtual ~CTouchComponent();

    void SetOnBeginTouch(const TTouchFunc& touchFunc);
    void SetOnMoveTouch(const TTouchFunc& touchFunc);
    void SetOnEndTouch(const TTouchFunc& touchFunc);

    void OnBeginTouch(const glm::vec2& touch);
    void OnMoveTouch(const glm::vec2& touch);
    void OnEndTouch(const glm::vec2& touch);

private:
    TTouchFunc m_BeginTouchFunc;
    TTouchFunc m_MoveTouchFunc;
    TTouchFunc m_EndTouchFunc;
};

}; // namespace jam

#endif /* CTOUCHCOMPONENT_H */
