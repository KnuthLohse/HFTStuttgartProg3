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
            //Task has steps to handle
            ret=0;
            int processor=(*tasks)[i].findPossibleTaskProcessorForNextStep(taskProcessors);
            if (processor>=0) {
                //task has found a taskprocessor that can handle its next step
                //processor is the TaskProcessor which will handle the process -> prepare the return
                ret=1;
                sRequestV_t * requests=(*tasks)[i].getNextStep();
                for (int i=0; i<requests->size(); i++) {
                    //register all Servicerequests
                    ServiceProcessor * serviceProcessor=(*taskProcessors)[i].registerServiceRequest(&(*requests)[i]);
                    int index=serviceProcessor->getID();
                    procParamP_t param=std::make_pair(this->nextServiceRequestID, (*requests)[i].getDuration());
                    (*procsToStart)->push_back(std::make_pair(index, param));
                    //register here
                    this->requestIDMap.insert(std::make_pair(this->nextServiceRequestID, serviceProcessor));
                    this->nextServiceRequestID++;
                }
                return (int)(*procsToStart)->size();
            }
        }
    }
    return ret;
}

/**
 * Tells the Controller that a single Taskprocessor has finished the job with the given ID
 * @Param id ID of the job that has finished
 * @Return true if everything's allright; False if an error occurred
 */
int Controller::jobFinished(int jobID) {
    ServiceRequestIDM_t::iterator pos = this->requestIDMap.find(jobID);
    if (pos== this->requestIDMap.end()) return -1;
    ServiceProcessor * serviceProcessor = pos->second;
    serviceProcessor->jobFinished();
    return 1;
}