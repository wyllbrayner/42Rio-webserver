/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmoreau <vmoreau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 12:55:24 by vmoreau           #+#    #+#             */
/*   Updated: 2021/12/04 14:23:04 by vmoreau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
#define COLORS_HPP
	#ifndef uncolored
		#define BLACK "\033[0;30m"
		#define RED "\033[0;31m"
		#define GREEN "\033[32m"
		#define YELLOW "\033[33;33m"
		#define BLUE "\033[0;34m"
		#define PURPLE "\033[35m"
		#define CYAN "\033[1;36m"
		#define NC "\033[0;37m"

	#else
		#define BLACK ""
		#define RED ""
		#define GREEN ""
		#define YELLOW ""
		#define BLUE ""
		#define PURPLE ""
		#define CYAN ""
		#define NC ""
	#endif
#endif