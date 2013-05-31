//
//  ServiceProcessor.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ServiceProcessor.h"


ServiceProcessor::ServiceProcessor(ConfigurationObj *conf): ConfigurationObjWrapper(conf) {
    if (conf->getRootName()!="ServiceProcessor") {
        std::cout << conf->getName() << " seems not to be a TaskProcessor" << std::endl;
        exit(0);
    }
}

int ServiceProcessor::validate() {
    if (this->getType().length()<1) {
        std::cout << "Type of ServiceProcessor " << this->getName() << " not set" << std::endl;
        exit(1);
    }
    return 1;
}

std::string ServiceProcessor::getType() {
    stringV_t * values;
    size_t size=this->getValues("Type", &values);
    if (size<1 || values==NULL) {
        std::cout  << "Type of Service Processor " << this->getName() << " not defined" << std::endl;
        exit(0);
    }
    if (size>1) {
        std::cout  << "Type of Service Processor " << this->getName() << " has more than one value" << std::endl;
        exit(0);
    }
    return (*values)[0];
}