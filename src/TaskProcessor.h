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
    /**
     * @param conf ConfigurationObject to retrive the Attributes from
     * @param reader ConfigurationReader to get the ServiceProcessor information from
     */
    TaskProcessor(ConfigurationObj *conf, ConfigurationReader *reader);
    
    /**
     * @return True if the Taskprocessor has at least one Serviceprocessor of the given type, else false
     */
    bool supports(std::string type);
    
    /**
     * @return QueueSize of the TaskProcessor
     */
    int getQueueSize();
    
private:

    ServiceProcessorV_t serviceProcessors; ///Vector of the serviceProcessors of this TaskProcessor
};




#endif /* defined(__Prog3__TaskProcessor__) */
