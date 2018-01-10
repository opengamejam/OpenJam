//
//  CIndexBufferOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CINDEXBUFFEROGLBASE_H
#define CINDEXBUFFEROGLBASE_H

#include "IIndexBuffer.h"

namespace jam {

class CIndexBufferOGLBase : public IIndexBuffer {
    JAM_OBJECT
public:
    CIndexBufferOGLBase();
    virtual ~CIndexBufferOGLBase();

    virtual void Initialize(DataTypes dataType) override;
    virtual void Shutdown() override;
    virtual bool IsValid() const override;

    virtual uint64_t SizeRaw() const override;
    virtual void ResizeRaw(uint64_t newSize) override;

    virtual uint64_t ElementSize() const override;

    virtual void* LockRaw() override;
    virtual SIndexStream& Lock() override;
    virtual bool IsLocked() const override;
    virtual void Unlock(bool isNeedCommit = false) override;

    virtual void Bind() override;
    virtual void Unbind() override;

protected:
    virtual void ElementSize(uint64_t elementSize) override;

private:
    unsigned int m_Id;
    std::vector<char> m_Buffer;
    uint64_t m_ElementSize;
    bool m_IsLocked;
    SIndexStream m_Stream;
};

}; // namespace jam

#endif /* CINDEXBUFFEROGLBASE_H */

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
