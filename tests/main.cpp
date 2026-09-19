#include "universitySystem/UniversitySystem.h"
#include "universitySystem/LMSConsole.h"

int main()
{
    UniversitySystem system;
    LMSConsole console(system);
    console.run();
    return 0;
}
