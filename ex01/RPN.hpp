/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:36:45 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/08/06 16:15:41 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <string>

class RPN
{
private:
	std::stack<int> _stack;
	std::string _expression;
	RPN();
	void parseExpression();
	void calculate();
public:
	RPN(const std::string &expression);
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();
};

#endif