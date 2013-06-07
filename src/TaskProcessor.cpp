//
//  TaskProcessor.cpp
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskProcessor.h"
#include "ServiceRequest.h"
#include "Task.h"
#include "TaskStartException.h"

static stringV_t assignedServiceProcessors=stringV_t();

TaskProcessor::TaskProcessor(ConfigurationObj *conf, ConfigurationReader *reader): ConfigurationObjWrapper(conf) {
    if (conf->getRootName()!="TaskProcessor") {
        std::cout << conf->getName() << " seems not to be a TaskProcessor" << std::endl;
        exit(0);
    }
    
    //create ServiceProvider
    stringV_t * values;
    size_t size=this->getValues("ServiceProcessors", &values);
    if (values==NULL &&size>0) {
        std::cout << "Invalid Value of ServiceProcessors in TaksProcessor " << this->getName() << std::endl;
        exit(3);
    }
    for (int i=0; i<size; i++) {
        ConfigurationObj *spCObj=reader->getConfigurationObj((*values)[i]);
        if (spCObj==NULL) { 
            std::cout << this->getName() << " requires Service Processor " << (*values)[i] << " which seems not to be defined" << std::endl;
            exit(3);
        }
        for (int j=0; j<assignedServiceProcessors.size(); j++) {
            if ((*values)[i]==(assignedServiceProcessors)[j]) {
                std::cout << "Service Processor " << (*values)[i] << " used at least twice" << std::endl;
                exit(3);                
            }
        }
        assignedServiceProcessors.push_back((*values)[i]);
        (this->serviceProcessors).push_back(ServiceProcessor(spCObj));
    }
    this->startedTasks=TaskV_t();
    this->serviceProcIDMap=ServiceProcessorIDM_t();
}


int TaskProcessor::supports(std::string type, bool ignoreIdleStatus) {
    int ret=0;
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        if (this->serviceProcessors[i].getType()==type) {
            if (ignoreIdleStatus) {
                ret++;
            }
            else {
                if (!this->serviceProcessors[i].isWorking()) {
                    ret++;
                }
            }
        }
    }
    return ret;
}

bool TaskProcessor::canHandleProcs(neededProcsM_t procsToTest) {
    for (int i=0; i<this->startedTasks.size(); i++) {
        if (this->startedTasks[i]->isDone()) {
            this->startedTasks.erase(this->startedTasks.begin()+i);
            i--;
        }
    }
    for(neededProcsM_t::iterator pos = procsToTest.begin(); pos != procsToTest.end(); ++pos) {
        if (pos->second > this->supports(pos->first)) {
            return false;
        }
    }
    return true;
}

bool TaskProcessor::canHandleProcsIgnoringIdleStatus(neededProcsM_t procsToTest) {
    for(neededProcsM_t::iterator pos = procsToTest.begin(); pos != procsToTest.end(); ++pos) {
        if (pos->second > this->supports(pos->first, 1)) {
            return false;
        }
    }
    return true;
}

int TaskProcessor::getQueueSize() {
    return this->getIntValue("Queue.Size");
}

int TaskProcessor::validate() {
    int ret=1;
    if (!this->isIntValue("Queue.Size")) {
        std::cout << "Queue.Size of " << this->getName() << " is not set correct" << std::endl;
        exit(10);
    }
    this->getQueueSize(); //will exit if Queuesize is not a number
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        if (this->serviceProcessors[i].validate()<=0) ret=-1;
    }
    return ret;
}

int TaskProcessor::registerServiceProceossors(stringV_t * serviceProcessorTypes, int startID) {
    for (int i=0; i<this->serviceProcessors.size(); i++) {
        serviceProcessorTypes->push_back((this->serviceProcessors)[i].getType());
        (this->serviceProcessors)[i].setID(startID);
        startID++;
    }
    return startID;
}

size_t TaskProcessor::startTask(procsToStartV_t ** procsToStart, int nextJobID, Task * task) {
    neededProcsM_t nProcs=task->getNeededProcessors();
    if (!this->canHandleProcsIgnoringIdleStatus(nProcs)) throw TaskStartException(-1);
    if (this->startedTasks.size()>=this->getQueueSize()) throw TaskStartException(0);
    if (task->isDone()) throw TaskStartException(1);
    nProcs.clear();
    nProcs=task->getNeededProcessors(0);
    if (!this->canHandleProcs(nProcs)) throw TaskStartException(0);
    this->startedTasks.push_back(task);
    sRequestV_t * requests;
    requests=task->getRequests(0);
    for (int i=0; i<requests->size(); i++) { //No Counter - will count when serviceRequest is assigned
        bool assigned=false;
        for (int j=0; j<this->serviceProcessors.size() && !assigned; j++) {
            if ((*requests)[i]->getServiceProcessorType()==this->serviceProcessors[j].getType() && !this->serviceProcessors[j].isWorking()) {
                assigned=true;
                this->serviceProcessors[j].registerRequest((*requests)[i]);
                int index=this->serviceProcessors[j].getID();
                procParamP_t param=std::make_pair(nextJobID, (*requests)[i]->getDuration());
                (*procsToStart)->push_back(std::make_pair(index, param));
                this->serviceProcIDMap.insert(std::make_pair(nextJobID, &this->serviceProcessors[j]));
                nextJobID++;
            }
        }
    }
    return requests->size();
}

