#include "../../include/utils/zip_utils.h"
#include <iostream>

short readFromJar(const char* jarPath, const char* file, std::string& output) {

    zip_t* archive = zip_open(jarPath, ZIP_RDONLY, NULL);
    if (!archive) {
        std::cerr << "Error opening file " << jarPath << '\n';
        return -1;
    }

    zip_int64_t fileIndex = zip_name_locate(archive, file, 0);
    if (fileIndex == -1) {
        std::cerr << "Could not find " << file << " in archive " << jarPath << '\n';
        zip_close(archive);
        return -1;
    }

    zip_file_t* compFile = zip_fopen_index(archive, fileIndex, 0);
    if (!compFile) {
        std::cerr << "Error opening " << file << " in archive " << jarPath << '\n';
        zip_close(archive);
        return -1;
    }

    char buffer[1024];
    zip_int64_t bytesRead;
    while ((bytesRead = zip_fread(compFile, &buffer, sizeof(buffer))) > 0) {
        output.append(buffer, (size_t)bytesRead);
    }
    zip_fclose(compFile);
    zip_close(archive);
    return 0;
}