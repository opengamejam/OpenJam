//
//  CNonCopyable.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#ifndef CNONCOPYABLE_H
#define CNONCOPYABLE_H

namespace jam {

/*
 * Inheritance from this class won't allow copy derived class
 */
class CNonCopyable
{
    /*
     * Private copy constructor
     */
    CNonCopyable(const CNonCopyable&);
    
    /*
     * Private assign operator
     */
    CNonCopyable& operator=(const CNonCopyable&);
};
    
}; // namespace jam

#endif /* CNONCOPYABLE_H */
