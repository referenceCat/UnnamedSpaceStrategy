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
