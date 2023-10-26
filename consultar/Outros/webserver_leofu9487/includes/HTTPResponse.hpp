#pragma once
#include "webserv.hpp"

typedef enum
{
	Undefined,
	Continue = 100,
	SwitchingProtocols,
	Processing,
	EarlyHints,
	OK = 200,
	Created,
	Accepted,
	Non_AuthoritativeInformation,
	NoContent,
	ResetContent,
	PartialContent,
	Multi_Status,
	AlreadyReported,
	IMUsed = 226,
	MultipleChoices = 300,
	MovedPermanently,
	Found,
	SeeOther,
	NotModified,
	UseProxy,
	SwitchProxy,
	TemporaryRedirect,
	PermanentRedirect,
	BadRequest = 400,
	Unauthorized,
	PaymentRequired,
	Forbidden,
	NotFound,
	MethodNotAllowed,
	NotAcceptable,
	ProxyAuthenticationRequired,
	RequestTimeout,
	Conflict,
	Gone,
	LengthRequired,
	PreconditionFailed,
	PayloadTooLarge,
	URITooLong,
	UnsupportedMediaType,
	RangeNotSatisfiable,
	ExpectationFailed,
	ImATeapot,
	MisdirectedRequest = 421,
	UnprocessableEntity,
	Locked,
	FailedDependency,
	TooEarly,
	UpgradeRequired,
	PreconditionRequired = 428,
	TooManyRequests,
	RequestHeaderFiledsTooLarge = 431,
	UnavailableForLegalReasons = 451,
	InternalServerError = 500,
	NotImplemented,
	BadGateway,
	ServiceUnavailable,
	GatewayTimeout,
	HTTPVersionNotSupported,
	VariantAlsoNegotiates,
	InsufficientStorage,
	LoopDetected,
	NotExtended = 510,
	NetworkAuthenticationRequired
} status_code;

// source : https://www.tutorialspoint.com/http/http_responses.htm
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

class HTTPResponse
{
	private:
		static std::map<int, std::string> init_code_status();
		static std::map<std::string, std::string> init_content_type();
		static std::string get_default_error_page(status_code error_code);
		std::string	_HTTP_version;
		status_code	_status_code;
		std::string _file_uri;
		std::string _second_file_uri;
		// std::string _date;
		// std::string _server;
		

		std::string	_msg;

	public:
		static std::map<int, std::string> _status_line;
		static std::map<std::string, std::string> _content_type;
		static void	send_error_page(int fd, std::string const &error_pages_root, status_code error_code);

		HTTPResponse(std::string const &HTTP_version, std::string const &method, std::string const &file_uri, std::string const &second_file_uri, std::string const &path, Location_behavior behavior, std::vector<UploadFile> const &, status_code status = Undefined);

		void	set_HTTP_version(std::string const &);
		void	set_status_code(status_code);
		void	set_file_uri(std::string const&);

		std::string const	&get_HTTP_version() const;
		status_code			get_status_code() const;
		std::string const	&get_file_uri() const;
		std::string const	&get_second_file_uri() const;
		std::string const	&get_msg() const;
};


/*
source : https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
*/

