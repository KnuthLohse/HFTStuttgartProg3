//
//  TaskDesription.h
//  Prog3
//
//  Created by Knuth Lohse on 27.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__Task__
#define __Prog3__Task__

#include <iostream>
#include <fstream>
#include "ConfigurationObjWrapper.h"
#include "ConfigurationReader.h"
#include "ServiceRequest.h"
#include "TaskProcessor.h"

class ServiceReader;
class TaskDescriptionReader;

typedef std::vector<TaskProcessor> TaskProcessorV_t;

typedef std::vector<std::string> stringV_t;
typedef std::vector<ServiceRequest *> sRequestV_t;
typedef std::vector<sRequestV_t> sRequestsV_t;
typedef std::map<std::string, int> neededProcsM_t;

class Task : public ConfigurationObjWrapper {
    
public:
    /**
     * @param conf Configuration of this Taks
     * @param tReader Taskreader that is creating this Object, to create the ServiceRequests
     *
     */
	Task(ConfigurationObj *conf, TaskDescriptionReader * tReader);
    
    ~Task();
    
    Task(Task &other);
    
    /**
     * @return Name of this task
     */
    std::string getNameAttribute();
    //std::string getName();
    
    /**
     * @return  List of ServiceRequest that need to be executed in the next step
     *          List of 0 Elements when nothing is to do for now
     *          Null if Task is finished
     * 
     */
    sRequestV_t * getNextStep();
    
    /**
     * @return true if task is done, else false
     */
    bool isDone();
    
    /**
     * Validates all necessary Settings read from the configurationFile
     * Will probably exit if it finds an Error
     *
     * @param sReader Service reader to check against (e.g. to check if a TaskProcessor exists that is capable of executing this task)
     * @return 1 if validated without error <=0 if an Error occured
     */
    int validate(ServiceReader * sReader);
    
    /**
     * @param taskProcessors List of all taskProcessors to choose from;
     * @return index of the first taskProcessor in the list that is capable of executing this Task; -1 if none of the given TaskProcessors is cabapble of executing this Task;
     *
     */
    int findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors);
    
    /**
     * @return Map of processors that are needed to execute this Task - key is the type of Processor, Value the number of processors needed
     */
    neededProcsM_t getNeededProcessors();
    
    /**
     * @param step Step of interrest
     * @return Map of processors that are needed to execute the given step of this Task - key is the type of Processor, Value the number of processors needed
     */
    neededProcsM_t getNeededProcessors(int step);
    
    
    /**
     * @return Map of processors that are needed to execute the next step
     */
    neededProcsM_t getNeededProcessorsForNextStep();
    
    /**
     * @return list of processorTypes needed to execute this task; each one is listed only once, even if more than one of a type is needed.
     */
    stringV_t getNeededProcessorTypes();
    
    /**
     * @param step Step of interest
     * @return list of processorTypes needed to execute this task; each one is listed only once, even if more than one of a type is needed.
     */
    stringV_t getNeededProcessorTypes(int step);
    
    /**
     * @return true if none of the Servicerequests of the actual step are started; false else
     */
    bool stepInProgress();
    
    /**
     * @param step Step of interest
     * @return Requests of the given step
     */
    sRequestV_t * getRequests(int step);
      
    //for Debuging purpose
    int getNumberOfJobs();
    
    /**
     * @return true if task is started; false else;
     */
    bool inProgress();
    
    /**
     * removes all Servicerequests of future steps
     */
    void abort();
    
    
private:
    /*
     * Checks if the aktuall position is finished and if yes increases it to the first position that is not finished or till the task is finished
     */
    void checkPosition();
    
    
    /**
     * @param taskProcessors List of all taskProcessors to choose from;
     * @param startProc Processor that should be tested first
     * @param pM Map of ServiceTypes that needs to be existend
     * @return index of the first taskProcessor in the list that is capable of executing this Task; -1 if none of the given TaskProcessors is cabapble of executing this Task;
     *
     */
    int findPossibleTaskProcessor(TaskProcessorV_t * taskProcessors, neededProcsM_t pM, int startProc=0);
    
    int position; /// first step that has not finished yet
    sRequestsV_t requests; ///List of Steps, each Step is a list of ServiceRequests to be handled parallel
    TaskProcessor * taskProcessor; ///TaskProcessor that handles this task
    
    friend class ServiceRequest;
};

typedef std::vector<Task *> TaskV_t;


#endif /* defined(__Prog3__Task__) */
