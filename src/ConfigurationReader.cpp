
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
    this->errorString=(std::ostringstream *) new std::ostringstream();
    this->errors=0;
    this->readFile();
}

ConfigurationReader::~ConfigurationReader() {
    for(confObjMap_t::iterator it = this->confObjects.begin(); it != this->confObjects.end(); ++it) {
        delete it->second;
    }
    delete this->errorString;
}

ConfigurationReader::ConfigurationReader(ConfigurationReader &toCopy) {
    std::cout << "Don't try to copy an ConfigurationReader - Bad things might haben on deconstruction";
    exit(10);
}

void ConfigurationReader::readFile()
{
    // File variables
    std::ifstream fileHandler(this->mPath.c_str());
    if (fileHandler.fail()) {
        *(this->errorString) << "File " << this->mPath << " could not be opened" << std::endl;
        this->errors++;
        return;
    }
    string line;
    
    // Regex variables
    _REGEX_PREFIX_::cmatch rxSearchResults;
    bool rxSearchReturn = false;

   _REGEX_PREFIX_::regex rxBrackets("\\s*\\[(.+)\\]\\s*");
   _REGEX_PREFIX_::regex rxBracketsColon("\\s*\\[(\\S+):(\\S+)\\]\\s*");
   _REGEX_PREFIX_::regex rxEqual("\\s*(\\S+)=(.*?)\\s*");
   _REGEX_PREFIX_::regex rxCommaSep("\\s*(.+)(,|( -> ))(.+)");
   _REGEX_PREFIX_::regex rxWhitespace("\\s*");
 
    string result;
    
    ConfigurationObj *lastObj=NULL; //=new ConfigurationObj("root");
    //confObjects.insert(std::make_pair("root", lastObj));
    bool ignoreAttributes=false;
    stringV_t toDelete=stringV_t(); //Objects to delete when finished reading
    while(std::getline(fileHandler, line))
    {
	#ifdef _DEBUG_
		std::cout << "parsing line: " << line << std::endl;
	#endif
        bool lineDone=0;
        rxSearchReturn = _REGEX_PREFIX_::regex_match(line.c_str(), rxSearchResults, rxBracketsColon);
        if(rxSearchReturn && !lineDone)
        {
            //object with parent
            string name = rxSearchResults[1];
            string parent = rxSearchResults[2];
#ifdef _DEBUG_
			std::cout << "Parsed new Object: " << name << " as child of " << parent <<std::endl; 
#endif
            ConfigurationObj * parentObj=this->getConfigurationObj(parent);
            if (parentObj==NULL) {
                (*(this->errorString)) << "Configurationobject " << parent << " not defined, but expected by " << name <<std::endl;
                this->errors++;
                ignoreAttributes=true;
            }
            else if (this->getConfigurationObj(name)!=NULL) {
                *(this->errorString) << "Double Definition of Object " << name << " in " << this->mPath << ". Ignoring this Object" << std::endl;
                this->errors++;
                toDelete.push_back(name);
                ignoreAttributes=true;
            }
            else {
                int e=errors;
                for (int i=0; i<toDelete.size(); i++) {
                    if (toDelete[i]==parentObj->getName()) {
                        *(this->errorString) << "Parent of Object " << name << " is invalid " << this->mPath << ". Ignoring this Object" << std::endl;
                        this->errors++;
                        ignoreAttributes=true;
                    }
                }
                if (e==this->errors) {
                    lastObj=new ConfigurationObj(name, parentObj);
                    confObjects.insert(std::make_pair(name, lastObj));
                    ignoreAttributes=false;
                }
#ifdef _DEBUG_
				ConfigurationObj* tpCObj=this->getConfigurationObj(name);
				if (tpCObj==NULL) {
					std::cout << "Insertion of new Object Failes" << std::endl;
				}
#endif
            }
            lineDone=1;
        }
        
        rxSearchReturn = _REGEX_PREFIX_::regex_match(line.c_str(), rxSearchResults, rxBrackets);
        if(rxSearchReturn && !lineDone)
        {
            //object without parent;
            string name = rxSearchResults[1];
#ifdef _DEBUG_
			std::cout << "Parsed new Object: " << name <<std::endl; 
#endif
            if (this->getConfigurationObj(name)!=NULL) {
                *(this->errorString) << "Double Definition of Object " << name << " in " << this->mPath << ". Ignoring this Object" << std::endl;
                this->errors++;
                toDelete.push_back(name);
            }
            else {
                lastObj=new ConfigurationObj(name);
                confObjects.insert(std::make_pair(name, lastObj));
                ignoreAttributes=false;
            }
            lineDone=1;
        }
        
        rxSearchReturn = _REGEX_PREFIX_::regex_match(line.c_str(), rxSearchResults, rxEqual);
        if(rxSearchReturn && !lineDone)
        {
            if (lastObj==NULL) {
                *(this->errorString) << "Attribute definition before Object Definition - Ignoring line" << std::endl;
                this->errors++;
            }
            else if (ignoreAttributes) {
                *(this->errorString) << "Ignoring Attribute definition line" << std::endl;
            }
            else {
                //Attributes
                string aName = rxSearchResults[1];
                string aValues=rxSearchResults[2];
                lastObj->addAttribute(aName);
#ifdef _DEBUG_
				std::cout << "Parsed new Attribute: " << aName << std::endl; 
#endif

                while (_REGEX_PREFIX_::regex_search(aValues.c_str(), rxSearchResults, rxCommaSep)) {
                
                    string value=rxSearchResults[4];
                    aValues=rxSearchResults[1];
                    lastObj->addAttributeValue(aName, value);
#ifdef _DEBUG_
					std::cout << "Parsed new AttributeValue: " << value << std::endl; 
#endif

					}
                if (aValues.length()) {
#ifdef _DEBUG_
			std::cout << "Parsed new AttributeValue(last): " << aValues << std::endl; 
#endif

                    lastObj->addAttributeValue(aName, aValues);
                }
            }
            lineDone=1;
        }
        rxSearchReturn = _REGEX_PREFIX_::regex_match(line.c_str(), rxSearchResults, rxWhitespace);
        if(rxSearchReturn && !lineDone){
            //ignore Whitespaceline
            lineDone=1;
        }
        if (!lineDone) {
            *(this->errorString) << "Ignoring Line - Couldn't Parse it: " << line << std::endl;
            this->errors++;
        }
        //std::cout << results[1] << " . " << results[2] << "\n";
    }
    for (int i=0; i<toDelete.size(); i++) {
        this->confObjects.erase(toDelete[i]);
    }
}

ConfigurationObj * ConfigurationReader::getConfigurationObj(std::string key) {
    confObjMap_t::iterator pos = this->confObjects.find(key);
    if (pos== this->confObjects.end()) return NULL;
    return pos->second;
}

int ConfigurationReader::getErrorString(std::string * error) {
    *error=(*(this->errorString)).str();
    return this->errors;
}
