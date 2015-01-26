#ifndef OCS_STRING_UTILITIES_H
#define OCS_STRING_UTILITIES_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

std::vector<std::string> split(const std::string&, char, char = '\0', char = '\0');
bool isInsideDelimeters(const std::string& str, int charPos, char leftDelim, char rightDelim);
void trimWhiteSpace(std::string&);

void removeOuterNonDigitCharacters(std::string& str);

void removeOuterSymbols(std::string& str, char leftSymbol, char rightSymbol);

//Used so the entire string is put into the stringstream instead of up to the first whitespace character
std::stringstream& operator<<(std::stringstream& out, const std::string& str);
std::stringstream& operator>>(std::stringstream& in, std::string& str);

//std::stringstream& operator>>(std::stringstream& in, bool value);

bool stringToBool(const std::string& str);

template<typename T>
std::string toString(const T& item){
    std::stringstream ss;
    ss << item;
    return ss.str();
}

template<typename T>
T fromString(const std::string& str)
{
    T value = T();
    std::stringstream ss(str);
    ss >> value;
    return value;
}

#endif
