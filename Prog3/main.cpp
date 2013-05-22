//
//  main.cpp
//  Prog3
//
//  Created by Knuth Lohse on 21.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include <iostream>
#include "ConfigurationReader.h"
#include "ServiceReader.h"
#include "TaskProcessor.h"


int main(int argc, const char * argv[])
{
    ServiceReader r("/Users/JoH/etc/System.ini");
    r.readFile();
    TaskProcessorV_t tps;
    size_t numberOfTaskProcs=r.getTaskProcessors(&tps);
    for (int i=0; i<numberOfTaskProcs; i++) {
        std::cout << "TP: " << tps[i].getName() << std::endl;
    }
    return 0;
}

