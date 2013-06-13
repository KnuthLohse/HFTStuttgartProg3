//
//  SemanticParseException.cpp
//  Prog3_2
//
//  Created by Knuth Lohse on 06.06.13.
//  Copyright (c) 2013 Knuth Lohse. All rights reserved.
//

#include "SemanticParseException.h"
SemanticParseException::SemanticParseException(const std::string error) {
    this->error=error;
}

std::string SemanticParseException::getError() {
    return this->error;
}
