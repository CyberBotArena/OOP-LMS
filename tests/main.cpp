#include "persistence/FileStorage.h"
#include "universitySystem/UniversitySystem.h"
#include "universitySystem/LMSConsole.h"

#include <iostream>
#include <exception>

using namespace std;

int main()
{
    FileStorage storage("data");

    UniversitySystem system(&storage);

    try
    {
        // Load users, courses, offerings, etc.
        system.load();

        LMSConsole console(system);

        console.run();

        // Save only after successful loading.
        system.save();
    }
    catch (const exception& e)
    {
        cerr << "LMS error: " << e.what() << endl;

        return 1;
    }

    return 0;
}