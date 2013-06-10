//
//  Task.cpp
//  Prog3
//
//  Created by Knuth Lohse on 28.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//


#include "ServiceRequest.h"
#include "TaskProcessor.h"
#include "ServiceReader.h"
#include "SemanticParseException.h"
#include "Task.h"
#include <stdio.h>
#include <stdlib.h>
typedef std::vector<TaskProcessor> TaskProcessorV_t;

ServiceRequest::ServiceRequest(ConfigurationObj * conf, Task * t): ConfigurationObjWrapper(conf) {
    this->done=false;
    this->started=false;
    this->task=t;
}

int ServiceRequest::getDuration() {
    return this->getIntValue("Duration.sec");
}

bool ServiceRequest::isDone() {
    return this->done;
}

bool ServiceRequest::setDone() {
    if (this->done) return false;
    this->done=true;
    return true;
}

std::string ServiceRequest::getServiceProcessorType() {
    return this->getValue("ServiceProcessorType");
}

int ServiceRequest::validate(ServiceReader * sReader) {
    if (!this->isIntValue("Duration.sec")) {
        throw SemanticParseException("Duration of " + this->getName() + " not set correctly");
    }
    if (this->valueIsSet("ServiceProcessorType")!=1) {
        throw SemanticParseException("ServiceProcessorType of " + this->getName() + " not set correctly");
    }
    std::string type=this->getServiceProcessorType();
    TaskProcessorV_t * taskProcessors;
    sReader->getTaskProcessors(&taskProcessors);
    int supported=0;
    for (int i=0; i<taskProcessors->size(); i++) {
        if ((*taskProcessors)[i].supports(type)) supported=1;
    }
    if (!supported) {
        throw SemanticParseException("ServiceRequest " + this->getName() + " needs a processor of type " + type + ". Such a processor is not found");
    }
    return 1;
}

bool ServiceRequest::isStarted() {
    return this->started;
}

Task * ServiceRequest::getTask() {
    return this->task;
}

void ServiceRequest::setStarted(bool s) {
    this->started=s;
}

std::string ServiceRequest::toString() {
	char temp[100];
	sprintf(temp, "%d", this->task->position);
    return this->task->getName() + ":" + this->getName() + " probably step " + temp;
}
