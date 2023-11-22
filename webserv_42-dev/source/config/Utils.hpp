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
# include <iostream> /*verificar se precisa imprimir alguma coisa na tela*/
# include "Error.hpp"/*verificar se precisa lançar exceção*/

class	Utils
{
	private:
		Utils( void );
		Utils( const Utils& copy );
		Utils	&operator=( const Utils &src );
		~Utils( void );
	
	public:
		static	void							trim( std::string & line );
		static	std::vector<std::string>		split( const std::string line, std::string sep);
		static	int								atoi(const std::string line);
};