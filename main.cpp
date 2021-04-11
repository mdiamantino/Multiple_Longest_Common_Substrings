#include <chrono>
#include "FileLCSWrapper.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        if (argv[0])
            std::cout << "Usage: " << argv[0] << " <path_to_directory>" << '\n';
        else
            std::cout << "Usage: <program name> <path_to_directory>" << '\n';
        return 1;
    }
    std::string path = argv[1];
    FileLCSWrapper flcsw(path);
    auto start = std::chrono::high_resolution_clock::now();
    flcsw.RunAndDisplay();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Executed in : " << duration.count() << "ms" << std::endl;
    return 0;
}