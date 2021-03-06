//
//  CUpdateComponent.h
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#ifndef CUPDATECOMPONENT_H
#define CUPDATECOMPONENT_H

#include "Global.h"
#include "IComponent.h"

namespace jam {
CLASS_PTR(CUpdateComponent)

class CUpdateComponent final : public CComponentBase {
    JAM_OBJECT
public:
    typedef std::function<void(unsigned long)> TUpdateFunc;

public:
    CUpdateComponent(const TUpdateFunc& updateFunc = nullptr);
    virtual ~CUpdateComponent();

    void SetUpdateFunc(const TUpdateFunc& updateFunc);

    void Update(unsigned long dt);

private:
    TUpdateFunc m_UpdateFunc;
};

}; // namespace jam

#endif /* defined(CUPDATECOMPONENT_H) */
