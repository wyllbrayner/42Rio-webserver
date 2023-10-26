/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 11:16:47 by mabriand          #+#    #+#             */
/*   Updated: 2022/02/17 14:27:25 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "../incs/webserv.hpp"
#include "../conf/serv_block.hpp"

#define BUF_SIZE 65536

class   Request
{
	private:
		Request();
		Request(Request& toCopy);
		Request&	operator=(Request& toAssign);

		int									_socket;
		serv_block							*_block;
		std::string							_request;
		std::vector<unsigned char>		_vec_request;
		std::map<std::string, std::string>	_stock;
		/*	Attributes corresponding to the fields of the HTTP request:
		*/
		std::string	_method;
		std::string	_url;
		std::string _base_url;
		std::string	_protocol_version;
		std::string	_host;
		std::string	_user_agent;
		std::string	_accept;
		std::string	_accept_language;
		std::string	_accept_encoding;
		std::string	_connection;
		std::string	_body;
		std::vector<unsigned char> _vec_body;
		std::string	_queryString;
		std::string _referer;
		bool		_err_referer;
		bool		_url_dir;

		std::string	_content_length;
		std::string	_content_type;

		/*	Attributes corresponding to the parameters of the reponse's constructor that will be asked
			when creating it to send it back to the client:
		*/
		std::string	_response_protocol_version;
		int			_response_status_code;
		std::string	_response_url;

		bool _request_ready;
		bool _connexion_end;
	public:
		Request(int socket, serv_block *block);
		~Request();

		/*
			Some utils needed to extract information from the constructor's parameter 'buf' and to
			return some when the appropriate functions are called.
		*/
		std::string	extractInfo(std::string& line) const;// const std::string& --> pour le retour ?
		std::string	extractMapped(std::string& line) const;// const std::string& --> pour le retour ?
		void		buildMap(std::string& buf);
		void		parseBuf();
		int			parse();

		/*
			All setters (one for each attribute i.e. a field of the HTTP request):
		*/
		void		setMethod(std::string& buf);
		void		setUrl(std::string& buf);
		void		setProtocolVersion(std::string& buf);
		void		setHost();
		void		setUserAgent();
		void		setAccept();
		void		setAcceptLanguage();
		void		setAcceptEncoding();
		void		setConnection();
		void		setReferer();
		void		setBody(std::string& full_resp);
		void		setError(int code);
		void		set_error_recv();

		/*
			Processing URL
		*/
		// Utils
		std::vector<loc_block>::iterator	location_found(std::string url, std::vector<loc_block> *loc);
		bool		is_url_dir(std::string url);
		bool		is_referer_error(std::string *ref_code, bool *ref_autoindex);
		bool		is_referer_autoindex(std::string url);
		bool		is_method_available(std::string url);
		bool		is_method_implemented(std::string url);
		size_t		find_pos(std::string line_to_find);

		// Processing
		void		treatUrl();
		std::string	treat_void_url();
		std::string	treat_dir_url(const std::string url);
		std::string	treat_full_url(const std::string url);

		void		treatUrl_with_referer();
		void		treatUrl_with_void_referer();
		void		treatUrl_with_dir_referer();
		void		treatUrl_with_full_referer();
		void		treatUrl_with_err_referer(const std::string ref_code);

		void		set_queryString();

		void		set_contentLength();
		void		set_contentType();
		/*
		*/
		void		defineProtocolVersion();
		void		defineStatusCode();
		void		defineUrl();
		std::string	treat_referer(std::string ref);


		/*
			All getters (one for each attribute i.e. a field of the HTTP request):
		*/
		int	get_socket() {return this->_socket; }
		const std::string&	getMethod() const;
		const std::string&	getUrl() const;
		const std::string&	getBaseUrl() const;
		const std::string&	getProtocolVersion() const;
		const std::string&	getHost() const;
		const std::string&	getUserAgent() const;
		const std::string&	getAccept() const;
		const std::string&	getAcceptLanguage() const;
		const std::string&	getAcceptEncoding() const;
		const std::string&	getConnection() const;
		const std::string&	getReferer() const;
		const std::string&	getBody() const;
		const std::vector<unsigned char> getVecBody() const;
		serv_block*	getBlock();
		const bool&	is_request_ready() const;
		const bool&	is_connection_end() const { return this->_connexion_end; }
		const bool& get_url_dir() const;
		const std::string&	getRequest() const;

		const std::string&	get_queryString() const;

		const std::string&		get_contentLength() const;
		const std::string&		get_contentType() const;
		/*
		*/
		const std::string&	returnProtocolVersion() const;
		int					returnStatusCode() const;
		const std::string&	returnUrl() const;

};

std::ostream&		operator<<(std::ostream& os, const Request& r);

#endif
