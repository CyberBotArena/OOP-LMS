#include "FileReplayCapture.h"

#include <stdexcept>

using namespace std;


// Constructor
FileReplayCapture::FileReplayCapture(
    const string& file_path)
    : filePath(file_path),
      input(nullptr)
{
}


// Start file replay
void FileReplayCapture::beginSession(
    const AttendanceSession& session)
{
    // Session is currently not needed
    (void)session;

    // Close an existing file first
    endSession();

    input = new ifstream(filePath);

    if (!input->is_open())
    {
        delete input;
        input = nullptr;

        throw runtime_error(
            "Could not open attendance file: " +
            filePath
        );
    }
}


// Capture the next student ID from the file
string FileReplayCapture::captureNext()
{
    if (input == nullptr)
    {
        throw runtime_error(
            "Attendance capture has not started yet."
        );
    }

    string student_ID;

    if (getline(*input, student_ID))
    {
        // Remove Windows carriage return if present
        if (!student_ID.empty() &&
            student_ID.back() == '\r')
        {
            student_ID.pop_back();
        }

        return student_ID;
    }

    // Empty string means end of capture
    return "";
}


// End file replay
void FileReplayCapture::endSession()
{
    if (input != nullptr)
    {
        input->close();

        delete input;

        input = nullptr;
    }
}


// Return capture method name
string FileReplayCapture::getMethodName() const
{
    return "File Replay";
}


// Destructor
FileReplayCapture::~FileReplayCapture()
{
    endSession();
}