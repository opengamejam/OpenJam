//
//  IMesh.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef IRENDERVIEW_H
#define	IRENDERVIEW_H

#include "Global.h"

namespace jam
{    
    
CLASS_PTR(IRenderTarget);
    
class IRenderView : public std::enable_shared_from_this<IRenderView>
{
public:
	IRenderView(uint32_t width, uint32_t height)
		: m_Width(width)
		, m_Height(height)
	{}
    virtual ~IRenderView() = default;

	virtual void CreateView() = 0;
    virtual void Begin() const = 0;
    virtual void End() const = 0;
    virtual void UpdateEvents() const = 0;

    virtual IRenderTargetPtr DefaultRenderTarget() const = 0;
	INL uint32_t Width() const { return m_Width; }
	INL uint32_t Height() const { return m_Height; }

private:
	uint32_t m_Width;
	uint32_t m_Height;
};

}; // namespace jam

#endif	/* IRENDERVIEW_H */

