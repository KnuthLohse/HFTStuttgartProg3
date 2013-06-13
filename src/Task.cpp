//
//  TaskDesription.cpp
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "Task.h"
#include "Prog3Settings.h"
#include "TaskProcessor.h"
#include "ServiceReader.h"
#include "TaskDescriptionReader.h"
#include "SemanticParseException.h"


#ifdef _USE_BOOST_REGEX_
    #include <boost/regex.hpp>
#endif
#ifdef _USE_STD_REGEX_
    #include <regex>
#endif

Task::Task(ConfigurationObj *conf, TaskDescriptionReader * tReader): ConfigurationObjWrapper(conf) {
    stringV_t * stepstrings=NULL;
    this->position=0;
    this->getValues("Steps", &stepstrings);
    _REGEX_PREFIX_::regex rxAndSep("\\s*(.+) & (.+)\\s*");
    _REGEX_PREFIX_::cmatch rxSearchResults;
    //TODO Check for 0 -> and -> X
    for (int i=1; i<(stepstrings->size()-1); i++) { // step over O and X
        this->requests.push_back(sRequestV_t());
#ifdef _DEBUG_
		std::cout << "Trying to find parallel ServiceRequests" << std::endl;
#endif
        while (_REGEX_PREFIX_::regex_match((*stepstrings)[i].c_str(), rxSearchResults, rxAndSep)) {
                
            std::string name=rxSearchResults[2];
            (*stepstrings)[i]=rxSearchResults[1];
            ConfigurationObj * srConf=tReader->getConfigurationObj(name);
            if (srConf==NULL) {
                throw SemanticParseException("Could not find Service Request " + name);
            }
            this->requests[i-1].push_back(new ServiceRequest(srConf, this));
        }
        if ((*stepstrings)[i].length()) {
            ConfigurationObj * srConf=tReader->getConfigurationObj((*stepstrings)[i]);
            if (srConf==NULL) {
                throw SemanticParseException("Could not find Service Request " + (*stepstrings)[i]);
            }
            this->requests[i-1].push_back(new ServiceRequest(srConf, this));
        }
        //now we have to turn around the requests to get them in the right order
        sRequestsV_t temp=sRequestsV_t();
        for (int i=((int)this->requests.size())-1; i>=0; i--) {
            temp.push_back(this->requests[i]);
        }
        this->requests=temp;
    }
    this->taskProcessor=NULL;
}

Task::~Task() {
    for (int i=0; i<this->requests.size(); i++) {
        for (int j=0; j<this->requests[i].size(); j++) {
            delete this->requests[i][j];
        }
    }
}

Task::Task(Task &other): ConfigurationObjWrapper(other.getConfRef()) {
    std::cout << "Don't try to copy a task" << std::endl;
    exit(10);
}

std::string Task::getNameAttribute() {
    stringV_t * values;
    size_t size=this->getValues("Name", &values);
    if (size<1 || values==NULL) {
        throw SemanticParseException("Name of task not defined");
    }
    if (size>1) {
        throw SemanticParseException("Task has more than one name");
    }
    return (*values)[0];
}

sRequestV_t * Task::getNextStep() {
    this->checkPosition();
    //Check if we have done evereything;
    if (this->position>=this->requests.size()) {
        return NULL;
    }
    return &(this->requests[this->position]);
}

bool Task::isDone() {
    if (this->getNextStep()==NULL) return true;
    return false;
}


stringV_t Task::getNeededProcessorTypes(int step) {
    stringV_t ret=stringV_t();
    for (int i=0; i<this->requests[step].size(); i++) {
        std::string type=this->requests[step][i]->getServiceProcessorType();
        bool insert=true;
        for (int j=0; j<ret.size(); j++) {
            if (ret[j]==type) insert=false;
        }
        if (insert) ret.push_back(type);
    }
    return ret;
}

stringV_t Task::getNeededProcessorTypes() {
    stringV_t ret=stringV_t();
    for (int i=0; i<this->requests.size(); i++) {
        stringV_t types=this->getNeededProcessorTypes(i);
        for (int j=0; j<types.size(); j++) {
            bool insert=true;
            for (int k=0; k<ret.size(); k++) {
                if (ret[k]==types[j]) insert=false;
            }
            if (insert) ret.push_back(types[j]);
        }
    }
    return ret;
}


