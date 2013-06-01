//
//  ServiceProcessor.h
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__ServiceProcessor__
#define __Prog3__ServiceProcessor__

#include <iostream>
#include "ConfigurationObj.h"
#include "ConfigurationObjWrapper.h"

class ServiceProcessor :public ConfigurationObjWrapper
{
public:
    /**
     * @param conf ConfigurationObject to get the Attributes from
     */
    ServiceProcessor(ConfigurationObj *conf);
    
    /**
     * @return Type of the Serviceprocessor
     */
    std::string getType();
    
    /**
     * Will check if all necessary Attributes are set to valid values and exit or return an value smaller 1 if not;
     */
    int validate();
    
    /**
     * @return Index of the ServiceProcessor given to roseRT
     */
    int getID();
    
    /**
     * @param iD Index of the ServiceProcessor given to roseRT
     */
    int setID(int iD);
private:
    
    int iD; /// Index of the ServiceProcessor given to roseRT; -1 if not set yet
};

#endif /* defined(__Prog3__ServiceProcessor__) */
