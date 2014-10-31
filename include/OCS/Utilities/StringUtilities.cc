#include "OCS/Utilities/StringUtilities.hpp"
#include <iostream>
#include <cstdlib>

std::vector<std::string> split(const std::string& str, char separator, char leftDelim, char rightDelim)
{
    std::string tempStr = str;
    std::vector<std::string> substrings;
    std::size_t index = tempStr.find_first_of(separator);

    //While the separator was in the string
    while(index != std::string::npos)
    {
     //   std::cout << "index " << index << "\n";
        if(leftDelim == '\0' && rightDelim == '\0')
        {
            //Take the sub string before the separator and store it.
            substrings.push_back(tempStr.substr(0, index));
            //Look for more separating characters in the remaining string.
            tempStr = tempStr.substr(index + 1, tempStr.size() - index);
            index = tempStr.find_first_of(separator);
        }
        else
        {
            //If the separator is not inside of the given delimiters
            if(!isInsideDelimeters(tempStr, index, leftDelim, rightDelim))
            {
                //Take the sub string before the separator and store it.
                substrings.push_back(tempStr.substr(0, index));
                tempStr = tempStr.substr(index + 1, tempStr.size() - index);
                index = tempStr.find_first_of(separator);
            }
            else
                index = tempStr.find_first_of(separator, index + 1);
        }
    }

    //Add the last part of the string
    substrings.push_back(tempStr);

    return substrings;
}

void trimWhiteSpace(std::string& str)
{
    if(str.size() > 0)
    {
        std::string whiteSpace(" \t\n\r\f\v");

        str.erase(0, str.find_first_not_of(whiteSpace));
        str.erase(str.find_last_not_of(whiteSpace) + 1);
    }
}

void removeOuterNonDigitCharacters(std::string& str)
{

    if(str.size() > 0)
    {
        unsigned int lettersToErase = 0;

        while(lettersToErase < str.size() && !(std::isdigit(str[lettersToErase])) )
            ++lettersToErase;

        str.erase(0, lettersToErase);

        unsigned int firstNonDigitCharacter = str.size() - 1;

        while( firstNonDigitCharacter >= 0 && !(std::isdigit(str[firstNonDigitCharacter])) )
            --firstNonDigitCharacter;
        ++firstNonDigitCharacter;

        if(str.size() > 0)
            str.erase(firstNonDigitCharacter);
    }
}

bool isInsideDelimeters(const std::string& str, int charPos, char leftDelim, char rightDelim)
{
    bool inside = true;
    auto temp = charPos;
    char separator = str[charPos];

    //Look to the left for the delimiter or another separator

    --charPos;
    while(charPos >= 0 && str[charPos] != leftDelim && str[charPos] != separator)
    {
       // std::cout << str.size() << " : " << charPos << "\n";
        --charPos;
    }

    if(charPos < 0 || str[charPos] == separator)
        inside = false;
    else
    {
        charPos = temp + 1;
        //Cast to an int to suppress the warning
        while(charPos < (int)(str.size()) && str[charPos] != rightDelim)
            ++charPos;

        if(charPos >= (int)(str.size()))
            inside = false;
    }

    return inside;
}

void removeOuterSymbols(std::string& str, char leftSymbol, char rightSymbol)
{
   // std::cout << "Before String " << str << "\nLeft Symbol " << leftSymbol << "\nRight Symbol " << rightSymbol << "\n";
    if(str.size() >= 1)
    {
        trimWhiteSpace(str);
        unsigned int leftIndex = 0;
        unsigned int rightIndex = str.size() - 1;

        if(str[leftIndex] == leftSymbol)
            ++leftIndex;
        if(str[rightIndex] == rightSymbol)
            --rightIndex;

        if(str.size() > 2 && leftIndex != std::string::npos)
            str = str.substr((leftIndex), rightIndex - (leftIndex- 1));
    }
   // std::cout << "After String " << str << "\n";
}

bool stringToBool(const std::string& str)
{
    std::string temp = str;
    for( auto& ch : temp )
        ch = std::tolower(ch);

    if( str == "true" || fromString<int>(str) == 1)
        return true;
    return false;
}

//Used so the entire string is put into the stringstream instead of up to the first whitespace character
std::stringstream& operator<<(std::stringstream& out, const std::string& str)
{
    out.str(str);
    return out;
}

std::stringstream& operator>>(std::stringstream& in, std::string& str)
{
    str = in.str();
    return in;
}

/*
std::stringstream& operator>>(std::stringstream& in, bool value)
{
    std::string temp(in.str());

    for( auto& ch : temp )
        ch = std::tolower(ch);

    if( temp == "true" || fromString<int>(temp) > 0 )
        value = true;
    else
        value = false;
}*/
