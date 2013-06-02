//
//  TaskProcessor.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskProcessor.h"
#include "ServiceRequest.h"


TaskProcessor::TaskProcessor(ConfigurationObj *conf, ConfigurationReader *reader): ConfigurationObjWrapper(conf) {
    if (conf->getRootName()!="TaskProcessor") {
        std::cout << conf->getName() << " seems not to be a TaskProcessor" << std::endl;
        exit(0);
    }
    
    //create ServiceProvider
    stringV_t * values;
    size_t size=this->getValues("ServiceProcessors", &values);
    if (values==NULL &&size>0) {
        std::cout << "Invalid Value of ServiceProcessors in TaksProcessor " << this->getName() << std::endl;
        exit(3);
    }
    for (int i=0; i<size; i++) {
        ConfigurationObj *spCObj=reader->getConfigurationObj((*values)[i]);
        (this->serviceProcessors).push_back(ServiceProcessor(spCObj));
    }
}


int TaskProcessor::supports(std::string type) {
    int ret=0;
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        if (this->serviceProcessors[i].getType()==type) {
            if (!this->serviceProcessors[i].isWorking()) {
                ret++;
            }
        }
    }
    return ret;
}

bool TaskProcessor::canHandleProcs(neededProcsM_t procsToTest) {
    for(neededProcsM_t::iterator pos = procsToTest.begin(); pos != procsToTest.end(); ++pos) {
        if (pos->second > this->supports(pos->first)) {
            return false;
        }
    }
    return true;
}

ServiceProcessor * TaskProcessor::registerServiceRequest(ServiceRequest * serviceRequest) {
    for(int i=0; i<this->serviceProcessors.size(); i++) {
        if (serviceProcessors[i].registerRequest(serviceRequest)>0) {
            return &serviceProcessors[i];
        }
    }
    return NULL;
}

int TaskProcessor::getQueueSize() {
    return this->getIntValue("Queue.Size");
}

int TaskProcessor::validate() {
    int ret=1;
    this->getQueueSize(); //will exit if Queuesize is not a number
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        if (this->serviceProcessors[i].validate()<=0) ret=-1;
    }
    return ret;
}

int TaskProcessor::registerServiceProceossors(stringV_t * serviceProcessorTypes, int startID) {
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        serviceProcessorTypes->push_back((this->serviceProcessors)[i].getType());
        (this->serviceProcessors)[i].setID(startID);
        startID++;
    }
    return startID;
}

