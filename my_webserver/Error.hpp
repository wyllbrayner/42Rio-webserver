/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 15:23:54 by woliveir          #+#    #+#             */
/*   Updated: 2022/05/09 15:09:49 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>

class	Error
{
	private:

	public:
		class		InvalidArg : public std::exception
		{
			public:
				virtual const char* what( void ) const throw();
		};

		class	InvalidPathServer : public std::exception
		{
			public:
				virtual const char* what( void ) const throw();
		};

		class	InvalidConfigurationServer : public std::exception
		{
			public:
				virtual const char* what( void ) const throw();
		};

		class	InvalidServer : public std::exception
		{
			public:
				virtual const char* what( void ) const throw();
		};

		class	InvalidParameter : public std::exception
		{
			public:
				virtual const char* what( void ) const throw();
		};

};
