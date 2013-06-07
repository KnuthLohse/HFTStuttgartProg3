//
//  ServiceProcessor.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ServiceProcessor.h"
#include "ServiceRequest.h"


ServiceProcessor::ServiceProcessor(ConfigurationObj *conf): ConfigurationObjWrapper(conf) {
    if (conf->getRootName()!="ServiceProcessor") {
        std::cout << conf->getName() << " seems not to be a TaskProcessor" << std::endl;
        exit(0);
    }
    this->iD=-1;
    this->serviceRequestInProgress=NULL;
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

int ServiceProcessor::registerRequest(ServiceRequest * serviceRequest) {
    if (this->isWorking()) return 0;
    if (this->getType()==serviceRequest->getServiceProcessorType()) {
        this->serviceRequestInProgress=serviceRequest;
        serviceRequest->setStarted(true);
        return 1;
    }
    return -1;
}

void ServiceProcessor::jobFinished() {
    this->serviceRequestInProgress->setDone();
    this->serviceRequestInProgress=NULL;
}

int ServiceProcessor::setID(int iD) {
    this->iD=iD;
    return this->iD;
}

int ServiceProcessor::getID() {
    return this->iD;
}

bool ServiceProcessor::isWorking() {
    if (this->serviceRequestInProgress==NULL) return false;
    return true;
}

std::string ServiceProcessor::getRunningSRequestString() {
    if (this->serviceRequestInProgress==NULL) return this->getName() + " is idle";
    return this->getName() + " ::-:: " + this->serviceRequestInProgress->toString();
}