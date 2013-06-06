//
//  TaskDescriptionReader.cpp
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskDescriptionReader.h"
#include "Prog3Settings.h"
#include "SemanticParseException.h"

TaskDescriptionReader::TaskDescriptionReader(std::string path):ConfigurationReader(path) {
    
}


size_t TaskDescriptionReader::getTasks(TaskV_t * tasks) {
    ConfigurationObj* root=this->getConfigurationObj("Tasks");
    if (root==NULL) {
        throw SemanticParseException("Rootconfiguration of Task ini not found");
    }
    stringV_t * values;
    size_t size=root->getValues("Tasks", &values);
    
    if (size>0 && values==NULL) {
        throw SemanticParseException("Invalid definiton of TaskProcessors");
    }
    for (int i=0; i<size; i++) {
        ConfigurationObj* tpCObj=this->getConfigurationObj((*values)[i]);
        if (tpCObj==NULL) {
            this->errorString << "Definition of TaskProcessors " << (*values)[i] << " not found" << std::endl;
            this->errors++;
        }
        else {
            try {
                tasks->push_back(new Task(tpCObj, this));
            }
            catch(SemanticParseException &e) {
                this->errorString << e.getError() << std::endl;
                this->errors++;
            }
        }
    }
    return tasks->size();
}
