/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lenygarcia <lenygarcia@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 11:03:01 by lenygarcia        #+#    #+#             */
/*   Updated: 2025/08/06 12:16:17 by lenygarcia       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string& filename):
				_filename(filename)
{
	std::ifstream file("data.csv");
	if (!file.is_open())
		throw std::runtime_error("Could not open data.csv");
	
	std::string line;
	while (std::getline(file, line))
	{
		std::string date;
		float value;
		std::istringstream ss(line);

		if (std::getline(ss, date, ',') && ss >> value)
		{
			if (ss.peek() == '\n')
				ss.ignore();
			_data[date] = value;
		}
		else
			continue;
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other):
				_data(other._data), _filename(other._filename) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
	{
		_data = other._data;
		_filename = other._filename;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {
	if (_file.is_open())
		_file.close();
}

void BitcoinExchange::printData() const
{
	std::map<std::string, float>::const_iterator it;
	for (it = _data.begin(); it != _data.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
}

void BitcoinExchange::openFile()
{
	_file.open(_filename);
	if (!_file.is_open())
		throw std::runtime_error("Could not open file: " + _filename);
}

static void parse_date_value(const std::string &date, float value, bool *ok)
{
	if (date.empty() || value < 0 || value > 1000)
	{
		*ok = false;
		return;
	}
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
	{
		*ok = false;
		return;
	}
	std::istringstream ss(date);
	int year, month, day;
	ss >> year;
	if (ss.peek() != '-' || !(ss >> month) || ss.peek() != '-' || !(ss >> day) || !ss.eof())
	{
		*ok = false;
		return;
	}
	month *= -1;
	day *= -1;
	if (year > 2025 || month < 1 || month > 12 || day < 1 || day > 31)
	{
		*ok = false;
		return;
	}
}

void BitcoinExchange::parseFile()
{
	std::string line;
	bool firstLine = false;

	while (std::getline(_file, line))
	{
		if (!firstLine && line == "date | value")
		{
			firstLine = true;
			continue;
		}
		std::string date;
		float value;
		std::istringstream ss(line);
		if (std::getline(ss, date, '|') && ss >> value)
		{
			bool ok = true;
			date.pop_back();
			parse_date_value(date, value, &ok);
			if (!ok)
			{
				std::cerr << "Error: Invalid date or value" << std::endl;
				continue;
			}
			if (ss.peek() == '\n')
				ss.ignore();
			if (_data.find(date) != _data.end())
				std::cout << date << " => " << value << " = " << _data[date] * value << std::endl;
			else
			{
				std::map<std::string, float>::iterator it = _data.upper_bound(date);
				if (it != _data.begin())
				{
					if (it != _data.begin() && it->first != date)
						--it;
					std::cout << date << " => " << value << " = " << it->second * value << std::endl;
				}
				else
					std::cerr << "Error: Date not found in data.csv - " << date << std::endl;
			}
		}
		else
			std::cerr << "Error: Invalid line format - " << line << std::endl;
	}
}