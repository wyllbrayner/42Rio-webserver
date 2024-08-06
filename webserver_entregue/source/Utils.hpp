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

class	Utils
{
	private:
		Utils(void);
		Utils(const Utils& copy);
		Utils						&	operator=(const Utils &src);
		~Utils(void);
	public:
		static bool						_serverRunning;
		static int						atoi(const std::string line);
		static int						getTypePath(const std::string & path);
		static void						trim(std::string & line);
		static const std::string		setPlace(std::string path);
		static std::vector<std::string>	split(const std::string line, \
										std::string sep);
};