////////////////////////////////////////////////////////////////////////////////
// Simple Mail Sender (Interface)
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

#ifndef _SMAILER_H_
#define _SMAILER_H_

#pragma warning( disable : 4786 )	// warning C4786: identifier was truncated to '255' characters in the browser information

//
#include <string>
#include <vector>
#include <map>
// #include <exception>
// #include <winsock2.h>
//
namespace SMailer {

// class MimeContent(Abstract class)
/////////////////////////////////////

class MimeContent
{
public:

    MimeContent(const std::string content = "");

    virtual std::string  getType() const = 0;
    virtual std::string  getDisposition() const;
    virtual std::string  getTransEncoding() const = 0;
    virtual std::string& getContent() = 0;

protected:

    std::string _content;
};

typedef std::vector<MimeContent*> MimeContents;

// class PlainTextContent
/////////////////////////////////////

class TextPlainContent : public MimeContent
{
public:

    TextPlainContent(const std::string content, 
                     const std::string charset = "gb2312");

    virtual std::string  getType() const;
    virtual std::string  getTransEncoding() const;
    virtual std::string& getContent();

private:

    std::string _charset;
};

// class TextHtmlContent
/////////////////////////////////////

class TextHtmlContent : public MimeContent
{
public:

    TextHtmlContent(const std::string content, 
                    const std::string charset = "gb2312");

    virtual std::string  getType() const;
    virtual std::string  getTransEncoding() const;
    virtual std::string& getContent();

private:

    std::string _charset;
};

// class AppOctStrmContent
/////////////////////////////////////

class AppOctStrmContent : public MimeContent
{
public:

    AppOctStrmContent(const std::string file_name);

    virtual std::string  getType() const;
    virtual std::string  getDisposition() const;
    virtual std::string  getTransEncoding() const;
    virtual std::string& getContent();

private:

    std::string _file_name;
    std::string _name;
};

// class Priority(Helper class)
/////////////////////////////////////

class Priority
{
public:

    static const std::string important;
    static const std::string normal;
    static const std::string trivial;
};

// class MailInfo
/////////////////////////////////////

typedef std::multimap<std::string, std::string> Receivers;

class MailInfo
{
public:

    MailInfo();

    void setSenderName(const std::string name);
    void setSenderAddress(const std::string address);
    std::string getSenderName() const;
    std::string getSenderAddress() const;

    void addReceiver(const std::string name, const std::string address);
    void setReceiver(const std::string name, const std::string address);
    const Receivers& getReceivers() const;

    void setPriority(std::string priority);
    std::string getPriority() const;

    void setSubject(const std::string subject);
    std::string getSubject() const;

    void addMimeContent(MimeContent* content);
    void clearMimeContents();
    const MimeContents& getMimeContents() const;

private:

    std::string  _sender_name;
    std::string  _sender_address;
    Receivers    _receivers;
    std::string  _priority;
    std::string  _subject;
    MimeContents _contents;
};

// class MailWrapper
/////////////////////////////////////

class MailWrapper
{
public:

    MailWrapper(MailInfo* mail_info);

    std::string getSenderAddress();
    std::string getHeader();
    std::string getEnd();

    void traverseReceiver();
    bool hasMoreReceiver();
    std::string nextReceiverAddress();

    void traverseContent();
    bool hasMoreContent();
    std::string& nextContent();

private:

    static const std::string _mailer_name;
    static const std::string _boundary;

    MailInfo* _mail_info;
    Receivers::const_iterator _rcv_itr;

    std::string _content;
    MimeContents::const_iterator _con_itr;

    std::string prepareFrom();
    std::string prepareTo();
    std::string prepareDate();
    std::string prepareName(const std::string raw_name);
};

// class MailSender
/////////////////////////////////////

class MailSender
{
public:

    MailSender(const std::string server_name, 
               const std::string user_name = "", 
               const std::string user_pwd = "");

    ~MailSender();

    void setMail(MailWrapper* mail);

    bool sendMail();
    
    const char * getErrMessage()
    {
    	return _err_message.c_str();
    }

private:

    enum {SERVICE_PORT = 25};

    enum Operaion
    {
        send_helo_cmd, 
        send_auth_cmd, 
        send_username, 
        send_password, 
        send_mail_cmd, 
        send_rcpt_cmd, 
        send_data_cmd, 
        send_header, 
        send_content, 
        send_end, 
        send_quit_cmd, 
    };

    std::string _server_name;
    std::string _user_name;
    std::string _user_pwd;
    SOCKET _socket;

    MailWrapper* _mail;
    
    std::string _err_message;

    std::string getSenderAddress() const;

    bool conn();
    bool hello();
    bool login();

    bool sendHeader();
    bool sendContent();
    bool sendEnd();

    bool quit();

    bool sendRequest(Operaion operation, const std::string content);
    bool rcvResponse(const std::string expected_response);

    friend class ErrorMessage;
};

// class ErrorMessage(Helper class)
/////////////////////////////////////

class ErrorMessage
{
public:

    static ErrorMessage& getInstance();

    std::string request (MailSender::Operaion request_operation);
    std::string response(const std::string expected_response);

private:

    std::map<MailSender::Operaion, std::string> _request_errmsg_map;
    std::map<std::string, std::string> _respons_errmsg_map;

    ErrorMessage();
};

// class MailException(Helper class)
/////////////////////////////////////

class MailException : public std::exception
{

public:
    MailException(std::string message = "")
     : _message(message)
    {
    }

	~MailException() { }

    const char *what() const
    {
        return _message.c_str();
    }

private:

    std::string _message;
};

} // namespace SMailer

#endif // _SMAILER_H_
