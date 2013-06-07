//
//  TaskProcessor.h
//  Prog3
//
//  Created by Knuth Lohse on 22.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__TaskProcessor__
#define __Prog3__TaskProcessor__

#include <iostream>
#include "ConfigurationObj.h"
#include "ServiceProcessor.h"
#include "ConfigurationReader.h"
#include "ConfigurationObjWrapper.h"
#include "Controller.h"

class Task;
typedef std::vector<Task *> TaskV_t;

class ServiceRequest;
typedef std::vector<ServiceProcessor> ServiceProcessorV_t;
typedef std::map<std::string, int> neededProcsM_t;

class TaskProcessor : public ConfigurationObjWrapper
{
public:
    /**
     * @param conf ConfigurationObject to retrive the Attributes from
     * @param reader ConfigurationReader to get the ServiceProcessor information from
     */
    TaskProcessor(ConfigurationObj *conf, ConfigurationReader *reader);
    
    /**
     * @return Number of Serviceprocessor of the given type belonging to this TaskProcessor that are not working right now
     */
    int supports(std::string type, bool ignoreIdleStatus=false);
    
    /**
     * @return QueueSize of the TaskProcessor
     */
    int getQueueSize();
    
    /**
     * Validates the values of this TaskProcessor and its members
     * Might exit if the validation is not successfull
     * return 1 if everything seems to be allright; >=0 if something went wrong
     */
    int validate();
    
    /**
     * Assigns an ID to all its ServiceProcessors, starting by startID
     *
     * @param ServiceProcessorTypes OUT Pointer to a Vector where the types of the ServiceProcessors are added in order of their ID
     * @param startID ID that the first ServiceProcessor of this Task should get
     * @return ID to give to the next ServiceProcessor
     */
    int registerServiceProceossors(stringV_t * ServiceProcessorTypes, int startID);
    
    /**
     * @param serviceRequest ServiceRequest to assign to a ServiceProcessor
     * @return ServiceProcessor that will handle the task
     */
    ServiceProcessor * registerServiceRequest(ServiceRequest * serviceRequest);

    /**
     * @param Map of free processors needed
     * @return true if at least the given number of processors are idle; false else
     */
    bool canHandleProcs(neededProcsM_t procsToTest);
    
    /**
     * @param Map of free processors needed
     * @return true if at least the given number of processors are existent; false else
     */
    bool canHandleProcsIgnoringIdleStatus(neededProcsM_t procsToTest);
    
    /**
     * Adds the Task to the list of running tasks
     * @param t Task to add
     */
    void addStartedTask(Task * t);

    /**
     * @param procsToStart OUT List to return to Rose/RT - musst be initialized and empty
     * @param nextJobID number of the next Job
     * @param task Task to start
     * @return number of processes that are started
     *
     * Might Throw TaskStartException
     */
    size_t startTask(procsToStartV_t ** procsToStart, int nextJobID, Task * task);
    
    /**
     * Trys to resume a task that is already startet on this processor
     * @param procsToStart OUT List to return to Rose/RT - musst be initialized and empty
     * @param nextJobID number of the next Job
     * @return number of processes that are started
     *
     * Might Throw TaskStartException
     */
    size_t resumeTask(procsToStartV_t ** procsToStart, int nextJobID);
    
    /**
     * Marks the given job as finished
     * @param jobID job to mark as finished
     * @return -1 if the job is not found
     */
    int jobFinished(int jobID);
    
private:
    
    ServiceProcessorIDM_t serviceProcIDMap; ///Map to identifiy the ServiceRequests that have finished;
    
    ServiceProcessorV_t serviceProcessors; ///Vector of the serviceProcessors of this TaskProcessor
    TaskV_t startedTasks; ///Tasks that are running on this Processor
};





#endif /* defined(__Prog3__TaskProcessor__) */
