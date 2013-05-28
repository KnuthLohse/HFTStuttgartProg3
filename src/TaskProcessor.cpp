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


bool TaskProcessor::supports(std::string type) {
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        if (this->serviceProcessors[i].getType()==type) {
            return true;
        }
    }
    return false;
}

int TaskProcessor::getQueueSize() {
    stringV_t * values;
    size_t size=this->getValues("Queue.Size", &values);
    if (size<1 || values==NULL) {
        std::cout  << "Queue.Size of Task Processor " << this->getName() << " not defined" << std::endl;
        exit(0);
    }
    if (size>1) {
        std::cout  << "Queue.Size of Task Processor " << this->getName() << " has more than one Value" << std::endl;
        exit(0);
    }
    std::stringstream sstr(*(std::string*)values);
    int ret;
    sstr >> ret;
    return ret;
}
