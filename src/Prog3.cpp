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
    procsToStartV_t * procsToStart;
    procsToStartV_t * procsToStop=new procsToStartV_t();
    bool add=true;
    while (add) {
        if (c.getNextJobs(&procsToStart)>0) {
            for (int i=0; i<procsToStart->size(); i++) {
                c.jobStarted((*procsToStart)[i].second.first);
                std::cout << "started proc with procID " << (*procsToStart)[i].second.first << " on ServiceProcessor " << (*procsToStart)[i].first << std::endl;
            }
            procsToStop->insert(procsToStop->end(), procsToStart->begin(), procsToStart->end());
        }
        else {
            if (procsToStop->size()>0) {
                c.jobFinished((*procsToStop)[procsToStop->size()-1].second.first);
                std::cout << "---- Stoped proc with procID ---- " << (*procsToStop)[procsToStop->size()-1].second.first << " on ServiceProcessor " << (*procsToStop)[procsToStop->size()-1].first << std::endl;
                procsToStop->pop_back();
            }
            else add=false;
        }
    }
}

