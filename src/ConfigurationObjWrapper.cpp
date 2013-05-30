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

std::string ConfigurationObjWrapper::getValue(const std::string key) {
    stringV_t * values;
    size_t size=this->getValues(key, &values);
    if (size<1 || values==NULL) {
        std::cout  << key <<" of " << this->getName() << " not defined" << std::endl;
        exit(0);
    }
    if (size>1) {
        std::cout  << key <<" of " << this->getName() << " seems to have more than one Value" << std::endl;
        exit(0);
    }
    return (*values)[0];
}

int ConfigurationObjWrapper::getIntValue(const std::string key) {
    stringV_t * values;
    size_t size=this->getValues(key, &values);
    if (size<1 || values==NULL) {
        std::cout  << key <<" of " << this->getName() << " not defined" << std::endl;
        exit(0);
    }
    if (size>1) {
        std::cout  << key <<" of " << this->getName() << " seems to have more than one Value" << std::endl;
        exit(0);
    }
    std::stringstream sstr(*(std::string*)values);
    int ret;
    sstr >> ret;
    return ret;
}