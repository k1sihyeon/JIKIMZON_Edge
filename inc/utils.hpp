#include <string>

class Utils {
public:
    Utils() = default;
    ~Utils() = default;

    std::string GetCurrentTime();
    std::string GetWorkingDir();
};