#include "../../include/utils/file_utils.h"

std::error_code renameFile(std::string& from, std::string& to) {
    std::error_code err;
    std::filesystem::rename(from, to, err);
    return err;
}

std::error_code getFiles(std::string& directory, std::vector<std::filesystem::path>& vec) {
    std::error_code err;
    for (auto path : std::filesystem::directory_iterator(directory, err)) {
        if (path.is_directory()) continue;
        if (!path.path().extension().compare(".jar")) {
            //- TODO maybe do some extra checks here instead
            vec.push_back(path.path());
        }
    }
    return err;
}
