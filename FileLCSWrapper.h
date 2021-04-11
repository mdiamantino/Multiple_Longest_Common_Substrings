//
// Created by mdc on 11.04.21.
//

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


void CheckPathExist(const std::string &path, uint8_t dir_or_file) {
    struct stat buffer{};
    int res = stat(path.c_str(), &buffer);
    if (res != 0 || (dir_or_file == MUST_BE_DIRECTORY && !S_ISDIR(buffer.st_mode)) ||
        (dir_or_file == MUST_BE_FILE && !S_ISREG(buffer.st_mode))) {
        std::cout << "[!] Invalid path : " << path << std::endl;
        exit(1);
    }
}

class FileLCSWrapper final {
private:
    std::string _path_to_dir;
    std::vector<std::string> _paths_to_files;
    std::vector<std::vector<int>> _lst_of_binaries;
    std::tuple<int, std::vector<std::map<int, int>>> _results;

    void ReadFilesInDirectory() {
        for (const auto &file : std::filesystem::directory_iterator(_path_to_dir)) {
            _paths_to_files.push_back(file.path());
            ReadFile(file.path());
        }
        if (_paths_to_files.empty()) {
            std::cout << "[!] The directory does not contain any file." << std::endl;
            exit(1);
        }
    }

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

    void PerformSearch() {
        MultipleLongestCommonSubstr<int> obj(_lst_of_binaries);
        _results = obj.ComputeResultsStats();
    }

    void DisplayResults() const {
        int length = std::get<0>(_results);
        std::vector<std::map<int, int>> different_results = std::get<1>(_results);
        for (const std::map<int, int> &map_file_num_offset : different_results) {
            std::cout << "Longest substring found : \n\tLength : " << length << "\n\tFound in :";
            for (const auto&[file_num, offset] : map_file_num_offset) {
                std::cout << "\n\t\t" << _paths_to_files[file_num] << " - Offset (# bytes) : " << offset << " (0x"
                          << std::hex << offset << ")." << std::dec;
            }
            std::cout << std::endl;
        }
    }

public:
    explicit FileLCSWrapper(const std::string &pathToDir) {
        CheckPathExist(pathToDir, MUST_BE_DIRECTORY);
        _path_to_dir = pathToDir;
    }

    void RunAndDisplay() {
        ReadFilesInDirectory();
        PerformSearch();
        DisplayResults();
    }
};

#endif //LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H
