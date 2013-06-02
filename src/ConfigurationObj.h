//
//  ConfigurationObj.h
//  Prog3
//
//  Created by Knuth Lohse on 21.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__ConfigurationObj__
#define __Prog3__ConfigurationObj__

#include <iostream>
#include <map>
#include "ConfigurationAttribute.h"

typedef std::map<std::string, ConfigurationAttribute*> attrMap_t;

class ConfigurationObj
{
public:
    
    /**
     * @param name Name of the Configuration Object
     * for ConfigurationObjects that don't have parents
     */
    ConfigurationObj(std::string name);
    
    ~ConfigurationObj();
    
    ConfigurationObj(ConfigurationObj &copy);
    
    /**
     * @param name Name of the Configuration Object
     * @param parent Pointer to the parent CounterObject to get the Attributes from
     */
    ConfigurationObj(std::string name, ConfigurationObj *parent);
    
    /**
     * @param attrName Name of the Attribute to add to the ConfigurationObject
     */
    void addAttribute(std::string attrName);
    
    /**
     * @param attrName Name of the attribute to add a value to
     * @param newValue Value to add to the attribute of this ConfigurationObject
     *
     * An attribute with name attrName must allready exist
     */
    void addAttributeValue(std::string attrName, std::string newValue);
    
    /**
     * @param key Name of the attribute to get the values from
     * @param values OUT Will point to the vector with the values of the attribute - Null if attribute is not set
     * @return Number of values of the attribute
     */
    size_t getValues(const std::string key, stringV_t ** values);
    
    /**
     * @return Name of the ConfigurationObject that is the root of the parent tree 
     */
    std::string getRootName();
    
    /**
     * @return Name of the ConfigurationObject
     */
    std::string getName();
    
private:
    /**
     * @param key Name of the attribute of interest
     * @return Pointer to the AttributeObject with the given name
     */
    ConfigurationAttribute* getConfigurationAttribute(std::string key);
    
    std::string name; ///Name of the ConfigurationObject
    ConfigurationObj *parent; ///Pointer to the parent ConfigurationObject to inherit type and attributes
    attrMap_t confAttributes; ///Map that contains the attributes. Key is the name of the Attribute
};
#endif /* defined(__Prog3__ConfigurationObj__) */
