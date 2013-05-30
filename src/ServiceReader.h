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
#include "Task.h"
#include "TaskDescriptionReader.h"

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
    size_t getTaskProcessors(TaskProcessorV_t ** tps);
    
    /**
     * @param tasks OUT list of all Tasks
     * @return Number of Tasks
     */
    size_t getTasks(TaskV_t ** tasks);
    
private:
    /**
     * Will check the read Object creation and exit on failure
     */
    int validate();
    
    TaskDescriptionReader * tdReader;
    TaskProcessorV_t * taskProcessors;
    TaskV_t * tasks;
};


#endif /* defined(__Prog3__ServiceReader__) */
