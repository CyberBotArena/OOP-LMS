#ifndef CARD_TAP_CAPTURE_H
#define CARD_TAP_CAPTURE_H

#include <string>

#include "capture/AttendanceCapture.h"
#include "capture/ConsoleCardReader.h"

class CardTapCapture : public AttendanceCapture {
private:
    ConsoleCardReader reader;
    bool capturing;

public:
    CardTapCapture();

    void beginSession(
        const AttendanceSession& session
    ) override;

    std::string captureNext() override;

    void endSession() override;

    std::string getMethodName() const override;
};

#endif