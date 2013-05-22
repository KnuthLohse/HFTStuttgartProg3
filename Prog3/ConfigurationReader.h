/* 
 * File:   ConfigurationReader.h
 * Author: Thimo
 *
 * Created on 19. Mai 2013, 18:09
 */

#ifndef CONFIGURATIONREADER_H
#define	CONFIGURATIONREADER_H

#include <map>
#include <string>
#include "ConfigurationObj.h"

typedef std::map<std::string, ConfigurationObj*> confObjMap_t;

class ConfigurationReader
{
public:
    ConfigurationReader(std::string filename);
    ConfigurationObj * getConfigurationObj(std::string key);
    // Read configuration file
    void readFile();

private:
    std::string mPath;

protected:
    confObjMap_t confObjects;

};

#endif	/* CONFIGURATIONREADER_H */