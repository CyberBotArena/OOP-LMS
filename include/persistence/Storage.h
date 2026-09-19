#ifndef STORAGE_H
#define STORAGE_H

class UniversitySystem;


class Storage {
public:
    virtual void saveAll(UniversitySystem& system) = 0;

    virtual void loadAll(UniversitySystem& system) = 0;

    virtual ~Storage() = default;
};

#endif