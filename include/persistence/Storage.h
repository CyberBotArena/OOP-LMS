#ifndef STORAGE_H
#define STORAGE_H

class Storage {
public:
    virtual void saveAll() = 0;

    virtual void loadAll() = 0;

    virtual ~Storage() = default;
};

#endif