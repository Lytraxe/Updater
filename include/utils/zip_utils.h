#ifndef ZIP_UTILS_H
#define ZIP_UTILS_H

#include <zip.h>
#include <string>

/**
 * Read a text file from the archive
 * Returns -1 if an error occured or the file couldn't be found inside archive
*/
short readFromJar(const char* jarPath, const char* file, std::string& output);

#endif