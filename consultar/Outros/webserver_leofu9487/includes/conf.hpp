/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 13:13:31 by xli               #+#    #+#             */
/*   Updated: 2021/11/30 14:48:00 by yfu              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"
#include <iostream>
#include <fstream>
#include <stack>

class ServerInfo;

enum e_server
{
	PORT,
	NAME,
	ERROR,
	SIZE,
};

enum e_location
{
	URI,
	AUTOINDEX,
	INDEX,
	ROOT,
	REDIRECT,
	METHOD,
	UPLOADPATH,
	CGI
};

static const char* server_attributes[] = {
	"listen",
	"server_name",
	"error_page",
	"max_size",
	"location",
	0
};

bool		valid_bracket(std::string str);
int			closing_bracket(std::string str, int pos);
int			nb_lines(std::string str);
std::string get_line(std::string str, int n);
int			nb_tokens(const char *);
bool		is_valid_attribute(std::string str, const char *valid_names[]);
bool		is_skippable(std::string str, int pos);
void		new_server(std::vector<ServerInfo>&, std::string &str, int &pos);
void		new_location(ServerInfo &n_server, std::string &str, int &ct);
void		parse_servers(std::vector<ServerInfo> &result, char *conf_file_path);
