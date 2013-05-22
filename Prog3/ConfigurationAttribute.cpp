//
//  ConfigurationAttribute.cpp
//  Prog3
//
//  Created by Knuth Lohse on 21.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ConfigurationAttribute.h"

ConfigurationAttribute::ConfigurationAttribute(std::string name) {
    this->name=name;
    stringV_t v(0);
    this->values=v;
}

ConfigurationAttribute::ConfigurationAttribute(ConfigurationAttribute& toCopy) {
    this->name=toCopy.name;
    stringV_t v(toCopy.values.size());
    this->values=v;
    for (int i=0; i<toCopy.values.size(); i++){
        (this->values)[i]=(toCopy.values)[i];
    }
}

void ConfigurationAttribute::addValue(std::string value) {
    this->values.push_back(value);
}

stringV_t * ConfigurationAttribute::getValues() {
    return &(this->values);
}

size_t ConfigurationAttribute::getNumberOfValues() {
    return this->values.size();
}