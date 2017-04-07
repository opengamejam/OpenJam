//
//  CNonCopyable.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#include "CNonCopyable.h"

using namespace jam;

CNonCopyable::CNonCopyable(const CNonCopyable&)
{

}

CNonCopyable& CNonCopyable::operator=(const CNonCopyable&)
{
    return *this;
}
