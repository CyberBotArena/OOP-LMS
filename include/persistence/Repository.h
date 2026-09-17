#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T>
class Repository {
private:
    std::map<std::string, T*> items;

public:
    void add(
        const std::string& id,
        T* item)
    {
        if (item == nullptr)
        {
            throw std::invalid_argument(
                "Cannot add a null item."
            );
        }

        if (items.find(id) != items.end())
        {
            throw std::runtime_error(
                "ID already exists."
            );
        }

        items.insert({id, item});
    }


    T* find(const std::string& id) const
    {
        auto position = items.find(id);

        if (position == items.end())
        {
            throw std::runtime_error(
                "Item was not found."
            );
        }

        return position->second;
    }


    void remove(const std::string& id)
    {
        auto position = items.find(id);

        if (position == items.end())
        {
            throw std::runtime_error(
                "Item was not found."
            );
        }

        items.erase(position);
    }


    std::vector<T*> getAll() const
    {
        std::vector<T*> result;

        for (const auto& item : items)
        {
            result.push_back(item.second);
        }

        return result;
    }
};

#endif