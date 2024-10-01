#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <openssl/sha.h>
#include <iomanip>
#include <algorithm>

const size_t BUFFER_SIZE = 128 * 1024 * 1024;   // 128 MiB
const size_t MIN_BUFFER_SIZE = 4 * 1024 * 1024; // 4 MiB
std::string pre_dir;
std::string current_dir;
int day = 0;
