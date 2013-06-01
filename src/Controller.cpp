#include "Controller.h"
#include "ServiceReader.h"


/**
 * Reads the ini files, and might end the program if there's an error
 *
 * @Param filename Name including path of the System.ini
 */
Controller::Controller(std::string filename) {
    this->serviceReader=new ServiceReader(filename);
}


/**
 * Reads the ini files from a standard location, and might end the program if there's an error
 *
 * @Param filename Name including path of the System.ini
 */
Controller::Controller() {
    this->serviceReader=new ServiceReader("Service.ini");
}

Controller::~Controller() {
    delete this->serviceReader;
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