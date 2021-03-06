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
    
    ~ServiceReader();
    
    ServiceReader(ServiceReader &toCopy);
    
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
    
    /**
     *
     *
     */
    std::ofstream * getLogStream();
    
private:
    /**
     * Will check the read Object creation and exit on failure
     * returns 1 if everythings allright
     */
    int validateProcessors();
    
    /**
     * Will check the read Object creation and write errors to the logStream
     * Will delete invalid Tasks
     */
    int validateTasks(std::ofstream *logStream);
    
    TaskDescriptionReader * tdReader; ///Reader of the TaskDescription identified in the Service.ini
    TaskProcessorV_t * taskProcessors; ///Task Processors as declared by the Service.ini
    TaskV_t * tasks; ///Tasks as declared by the TaskDescriptionFile read by tdReader
    std::ofstream * logStream; //Filestream for loging;
};


#endif /* defined(__Prog3__ServiceReader__) */
