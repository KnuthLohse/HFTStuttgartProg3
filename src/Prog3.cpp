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
    procsToStartV_t * procsToStart=NULL;
    procsToStartV_t * procsToStop=new procsToStartV_t();
    bool add=true;
    while (add) {
        int test=c.getNextJobs(&procsToStart);
        if (test>0) {
            for (int i=0; i<procsToStart->size(); i++) {
                c.jobStarted((*procsToStart)[i].second.first);
                std::cout << "started proc with jobID " << (*procsToStart)[i].second.first << " on ServiceProcessor " << (*procsToStart)[i].first << std::endl;
            }
            procsToStop->insert(procsToStop->end(), procsToStart->begin(), procsToStart->end());
        }
        else if (test==-1) {
            add=false;
        }
        else {
            if (procsToStop->size()>0) {
                if (((*procsToStop)[procsToStop->size()-1].second.first)==100) {
                    JobsToKillV_t toKill=c.jobUnexpectedTerminated((*procsToStop)[procsToStop->size()-1].second.first);
                    std::cout << "---- KILLING proc with jobID ---- " << (*procsToStop)[procsToStop->size()-1].second.first << " on ServiceProcessor " << (*procsToStop)[procsToStop->size()-1].first << std::endl;
                    for (int i=0; i<toKill.size(); i++) {
                        for(int j=0; j<procsToStop->size(); j++) {
                            if ((*procsToStop)[j].second.first==toKill[i]) {
                                c.jobAbortConfirmation(toKill[i]);
                                procsToStop->erase(procsToStop->begin()+j);
                            }
                        }
                    }
                    procsToStop->pop_back();
                }
                else {
                    c.jobFinished((*procsToStop)[procsToStop->size()-1].second.first);
                    std::cout << "---- Stoped proc with jobID ---- " << (*procsToStop)[procsToStop->size()-1].second.first << " on ServiceProcessor " << (*procsToStop)[procsToStop->size()-1].first << std::endl;
                    procsToStop->pop_back();
                }
            }
        }
    }
}

