//
//  TaskDesription.cpp
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskDescription2.h"
#include <regex>


TaskDescription::TaskDescription(ConfigurationObj *conf, TaskDescriptionReader * tReader): ConfigurationObjWrapper(conf) {
    stringV_t * stepstrings;
    this->getValues("Steps", &stepstrings);
    std::regex rxAndSep("(.+) & (.+)");
    std::cmatch rxSearchResults;
    for (int i=1; i<(stepstrings->size()-1); i++) { // step over O and X
        requests[i-1]=sRequestV_t();
        while (std::regex_match((*stepstrings)[i].c_str(), rxSearchResults, rxAndSep)) {
                
            std::string name=rxSearchResults[2];
            (*stepstrings)[i]=rxSearchResults[1];
            ConfigurationObj * srConf=tReader->getConfigurationObj(name);
            if (srConf==NULL) {
                std::cout << "Could not find Service Request " << name << std::endl;
                exit(9);
            }
            requests[i-1].push_back(ServiceRequest(srConf));
        }
        if ((*stepstrings)[i].length()) {
            ConfigurationObj * srConf=tReader->getConfigurationObj((*stepstrings)[i]);
            if (srConf==NULL) {
                std::cout << "Could not find Service Request " << (*stepstrings)[i] << std::endl;
                exit(9);
            }
            requests[i-1].push_back(ServiceRequest(srConf));
        }
    }
}