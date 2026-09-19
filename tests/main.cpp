#include "persistence/FileStorage.h"
#include "universitySystem/UniversitySystem.h"
#include "universitySystem/LMSConsole.h"

int main()
{
    FileStorage storage("data");

    UniversitySystem system(&storage);

    LMSConsole console(system);

    console.run();

    system.save();

    return 0;
}