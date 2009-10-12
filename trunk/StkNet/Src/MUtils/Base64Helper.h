////////////////////////////////////////////////////////////////////////////////
// General utilities : Base64 encode/decode helper class
//
// Copyright (c) 2003 by Morning
// http://morningspace.51.net
// mailto:moyingzz@etang.com
//
// Permission to use, copy, modify, distribute and sell this program for any 
// purpose is hereby granted without fee, provided that the above copyright 
// notice appear in all copies and that both that copyright notice and this 
// permission notice appear in supporting documentation.
//
// It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _BASE64HELPER_H_
#define _BASE64HELPER_H_
//
#include <string>
//
namespace MUtils {

class Base64Helper
{
public:

    // convert from Base64 to ANSI
    static std::string encode(const std::string in_str);

    // convert from ANSI to Base64
    static std::string decode(const std::string in_str);

private:

    // encode table
    const static std::string _base64_encode_chars;
 
    // decode table
    const static char _base64_decode_chars[128];
};

} // namespace MUtils

#endif  // _BASE64HELPER_H_