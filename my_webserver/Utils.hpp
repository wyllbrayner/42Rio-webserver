/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma	once

# include <string>
# include <vector>
# include <sys/stat.h>
# include <unistd.h>

class	Utils
{
	private:
		Utils( void );
		Utils( const Utils& copy );
		Utils	&operator=( const Utils &src );
		~Utils( void );
	
	public:
		static	void						trim(std::string & line, std::string c);
		static	void						ltrim(std::string & line, std::string c);
		static	void						rtrim(std::string & line, std::string c);
		static	std::vector<std::string>	split(const std::string line, std::string sep);
		static	int							atoi(const std::string line);
		static	short int					getTypePath(const std::string & path);
		static	short int					checkFile(const std::string & path, short int mode);
		static	bool						isFileExistAndReadable(const std::string & path, \
		const std::string & index);
};