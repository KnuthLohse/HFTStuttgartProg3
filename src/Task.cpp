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
        std::string type=this->requests[step][i].getServiceProcessorType();
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
    neededProcsM_t ret=neededProcsM_t();
    for (int i=0; i<this->requests[step].size(); i++) {
        std::string name=this->requests[step][i].getServiceProcessorType();
        neededProcsM_t::iterator pos = ret.find(name);
        if (pos == ret.end()) {
            ret.insert(std::make_pair(name,i));
        }
        else {
            pos->second++;
        }
    }
    return ret;
}

int Task::findPossibleTaskProcessorForNextStep(TaskProcessorV_t * taskProcessors, int startProc) {
    this->checkPosition();
    neededProcsM_t neededProcessors=getNeededProcessors(this->position);
    return this->findPossibleTaskProcessor(taskProcessors, neededProcessors, startProc);
}

int Task::findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors) {
    neededProcsM_t pM=this->getNeededProcessors();
    return this->findPossibleTaskProcessor(taskProcessors, pM);
}

int Task::findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors, neededProcsM_t pM, int startProc) {
    stringV_t types=getNeededProcessorTypes();
    int proc=0;
    for (int i=0; i<types.size() && proc<taskProcessors->size(); i++) {
        neededProcsM_t::iterator pos = pM.find(types[i]);
        if (pos==pM.end()) {
            //Something went wrong;
            std::cout << "Something wen't wrong while trying to find a matching TaskProcesser" << std::endl;
            exit(3);
        }
        else {
            if (pos->second>(*taskProcessors)[(proc+startProc)%taskProcessors->size()].supports(types[i])) {
                proc++;
                i=0;
            }
        }
    }
    if (proc>taskProcessors->size()) {
        return -1;
    }
    return proc;
}

void Task::checkPosition() {
    if (this->position>=this->requests.size()) {
        return;
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
        this->checkPosition();
    }
    return;
}

int Task::validate(ServiceReader * sReader) {
    for (int i=0; i<this->requests.size(); i++) {
        for(int j=0; j<this->requests[i].size(); j++) {
            this->requests[i][j].validate(sReader);
        }
    }
    TaskProcessorV_t * tps;
    sReader->getTaskProcessors(&tps);
    int check=this->findPossibleTaskProcessor(tps);
    if (check<0) {
        std::cout << "Couldn't find a TaskProcessor with matching ServiceProcessors for ServiceRequest " << this->getName() << std::endl;
        exit(1);
    }

    return 1;
}
