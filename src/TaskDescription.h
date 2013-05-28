//
//  TaskDesription.h
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__TaskDesription__
#define __Prog3__TaskDesription__

#include <iostream>
#include "ConfigurationObjWrapper.h"
#include "ConfigurationReader.h"
#include "ServiceRequest.h"

typedef std::vector<std::string> stringV_t;
typedef std::vector<ServiceRequest> sRequestV_t;
typedef std::vector<sRequestV_t> sRequestsV_t;

class TaskDescription : public ConfigurationObjWrapper {
    
public:
    /**
     * 
     */
	TaskDescription(ConfigurationObj *conf, ConfigurationReader * tReader);
    
    stringV_t getNextTasks();
    bool isDone();
    
private:
    
    int position;
    sRequestsV_t requests;
    
};

typedef std::vector<TaskDescription> TaskDescriptionV_t;


#endif /* defined(__Prog3__TaskDesription__) */
