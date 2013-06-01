#include "Controller.h"
#include "ServiceReader.h"


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