//
//  TaskDescriptionReader.h
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__TaskDescriptionReader__
#define __Prog3__TaskDescriptionReader__

#include <iostream>
#include <fstream>
#include "ConfigurationReader.h"
#include "Task.h"

class TaskDescriptionReader : public ConfigurationReader {
    
public:
    /**
     * @param path Path and filename of the configurationFile to read
     */
    TaskDescriptionReader(std::string path);
    
    /**
     * @param tps OUT Vector of all TaskProcessors
     * @return Number of TaskProcessors
     */
    size_t getTasks(TaskV_t * tds);
    
protected:
};
#endif /* defined(__Prog3__TaskDescriptionReader__) */
