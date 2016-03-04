//
//  CMathFunc.h
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 2/7/15.
//
//

#ifndef __Arkanoid__CMathFunc__
#define __Arkanoid__CMathFunc__

#include "Global.h"
#include "CStringUtils.h"

namespace jam
{
    
template <class T>
inline static T clamp(const T& n, const T& lower, const T& upper)
{
    return std::max(lower, std::min(n, upper));
}
    
}; // namespace jam

#endif /* defined(__Arkanoid__CMathFunc__) */
