#include <chrono>
#include "FileLCSWrapper.h"

int main() {
    FileLCSWrapper flcsw("/home/mdc/CLionProjects/logest_common_substring/samples");
    auto start = std::chrono::high_resolution_clock::now();
    flcsw.RunAndDisplay();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Executed in : " << duration.count() << "ms" << std::endl;
    return 0;
}