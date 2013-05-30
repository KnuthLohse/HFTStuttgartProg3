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
