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
    /**
     * @param filename Path and filename ot the Configurationfile to read
     */
    ConfigurationReader(std::string filename);
    
    ~ConfigurationReader();
    
    ConfigurationReader(ConfigurationReader &toCopy);

    /**
     * @param key Name of the Attribute of interrest
     * @return ConfigurationObject with the given Name
     */
    ConfigurationObj * getConfigurationObj(std::string key);

protected:
    // Read configuration file
    void readFile();
private:
    std::string mPath; ///Filename of the Configuration File

protected:
    confObjMap_t confObjects; ///Map of Configuration Objects to store the Attributes in - Key is the name of the attribute
};

#endif	/* CONFIGURATIONREADER_H */