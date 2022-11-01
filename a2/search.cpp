// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


// 函数 cleanToken 接收一个在正文文本中出现的字符串作为参数，返回一个“纯净版”的字符串 Token，
//以便后续以优雅的形式存放到索引中。所谓“纯净版”，就是去除字符串首尾的标点，并将所有字母转换为小写.
//使用ispunct()函数来判断某个字符是否是一个标点符号。
//使用isalpha()函数来判断某个字符是否是一个字母。
string cleanToken(string s)
{

    if (s == ""){
        return "";
    }
    //从前往后数，再从后往前数，得到前面和后面的标点符号的数量，然后使用substr()函数，对s进行替换
    int countToken = 0;
    while(true){
       if(!ispunct(s[countToken])){
           break;
       };
       if(countToken == s.length() - 1){
           break;
       }
       countToken++;
    }

    s = s.substr(countToken);


    countToken = s.size() - 1;
    while(true){
       if(!ispunct(s[countToken])){
           break;
       };
       if(countToken == 0){
           break;
       }
       countToken--;
    }

    s = s.substr(0, countToken + 1);


    //确认字符至少包含一个字母，用isalpha()函数来实现检验
    //用《子弹列车》中的一句话，本来是要杀你的，但是你给我一个不杀你的理由，我就不杀你
    //本来要return "", 但是字符串中有alpha，就不用返回空字符("")
    bool flagAlpha = true;

    for(int i = 0; i < s.length(); i++){
        if(isalpha(s[i])){
            flagAlpha = false;
            break;
        }
    }
    if(flagAlpha){
        return "";
    }

    s = toLowerCase(s);

    return s;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> textSplit = stringSplit(text, " ");

    for(string item : textSplit){
        item = cleanToken(item);
        tokens.add(item);
    }
    tokens.remove("");
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    //首先，实现正向索引
    Map<string, Set<string>> forwardDirection;
    Set<string> valueInForwardDirection;
    //循环lines, 输入到forwardDirection
    int returnValue = 0;
    for(int i = 0; i < lines.size() - 1; ){
        valueInForwardDirection = stringToSet(lines[i+1]);
        forwardDirection.put(lines[i], valueInForwardDirection);
        returnValue++;
        i = i + 2;
    }

    //实现反向索引
    Vector<Set<string>> valueCopy = forwardDirection.values();
    Set<string> wordValueCopy;
    for(int i = 0; i < valueCopy.size(); i++){
        wordValueCopy += valueCopy[i];
    }

    Set<string> valueInBackDirection;
    for(string item : wordValueCopy){
        for(string key : forwardDirection){
            if(forwardDirection[key].contains(item)){
                valueInBackDirection.add(key);
            }
        }
        index.put(item, valueInBackDirection);
        valueInBackDirection.clear();
    }

    return returnValue;
}

Set<string> stringToSet(string stringInlines)
{
    return gatherTokens(stringInlines);
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> querySplit = stringSplit(query, " ");
//    Set<string> analyzeQuery;      //对query 解析
//    for(string item : querySplit){
//        analyzeQuery.add(item);
//    }

    for(string item : querySplit){
        if(item.find("+") != string::npos ){
            string temp = item.substr(1);
            if(index.containsKey(temp)){
                result = result * index[temp];
            }
        }
        else if(item.find("-") != string::npos ){
            string temp = item.substr(1);
            if(index.containsKey(temp)){
                result = result.difference(index[temp]);
            }
        }
        else{
            if(index.containsKey(item)){
                result = result.unionWith(index[item]);
            }
        }
    }

    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;
//    int buildIndex(string dbfile, Map<string, Set<string>>& index);
    int numberOfWebsites = buildIndex(dbfile, index);
    int numberOfUniqueterms = index.size();
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << numberOfWebsites << " pages containing " << numberOfUniqueterms << " unique terms " << endl << endl;

    string query = getLine("Enter query sentence (RETURN/ENTER to quit):");
    while(query != ""){
        Set<string> queryCorrespondingResult = findQueryMatches(index, query);
        cout << "Found " << queryCorrespondingResult.size() << " matching pages" << endl;
        cout << queryCorrespondingResult << endl << endl;
        query = getLine("Enter query sentence (RETURN/ENTER to quit):");
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

STUDENT_TEST("cleanToken on null strings"){
    EXPECT_EQUAL(cleanToken(""), "");
}

STUDENT_TEST("cleanToken on strings with some punctuation at one boundary and middle"){
    EXPECT_EQUAL(cleanToken("/hel*lo"), "hel*lo");
    EXPECT_EQUAL(cleanToken("woR*LD!"), "wor*ld");
}

STUDENT_TEST("cleanToken on strings with some punctuation at both boundary and middle"){
    EXPECT_EQUAL(cleanToken("/hel*lo**************"), "hel*lo");
    EXPECT_EQUAL(cleanToken("*************woR*LD!"), "wor*ld");
}

STUDENT_TEST("cleanToken on strings with single punctuation"){
    EXPECT_EQUAL(cleanToken("/"), "");
}

STUDENT_TEST("cleanToken on strings with single letter"){
    EXPECT_EQUAL(cleanToken("D"), "d");
}

STUDENT_TEST("cleanToken on strings with single letter"){
    EXPECT_EQUAL(cleanToken("+D"), "d");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("/ _love_ CS*106B!"), expected);
}

STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("////// _love_ CS*106B!"), expected);
}

STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I /// CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
