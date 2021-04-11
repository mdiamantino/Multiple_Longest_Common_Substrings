//
// Created by mdc on 11.04.21.
//

#ifndef LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H
#define LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H

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

class FileLCSWrapper {
private:
    std::string _path_to_dir;
    std::vector<std::string> _paths_to_files;
    std::vector<std::vector<int>> _lst_of_binaries;

    static bool IsPathExist(const std::string &s) {
        struct stat buffer{};
        return (stat(s.c_str(), &buffer) == 0);
    }

    void ReadFilesInDirectory() {
        for (const auto &file : std::filesystem::directory_iterator(_path_to_dir)) {
            _paths_to_files.push_back(file.path());
            ReadFile(file.path());
        }
    }

    void ReadFile(const std::string &path_to_file) {
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
        auto results = obj.ComputeResultsStats();
        int length = std::get<0>(results);
        std::vector<std::map<int, int>> different_results = std::get<1>(results);
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
        if (!IsPathExist(pathToDir)) {
            std::cout << "Invalid path." << std::endl;
            return;
        } else {
            _path_to_dir = pathToDir;
            ReadFilesInDirectory();
            PerformSearch();
//            ReadFile("/home/mdc/CLionProjects/logest_common_substring/samples/sample.3");
        }
    }
};

#endif //LOGEST_COMMON_SUBSTRING_FILELCSWRAPPER_H
