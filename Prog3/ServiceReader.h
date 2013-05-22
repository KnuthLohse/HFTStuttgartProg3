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
    ServiceReader(std::string path);
    size_t getTaskProcessors(TaskProcessorV_t *);
};


#endif /* defined(__Prog3__ServiceReader__) */
