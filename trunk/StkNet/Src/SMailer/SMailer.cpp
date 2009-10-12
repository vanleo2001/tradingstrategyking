////////////////////////////////////////////////////////////////////////////////
// Simple Mail Sender (Implementation)
// Version 0.0
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

#include "StdAfx.h"

#include "SMailer.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "../MUtils/Base64Helper.h"
#include "../MUtils/FileHelper.h"
//
namespace SMailer {

// constants defination
/////////////////////////////////////

enum {MAX_BUFFER_SIZE = 255};

const std::string Priority::important = "1";
const std::string Priority::normal    = "3";
const std::string Priority::trivial   = "5";

const std::string MailWrapper::_mailer_name = "SMailer";
const std::string MailWrapper::_boundary    = "#BOUNDARY#";

// Member functions of class MimeContent
/////////////////////////////////////

MimeContent::MimeContent(const std::string content) : _content(content)
{
}

std::string MimeContent::getDisposition() const
{
    return "";
}

// Member functions of class PlainTextContent
/////////////////////////////////////

TextPlainContent::TextPlainContent(const std::string content, 
                                   const std::string charset)
    : MimeContent(content), _charset(charset)
{
}

std::string TextPlainContent::getType() const
{
    return "text/plain; charset=" + _charset;
}

std::string TextPlainContent::getTransEncoding() const
{
    return "8bit";
}

std::string& TextPlainContent::getContent()
{
    // you can add more codes here, such as wrapping lines 
    // or replacing '\n' with '\r\n', etc.
    return _content;
}

// Member functions of class TextHtmlContent
/////////////////////////////////////

TextHtmlContent::TextHtmlContent(const std::string content, 
                                 const std::string charset)
    : MimeContent(content), _charset(charset)
{
}

std::string TextHtmlContent::getType() const
{
    return "text/html; charset=" + _charset;
}

std::string TextHtmlContent::getTransEncoding() const
{
    return "8bit";
}

std::string& TextHtmlContent::getContent()
{
    // you can add more codes here, such as wrapping lines 
    // or replacing '\n' with '\r\n', etc.
    return _content;
}

// Member functions of class AppOctStrmContent
/////////////////////////////////////

AppOctStrmContent::AppOctStrmContent(const std::string file_name)
    : _file_name(file_name)
{
    char drive[_MAX_DRIVE];
    char direc[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];
    _splitpath(file_name.c_str(), drive, direc, fname, ext);
    _name  = fname;
    _name += ext;
}

std::string AppOctStrmContent::getType() const
{
    return "application/octet-stream; name=" + _name;
}

std::string AppOctStrmContent::getDisposition() const
{
    return "attachment; filename=" + _name;
}

std::string AppOctStrmContent::getTransEncoding() const
{
    return "base64";
}

std::string& AppOctStrmContent::getContent()
{
    // you can add more codes here, such as wrapping lines 
    // or replacing '\n' with '\r\n', etc.
    MUtils::FileHelper::open(_file_name, _content);
    _content = MUtils::Base64Helper::encode(_content);
    return _content;
}

// Member functions of class MailInfo
/////////////////////////////////////

MailInfo::MailInfo() : _priority(Priority::normal)
{
}

void MailInfo::setSenderName(const std::string name)
{
    _sender_name = name;
}

void MailInfo::setSenderAddress(const std::string address)
{
    _sender_address = address;
}

std::string MailInfo::getSenderName() const
{
    return _sender_name;
}

std::string MailInfo::getSenderAddress() const
{
    return _sender_address;
}

void MailInfo::addReceiver(const std::string name, const std::string address)
{
    _receivers.insert(Receivers::value_type(name, address));
}

void MailInfo::setReceiver(const std::string name, const std::string address)
{
    _receivers.clear();
    _receivers.insert(Receivers::value_type(name, address));
}

const Receivers& MailInfo::getReceivers() const
{
    return _receivers;
}

void MailInfo::setPriority(std::string priority)
{
    _priority = priority;
}

std::string MailInfo::getPriority() const
{
    return _priority;
}

void MailInfo::setSubject(const std::string subject)
{
    _subject = subject;
}

std::string MailInfo::getSubject() const
{
    return _subject;
}

void MailInfo::addMimeContent(MimeContent* content)
{
    _contents.push_back(content);
}

void MailInfo::clearMimeContents()
{
    _contents.clear();
}

const MimeContents& MailInfo::getMimeContents() const
{
    return _contents;
}

// Member functions of class MailWrapper
/////////////////////////////////////

MailWrapper::MailWrapper(MailInfo* mail_info) : _mail_info(mail_info)
{
}

std::string MailWrapper::getSenderAddress()
{
    std::string address;

    address  = "<";
    address += _mail_info->getSenderAddress();
    address += ">";

    return address;
}

std::string MailWrapper::getHeader()
{
    std::string header;

    header  = "From: ";
    header += prepareFrom() + "\r\n";

    header += "Reply-To: ";
    header += _mail_info->getSenderAddress() + "\r\n";

    header += "To: ";
    header += prepareTo() + "\r\n";

    header += "Date: ";
    header += prepareDate() + "\r\n";

    header += "Subject: ";
    header += _mail_info->getSubject() + "\r\n";

    header += "X-Mailer: ";
    header += _mailer_name + "\r\n";

    header += "X-Priority: ";
    header += _mail_info->getPriority() + "\r\n";

    header += "MIME-Version: 1.0\r\n";
    header += "Content-type: multipart/mixed; boundary=\"";
    header += _boundary + "\"\r\n";

    header += "\r\n";

    return header;
}

void MailWrapper::traverseReceiver()
{
    _rcv_itr = _mail_info->getReceivers().begin();
}

bool MailWrapper::hasMoreReceiver()
{
    return ( _rcv_itr != _mail_info->getReceivers().end() );
}

std::string MailWrapper::nextReceiverAddress()
{
    std::string address;

    address  = "<";
    address += (_rcv_itr++)->second;
    address += ">";

    return address;
}

void MailWrapper::traverseContent()
{
    _con_itr = _mail_info->getMimeContents().begin();
}

bool MailWrapper::hasMoreContent()
{
    return ( _con_itr != _mail_info->getMimeContents().end() );
}

std::string& MailWrapper::nextContent()
{
    _content  = "--" + _boundary + "\r\n";

    _content += "Content-Type: ";
    _content += (*_con_itr)->getType() + "\r\n";

    std::string disposition = (*_con_itr)->getDisposition();
    if ( !disposition.empty() )
    {
        _content += "Content-Disposition: ";;
        _content += disposition + "\r\n";
    }

    _content += "Content-Transfer-Encoding: ";
    _content += (*_con_itr)->getTransEncoding() + "\r\n\r\n";

    _content += (*_con_itr)->getContent() + "\r\n\r\n";

    _con_itr++;

    return _content;
}

std::string MailWrapper::getEnd()
{
    std::string end;

    end += "\r\n--" + _boundary + "--\r\n";

    end += ".\r\n";

    return end;
}

std::string MailWrapper::prepareFrom()
{
    std::string from_string;

    from_string = prepareName(_mail_info->getSenderName());
    from_string += getSenderAddress();

    return from_string;
}

std::string MailWrapper::prepareTo()
{
    std::string to_string;

    traverseReceiver();
    while ( hasMoreReceiver() )
    {
        to_string += prepareName(_rcv_itr->first);
        to_string += nextReceiverAddress() + ", ";
    }

    return to_string.substr(0, to_string.length()-2);
}

std::string MailWrapper::prepareDate()
{
    char date_string[MAX_BUFFER_SIZE];

    time_t seconds;
    time(&seconds);
    strftime(date_string, MAX_BUFFER_SIZE, 
             "%a, %d %b %y %H:%M:%S +0800", 
             localtime(&seconds));          // +0800 maybe hard code

    return date_string;
}

std::string MailWrapper::prepareName(const std::string raw_name)
{
    std::string decorated_name;

    if (!raw_name.empty())
    {
        decorated_name  = "\"";
        decorated_name += raw_name;
        decorated_name += "\" ";
    }

    return decorated_name;
}

// Member functions of class MailSender
/////////////////////////////////////

MailSender::MailSender(const std::string server_name, 
                       const std::string user_name, 
                       const std::string user_pwd)
    : _server_name(server_name), _user_name(user_name), 
      _user_pwd(user_pwd), _mail(0)
{
    conn();

    hello();

    if ( !user_name.empty() )
        login();
}

MailSender::~MailSender()
{
    quit();
}

void MailSender::setMail(MailWrapper* mail)
{
    _mail = mail;
}

bool MailSender::sendMail()
{
    if (!_mail)
        return false;

    if (!sendHeader())
    	return false;
    if (!sendContent())
    	return false;
    if (!sendEnd())
    	return false;
    return true;
}

std::string MailSender::getSenderAddress() const
{
    std::string sender = _user_name;
	size_t index = sender.find('@');
	if( index >= 0 && index < sender.length() )
	{
		sender = "<" + sender + ">";
	}
	else
	{
		std::string server = _server_name;
		size_t h = server.find("mail.");
		if( h >= 0 && h < server.length() )
			server = server.substr(5,server.length()-5);
		h = server.find("smtp.");
		if( h >= 0 && h < server.length() )
			server = server.substr(5,server.length()-5);
		sender = sender + "." + server;
		sender = "<" + sender + ">";
	}
    return sender;
}

bool MailSender::conn()
{
    struct hostent* host = gethostbyname(_server_name.c_str());
    if (host == 0)
    {
    	// throw new MailException("Get server infomation error");
    	_err_message = "Get server infomation error";
    	return false;
    }

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = *(ULONG *)host->h_addr_list[0];
    server_addr.sin_port = htons(SERVICE_PORT);

    connect(_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    return rcvResponse("220");
}

bool MailSender::hello()
{
    char local_host[MAX_BUFFER_SIZE];
    if ( gethostname(local_host, MAX_BUFFER_SIZE) != 0 )
    {
        // throw new MailException("Get local host name error");
        _err_message = "Get local host name error";
        return false;
    }

    std::string msg;

    msg  = "HELO ";
    msg += std::string(local_host) + "\r\n";
    if(!sendRequest(send_helo_cmd, msg))
    	return false;
    return rcvResponse("250");
}

bool MailSender::login()
{
    std::string msg;

    msg = "AUTH LOGIN\r\n";
    if(!sendRequest(send_auth_cmd, msg))
    	return false;
    if(!rcvResponse("334"))
    	return false;

    msg = MUtils::Base64Helper::encode(_user_name) + "\r\n";
    if(!sendRequest(send_username, msg))
    	return false;
    if(!rcvResponse("334"))
    	return false;

    msg = MUtils::Base64Helper::encode(_user_pwd) + "\r\n";
    if(!sendRequest(send_password, msg))
    	return false;
    return rcvResponse("235");
}

bool MailSender::sendHeader()
{
    std::string msg;

    msg  = "MAIL FROM: ";
    msg += getSenderAddress() + "\r\n";
    if(!sendRequest(send_mail_cmd, msg))
    	return false;
    if(!rcvResponse("250"))
    	return false;

    _mail->traverseReceiver();
    while ( _mail->hasMoreReceiver() )
    {
        msg  = "RCPT TO: ";
        msg += _mail->nextReceiverAddress() + "\r\n";
        if(!sendRequest(send_rcpt_cmd, msg))
        	return false;
        if(!rcvResponse("250"))
        	return false;
    }

    msg  = "DATA\r\n";
    if(!sendRequest(send_data_cmd, msg))
    	return false;
    if(!rcvResponse("354"))
    	return false;

    return sendRequest(send_header, _mail->getHeader());
}

bool MailSender::sendContent()
{
    _mail->traverseContent();
    while ( _mail->hasMoreContent() )
    {
        if(!sendRequest(send_content, _mail->nextContent()))
        	return false;
    }
    return true;
}

bool MailSender::sendEnd()
{
    if(!sendRequest(send_end, _mail->getEnd()))
    	return false;
    return rcvResponse("250");
}

bool MailSender::quit()
{
    if(!sendRequest(send_quit_cmd, "QUIT\r\n"))
    	return false;
    if(!rcvResponse("221"))
    	return false;

    closesocket(_socket);
    return true;
}

bool MailSender::rcvResponse(const std::string expected_response)
{
    int recv_bytes = 0;
    char response_buffer[MAX_BUFFER_SIZE];
    if ( (recv_bytes = recv(_socket, response_buffer, MAX_BUFFER_SIZE, 0)) < 0 )
    {
    	_err_message = ErrorMessage::getInstance().response(expected_response);
    	return false;
        //throw new MailException( 
        //    ErrorMessage::getInstance().response(expected_response)
        //);
    }

    std::string response(response_buffer, recv_bytes);
    std::cout << "[INFO]RECV(" << expected_response << "):" 
              << response << std::endl;
    if ( response.substr(0, 3) != expected_response )
    {
    	_err_message = ErrorMessage::getInstance().response(expected_response);
    	return false;
        //throw new MailException( 
        //    ErrorMessage::getInstance().response(expected_response)
        //);
    }
    return true;
}

bool MailSender::sendRequest(Operaion operation, 
                             const std::string content)
{
    std::cout << "[INFO]SEND:" << content << std::endl;

    if ( send(_socket, content.c_str(), content.length(), 0) < 0 )
    {
    	_err_message = ErrorMessage::getInstance().request(operation);
    	return false;
        //throw new MailException(
        //    ErrorMessage::getInstance().request(operation)
        //);
    }
    return true;
}

// Member functions of class ErrorMessage
/////////////////////////////////////

ErrorMessage& ErrorMessage::getInstance()
{
    static ErrorMessage _instance;
    return _instance;
}

ErrorMessage::ErrorMessage()
{
    _request_errmsg_map[MailSender::send_helo_cmd] = "Send HELO cmd error";
    _request_errmsg_map[MailSender::send_auth_cmd] = "Send AUTH cmd error";
    _request_errmsg_map[MailSender::send_username] = "Send user name error";
    _request_errmsg_map[MailSender::send_password] = "Send user password error";
    _request_errmsg_map[MailSender::send_mail_cmd] = "Send MAIL FROM cmd error";
    _request_errmsg_map[MailSender::send_rcpt_cmd] = "Send RCPT TO cmd error";
    _request_errmsg_map[MailSender::send_data_cmd] = "Send DATA cmd error";
    _request_errmsg_map[MailSender::send_header  ] = "Send mail header error";
    _request_errmsg_map[MailSender::send_content ] = "Send mail content error";
    _request_errmsg_map[MailSender::send_end     ] = "Send mail end error";
    _request_errmsg_map[MailSender::send_quit_cmd] = "Send QUIT cmd error";

    _respons_errmsg_map["220"] = "Server connect error";
    _respons_errmsg_map["250"] = "General server error";
    _respons_errmsg_map["334"] = "Server authentication error";
    _respons_errmsg_map["235"] = "Password error";
    _respons_errmsg_map["354"] = "Server not ready for data";
    _respons_errmsg_map["221"] = "Server didn't terminate session";
}

std::string ErrorMessage::request(MailSender::Operaion request_operation)
{
    return _request_errmsg_map[request_operation];
}

std::string ErrorMessage::response(const std::string expected_response)
{
    return _respons_errmsg_map[expected_response];
}

} // namespace SMailer
