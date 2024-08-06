/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# include <string>
# include <iostream>
# include <vector>
# include "../Utils.hpp"

class	Location
{
	private:
		std::vector<std::string>			_allowedMethods;
		std::string							_path;
		std::vector<std::string>			_methods;
		std::vector<std::string>			_index;
		bool								_autoIndex;
		std::string							_return;
		void								printVecString(\
											const std::vector<std::string> & content, \
											std::string str) const;
	
	public:
		Location(void);
		Location(const Location & copy);
		Location						&	operator=(const Location & src);
		~Location(void);

		const std::vector<std::string>	&	getAllowedMethods(void) const;
		const std::string				&	getPath(void) const;
		const std::vector<std::string>	&	getMethods(void) const;
		const std::vector<std::string>	&	getIndex(void) const;
		const bool						&	getAutoIndex(void) const;
		const std::string				&	getReturn(void) const;
		void								setPath(std::string & path, std::string root);
		void								setPathSmart(std::string & path);
		void								setMethods(\
											std::vector<std::string> & vecLocation);
		void								setDefaultMethods(void);
		void								setIndex(\
											std::vector<std::string> & vecIndex);
		void								setIndexSmart(std::string parameter);
		void								setAutoIndex(bool flag);
		void								setReturn(std::string path, std::string root);
		void								setReturnSmart(std::string path);
		void								fixeReturn(void);
		void								printLocation(void) const;
		void								clearLocation(void);
};