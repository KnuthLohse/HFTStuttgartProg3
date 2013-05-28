//
//  ServiceReader.h
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__ServiceReader__
#define __Prog3__ServiceReader__

#include <iostream>
#include "ConfigurationReader.h"
#include "TaskProcessor.h"

typedef std::vector<TaskProcessor> TaskProcessorV_t;

class ServiceReader : public ConfigurationReader {

public:
    /**
     * @param path Path and filename of the configurationFile to read
     */
    ServiceReader(std::string path);
    
    /**
     * @param tps OUT Vector of all TaskProcessors
     * @return Number of TaskProcessors
     */
    size_t getTaskProcessors(TaskProcessorV_t * tps);
};


#endif /* defined(__Prog3__ServiceReader__) */
