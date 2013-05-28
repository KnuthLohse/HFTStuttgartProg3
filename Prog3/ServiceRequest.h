//
//  Task.h
//  Prog3
//
//  Created by Knuth Lohse on 28.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__Task__
#define __Prog3__Task__

#include <iostream>
#include "ConfigurationObjWrapper.h"
#include "ConfigurationObj.h"

#endif /* defined(__Prog3__Task__) */
 
class ServiceRequest : public ConfigurationObjWrapper
{
public:
    ServiceRequest(ConfigurationObj * conf);
    
    int getDuration();
    bool isDone();
    bool setDone();
private:
    bool done;


};