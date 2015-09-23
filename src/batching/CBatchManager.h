//
//  CBatchManager.h
//  TestApp
//
//  Created by yev on 8/25/15.
//
//

#ifndef __TestApp__CBatchManager__
#define __TestApp__CBatchManager__

#include "Global.h"

namespace jam
{
CLASS_PTR(CRenderComponent);
    
class CBatchManager
{
public:
    CBatchManager();
    virtual ~CBatchManager();
    
private:
    typedef std::list<CRenderComponentPtr> TBatchList;
    
    TBatchList m_StaticBatches;
    TBatchList m_DynamicBatches;
};
    
}; // namespace jam

#endif /* defined(__TestApp__CBatchManager__) */
