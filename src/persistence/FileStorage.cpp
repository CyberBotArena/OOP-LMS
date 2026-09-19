#include "persistence/FileStorage.h"

#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;


// Constructor
FileStorage::FileStorage(
    const string& path)
    : base_path(path)
{
}


// Save data
void FileStorage::saveAll()
{
    string file_path =
        base_path + "/system_data.txt";

    ofstream output_file(file_path);

    if (!output_file.is_open())
    {
        throw runtime_error(
            "Could not open the file for saving."
        );
    }

    output_file << "University system data\n";
    output_file << "Data saved successfully\n";

    output_file.close();
}


// Load data
void FileStorage::loadAll()
{
    string file_path =
        base_path + "/system_data.txt";

    ifstream input_file(file_path);

    if (!input_file.is_open())
    {
        throw runtime_error(
            "Could not open the file for loading."
        );
    }

    string line;

    while (getline(input_file, line))
    {
        // Reading each stored line
    }

    input_file.close();
}


// Return read-only base path
const string& FileStorage::getBasePath() const
{
    return base_path;
}