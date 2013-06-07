#include "Controller.h"
#include "ServiceReader.h"
#include "ServiceProcessor.h"
#include "ServiceRequest.h"
#include <vector>
#include "TaskStartException.h"


Controller::Controller(std::string filename) {
    this->serviceReader=new ServiceReader(filename);
    this->nextServiceRequestID=0;
    this->taskProcIDMap=TaskProcessorIDM_t();
}


Controller::Controller() {
    this->serviceReader=new ServiceReader("Service.ini");
    this->nextServiceRequestID=0;
    this->taskProcIDMap=TaskProcessorIDM_t();
}

Controller::~Controller() {
    delete this->serviceReader;
}

Controller::Controller(const Controller &toCopy) {
    std::cout << "Don't try to Copy an Controller - Bad things might happen on deconstruction" << std::endl;
    exit(10);
}

int Controller::getServiceProcessors(processorV_t ** processorList) {
    if (*processorList == NULL) {
        (*processorList)=new processorV_t();
    }
    else {
        (*processorList)->clear();
    }
    int ret=0;
    TaskProcessorV_t * taskProcessors;
    this->serviceReader->getTaskProcessors(&taskProcessors);
    for (int i=0; i<taskProcessors->size(); i++) {
        ret=(*taskProcessors)[i].registerServiceProceossors(*processorList, ret);
    }
    return ret;
}

/**
 * Returns a List of procs with params to start
 * @Param procsToStart OUT: List of procs to start; Undefined if we need to wait or all Jobs are done
 * @Return: Number of Procs to start; -1 If all Jobs are finished; 0 if we need to wait for jobs to finish
 */
int Controller::getNextJobs(procsToStartV_t ** procsToStart) {
    if (*procsToStart==NULL) {
        (*procsToStart)=new procsToStartV_t();
    }
    else {
        (*procsToStart)->clear();
    }
    int ret=-1; //all tasks finished
    TaskProcessorV_t * taskProcessors;
    this->serviceReader->getTaskProcessors(&taskProcessors);
    //try to continue Tasks that are in progress
    for (int i=0; i<taskProcessors->size(); i++) {
        try {
            size_t r=(*taskProcessors)[i].resumeTask(procsToStart, this->nextServiceRequestID);
            if (r>0) {
                this->writeJobMap(*procsToStart, &(*taskProcessors)[i]);
                return (int)r;
            }
            std::cout << "This shouldn't happen - tryed to continue Task and found no job to continue";
            exit(10);
            if (r==0) ret=0;
        }
        catch (TaskStartException &e) {
            if (e.getReason()!=1) ret=0;
            //TaskProcessor has not Tasks to resume - ignore
        }
    }
    TaskV_t * tasks;
    this->serviceReader->getTasks(&tasks);
    for (int j=0; j<tasks->size(); j++) {
        if (!(*tasks)[j]->inProgress() && !(*tasks)[j]->isDone()) {
            ret=0;
            for (int i=0; i<taskProcessors->size(); i++) {
                try {
                    size_t r=(*taskProcessors)[i].startTask(procsToStart, this->nextServiceRequestID, (*tasks)[j]);
                    if (r>0) {
                        //std::cout << "breakpoint";
                        this->writeJobMap(*procsToStart, &(*taskProcessors)[i]);
                        return (int)r;
                    }
                    //shouldnt get here
                    if (r==0) ret=0;
                }
                catch (TaskStartException &e) {
                }
            }
        }
    }
    return ret;
}
        
void Controller::writeJobMap(procsToStartV_t * procsToStart, TaskProcessor * tp) {
    for (int i=0; i<procsToStart->size(); i++) {
        this->taskProcIDMap.insert(std::make_pair((*procsToStart)[i].second.first, tp));
    }
    this->nextServiceRequestID+=procsToStart->size();
}

int Controller::jobFinished(int jobID) {
    TaskProcessorIDM_t::iterator pos = this->taskProcIDMap.find(jobID);
    if (pos==this->taskProcIDMap.end()) {
        std::cout << "Exit because job should be removed from Controller::taskProcIDMap but was not found";
        exit(10);
    };
    *(this->serviceReader->getLogStream()) << "Service Request finished: " << pos->second->getRunningSRequestString(jobID) << std::endl;
    TaskProcessor * taskProcessor = pos->second;
    taskProcessor->jobFinished(jobID);
    this->taskProcIDMap.erase(pos);
    return 1;
}

int Controller::jobStarted(int jobID) {
    TaskProcessorIDM_t::iterator pos = this->taskProcIDMap.find(jobID);
    if (pos==this->taskProcIDMap.end()) {
        std::cout << "Exit because job should be in Controller::taskProcIDMap but was not found (JobStarted)";
        exit(10);
    };
    *(this->serviceReader->getLogStream()) << "Service Request started: " << pos->second->getRunningSRequestString(jobID) << std::endl;
    return 1;
}

int Controller::jobException(int jobID) {
    TaskProcessorIDM_t::iterator pos = this->taskProcIDMap.find(jobID);
    if (pos==this->taskProcIDMap.end()) {
        std::cout << "Exit because job should be in Controller::taskProcIDMap but was not found (JobException)";
        exit(10);
    };
    *(this->serviceReader->getLogStream()) << "SERVICE REQUEST EXCEPTION: " << pos->second->getRunningSRequestString(jobID) << std::endl;
    return 1;
}

JobsToKillV_t Controller::jobUnexpectedTerminated(int jobID) {
    TaskProcessorIDM_t::iterator pos = this->taskProcIDMap.find(jobID);
    if (pos==this->taskProcIDMap.end()) {
        std::cout << "Exit because job should be in Controller::taskProcIDMap but was not found (JobException)";
        exit(10);
    };
    *(this->serviceReader->getLogStream()) << "SERVICE REQUEST/SERVICE PROCESSOR UNEXPECTED TERMINATED: " << pos->second->getRunningSRequestString(jobID) << std::endl;
    TaskProcessor * taskProcessor = pos->second;
    JobsToKillV_t ret=taskProcessor->unexpectedTermination(jobID);
    this->taskProcIDMap.erase(pos);
    return ret;
    
}

int Controller::jobAbortConfirmation(int jobID) {
    TaskProcessorIDM_t::iterator pos = this->taskProcIDMap.find(jobID);
    if (pos==this->taskProcIDMap.end()) {
        std::cout << "Exit because job should be removed from Controller::taskProcIDMap but was not found";
        exit(10);
    };
    *(this->serviceReader->getLogStream()) << "Service Request Aborted: " << pos->second->getRunningSRequestString(jobID) << std::endl;
    return 1;
}

void Controller::debug() {
    TaskV_t * tasks;
    serviceReader->getTasks(&tasks);
    int nOfJobs=0;
    for (int i=0; i<tasks->size(); i++) {
        int jobs=(*tasks)[i]->getNumberOfJobs();
        nOfJobs=nOfJobs+jobs;
    }
    std::cout << "Number of Jobs: " << nOfJobs << std::endl;
}
