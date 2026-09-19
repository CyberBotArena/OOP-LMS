#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <string>

#include "persistence/Storage.h"

class FileStorage : public Storage {
private:
    std::string base_path;

public:
    explicit FileStorage(
        const std::string& path
    );

    void saveAll() override;

    void loadAll() override;

    const std::string& getBasePath() const;
};

#endif