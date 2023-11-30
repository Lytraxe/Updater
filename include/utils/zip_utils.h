#ifndef ZIP_UTILS_H
#define ZIP_UTILS_H

#include <zip.h>
#include <string>

short readFromJar(const char* jarPath, const char* file, std::string& output);

#endif