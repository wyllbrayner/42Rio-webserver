/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_autoindex.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 15:35:49 by vmoreau           #+#    #+#             */
/*   Updated: 2022/02/08 14:24:24 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"
#define E_FILE "&#128209;"
#define E_FOLDER "&#128210;"
bool		Response::is_root_dir(std::string url)
{
	std::vector<loc_block> loc = this->_block->get_location();
	std::string tmp_url = url;

	if (tmp_url.back() == '/' && tmp_url.size() > 1)
		tmp_url.pop_back();
	if (tmp_url == "/")
		return (true);
	for (std::vector<loc_block>::iterator it = loc.begin(); it != loc.end(); it++)
		if (it->get_path() == tmp_url)
			return (true);

	return (false);
}

bool		Is_DirEntry_Dir(std::string url)
{
	std::string::size_type pos = url.find_last_of('.');

	return (pos == url.npos);
}

std::string			Response::getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host)
{
	std::string block;
	std::stringstream   ss;
	std::string dir_entry = dirEntry;

	block += "\t\t<tr>\n\
			<td>";

// ---------- NAME LINE ---------- //
	if (Is_DirEntry_Dir(dirEntry) == true)
	{
		dir_entry += "/";
		block += E_FOLDER;
	}
	else
		block += E_FILE;
	block += " <a href=\"http://" + host + dirName;
	if (dirName.back() != '/')
		block += "/";
	block += dir_entry + "\">" + dir_entry + "</a></td>\n";

// ---------- NATURE LINE ---------- //
	if (Is_DirEntry_Dir(dirEntry) == true)
		block += "\t\t\t<td><div class=\"center\">Folder</div></td>\n";
	else
		block += "\t\t\t<td><div class=\"center\">File</div></td>\n";

// ---------- DATE LINE ---------- //
	block += "\t\t\t<td><div class=\"center\">";

	struct stat result;

	std::string tmp_url = this->_req.getUrl();
	std::string url_dir(tmp_url.begin(), tmp_url.begin() + tmp_url.find_last_of('/'));
	char formated_date[18];

	if (url_dir.back() != '/')
		url_dir += "/";
	url_dir += dir_entry;

	stat(url_dir.c_str(), &result);
	std::tm *ptm = std::localtime(&result.st_mtime);
	std::strftime(formated_date, 18, "%d-%b-%Y %H:%M", ptm);
	std::string date = formated_date;

	block += date + "</div></td>\n";

// ---------- SIZE LINE ---------- //
	block += "\t\t\t<td><div class=\"center\">";

	std::string size;

	ss << result.st_size;
	block += ss.str() + " B</div></td>\n\t\t</tr>";


	if (dir_entry == "." || dir_entry == "..")
		if (is_root_dir(dirName) == true)
			return ("");
	return (block);
}
std::string			Response::build_autoindex_head(std::string DirName)
{
	std::string page ="\
<!DOCTYPE html>\n\
<html>\n\
	<head>\n\
		<title>Index of " + DirName + "</title>\n\
		<style>\n\
			body{\n\
				font-family: 'Lato', sans-serif;\n\
				color: rgb(0, 0, 0);\n\
				height: 100%;\n\
			}\n\
			h1{\n\
				text-align: center;\n\
			}\n\
			table, th, td{\n\
				border:1px solid rgb(0, 0, 0);\n\
			}\n\
			.center{\n\
				text-align: center;\n\
			}\n\
		</style>\n\
	</head>\n";
	return (page);
}

std::string			Response::build_autoindex_body(const char *path)
{
	std::string dirName(this->_req.getBaseUrl());
	DIR *dir = opendir(path);
	std::string page = build_autoindex_head(dirName);

	page += "\
	<body style=\"background-color:#ffffff;\">\n\
		<h1>Index of " + dirName + "</h1>\n\
		<table style=\"width:100%\">\n\
		<tr>\n\
			<th>Name</th>\n\
			<th>Nature</th>\n\
			<th>Last Modified</th>\n\
			<th>Size</th>\n\
		</tr>\n";

	if (dir == NULL) {
		std::cerr << RED << "Error: could not open [" << path << "]" << NC << std::endl;
		return "";
	}
	if (dirName[0] != '/')
		dirName = "/" + dirName;
	for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		std::string line = getLink(std::string(dirEntry->d_name), dirName, this->_req.getHost());

		// std::cout << BLUE << line << NC << '\n';
		page += line;
	}
	page += "\
	</table>\n\
	<p>Mabriand & Vmoreau</p>\n\
	</body>\n\
</html>\n";
	closedir(dir);
	return page;
}
