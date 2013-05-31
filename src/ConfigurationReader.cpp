
#include "ConfigurationReader.h"
#include <fstream>
//#include <regex>
#include <vector>
#include "Prog3Settings.h"

#ifdef _USE_BOOST_REGEX_
#include <boost/regex.hpp>
#endif
#ifdef _USE_STD_REGEX_
#include <regex>
#endif

using std::string;

ConfigurationReader::ConfigurationReader(string path)
{
    this->mPath = path;
    this->readFile();
}

void ConfigurationReader::readFile()
{
    // File variables
    std::ifstream fileHandler(this->mPath.c_str());
    if (fileHandler.fail()) {
        std::cout << "File " << this->mPath << " could not be opened" << std::endl;
        exit(10);
    }
    string line;
    
    // Regex variables
    _REGEX_PREFIX_::cmatch rxSearchResults;
    bool rxSearchReturn = false;
    _REGEX_PREFIX_::regex rxBrackets("\\[(.+)\\]");
    _REGEX_PREFIX_::regex rxBracketsColon("\\[(.+):(.+)\\]");
    _REGEX_PREFIX_::regex rxEqual("(.+)=(.*)");
    _REGEX_PREFIX_::regex rxCommaSep("(.+)(,|( -> ))(.+)");
    _REGEX_PREFIX_::regex rxWhitespace("\\s*");
    //
    string result;
    
    ConfigurationObj *lastObj=NULL; //=new ConfigurationObj("root");
    //confObjects.insert(std::make_pair("root", lastObj));
    
    while(std::getline(fileHandler, line))
    {
        bool lineDone=0;
        rxSearchReturn = _REGEX_PREFIX_::regex_search(line.c_str(), rxSearchResults, rxBracketsColon);
        if(rxSearchReturn && !lineDone)
        {
            //object with parent
            string name = rxSearchResults[1];
            string parent = rxSearchResults[2];
            ConfigurationObj * parentObj=this->getConfigurationObj(parent);
            if (parentObj==NULL) {
                std::cout << "Configurationobject " << parent << " not defined, but expected by " << name <<std::endl;
                exit(3);
            }
            if (this->getConfigurationObj(name)!=NULL) {
                std::cout << "Double Definition of Object " << name << " in " << this->mPath << std::endl;
                exit(3);
            }
            lastObj=new ConfigurationObj(name, parentObj);
            confObjects.insert(std::make_pair(name, lastObj));
            lineDone=1;
        }
        
        rxSearchReturn = _REGEX_PREFIX_::regex_search(line.c_str(), rxSearchResults, rxBrackets);
        if(rxSearchReturn && !lineDone)
        {
            //object without parent;
            string name = rxSearchResults[1];
            lastObj=new ConfigurationObj(name);
            if (this->getConfigurationObj(name)!=NULL) {
                std::cout << "Double Definition of Object " << name << " in " << this->mPath << std::endl;
                exit(3);
            }
            confObjects.insert(std::make_pair(name, lastObj));
            lineDone=1;
        }
        
        rxSearchReturn = _REGEX_PREFIX_::regex_search(line.c_str(), rxSearchResults, rxEqual);
        if(rxSearchReturn && !lineDone)
        {
            if (lastObj==NULL) {
                std::cout << "Attribute definition before Object Definition" << std::endl;
                exit(10);
            }
            //Attributes
            string aName = rxSearchResults[1];
            string aValues=rxSearchResults[2];
            lastObj->addAttribute(aName);
            while (_REGEX_PREFIX_::regex_match(aValues.c_str(), rxSearchResults, rxCommaSep)) {
                
                string value=rxSearchResults[4];
                aValues=rxSearchResults[1];
                lastObj->addAttributeValue(aName, value);
            }
            if (aValues.length()) {
                lastObj->addAttributeValue(aName, aValues);
            }
            lineDone=1;
        }
        rxSearchReturn = _REGEX_PREFIX_::regex_match(line.c_str(), rxSearchResults, rxWhitespace);
        if(rxSearchReturn && !lineDone){
            //ignore Whitespaceline
            lineDone=1;
        }
        if (!lineDone) {
            std::cout << "Couldn't Parse line: " << line << std::endl;
        }
        
        //std::cout << results[1] << ". " << results[2] << "\n";
    }
}

ConfigurationObj * ConfigurationReader::getConfigurationObj(std::string key) {
    confObjMap_t::iterator pos = this->confObjects.find(key);
    if (pos== this->confObjects.end()) return NULL;
    return pos->second;
}
