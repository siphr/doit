#include "cmdline.h"
#include "datastore.h"

#include <iostream>


int main (int ac, char** av)
{
	_cmdline cmdline(ac, av);
	_datastore ds;

	if (cmdline.show_help())
	{
		std::cout << cmdline.help();
	}
	else if (cmdline.show_categories())
	{
		std::vector<std::string> categories;
		ds.get_categories(categories);

		std::cout << std::endl << "[\t\tCATEGORIES\t\t]" << std::endl;
		for (std::vector<std::string>::const_iterator cat = categories.begin(); cat != categories.end(); ++cat)
		{
			std::cout << *cat << std::endl;
		}
	}
	else if (cmdline.show_contents())
	{
		std::vector< std::vector<std::string> > contents;
		ds.get_content(cmdline.category().c_str(), contents);

		std::cout << std::endl << "[\t\tCONTENTS\t\t]" << std::endl;
		for (std::vector< std::vector<std::string> >::const_iterator cnt = contents.begin(); cnt != contents.end(); ++cnt)
		{
			std::cout << cnt->at(0) << "\t" << cnt->at(1) << std::endl;
		}
	}
	else if (cmdline.show_stats_category())
	{
		_datastore::_stats_category stats;
		ds.get_stats_category(cmdline.category().c_str(), stats);
		std::cout << std::endl << "[\t\tSTATISTICS\t\t]" << std::endl;
		std::cout << "CURRENT: " << stats.m_current_item << std::endl;
		std::cout << "TODO: " << stats.m_todo_items << std::endl;
		std::cout << "DONE: " << stats.m_done_items << std::endl;
		std::cout << "RATE OF DISCOVERY: " << stats.m_rate_of_discovery << std::endl;
		std::cout << "RATE OF COMPLETION: " << stats.m_rate_of_completion << std::endl;
	}
	else if (cmdline.add_category())
	{
		ds.add_category(cmdline.category().c_str());
	}
	else if (cmdline.add_content())
	{
		ds.add_content(cmdline.category().c_str(), cmdline.content().c_str());
	}
	else if (cmdline.done_content())
	{
		ds.done_content(cmdline.category().c_str());
	}


	return 0;
}
