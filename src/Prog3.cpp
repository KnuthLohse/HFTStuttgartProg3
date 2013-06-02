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
    std::cout << "--- ServiceProcessorList ---" << std::endl;
    
//    typedef std::pair<int,int> procParamP_t; //first: jobID to return when the job is finished; second: Duration in seconds
//    typedef std::pair<int, procParamP_t> procIDParamP_t;
//    typedef std::vector<procIDParamP_t> procsToStartV_t; //first: Index of Processor; Second Params to start the job
//    typedef std::vector<std::string> processorV_t; //List of Processor Type Names to start
//    typedef std::map<int, ServiceProcessor *> ServiceRequestIDM_t; /// first: ID of the job; second: serviceProcessor where the ServiceRequest Runs on
    procsToStartV_t * procsToStart;
    procsToStartV_t * procsToStop=new procsToStartV_t();
    bool add=true;
    while (add) {
        if (c.getNextJobs(&procsToStart)>0) {
            for (int i=0; i<procsToStart->size(); i++) {
                std::cout << "started proc with procID " << (*procsToStart)[i].second.first << " on ServiceProcessor " << (*procsToStart)[i].first << std::endl;
            }
            procsToStop->insert(procsToStop->end(), procsToStart->begin(), procsToStart->end());
        }
        else {
            if (procsToStop->size()>0) {
//                if ((*procsToStop)[procsToStop->size()-1].second.first==9) {
//                    std::cout << "Ab hier läufts schief" << std::endl;
//                }
                c.jobFinished((*procsToStop)[procsToStop->size()-1].second.first);
                std::cout << "---- Stoped proc with procID ---- " << (*procsToStop)[procsToStop->size()-1].second.first << " on ServiceProcessor " << (*procsToStop)[procsToStop->size()-1].first << std::endl;
                procsToStop->pop_back();
            }
            else add=false;
        }
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

