/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "soundex.h"
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    bool flag = true;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
            flag = false;
        }
    }
    if(!flag) {
        return result;
    }
    else{
        return "";
    }
}

/*This function is intended to return a string which
 * contains only the number characters from the input string.
 * (all letter are convertd to number.)
 */
string lettersToNumbers(string s) {
    if(s.empty()) {
        return "";
    }
    else {
        string result = "";

        for(int i = 0; i < s.length(); i++) {
            switch(toupper(s[i])) {
                case 'A': case 'E': case 'I': case 'O':
                case 'U': case 'H': case 'W': case 'Y':
                    result += "0";
                    break;
                case 'B': case 'F': case 'P': case 'V':
                    result += "1";
                    break;
                case 'C': case 'G': case 'J': case 'K':
                case 'Q': case 'S': case 'X': case 'Z':
                    result += "2";
                    break;
                case 'D': case 'T':
                    result += "3";
                    break;
                case 'L':
                    result += '4';
                    break;
                case 'M': case 'N':
                    result += '5';
                    break;
                case 'R':
                    result += '6';
                    break;
            }
        }
        return result;
    }
}

/*This function is intended to return a string which
 * merge adjacent repetitive numbers.
 * (repetitive numbers are merge to one number.)
 */
string mergeAdjacentRepetitiveNumbers(string s) {

    if(s.empty()) {
        return "";
    }

    string result = "";
    for(int i = 0; i < s.length(); i++) {
        if (s[i] != s[i+1]) {
            result += charToString(s[i]);
        }
        else {
            int flag = i;
            int j = 1;
            while(s[flag] == s[flag + j]) {
                j++;
            }
            result += charToString(s[flag + j - 1]);
            i = flag + j - 1;
        }
    }
    return result;
}

/*This function is intended to replace the first number of differentNumbers
 * with the capital first letter of inputing string s.
 */
void replaceFirstNumber(string &differentNumers, string s) {
    differentNumers[0] = toupper(s[0]);
}

/*This function is intended to return a string which
 * delete '0'.
 */
string deleteZero(string s) {

    if(s == ""){
        return "";
    }

    string restult = "";
    for(int i = 0; i < s.size(); i++) {
        if(s.at(i) != '0') {
            restult += s.at(i);
        }
    }

    return restult;
}
/* This function is intended to return a string which
 * are converted by inputing string to soundex code.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    string pureNumbers = lettersToNumbers(removeNonLetters(s));
    string differentNumbers = mergeAdjacentRepetitiveNumbers(pureNumbers);
    replaceFirstNumber(differentNumbers, s);
    string noZeroNumber = deleteZero(differentNumbers);
    string result = makeSureFourNumbers(noZeroNumber);
    return result;
}




/* This function is intended to search the database for all surnames with the same encoding.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl << endl;;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    string surname;
    cout << "Enter a surname (RETURN to quit):";

    Vector<string> sameSoundexName;

    while(getline(cin, surname)){
        if(surname == ""){
            break;
        }
        string soundexCode = soundex(surname);
        cout << "Soundex code is " << soundexCode << endl;
        for(auto i : databaseNames) {
            if(soundex(i) == soundexCode) {
                sameSoundexName.add(i);
            }
        }

        sameSoundexName.sort();
        cout << "Matches from database: " << sameSoundexName << endl << endl;
        sameSoundexName.clear();
        cout << "Enter a surname (RETURN to quit):";
    }

    cout << "All down!" << endl;

}

/* This function is intended to make sure that the size of inputing string equal four.
 */
string makeSureFourNumbers(string s) {

    if(s == ""){
        return "";
    }
    if(s.size() == 4) {
        return s;
    }
    else{
        if(s.size() < 4){
            for(int i = s.size(); i < 4; i++){
                s.append("0");
            }
        }
        else{
            s = s.substr(0,4);
        }
    }
    return s;
}
/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here


STUDENT_TEST("Test null string and string without letter.(Edge Case.)") {

    string s = "";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "1239";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");

}

// Test lettersToNumbers.
STUDENT_TEST("We want to know wheather the lettersToNumbers give us a good result.") {
    string s = "OConner";
    string result = lettersToNumbers(s);
    EXPECT_EQUAL(result, "0205506");
    s = "Angelou";
    result = lettersToNumbers(s);
    EXPECT_EQUAL(result, "0520400");
}

STUDENT_TEST("lettersToNumbers: Edge Case.") {
    string s = "";
    string result = lettersToNumbers(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Test mergeAdjacentRepetitiveNumbers:.") {
    string s = "0205506";
    string result = mergeAdjacentRepetitiveNumbers(s);
    EXPECT_EQUAL(result, "020506");
    s = "000123555";
    result = mergeAdjacentRepetitiveNumbers(s);
    EXPECT_EQUAL(result, "01235");
    s = "";
    result = mergeAdjacentRepetitiveNumbers(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("Test deleteZero.") {
    string s = "C0205506";
    string result = deleteZero(s);
    EXPECT_EQUAL(result, "C2556");
    s = "C000123555";
    result = deleteZero(s);
    EXPECT_EQUAL(result, "C123555");
    s = "";
    result = deleteZero(s);
    EXPECT_EQUAL(result, "");
    s = "C00000";
    result = deleteZero(s);
    EXPECT_EQUAL(result, "C");
}

STUDENT_TEST("Test makeSureFourNumbers.") {
    string s = "C0205506";
    string result = makeSureFourNumbers(s);
    EXPECT_EQUAL(result, "C020");
    s = "C";
    result = makeSureFourNumbers(s);
    EXPECT_EQUAL(result, "C000");
    s = "";
    result = makeSureFourNumbers(s);
    EXPECT_EQUAL(result, "");
    s = "C000";
    result = makeSureFourNumbers(s);
    EXPECT_EQUAL(result, "C000");
}
