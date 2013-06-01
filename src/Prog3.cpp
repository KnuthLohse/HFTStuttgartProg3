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
#include "Task.h"
#include "Controller.h"

int main(int argc, const char * argv[])
{
    Controller c=Controller("/Users/JoH/etc/System.ini");
    processorV_t * serviceProcessorTypes=NULL;
    c.getServiceProcessors(&serviceProcessorTypes);
    std::cout << "--- ServiceProcessorList ---" << std::endl;
    for (int i=0; i< serviceProcessorTypes->size(); i++) {
        std::cout << (*serviceProcessorTypes)[i] << std::endl;
    }
    
//    
//    ServiceReader r("/Users/JoH/etc/System.ini");
//    TaskProcessorV_t * tps;
//    size_t numberOfTaskProcs=r.getTaskProcessors(&tps);
//    for (int i=0; i<numberOfTaskProcs; i++) {
//        std::cout << "TP: " << (*tps)[i].getName() << std::endl;
//        bool test=(*tps)[i].supports("eeTypeCServiceType");
//        if (test) std::cout << "Suports Type C" << std::endl;
//        else std::cout << "Doesn't Suports Type C" << std::endl;
//    }
//    TaskV_t * tasks;
//    size_t numberOfTasks=r.getTasks(&tasks);
//    for (int i=0; i<numberOfTasks; i++) {
//        std::cout << (*tasks)[i].getName() << std::endl;
//        
//    }
//    
//    return 0;
}

