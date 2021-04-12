#ifndef LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H
#define LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H
#define MUST_BE_DIRECTORY 0
#define MUST_BE_FILE 1

#include<string>
#include <utility>
#include <vector>
#include <filesystem>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <stack>
#include <cassert>
#include <iostream>
#include <map>
#include "MultipleLongestCommonSubstring.h"

/**
 * Checks that a path is valid
 * and that the referenced obejct is a file or a directory
 * @param path String path to verify
 * @param dir_or_file MUST_BE_DIRECTORY or MUST_BE_FILE
 */
void CheckPathExist(const std::string &path, uint8_t dir_or_file) {
    struct stat buffer{};
    int res = stat(path.c_str(), &buffer);
    if (res != 0 || (dir_or_file == MUST_BE_DIRECTORY && !S_ISDIR(buffer.st_mode)) ||
        (dir_or_file == MUST_BE_FILE && !S_ISREG(buffer.st_mode))) {
        std::cout << "[!] Invalid path : " << path << std::endl;
        exit(1);
    }
}

/**
 * This class uses MultipleLongestCommonSubstring
 * to find the lognest common substrings in multiple files, from a given directory.
 */
class FileLCSWrapper final {
private:
    std::string _path_to_dir{};
    std::vector<std::string> _paths_to_files{};
    std::vector<std::vector<int>> _lst_of_binaries{};
    std::tuple<int, std::vector<std::map<int, int>>> _results{};

    /**
     * List all files in directory and save their paths in _paths_to_files
     * If the list is empty, exit with error.
     */
    void ReadFilesInDirectory() {
        for (const auto &file : std::filesystem::directory_iterator(_path_to_dir)) {
            _paths_to_files.push_back(file.path());
            ReadFile(file.path());
        }
        if (_paths_to_files.size() < 2) {
            std::cout << "[!] The directory should contain at least two files." << std::endl;
            exit(1);
        }
    }

    /**
     * Saves the content of a file in a vector of int and stores it in _lst_of_binaries
     * @param path_to_file
     */
    void ReadFile(const std::string &path_to_file) {
        CheckPathExist(path_to_file, MUST_BE_FILE);
        std::ifstream testFile(path_to_file, std::ios::binary);
        std::vector<int, std::allocator<int>> fileContents;
        const unsigned long fileSize = std::filesystem::file_size(path_to_file);
        fileContents.reserve(fileSize);
        fileContents.assign(std::istreambuf_iterator<char>(testFile),
                            std::istreambuf_iterator<char>());
        _lst_of_binaries.push_back(fileContents);
    }

    /**
     * Runs the research of the longest substrings
     */
    void PerformSearchOfLongestCommonSubstrings() {
        MultipleLongestCommonSubstr<int> obj(_lst_of_binaries);
        _results = obj.GetResultsStats();
    }


    void DisplayResults()  {
        int length = std::get<0>(_results);
        if (length == 0) {
            std::cout << "No longst common strand of bytes found !" << std::endl;
            exit(0);
        }
        std::vector<std::map<int, int>> different_results = std::get<1>(_results);
        for (const std::map<int, int> &map_file_num_offset : different_results) {
            std::cout << "Longest common strand of bytes found : \n\tLength : " << length << "\n\tPresent in files :";
            for (const auto&[file_num, offset] : map_file_num_offset) {
                std::cout << "\n\t\t" << _paths_to_files[file_num] << " - Offset (# bytes) : " << offset << " (0x"
                          << std::hex << offset << ")." << std::dec;
            }
            std::cout << "\n" << std::endl;
        }
    }

public:
    explicit FileLCSWrapper(const std::string &pathToDir) {
        std::string abs_path = std::filesystem::absolute(pathToDir);
        CheckPathExist(abs_path, MUST_BE_DIRECTORY);
        _path_to_dir = abs_path;
    }

    /**
     * Main method
     */
    void RunAndDisplay() {
        ReadFilesInDirectory();
        PerformSearchOfLongestCommonSubstrings();
        DisplayResults();
    }
};

#endif //LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H