size_t TaskProcessor::resumeTask(procsToStartV_t ** procsToStart, int nextJobID) {
    if (this->startedTasks.size()==0) throw TaskStartException(2);
    for (int tNum=0; tNum<this->startedTasks.size(); tNum++) {
        try {
            if (this->startedTasks[tNum]->isDone()) {
                this->startedTasks.erase(startedTasks.begin()+tNum);
                tNum--;
                throw TaskStartException(1);
            }
            if (this->startedTasks[tNum]->stepInProgress()) throw TaskStartException(3);
            neededProcsM_t nProcs=this->startedTasks[tNum]->getNeededProcessorsForNextStep();
            if (this->startedTasks[tNum]->getName()=="T025") {
                ; //breakpoint
            }
            if (!this->canHandleProcs(nProcs)) throw TaskStartException(0);
            sRequestV_t * requests;
            requests=this->startedTasks[tNum]->getNextStep();
            for (int i=0; i<requests->size(); i++) {
                bool assigned=false;
                for (int j=0; j<this->serviceProcessors.size() && !assigned; j++) {
                    if ((*requests)[i]->getServiceProcessorType()==this->serviceProcessors[j].getType() && !this->serviceProcessors[j].isWorking()) {
                        assigned=true;
                        this->serviceProcessors[j].registerRequest((*requests)[i]);
                        int index=this->serviceProcessors[j].getID();
                        procParamP_t param=std::make_pair(nextJobID, (*requests)[i]->getDuration());
                        (*procsToStart)->push_back(std::make_pair(index, param));
                        this->serviceProcIDMap.insert(std::make_pair(nextJobID, &this->serviceProcessors[j]));
                        nextJobID++;
                    }
                }
            }
            //test may be removed in final version
            for (int i=0; i<requests->size(); i++) {
                if (!(*requests)[i]->isStarted()) {
                    std::cout << "something went wrong in TaskProcessor::resumeTask" <<std::endl;
                    exit(10);
                }
            }
            return requests->size();
        }
        catch (TaskStartException &e) {
            //ignore error
        }
    }
    throw TaskStartException(0);
}

int TaskProcessor::jobFinished(int jobID) {
    ServiceProcessorIDM_t::iterator pos = this->serviceProcIDMap.find(jobID);
    if (pos==this->serviceProcIDMap.end()) {
        std::cout << "This Shouldn't happen - Didn't find jobID in TaskProcessor::JobFinished" << std::endl;
        exit(10);
    }
    ServiceProcessor * serviceProcessor = pos->second;
    serviceProcessor->jobFinished();
    this->serviceProcIDMap.erase(pos);
    return 1;
}

std::string TaskProcessor::getRunningSRequestString(int jobID) {
    ServiceProcessorIDM_t::iterator pos = this->serviceProcIDMap.find(jobID);
    if (pos==this->serviceProcIDMap.end()) {
        std::cout << "This Shouldn't happen - Didn't find jobID in TaskProcessor::getRunningSRequestString" << std::endl;
        exit(10);
    }
    ServiceProcessor * serviceProcessor = pos->second;
    return  this->getName() + ":" + serviceProcessor->getRunningSRequestString();
}

JobsToKillV_t TaskProcessor::unexpectedTermination(int jobID) {
    JobsToKillV_t ret=JobsToKillV_t();
    ServiceProcessorIDM_t::iterator tpos = this->serviceProcIDMap.find(jobID);
    if (tpos==this->serviceProcIDMap.end()) {
        std::cout << "This Shouldn't happen - Didn't find jobID in TaskProcessor::getRunningJobIDs" << std::endl;
        exit(10);
    }
    Task * task=tpos->second->getRunningTask();
    task->abort();
    this->jobFinished(jobID);
    if (task==NULL) {
        std::cout << "Didn't find Task of Running Job in TaskProcessor::getRunningJobIDs" << std::endl;
        exit(10);
    }
    for (ServiceProcessorIDM_t::iterator pos = serviceProcIDMap.begin(); pos != serviceProcIDMap.end(); ++pos) {
        if (pos->second->getRunningTask()!=NULL) {
            if (pos->second->getRunningTask()==task) ret.push_back(pos->first);
        }
        else {
            std::cout << "this shouldn't happen - didn't found Task to process in serviceProcIDMap in TaskProcessor::getRunningJobIDs";
            exit(20);
        }
    }
    return ret;
}