neededProcsM_t Task::getNeededProcessors() {
    neededProcsM_t ret=neededProcsM_t();
    stringV_t types=this->getNeededProcessorTypes();
    for (int i=0; i<this->requests.size(); i++) {
        neededProcsM_t needs=this->getNeededProcessors(i);
        for (int j=0; j<types.size(); j++) {
            neededProcsM_t::iterator possibleInsert = needs.find(types[j]);
            if (possibleInsert != needs.end()) {
                neededProcsM_t::iterator retPos = ret.find(types[j]);
                if (retPos == ret.end()) {
                    ret.insert(std::make_pair(possibleInsert->first, possibleInsert->second));
                }
                else {
                    if (retPos->second<possibleInsert->second) retPos->second=possibleInsert->second;
                }
            }
        }
    }
    return ret;
}

neededProcsM_t Task::getNeededProcessors(int step) {
    if (step>=this->requests.size()) return neededProcsM_t();
    neededProcsM_t ret=neededProcsM_t();
    for (int i=0; i<this->requests[step].size(); i++) {
        std::string name=this->requests[step][i]->getServiceProcessorType();
        neededProcsM_t::iterator pos = ret.find(name);
        if (pos == ret.end()) {
            ret.insert(std::make_pair(name,1));
        }
        else {
            pos->second++;
        }
    }
    return ret;
}

neededProcsM_t Task::getNeededProcessorsForNextStep() {
    this->checkPosition();
    if (this->position>this->requests.size()) return neededProcsM_t();
    return this->getNeededProcessors(this->position);
}

int Task::findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors) {
    neededProcsM_t pM=this->getNeededProcessors();
    return this->findPossibleTaskProcessor(taskProcessors, pM);
}

int Task::findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors, neededProcsM_t pM, int startProc) {
    for (int i=0; i<taskProcessors->size(); i++) {
        if ((*taskProcessors)[(i+startProc)%taskProcessors->size()].canHandleProcs(pM)) {
            return (i+startProc)%taskProcessors->size();
        }
    }
    return -1;
}

void Task::checkPosition() {
    if (this->position>=this->requests.size()) {
        return;
    }
    //Check if this step is allready done
    int done=1;
    for (int i=0; i<this->requests[this->position].size(); i++) {
        if (!this->requests[this->position][i]->isDone()) {
            done=0;
        }
    }
    //if this step is done return the next step
    if (done) {
        this->position++;
        this->checkPosition();
    }
    return;
}

bool Task::stepInProgress() {
    this->checkPosition();
    if (this->position>=this->requests.size()) {
        return false;
    }
    bool inProgress=0;
    for (int i=0; i<this->requests[this->position].size(); i++) {
        if (this->requests[this->position][i]->isStarted()) {
            inProgress=true;
        }
    }
    return inProgress;
}

int Task::validate(ServiceReader * sReader) {
    for (int i=0; i<this->requests.size(); i++) {
        for(int j=0; j<this->requests[i].size(); j++) {
            this->requests[i][j]->validate(sReader);
        }
    }
    TaskProcessorV_t * tps;
    sReader->getTaskProcessors(&tps);
    int check=this->findPossibleTaskProcessor(tps);
    if (check<0) {
        throw SemanticParseException("Couldn't find a TaskProcessor with matching ServiceProcessors for ServiceRequest " + this->getName());
    }
    //will raise an error if name is not set;
    this->getNameAttribute();
    
    return 1;
}

int Task::getNumberOfJobs() {
    int ret=0;
    for (int i=0; i<this->requests.size(); i++) {
        ret+=this->requests[i].size();
    }
    //std::cout << this->getName() << " has " << ret << "Jobs to do" << std::endl;
    return ret;
}

sRequestV_t * Task::getRequests(int step) {
    return &this->requests[step];
}

bool Task::inProgress() {
    this->checkPosition();
    if (this->position>=this->requests.size()) {
        return false;
    }
    if (this->position==0) {
        for (int i=0; i<requests[0].size(); i++) {
            if (requests[0][i]->isStarted()) return true;
        }
        return false;
    }
    return true;
}

void Task::abort() {
    int start=this->position;
    if (this->isDone()) return;
    if (this->inProgress()) start++;
    for (int i=start; i<this->requests.size(); i++) {
        for (int j=0; j<this->requests[i].size(); j++) {
            delete this->requests[i][j];
        }
    }
    while (start<=this->requests.size()) {
        this->requests.pop_back();
    }
}



