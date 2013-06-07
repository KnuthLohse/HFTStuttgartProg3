//
//  ConfigurationObjWrapper.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ConfigurationObjWrapper.h"
#include "Prog3Settings.h"
#include <stdlib.h>

#ifdef _USE_BOOST_REGEX_
#include <boost/regex.hpp>
#endif
#ifdef _USE_STD_REGEX_
#include <regex>
#endif

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

ConfigurationObj * ConfigurationObjWrapper::getConfRef() {
    return this->conf;
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

size_t ConfigurationObjWrapper::valueIsSet(const std::string key) {
    stringV_t * values;
    size_t size=this->getValues(key, &values);
    if (size<1 || values==NULL) {
        return 0;
    }
    if ((*values)[0]=="") return 0;
    return values->size();
}


bool ConfigurationObjWrapper::isIntValue(const std::string key) {
    static _REGEX_PREFIX_::regex rxDigit("\\d*");
    stringV_t * values;
    size_t size=this->getValues(key, &values);
    if (size<1 || values==NULL) {
        return false;
    }
    if (size>1) {
        return false;
    }
    _REGEX_PREFIX_::cmatch rxSearchResults;
    if (_REGEX_PREFIX_::regex_match((*values)[0].c_str(), rxSearchResults, rxDigit)) return true;
    return false;
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
    int ret = atoi((*values)[0].c_str());
    return ret;
}