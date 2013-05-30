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
#include "ServiceReader.h"
#include "ConfigurationObjWrapper.h"
#include "ConfigurationReader.h"
#include "ServiceRequest.h"


typedef std::vector<std::string> stringV_t;
typedef std::vector<ServiceRequest> sRequestV_t;
typedef std::vector<sRequestV_t> sRequestsV_t;

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
    
private:
    
    int position;
    sRequestsV_t requests;
    
};

typedef std::vector<Task> TaskV_t;


#endif /* defined(__Prog3__Task__) */
