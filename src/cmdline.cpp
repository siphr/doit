#include "cmdline.h"

namespace bpo = boost::program_options;

_cmdline::_cmdline(int ac, char** av)
{
	bpo::options_description root_help("doit - facilitator utility v1.0");
	root_help.add_options()
		("help,h", "Somebody called a doctor?")
		("show,s", "Show categories, content or statistics for a specified category.")
		("info,i", "Show summary statistics for all categories or detailed statistics for specified category.")
		("add,a", "Add a category or content for a specified category.")
		("done,d", "Marks the top most content in queue of a category as done.")
		("category,C", bpo::value<std::string>(), "Category to show or add.")
		("content,c", bpo::value<std::string>(), "Content to add.")
		("purge,p", "Purge all done items from all categories.")
	;

	bpo::store(bpo::parse_command_line(ac, av, root_help), m_vm);
	bpo::notify(m_vm);

	std::ostringstream ss;
	ss << root_help;
	m_help = ss.str();
}

_cmdline::~_cmdline() { }

bool _cmdline::show_help() const
{
	return m_vm.count("help") == 1;
}

bool _cmdline::show_categories() const
{
	return m_vm.count("show") == 1 && m_vm.count("category") == 0 && m_vm.count("info") == 0;
}

bool _cmdline::show_contents() const
{
	return m_vm.count("show") == 1 && m_vm.count("category") == 1 && m_vm.count("info") == 0;
}

bool _cmdline::show_stats_summary() const
{
	return m_vm.count("show") == 1 && m_vm.count("info") == 1 && m_vm.count("category") == 0;
}

bool _cmdline::show_stats_category() const
{
	return m_vm.count("show") == 1 && m_vm.count("info") == 1 && m_vm.count("category") == 1;
}

bool _cmdline::add_category() const
{
	return m_vm.count("add") == 1 && m_vm.count("category") == 1 && m_vm.count("content") == 0;
}

bool _cmdline::add_content() const
{
	return m_vm.count("add") == 1 && m_vm.count("category") == 1 && m_vm.count("content") == 1;
}

bool _cmdline::done_content() const
{
	return m_vm.count("done") == 1 && m_vm.count("category") == 1;
}

char const * const _cmdline::help() const
{
	return m_help.c_str();
}

std::string _cmdline::category() const
{
	return m_vm["category"].as<std::string>();
}

std::string _cmdline::content() const
{
	return m_vm["content"].as<std::string>();
}

