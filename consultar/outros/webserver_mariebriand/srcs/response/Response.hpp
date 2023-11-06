/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 15:24:45 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/18 03:39:24 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "../incs/webserv.hpp"
#include "../conf/serv_block.hpp"
#include "../request/Request.hpp"

class   Response
{
	private:
		Response(Response& toCopy);
		Response&	operator=(Response& toAssign);

		std::vector<unsigned char>		_response;

		std::map<std::string, std::string>	_stock;
		std::map<std::string, std::string>	_mime;
		std::map<int, std::string>			_messages;
		const Request						&_req;
		serv_block							*_block;
		/*	Attributes corresponding to the fields of the HTTP request:
		*/
		std::string						_protocol_version;
		std::string						_status;
		std::string						_status_message;
		std::string						_date;
		std::string						_server;
		std::string						_connection;
		std::string						_keep_alive;
		std::vector<unsigned char>				_body;
		std::string						_bodyStr;
		std::vector<unsigned char>				_cgi_output;
		std::string						_content_type;
		std::string						_content_length; // Content-Lenght : ne doit pas dépasser le client body size max
		std::string 					_selected_mime;
		bool							_target_dir;

		std::vector<unsigned char>	_cgi_head;
		std::vector<unsigned char>	_cgi_body;

	public:
		Response();
		// Response(const std::string& protocol_version, int status, const std::string& url, serv_block *block);
		Response(const Request &req, serv_block *block, bool cgi, std::vector<unsigned char> cgiOutput);
		// Response(Request *current_request, bool cgi, std::vector<unsigned char> cgiOutput);
		virtual ~Response();

		/*	All setters (one for each attribute corresponding to a field of the HTTP request):
		*/
		void		setProtocolVersion(const std::string& protocol_version);
		void		setStatus(int status);
		void		setStatusMessage(int status);
		void		setLocation(std::string url);
		void		setDate();
		void		setServer();
		void		setConnection();
		void		setKeepAlive();
		void		setContentType(const std::string& content_type);
		void		setContentLenght();
		void		setBody(const std::string& body);
		std::string	build_autoindex_body(const char *path);
		void		set_newContentType(std::vector<unsigned char> header);
		void		set_cgiOutput(std::vector<unsigned char> body, bool cgi);
		void		buildMime(const std::string& key, const std::string& mapped);
		void		setMimeMap();
		void		buildMessages(int key, const std::string& mapped);
		void		setMessagesMap();

		void		fill_resp(const std::string& line);
		void		buildPartResp(const std::string& key);
		void		buildResponse();

		bool		is_root_dir(std::string url);
		std::string	getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host);
		std::string	build_autoindex_head(std::string DirName);
		/*	All getters (one for each attribute corresponding to a field of the HTTP request):
		*/
		const std::string&	get_ProtocolVersion() const;
		const std::string&	get_Status() const;
		const std::string&	get_StatusMessage() const;
		const std::string&	get_Date() const;
		const std::string&	get_Server() const;
		const std::string&	get_ContentType() const;
		const std::string&	get_ContentLenght() const;
		const std::string&	get_bodyStr() const;
		const std::vector<unsigned char>&	getBody() const;
		const std::string&	get_Mime() const;

		const std::string&	getResponse() const;
		const std::vector<unsigned char>& getVecResponse() const;
};

std::ostream&		operator<<(std::ostream& os, const Response& r);

#endif
