#include <iostream>
#include "datastore.h"
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

void setup_help(int ac, char** av)
{
	bpo::options_description root_help("doit - facilitator utility v1.0");
	root_help.add_options()
		("help", "produce help message.")
	;

	bpo::variables_map vm;
	bpo::store(bpo::parse_command_line(ac, av, root_help), vm);
	bpo::notify(vm);    

	if (vm.count("help")) {
		std::cout << root_help << "\n";
		return;
	}
}


int main (int ac, char** av)
{
	setup_help(ac, av);
	
	_datastore ds;

	ds.add_category("books");

	std::vector<std::string> categories;
	ds.get_categories(categories);

	for (std::vector<std::string>::const_iterator cat = categories.begin(); cat != categories.end(); ++cat)
	{
		std::cout << *cat << std::endl;
	};

	return 0;
}
