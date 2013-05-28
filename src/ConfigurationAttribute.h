//
//  ConfigurationAttribute.h
//  Prog3
//
//  Created by Knuth Lohse on 21.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__ConfigurationAttribute__
#define __Prog3__ConfigurationAttribute__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>

typedef std::vector<std::string> stringV_t;


class ConfigurationAttribute
{
public:
    /**
     * @param name: Name of the Attribute
     *
     * initialises an Attribute without any values
     */
    ConfigurationAttribute(std::string name);
    
    /**
     * @param value: value to add to the attribute
     * 
     * values added don't overwrite existing ones
     */
    void addValue(std::string value);
    
    ConfigurationAttribute(ConfigurationAttribute& toCopy);
    
    /**
     * @return Pointer to the vector of this attribute
     */
    stringV_t * getValues();
    
    /**
     * @return Number of Values stored in this attribute
     */
    size_t getNumberOfValues();
    
private:
    std::string name; /// Name of the Attribute
    stringV_t values; /// Set values of the Attribute
};

#endif /* defined(__Prog3__ConfigurationAttribute__) */
