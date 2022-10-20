/**
 * File: soundex.h
 *
 * This is just here to communicate the prototype 
 * information for the functions in soundex.cpp that 
 * will be called from main.cpp
 */
#pragma once
#include <string>

void soundexSearch(std::string filepath);
std::string soundex(std::string s);
std::string removeNonLetters(std::string s);
std::string lettersToNumbers(std::string s);
std::string mergeAdjacentRepetitiveNumbers(std::string s);
std::string deleteZero(std::string s);
void replaceFirstNumber(std::string & differentNumers, std::string s);
std::string makeSureFourNumbers(std::string s);
