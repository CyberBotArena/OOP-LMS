
#include "capture/CardTapCapture.h"

using namespace std;

CardTapCapture::CardTapCapture()
    : capturing(false)
{
}

void CardTapCapture::beginSession(
    const AttendanceSession& session)
{
    (void)session;
    capturing = true;
}

string CardTapCapture::captureNext()
{
    if (!capturing)
    {
        return "";
    }

    return reader.readCard();
}

void CardTapCapture::endSession()
{
    capturing = false;
}

string CardTapCapture::getMethodName() const
{
    return "CardTap";
}