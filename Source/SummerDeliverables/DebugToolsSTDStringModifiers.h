
#pragma once

#include <string>

namespace std
{
    inline std::string to_string(const FVector& v)
    {
        std::string s = "vec4(";
        s += std::to_string(v.X) + ", ";
        s += std::to_string(v.Y) + ", ";
        s += std::to_string(v.Z) + ")";
        return s;
    }

    inline std::string to_string(const FString s)
    {
        return std::string(TCHAR_TO_UTF8(*s));
    }
}