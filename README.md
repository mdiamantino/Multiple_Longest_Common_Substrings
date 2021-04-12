# Linear Multiple Longest Common Substring ( C++ implementation )

## About

* **Problem :** Given at least two strings, find the longest common substring(s) - Note: there can be many of same
  length.
* **This solution** has a O(n) time complexity. This is achieved via suffix arrays, longest common prefix arrays and
  sliding windows.
* **Clarifications :** The project can be executed as:
    * Command line program : to find the longest common strand of bytes between *n* files in a specified directory;
    * An API to use the algorithm with any vector of strings (or vector of characters, or integers).

## Installation

### Requirements

* CMake (version >= 3.16) : [Installing CMake](https://cmake.org/install/)
* C++ compiler (g++ version >= 9.3.0)

### Installation & Compilation (on Unix)

1. Clone the repo
   ```sh
   $ git clone https://github.com/mdiamantino/Multiple_Longest_Common_Substrings.git
   ```
2. Enter the directory of the cloned repository
   ```sh
   $ cd Multiple_Longest_Common_Substrings/
   ```
3. Run CMake
   ```sh
   $ ccmake . 
   ```
4. Compile
   ```sh
   $ make
   ```

## Usage

### As Command Line Program

* **Prerequisites :**
    * A valid path to a non empty directory
        * The directory must contain **at least two files.**
        * The directory cannot contain subdirectories.

* **Usage:** `./longest_common_substring <path_to_directory>`
* **Example:**
   ```sh
   $ ./longest_common_substring samples/
   ```

### Via API

* **Prerequisites :**
    * Import the main prgram into your application:
    ```c++
       #include "MultipleLongestCommonSubstring.h"
    ```
    * A valid vector of strands of chars or int:
      Type `std::vector<std::vector<T>>`
* **Usage:**
    1. Instantiate an object of the class *MultipleLongestCommonSubstring* by passing the vector (mentioned above) to
       the constructor.
    ```c++
       MultipleLongestCommonSubstr<int> obj(_lst_of_binaries);
    ```
    2. Query the results of the longest common substrings:
    ```c++
       std::tuple<int, std::vector<std::map<int, int>>> results = obj.GetResultsStats();
    ```
  **Note:** Results are converted in a better form : `tuple(length, vector[ res1, res2, ... ])` 
   * where res is of the form: `hashmap[(file1_containing_substring, offset_in_file1), (file2_containing_substring, offset_in_file2), ...]`.

### Algorithm
1. Combine all strings in one strings and use separators (sentinels).
2. Compute the suffix array
3. Compute the longest common prefix array
4. Use a sliding window to capture at least K >= 2 suffixes
5. Use the longest common prefix array to retrieve the longest substring

**Based on paper :** [Computing Longest Common Substrings Via Suffix Arrays](https://link.springer.com/chapter/10.1007/978-3-540-79709-8_10)

## License

[Apache License, Version 2.0](https://github.com/mdiamantino/Multiple_Longest_Common_Substrings/blob/master/LICENSE)
