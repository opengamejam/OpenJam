//
//  CTouchSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 8/31/16.
//
//

#ifndef CTOUCHSYSTEM_H
#define CTOUCHSYSTEM_H

#include "ISystem.h"

namespace jam {
CLASS_PTR(CTouchSystem)
CLASS_PTR(IRenderer)
CLASS_PTR(IRenderView)
CLASS_PTR(IFrameBuffer)

class CTouchSystem : public CSystemBase {
    JAM_OBJECT
public:
    CTouchSystem(IRenderViewPtr renderView);
    virtual ~CTouchSystem();

    virtual void Update(unsigned long dt) override;

private:
    IFrameBufferPtr m_RenderTarget;
};

} // namespace jam

#endif /* CTOUCHSYSTEM_H */
