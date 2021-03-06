//
//  ConfigurationObjWrapper.h
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__ConfigurationObjWrapper__
#define __Prog3__ConfigurationObjWrapper__

#include <iostream>
#include "ConfigurationObj.h"

class ConfigurationObjWrapper
{
public:
    /**
     * @param conf ConfigurationObject to get the Attributes from. Should not be deleted
     */
    ConfigurationObjWrapper(ConfigurationObj * conf);
    
    ConfigurationObj * getConfRef();
    
    /**
     * @param key Name of the Attribute to get the values from
     * @param values OUT Will point to the vector with the values of the attribute - Null if Attribute is not defined
     * @return Number of values the attribute has set
     */
    size_t getValues(const std::string key, stringV_t ** values);
    
    /**
     * Will exit if Attribute is not defined or has more than one value
     * @param key Name of the attribute of interrest
     * @return Value of the attribute
     */
    std::string getValue(const std::string key);
    
    /**
     * returns the integer value for the attribute identified by key
     * Will exit if an error occours - e.g. more than one value; no Value; no Integer
     *
     * @param key Name of the Attribute
     * @return Value of the Attribute
     */
    int getIntValue(const std::string key);

    /**
     * Will return number of values set for the given attribute
     * if the attribute is defined but not set it will return 0
     * if more than one value is existend only the first is checked if it is not ""
     * @param key Name of the attribute of interrest
     * @return number of values set
     */
    size_t valueIsSet(const std::string key);

    /**
     * returns true if the value for the attribute is a single integer
     *
     * @param key Name of the Attribute
     * @return true if value is an positive integer
     */
    bool isIntValue(const std::string key);
    
    /**
     * @return Name of root ConfigurationObject
     */
    std::string getRootName();
    
    /**
     * @return Name of the Configuration Object
     */
    std::string getName();
    
private:
    ConfigurationObj *conf; //Configuration
};

#endif /* defined(__Prog3__ConfigurationObjWrapper__) */
