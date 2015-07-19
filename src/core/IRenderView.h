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
	IRenderView(unsigned int width, unsigned int height)
		: m_Width(width)
		, m_Height(height)
	{}
    virtual ~IRenderView() = default;

	virtual void CreateView() = 0;
    virtual void Begin() const = 0;
    virtual void End() const = 0;
    virtual void UpdateEvents() const = 0;

    virtual IRenderTargetPtr DefaultRenderTarget() const = 0;
	INL unsigned int Width() const { return m_Width; }
	INL unsigned int Height() const { return m_Height; }

private:
	unsigned int m_Width;
	unsigned int m_Height;
};

}; // namespace jam

#endif	/* IRENDERVIEW_H */

