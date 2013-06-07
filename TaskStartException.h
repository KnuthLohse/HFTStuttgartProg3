//
//  TaskStartException.h
//  Prog3_2
//
//  Created by Knuth Lohse on 06.06.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3_2__TaskStartException__
#define __Prog3_2__TaskStartException__

#include <iostream>

class TaskStartException {
public:
    //3: Task busy
    //2: No Task to Start
    //1: No Job To start
    //0: Processor busy
    //-1: Processor can't handle task
    TaskStartException(int reason);
    
    int getReason();

private:
    int reason;
};

#endif /* defined(__Prog3_2__TaskStartException__) */
