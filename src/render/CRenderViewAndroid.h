//
//  CRenderViewAndroid.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(OS_ANDROID)

#ifndef CRENDERVIEWANDROID_H
#define	CRENDERVIEWANDROID_H

#include "IRenderView.h"

namespace jam
{

class CRenderViewAndroid : public IRenderView
{
public:
	CRenderViewAndroid(unsigned int width, unsigned int height);
	virtual ~CRenderViewAndroid();

	virtual void CreateView();
	virtual void SwapBuffer() const;
    virtual void UpdateEvents() const;
};

}; // namespace jam

#endif /* CRENDERVIEWANDROID_H */

#endif /* OS_ANDROID */



