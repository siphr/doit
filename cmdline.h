#pragma once

#include <boost/program_options.hpp>

class _cmdline
{
	public:
	
	_cmdline(int ac, char** av);
	~_cmdline();

	bool show_help() const;
	bool show_categories() const;
	bool show_contents() const;

	bool add_category() const;
	bool add_content() const;

	std::string category() const;
	std::string content() const;

	private:

	boost::program_options::variables_map m_vm;
};
