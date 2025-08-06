/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:36:59 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/08/06 16:16:33 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN(const std::string &expression) : _expression(expression)
{
	this->parseExpression();
	this->calculate();
}

RPN::RPN(const RPN &other) : _stack(other._stack), _expression(other._expression)
{
	this->parseExpression();
	this->calculate();
}

RPN::RPN() {}

RPN &RPN::operator=(const RPN &other) {
	if (this != &other) {
		_stack = other._stack;
		_expression = other._expression;
	}
	return *this;
}

RPN::~RPN() {}

static void check_chars(const std::string &expression)
{
	for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
		char c = *it;
		if (!isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != ' ') {
			throw std::invalid_argument("Invalid character in expression");
		}
	}
	for (size_t i = 0; i < expression.size(); ++i) {
		if (i % 2 == 0)
		{
			if (!isdigit(expression[i]) && expression[i] != '+'
			&& expression[i] != '-' && expression[i] != '*'
			&& expression[i] != '/') {
				throw std::invalid_argument("Invalid character in expression");
			}
		}
		else if (expression[i] != ' ') {
			throw std::invalid_argument("Invalid character in expression");
		}
	}
}

static void check_counts(const std::string& expression)
{
	int num_count = 0;
	int op_count = 0;

	for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
		if (isdigit(*it)) {
			num_count++;
		} else if (*it == '+' || *it == '-' || *it == '*' || *it == '/') {
			op_count++;
		}
	}

	if (num_count < 1) {
		throw std::invalid_argument("Expression must contain at least one number");
	}
	if (op_count < 1) {
		throw std::invalid_argument("Expression must contain at least one operator");
	}
	if (num_count - op_count != 1) {
		throw std::invalid_argument("Invalid expression: number of operators must be one less than number of operands");
	}
}

static void check_consistency(const std::string& expression)
{
	size_t num_count = 0;
	for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
		if (*it == ' ') {
			continue;
		}
		if (isdigit(*it)) {
			num_count++;
		}
		if (*it == '+' || *it == '-' || *it == '*' || *it == '/') {
			if (num_count < 2) {
				throw std::invalid_argument("Invalid expression: operator found without sufficient operands");
			}
			num_count--;
		}
	}
}

void RPN::parseExpression()
{
	if (_expression.empty())
		throw std::invalid_argument("Expression cannot be empty");
	check_chars(_expression);
	check_counts(_expression);
	check_consistency(_expression);
}


void RPN::calculate()
{
	for (std::string::const_iterator it = _expression.begin(); it != _expression.end(); ++it) {
		if (*it == ' ') {
			continue;
		}
		if (isdigit(*it))
			_stack.push(*it - '0');
		else
		{
			int b = _stack.top(); _stack.pop();
			int a = _stack.top(); _stack.pop();
			switch (*it)
			{
				case '+':
					_stack.push(a + b);
					break;
				case '-':
					_stack.push(a - b);
					break;
				case '*':
					_stack.push(a * b);
					break;
				case '/':
					if (b == 0)
						throw std::invalid_argument("Division by zero");
					_stack.push(a / b);
					break;
				default:
					throw std::invalid_argument("Invalid operator");
			}
		}
	}
	if (_stack.size() != 1)
		throw std::invalid_argument("Invalid expression: stack should contain exactly one result");
	std::cout << "Result: " << _stack.top() << std::endl;
}