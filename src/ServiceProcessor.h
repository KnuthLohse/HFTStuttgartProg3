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
     * returns the Type of the Serviceprocessor
     */
    std::string getType();
    
    int validate();
    
private:
};

#endif /* defined(__Prog3__ServiceProcessor__) */
