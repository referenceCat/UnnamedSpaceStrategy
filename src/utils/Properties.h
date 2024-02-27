//
// Created by referencecat on 20.02.24.
//

#ifndef UNTITLEDSPACESTRATEGY_PROPERTIES_H
#define UNTITLEDSPACESTRATEGY_PROPERTIES_H

#include <string>
#include <hash_map>
#include <cassert>
#include <unordered_map>


class Properties {
    std::unordered_map<std::string, bool> booleanProperties;
    std::unordered_map<std::string, int> integerProperties;
    std::unordered_map<std::string, std::string> stringProperties;
public:
    bool isBooleanPropertyExists(std::string);

    void setBooleanPropertyValue(std::string, bool);

    bool getBooleanPropertyValue(std::string, bool);

    bool isIntegerPropertyExists(std::string);

    void setIntegerPropertyValue(std::string, int);

    int getIntegerPropertyValue(std::string, int);

    bool isStringPropertyExists(std::string);

    void setStringPropertyValue(std::string, std::string);

    std::string getStringPropertyValue(std::string, std::string);

};


#endif //UNTITLEDSPACESTRATEGY_PROPERTIES_H
