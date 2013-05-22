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
    ConfigurationObjWrapper(ConfigurationObj * conf);
    size_t getValues(const std::string key, stringV_t ** values);
    std::string getRootName();
    std::string getName();
    
private:
    ConfigurationObj *conf;
};

#endif /* defined(__Prog3__ConfigurationObjWrapper__) */
