//
//  TaskDesription.cpp
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "Task.h"
#include "Prog3Settings.h"


#ifdef _USE_BOOST_REGEX_
    #include <boost/regex.hpp>
#endif
#ifdef _USE_STD_REGEX_
    #include <regex>
#endif


Task::Task(ConfigurationObj *conf, ConfigurationReader * tReader): ConfigurationObjWrapper(conf) {
    stringV_t * stepstrings;
    this->position=0;
    this->getValues("Steps", &stepstrings);
    _REGEX_PREFIX_::regex rxAndSep("(.+) & (.+)");
    _REGEX_PREFIX_::cmatch rxSearchResults;
    //TODO Check for 0 -> and -> X
    for (int i=1; i<(stepstrings->size()-1); i++) { // step over O and X
        this->requests.push_back(sRequestV_t());
        while (_REGEX_PREFIX_::regex_match((*stepstrings)[i].c_str(), rxSearchResults, rxAndSep)) {
                
            std::string name=rxSearchResults[2];
            (*stepstrings)[i]=rxSearchResults[1];
            ConfigurationObj * srConf=tReader->getConfigurationObj(name);
            if (srConf==NULL) {
                std::cout << "Could not find Service Request " << name << std::endl;
                exit(9);
            }
            this->requests[i-1].push_back(ServiceRequest(srConf));
        }
        if ((*stepstrings)[i].length()) {
            ConfigurationObj * srConf=tReader->getConfigurationObj((*stepstrings)[i]);
            if (srConf==NULL) {
                std::cout << "Could not find Service Request " << (*stepstrings)[i] << std::endl;
                exit(9);
            }
            this->requests[i-1].push_back(ServiceRequest(srConf));
        }
    }
}

std::string Task::getName() {
    stringV_t * values;
    size_t size=this->getValues("Name", &values);
    if (size<1 || values==NULL) {
        std::cout  << "Name of task not defined" << std::endl;
        exit(0);
    }
    if (size>1) {
        std::cout  << "Task has more than one name" << std::endl;
        exit(0);
    }
    return (*values)[0];
}

sRequestV_t * Task::getNextStep() {
    //Check if we have done evereything;
    if (this->position>=this->requests.size()) {
        return NULL;
    }
    //Check if this step is allready done
    int done=1;
    for (int i=0; i<this->requests[this->position].size(); i++) {
        if (!this->requests[this->position][i].isDone()) {
            done=0;
        }
    }
    //if this step is done return the next step
    if (done) {
        this->position++;
        return this->getNextStep();
    }
    return &(this->requests[this->position]);
}

bool Task::isDone() {
    if (this->getNextStep()==NULL) return true;
    return false;
}
