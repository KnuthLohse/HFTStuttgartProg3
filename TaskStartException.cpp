//
//  TaskStartException.cpp
//  Prog3_2
//
//  Created by Knuth Lohse on 06.06.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "TaskStartException.h"

TaskStartException::TaskStartException(int reason) {
    this->reason=reason;
}

int TaskStartException::getReason() {
    return this->reason;
}