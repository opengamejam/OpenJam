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

	virtual void CreateView();
    virtual void Begin() const;
    virtual void End() const;
    virtual void UpdateEvents() const;

    virtual IRenderTargetPtr DefaultRenderTarget() const;

private:
    IRenderTargetPtr m_DefaultRenderTarget;
    maple_device_t* m_Joy;
};

}; // namespace jam

#endif /* CRENDERVIEWDREAMCAST_H */
#endif// defined(OS_KOS)
