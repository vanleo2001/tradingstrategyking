////////////////////////////////////////////////////////////////////////////////
// General utilities : WinSock helper classes
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

#ifndef _WINSOCK_HELPER_H_
#define _WINSOCK_HELPER_H_
//
#include <exception>
#include <string>
// #include <winsock2.h>
//
namespace MUtils {

class WinSockException : public std::exception
{
public:

    WinSockException(const std::string message = "")
     : _message(message)
    {
    }

    const char *what() const throw ()
    {
        return _message.c_str();
    }

private:

    std::string _message;
};

class WinSockHelper
{
public:

    WinSockHelper()
    {
        WSADATA wsaData;
        int result = WSAStartup(0x0002, &wsaData);

        if ( result != 0 )
        {
            throw WinSockException("WSAStartup error");
        }

        if ( wsaData.wVersion != 0x0002)
        {
            WSACleanup( );
            throw WinSockException("Winsock version not support");
        }
    }

    ~WinSockHelper() throw()
    {
        WSACleanup();
    }
};

} // namespace MUtils

#endif // _WINSOCK_HELPER_H_