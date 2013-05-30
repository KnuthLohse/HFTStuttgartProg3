//
//  ServiceReader.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ServiceReader.h"
#include "Prog3Settings.h"

ServiceReader::ServiceReader(std::string path):ConfigurationReader(path) {
    //this->readFile();
    ConfigurationObj* root=this->getConfigurationObj("System");
    if (root==NULL) {
        std::cout << "Rootconfiguration of System.ini not found" << std::endl;
        exit(0);
    }
    stringV_t * values;
    size_t size=root->getValues("TaskProcessors", &values);
    if (size>0 && values==NULL) {
        std::cout  << "Invalid definiton of TaskProcessors" << std::endl;
        exit(0);
    }
    this->taskProcessors=new TaskProcessorV_t();
    for (int i=0; i<size; i++) {
        ConfigurationObj* tpCObj=this->getConfigurationObj((*values)[i]);
        if (tpCObj==NULL) {
            std::cout  << "Definition of TaskProcessors " << (*values)[i] << " not found" << std::endl;
            exit(0);
        }
        this->taskProcessors->push_back(TaskProcessor(tpCObj, this));
    }
    
    size=root->getValues("TaskDescription.File.1", &values);
    std::string taskfile=(*values)[0];
#ifdef _USE_HARDCODED_TASKCONFIG_
    taskfile="/Users/JoH/etc/TaskDescriptions.0001.ini";
#ifdef _DEBUG_
    std::cout << "TaskIni " << (*values)[0] << " replaced with hardcoded file " << taskfile << std::endl;
#endif /*_DEBUG_*/
#endif /*_USE_HARDCODED_TASKCONFIG_*/
    
    this->tdReader=new TaskDescriptionReader(taskfile);
    this->tasks=new TaskV_t();
    this->tdReader->getTasks(this->tasks);
}

size_t ServiceReader::getTasks(TaskV_t ** tasks) {
    (*tasks)=this->tasks;
    return this->tasks->size();
}

size_t ServiceReader::getTaskProcessors(TaskProcessorV_t ** taskProcessors) {
    (*taskProcessors)=this->taskProcessors;
    return this->taskProcessors->size();
}