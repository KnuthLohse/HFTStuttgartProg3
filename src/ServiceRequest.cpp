//
//  Task.cpp
//  Prog3
//
//  Created by Knuth Lohse on 28.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ServiceRequest.h"

ServiceRequest::ServiceRequest(ConfigurationObj * conf): ConfigurationObjWrapper(conf) {
    this->done=false;
}

int ServiceRequest::getDuration() {
    return this->getIntValue("Duration.sec");
}

bool ServiceRequest::isDone() {
    return this->done;
}

int ServiceRequest::validate(ServiceReader * sReader) {
    this->getDuration(); //will exit if it is not set or it is not a number
    std::string type=this->getValue("ServiceProcessorType");
    TaskProcessorV_t * taskProcessors;
    sReader->getTaskProcessors(&taskProcessors);
    int supported=0;
    for (int i=0; i<taskProcessors->size(); i++) {
        if ((*taskProcessors)[i].supports(type)) supported=1;
    }
    if (!supported) {
        std::cout << "ServiceRequest " << this->getName() << " needs a processor of type " << type <<". Such a processor is not found";
        exit(3);
    }
    return 1;
}