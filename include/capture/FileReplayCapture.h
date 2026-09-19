#ifndef FILE_REPLAY_CAPTURE_H
#define FILE_REPLAY_CAPTURE_H

#include <fstream>
#include <string>

#include "capture/AttendanceCapture.h"

class FileReplayCapture : public AttendanceCapture {
private:
    std::string filePath;
    std::ifstream* input;

public:
    explicit FileReplayCapture(
        const std::string& file_path
    );

    void beginSession(
        const AttendanceSession& session
    ) override;

    std::string captureNext() override;

    void endSession() override;

    std::string getMethodName() const override;

    ~FileReplayCapture() override;

    FileReplayCapture(
        const FileReplayCapture&
    ) = delete;

    FileReplayCapture& operator=(
        const FileReplayCapture&
    ) = delete;
};

#endif