//
//  TaskProcessor.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskProcessor.h"


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
            ret++;
        }
    }
    return ret;
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

/**
 * Assigns an ID to all its ServiceProcessors, starting by startID
 *
 * @param ServiceProcessorTypes OUT Pointer to a Vector where the types of the ServiceProcessors are added in order of their ID
 * @param startID ID that the first ServiceProcessor of this Task should get
 * @return ID to give to the next ServiceProcessor
 */
int TaskProcessor::registerServiceProceossors(stringV_t * serviceProcessorTypes, int startID) {
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        serviceProcessorTypes->push_back((this->serviceProcessors)[i].getType());
        (this->serviceProcessors)[i].setID(startID);
        startID++;
    }
    return startID;
}

