//
//  ServiceReader.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "ServiceReader.h"
#include "Prog3Settings.h"
#include "SemanticParseException.h"

ServiceReader::ServiceReader(std::string path):ConfigurationReader(path) {
    //this->readFile();
    if (this->errors) {
        //Fehler beim einlesen der System.ini
		std::cout << "Error reading System.ini:" << std::endl;
        std::cout << (*(this->errorString)).str();
        exit(-1);
    }
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
#ifdef _DEBUG_
			std::cout << "test: \"" << (*values)[i]; 
			std::cout << "\"" << std::endl;
#endif
            std::cout << "Definition of TaskProcessor: " << (*values)[i] << " not found" << std::endl;
            exit(0);
        }
        this->taskProcessors->push_back(TaskProcessor(tpCObj, this));
    }
    this->validateProcessors();
    std::cout << "---------- Service file initialized ----------" << std::endl;
    //Preparing parsing the Task ini
    size=root->getValues("TaskDescription.File.1", &values);
    std::string taskfile=(*values)[0];
#ifdef _USE_HARDCODED_TASKCONFIG_
    taskfile="/Users/JoH/etc/TaskDescriptions.0001.ini";
    std::cout << "TaskIni " << (*values)[0] << " replaced with hardcoded file " << taskfile << std::endl;
#endif /*_USE_HARDCODED_TASKCONFIG_*/
    //Open logfile
    this->logStream=(std::ofstream *) new std::ofstream();
    //(*(this->logStream)).open(path+".log", std::ios::out);
    std::string temp=path+".log";
    (*(this->logStream)).open(temp.c_str());

    if (!(*(this->logStream)).is_open()) {
        std::cout << "Could not open logfile: " << path+".log" << std::endl;
        exit(-1);
    }
    //Read taskfile
    this->tdReader=new TaskDescriptionReader(taskfile);
    std::string tdErrorString=std::string();
    this->tasks=new TaskV_t();
    try {
        this->tdReader->getTasks(this->tasks);
    } catch (SemanticParseException &e) {
        //if we get an error creating the tasks print it to the log file
        (*(this->logStream)) << e.getError() << std::endl;
    }
    //write taskfileparsingerrors to logfile
    if (this->tdReader->getErrorString(&tdErrorString)) {
        (*(this->logStream)) << tdErrorString;
    }
    this->validateTasks(this->logStream);
}

std::ofstream * ServiceReader::getLogStream() {
    return this->logStream;
}

ServiceReader::~ServiceReader() {
    delete this->tdReader;
    for (int i=0; i<this->tasks->size(); i++) {
        delete (*(this->tasks))[i];
    }
    delete this->tasks;
    delete this->taskProcessors;
    (*(this->logStream)).close();
    delete this->logStream;
    //tasks and taskprocessors should be
}

ServiceReader::ServiceReader(ServiceReader &toCopy):ConfigurationReader("No Copy Of ServiceReader Allowed") {
    std::cout << "Don't copy a Service Reader - Bad things might happen on deconstruction" << std::endl;
    exit(10);
}

size_t ServiceReader::getTasks(TaskV_t ** tasks) {
    (*tasks)=this->tasks;
    return this->tasks->size();
}

size_t ServiceReader::getTaskProcessors(TaskProcessorV_t ** taskProcessors) {
    (*taskProcessors)=this->taskProcessors;
    return this->taskProcessors->size();
}

int ServiceReader::validateProcessors() {
    for (int i=0; i<this->taskProcessors->size(); i++) {
        if (!(*this->taskProcessors)[i].validate()) {
            std::cout << "Validation of TaskProcessors failed" << std::endl;
            exit(10);
        }
    }
    return 1;
}

int ServiceReader::validateTasks(std::ofstream *logStream) {
    for (int i=0; i<this->tasks->size(); i++) {
        bool del=false;
        try {
            if (!(*this->tasks)[i]->validate(this)) {
                //Ignoring task because of failure of validation
                (*logStream) << "Unknown Error validating a Task; ignoring it" << std::endl;
                del=true;
            }
        }
        catch (SemanticParseException &e) {
            (*logStream) << e.getError() << " - Ignoring Task" << std::endl;
            del=true;
        }
        if (del) {
            this->tasks->erase(this->tasks->begin()+i);
            i--;
        }
    }
    if (this->tasks->size()==0) {
        (*logStream) << "No Tasks found" << std::endl;
    }
    return 1;
}
