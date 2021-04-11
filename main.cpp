#include <vector>
#include "FileLCSWrapper.h"

int main() {

    std::vector<std::vector<char>> texts;

    std::string w1 = "AABC";
    std::vector<char> word1(w1.begin(), w1.end());
    texts.push_back(word1);

    std::string w2 = "BCDC";
    std::vector<char> word2(w2.begin(), w2.end());
    texts.push_back(word2);

    std::string w3 = "BCDE";
    std::vector<char> word3(w3.begin(), w3.end());
    texts.push_back(word3);

    std::string w4 = "CDED";
    std::vector<char> word4(w4.begin(), w4.end());
    texts.push_back(word4);

//    MultipleLongestCommonSubstr obj(texts);
//    obj.PrintResults();

    FileLCSWrapper flcsw("/home/mdc/CLionProjects/logest_common_substring/samples");
    return 0;
}