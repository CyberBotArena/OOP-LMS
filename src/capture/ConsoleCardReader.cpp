
#include "capture/ConsoleCardReader.h"

#include <iostream>
#include <string>

std::string ConsoleCardReader::readCard()
{
    std::string cardId;

    std::cout << "Tap card (enter Student ID, or END to finish): ";
    std::getline(std::cin, cardId);

    if (cardId == "END") {
        return "";
    }

    return cardId;
}