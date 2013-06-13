//
//  Task.h
//  Prog3
//
//  Created by Knuth Lohse on 28.05.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3__ServiceRequest__
#define __Prog3__ServiceRequest__

#include <iostream>
#include <fstream>
#include "ConfigurationObjWrapper.h"
#include "ConfigurationObj.h"


class ServiceReader;
class Task;
 
class ServiceRequest : public ConfigurationObjWrapper
{
public:
    
    /**
     * @param conf Configuration of the ServiceRequest
     * @param t Task to which this ServiceRequest belongs
     */
    ServiceRequest(ConfigurationObj * conf, Task * t);
    
    /**
     * @return Duration in seconds
     */
    int getDuration();
    
    /**
     * @return true if job has finished; false if it has not finished or even started yet
     */
    bool isDone();
    
    /**
     * to call when the job is finished
     * @return true; false on error (when it had allready finished before)
     */
    bool setDone();
    
    /**
     * @return Type of ServiceProcessor that is needed to run this task
     */
    std::string getServiceProcessorType();
    
    /**
     * validates the information read from the Configurationfile
     * Checks if all neccessary Values are set, and  if an according ServiceProcessor exists
     */
    int validate(ServiceReader * sReader);
    
    /**
     * @returns Name of Task and ServiceRequest
     */
    std::string toString();

    bool isStarted();
    void setStarted(bool s);
    Task * getTask();
    
private:
    
    bool done; ///true if job has finished; else false
    bool started; ///true if the job is started
    Task * task; ///Task that the ServiceRequest belongs to

};

#endif /* defined(__Prog3__Task__) */
