#include "cmdline.h"

namespace bpo = boost::program_options;

_cmdline::_cmdline(int ac, char** av)
{
	bpo::options_description root_help("doit - facilitator utility v1.0");
	root_help.add_options()
		("help", "Produce this help.")
		("show,s", "Show categories or content of a specific category.")
		("add,a", "Add categories or content of a specific category.")
		("category,C", bpo::value<std::string>(), "Category to show or add.")
		("content,c", bpo::value<std::string>(), "Content to add.")
	;

	bpo::store(bpo::parse_command_line(ac, av, root_help), m_vm);
	bpo::notify(m_vm);
}

_cmdline::~_cmdline() { }

bool _cmdline::show_help() const
{
	return m_vm.count("help") == 1;
}

bool _cmdline::show_categories() const
{
	return m_vm.count("show") == 1 && m_vm.count("category") == 0;
}

bool _cmdline::show_contents() const
{
	return m_vm.count("show") == 1 && m_vm.count("category") == 1;
}

bool _cmdline::add_category() const
{
	return m_vm.count("add") == 1 && m_vm.count("category") == 1 && m_vm.count("content") == 0;
}

bool _cmdline::add_content() const
{
	return m_vm.count("add") == 1 && m_vm.count("category") == 1 && m_vm.count("content") == 1;
}

std::string _cmdline::category() const
{
	return m_vm["category"].as<std::string>();
}

std::string _cmdline::content() const
{
	return m_vm["content"].as<std::string>();
}

