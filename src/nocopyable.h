/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: nocopyable.h
Version: 1.0
Date: 2016.1.13

History:
shengkaishan     2016.1.13   1.0     Create
******************************************************************************/


#ifndef __UTILITY_NOCOPYABLE_H__
#define __UTILITY_NOCOPYABLE_H__

namespace future {
    class nocopyable
    {
    protected:
        nocopyable(){}
        virtual ~nocopyable(){}
    private:
        nocopyable(const nocopyable&);
        nocopyable& operator = (const nocopyable&);
    };
}
#endif

