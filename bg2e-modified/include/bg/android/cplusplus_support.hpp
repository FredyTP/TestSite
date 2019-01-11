#ifndef _bg2e_android_cplusplus_support_hpp_
#define _bg2e_android_cplusplus_support_hpp_

#include <bg/platform.hpp>

#include <string>
#include <sstream>


#if BG2E_ANDROID==1

namespace std {

template <typename T>
std::string to_string(const T& value)
{
    std::ostringstream os ;
    os << value ;
    return os.str();
}

}

#endif

#endif