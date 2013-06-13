//
//  ConfigurationObj.cpp
//  Prog3
//
//  Created by Knuth Lohse on 21.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ConfigurationObj.h"

ConfigurationObj::ConfigurationObj(std::string name) {
    this->name=name;
    this->parent=NULL;
}

ConfigurationObj::ConfigurationObj(std::string name, ConfigurationObj *parent) {
    this->name=name;
    this->parent=parent;
    for(attrMap_t::iterator it = parent->confAttributes.begin(); it != parent->confAttributes.end(); ++it) {
        ConfigurationAttribute *c=new ConfigurationAttribute(*(it->second));
        this->confAttributes.insert(std::make_pair(it->first, c));
    }
}

ConfigurationObj::~ConfigurationObj() {
    //
    // Bad things might happen when this was not a leaf
    //
    // Delete all Attributes in map
    for(attrMap_t::iterator it = this->confAttributes.begin(); it != this->confAttributes.end(); ++it) {
        delete it->second;
    }
    // Don't delete the parent
}

ConfigurationObj::ConfigurationObj(ConfigurationObj &copy) {
    std::cout << "Don't try to copy a ConfigurationObj - Bad thins will happen on deconstruction";
    exit (10);
}

ConfigurationAttribute* ConfigurationObj::getConfigurationAttribute(std::string key) {
    attrMap_t::iterator pos = this->confAttributes.find(key);
    if (pos== this->confAttributes.end()) return NULL;
    return pos->second;
}

void ConfigurationObj::addAttribute(std::string attrName) {
    ConfigurationAttribute *c=new ConfigurationAttribute(attrName);

    attrMap_t::iterator pos = this->confAttributes.find(attrName);
    if (pos != this->confAttributes.end()) {
        //if Attribute is allready set, delete the old one
        ConfigurationAttribute *value = pos->second;
        delete value;
        this->confAttributes.erase(attrName);
    }    
    this->confAttributes.insert(std::make_pair(attrName, c));
}

void ConfigurationObj::addAttributeValue(std::string attrName, std::string newValue){
    ConfigurationAttribute *cAttr=this->getConfigurationAttribute(attrName);
    if (cAttr==NULL) {
        std::cout << "Trying to add an Value to a non existing Attribute. Attribute: " << attrName << std::endl;
        exit(2);
    }
    cAttr->addValue(newValue);
}

std::string ConfigurationObj::getRootName() {
    if (this->parent==NULL) return this->name;
    return this->parent->getRootName();
}

std::string ConfigurationObj::getName() {
    return this->name;
}
/**
 * 
 *
 * @param key Name of Attribute to get the Values of
 * @param values OUT pointer to the array of values - NULL pointer if the attribute is not set
 * @return Number of Values returned 
 *
 */
size_t ConfigurationObj::getValues(const std::string key, stringV_t ** values) {
    attrMap_t::iterator pos = this->confAttributes.find(key);
    if (pos == this->confAttributes.end()) {
        *values=NULL;
        return 0;
    }
    *values=pos->second->getValues();
    return pos->second->getNumberOfValues();
}
