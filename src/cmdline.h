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
	bool show_stats_summary() const;
	bool show_stats_category() const;

	bool add_category() const;
	bool add_content() const;

	bool done_content() const;

	char const * const help() const;
	std::string category() const;
	std::string content() const;

	private:

	std::string m_help; 
	boost::program_options::variables_map m_vm;
};
