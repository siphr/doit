#include "cmdline.h"
#include "datastore.h"

#include <iostream>


int main (int ac, char** av)
{
	_cmdline cmdline(ac, av);
	_datastore ds;

	if (cmdline.show_help())
	{

	}
	else if (cmdline.show_categories())
	{
		std::vector<std::string> categories;
		ds.get_categories(categories);

		for (std::vector<std::string>::const_iterator cat = categories.begin(); cat != categories.end(); ++cat)
		{
			std::cout << *cat << std::endl;
		}
	}
	else if (cmdline.show_contents())
	{
		std::vector< std::vector<std::string> > contents;
		ds.get_content(cmdline.category().c_str(), contents);

		for (std::vector< std::vector<std::string> >::const_iterator cnt = contents.begin(); cnt != contents.end(); ++cnt)
		{
			std::cout << cnt->at(0) << std::endl;
		}
	}
	else if (cmdline.add_category())
	{
		ds.add_category(cmdline.category().c_str());
	}
	else if (cmdline.add_content())
	{
		ds.add_content(cmdline.category().c_str(), cmdline.content().c_str());
	}


	return 0;
}
