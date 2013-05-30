//
//  TaskDescriptionReader.cpp
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskDescriptionReader.h"
#include "Prog3Settings.h"

TaskDescriptionReader::TaskDescriptionReader(std::string path):ConfigurationReader(path) {
    
}


size_t TaskDescriptionReader::getTasks(TaskV_t * tasks) {
    ConfigurationObj* root=this->getConfigurationObj("Tasks");
    if (root==NULL) {
        std::cout << "Rootconfiguration of Task ini not found" << std::endl;
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
        tasks->push_back(Task(tpCObj, this));
    }
    return size;
}
