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
    
    /**
     * @param key Name of the Attribute to get the values from
     * @param values OUT Will point to the vector with the values of the attribute - Null if Attribute is not defined
     * @return Number of values the attribute has set
     */
    size_t getValues(const std::string key, stringV_t ** values);
    
    /**
     * returns the integer value for the attribute identified by key
     * Will exit if an error occours
     *
     * @param key Name of the Attribute
     * @return Value of the Attribute
     */
    int getIntValue(const std::string key);
    
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
