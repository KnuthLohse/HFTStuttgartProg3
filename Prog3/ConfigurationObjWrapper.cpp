//
//  ConfigurationObjWrapper.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ConfigurationObjWrapper.h"

ConfigurationObjWrapper::ConfigurationObjWrapper(ConfigurationObj * conf) {
    this->conf=conf;
}

size_t ConfigurationObjWrapper::getValues(const std::string key, stringV_t ** values) {
    return this->conf->getValues(key, values);
}

std::string ConfigurationObjWrapper::getRootName() {
    return this->conf->getRootName();
}

std::string ConfigurationObjWrapper::getName() {
    return this->conf->getName();
}
    