#include "utils.hpp"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <limits.h>

std::string Utils::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_r(&currentTime, &tm);

    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    oss << "." << std::setfill('0') << std::setw(3) << milliseconds.count();

    // format: YYYYMMDD_HHMMSS.sss

    return oss.str();
}

std::string Utils::GetWorkingDir()
{
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    std::string path(buf);

    return path;
}