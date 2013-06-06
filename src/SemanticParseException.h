//
//  SemanticParseException.h
//  Prog3_2
//
//  Created by Knuth Lohse on 06.06.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#ifndef __Prog3_2__SemanticParseException__
#define __Prog3_2__SemanticParseException__

#include <iostream>

class SemanticParseException {
public:
    SemanticParseException(std::string error);
    
    std::string getError();
    
private:
    std::string error;
};

#endif /* defined(__Prog3_2__SemanticParseException__) */
