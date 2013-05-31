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
#include "ConfigurationObjWrapper.h"
#include "ConfigurationObj.h"


class ServiceReader;
 
class ServiceRequest : public ConfigurationObjWrapper
{
public:
    
    /**
     * @param conf Configuration of the ServiceRequest
     */
    ServiceRequest(ConfigurationObj * conf);
    
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

private:
    
    bool done; ///true if job has finished; else false


};

#endif /* defined(__Prog3__Task__) */