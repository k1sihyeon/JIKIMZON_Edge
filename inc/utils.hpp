#ifndef JIKIMZON_UTILS_H
#define JIKIMZON_UTILS_H

#include <string>

class Utils {
public:
    Utils() = default;
    ~Utils() = default;

    std::string GetCurrentTime();
    std::string GetWorkingDir();
};

#endif //JIKIMZON_UTILS_H