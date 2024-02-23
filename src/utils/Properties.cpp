//
// Created by referencecat on 20.02.24.
//

#include "Properties.h"

bool Properties::isBooleanPropertyExists(std::string key) {
    return booleanProperties.contains(key);
}

void Properties::setBooleanPropertyValue(std::string key, bool value) {
    booleanProperties.insert({key, value});
}

bool Properties::getBooleanPropertyValue(std::string key, bool default_value) {
    return isBooleanPropertyExists(key) ? booleanProperties.at(key) : default_value;
}

bool Properties::isIntegerPropertyExists(std::string key) {
    return integerProperties.contains(key);
}

void Properties::setIntegerPropertyValue(std::string key, int value) {
    integerProperties.insert({key, value});
}

int Properties::getIntegerPropertyValue(std::string key, int default_value) {
    return isIntegerPropertyExists(key) ? integerProperties.at(key) : default_value;
}

bool Properties::isStringPropertyExists(std::string key) {
    return stringProperties.contains(key);
}

void Properties::setStringPropertyValue(std::string key, std::string value) {
    stringProperties.insert({key, std::move(value)});
}

std::string Properties::getStringPropertyValue(std::string key, std::string default_value) {
    return isStringPropertyExists(key) ? std::move(stringProperties.at(key)) : std::move(default_value);
}
