//
//  TaskDesription.h
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__Task__
#define __Prog3__Task__

#include <iostream>
#include "ConfigurationObjWrapper.h"
#include "ConfigurationReader.h"
#include "ServiceRequest.h"
#include "TaskProcessor.h"

class ServiceReader;
typedef std::vector<TaskProcessor> TaskProcessorV_t;

typedef std::vector<std::string> stringV_t;
typedef std::vector<ServiceRequest> sRequestV_t;
typedef std::vector<sRequestV_t> sRequestsV_t;
typedef std::map<std::string, int> neededProcsM_t;

class Task : public ConfigurationObjWrapper {
    
public:
    /**
     * 
     */
	Task(ConfigurationObj *conf, ConfigurationReader * tReader);
    
    std::string getName();
    
    sRequestV_t * getNextStep();
    bool isDone();
    int validate(ServiceReader * sReader);
    
    int findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors);
    
    neededProcsM_t getNeededProcessors();
    
    neededProcsM_t getNeededProcessors(int step);
    
    stringV_t getNeededProcessorTypes();
    
    stringV_t getNeededProcessorTypes(int step);
    
private:
    
    int position;
    sRequestsV_t requests;
    
};

typedef std::vector<Task> TaskV_t;


#endif /* defined(__Prog3__Task__) */
