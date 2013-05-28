//
//  TaskDescriptionReader.cpp
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskDescriptionReader.h"

TaskDescriptionReader::TaskDescriptionReader(std::string path):ConfigurationReader(path) {
    
}


size_t TaskDescriptionReader::getTaskDescriptions(TaskDescriptionV_t * taskDescriptions) {
    ConfigurationObj* root=this->getConfigurationObj("Tasks");
    if (root==NULL) {
        std::cout << "Rootconfiguration Object not found" << std::endl;
        exit(0);
    }
    stringV_t * values;
    size_t size=root->getValues("Tasks", &values);
    
    if (size>0 && values==NULL) {
        std::cout  << "Invalid definiton of TaskProcessors" << std::endl;
        exit(0);
    }
    for (int i=0; i<size; i++) {
        ConfigurationObj* tpCObj=this->getConfigurationObj((*values)[i]);
        if (tpCObj==NULL) {
            std::cout  << "Definition of TaskProcessors " << (*values)[i] << " not found" << std::endl;
            exit(0);
        }
        taskDescriptions->push_back(TaskDescription(tpCObj, this));
    }
    return size;
}