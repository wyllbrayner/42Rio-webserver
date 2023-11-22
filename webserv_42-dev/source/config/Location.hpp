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

#pragma	once

# include <string>
# include <iostream> //confirmar se será necessário para a entrega
# include "Error.hpp"
# include "Utils.hpp"

class	Location
{
	private:
		std::string					_path;
		std::vector<std::string>	_allowed_methods;
	
	public:
		Location( void );
		Location( const Location& copy );
		Location	&operator=( const Location &src );
		~Location( void );

		const std::string &					getPath(void) const;
		const std::vector<std::string>		getMethods(void) const;

		void								setPath(std::string & path);
		void								setMethods(std::vector<std::string> & vecLocation);
};