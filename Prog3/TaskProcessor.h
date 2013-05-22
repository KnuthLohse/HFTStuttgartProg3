//
//  TaskProcessor.h
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__TaskProcessor__
#define __Prog3__TaskProcessor__

#include <iostream>
#include "ConfigurationObj.h"
#include "ServiceProcessor.h"
#include "ConfigurationReader.h"
#include "ConfigurationObjWrapper.h"
typedef std::vector<ServiceProcessor> ServiceProcessorV_t;


class TaskProcessor : public ConfigurationObjWrapper
{
public:
    TaskProcessor(ConfigurationObj *conf, ConfigurationReader *reader);
    bool supports(std::string type);
    int getQueueSize();
    
private:
    void createServiceProcessors();
    
    ServiceProcessorV_t serviceProcessors;
};




#endif /* defined(__Prog3__TaskProcessor__) */
