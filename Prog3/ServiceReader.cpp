//
//  ServiceReader.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ServiceReader.h"

ServiceReader::ServiceReader(std::string path):ConfigurationReader(path) {
    
}


size_t ServiceReader::getTaskProcessors(TaskProcessorV_t * taskProcessors) {
    ConfigurationObj* root=this->getConfigurationObj("root");
    if (root==NULL) {
        std::cout << "Rootconfiguration Object not found" << std::endl;
        exit(0);
    }
    stringV_t * values;
    size_t size=root->getValues("TaskProcessors", &values);
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
        taskProcessors->push_back(TaskProcessor(tpCObj, this));
    }
    return size;
}