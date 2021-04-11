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
    }

public:
    explicit FileLCSWrapper(const std::string& pathToDir) {
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
