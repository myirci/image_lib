#pragma once

#include <fstream>
#include <iostream>

template <typename CharType = char>
class Logger 
{
public:
	
	Logger(const std::string& logFilePath, std::ios_base::openmode mode = std::ios::out)
	{
		m_os.open(logFilePath, mode);
		if (!m_os.is_open())
			throw std::runtime_error("Cannot open the log file: " + logFilePath);
	}

	~Logger() 
	{
		if (m_os.is_open())
			m_os.close();
	}

	template <typename Arg, typename ... Args>
	void WriteLine(Arg&& arg, Args&& ... args) 
	{
		((m_os << arg << " " << args), ...);
		m_os << std::endl;
	}

private:

	std::basic_ofstream<CharType> m_os{};
};