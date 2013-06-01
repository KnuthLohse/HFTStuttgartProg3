#include "Controller.h"
#include "ServiceReader.h"
#include "ServiceProcessor.h"
#include "ServiceRequest.h"
#include <vector>


Controller::Controller(std::string filename) {
    this->serviceReader=new ServiceReader(filename);
}


Controller::Controller() {
    this->serviceReader=new ServiceReader("Service.ini");
    this->nextServiceRequestID=0;
    this->requestIDMap=ServiceRequestIDM_t();
}

Controller::~Controller() {
    delete this->serviceReader;
    this->nextServiceRequestID=0;
    this->requestIDMap=ServiceRequestIDM_t();
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


//typedef std::pair<int,int> procParamP_t; //first: jobID to return when the job is finished; second: Duration in seconds
//typedef std::vector<int, procParamP_t> procsToStartV_t; //first: Index of ServiceProcessor; Second Params to start the job
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
    TaskProcessorV_t * taskProcessors;
    this->serviceReader->getTaskProcessors(&taskProcessors);
    TaskV_t * tasks;
    this->serviceReader->getTasks(&tasks);
    int ret=-1;
    for (int i=0; i<tasks->size(); i++) {
        if (!(*tasks)[i].isDone()) {
            ret=0;
            int processor=(*tasks)[i].findPossibleTaskProcessorForNextStep(taskProcessors);
            if (processor>=0) {
                ret=1;
                sRequestV_t * requests=(*tasks)[i].getNextStep();
                for (int i=0; i<requests->size(); i++) {
                    ServiceProcessor * serviceProcessor=(*taskProcessors)[i].registerServiceRequest(&(*requests)[i]);
                    int index=serviceProcessor->getID();
                    procParamP_t param=std::make_pair(this->nextServiceRequestID, (*requests)[i].getDuration());
                    (*procsToStart)->push_back(std::make_pair(index, param));
                    //register here
                    this->requestIDMap.insert(std::make_pair(this->nextServiceRequestID, serviceProcessor));
                    this->nextServiceRequestID++;
                }
                //processor is the TaskProcessor which will handle the process -> prepare the return
            }
        }
    }
    return ret;
}