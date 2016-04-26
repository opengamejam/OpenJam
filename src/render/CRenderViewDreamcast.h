/*
 * CRenderViewDreamcast.h
 *
 *  Created on: Jan 31, 2015
 *      Author: yev
 */

#if defined(OS_KOS)

#ifndef CRENDERVIEWDREAMCAST_H
#define CRENDERVIEWDREAMCAST_H

#include "Global.h"
#include "IRenderView.h"
#include "IEventable.h"

namespace jam
{

class CRenderViewDreamcast : public IRenderView, public IEventable
{
public:
	CRenderViewDreamcast();
	virtual ~CRenderViewDreamcast();

	virtual void CreateView() override;
    virtual void Begin() const override;
    virtual void End() const override;
    virtual void UpdateEvents() const override;

    virtual IRendererPtr Renderer() const override;
    virtual IRenderTargetPtr DefaultRenderTarget() const override;

private:
    IRendererPtr m_Renderer;
    IRenderTargetPtr m_DefaultRenderTarget;
    maple_device_t* m_Joy;
};

}; // namespace jam

#endif /* CRENDERVIEWDREAMCAST_H */
#endif// defined(OS_KOS)
