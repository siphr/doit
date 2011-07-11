module DoIt;

import std.stdio;
import std.string;
import std.file;
import std.path;
import std.getopt;
import Options;

int verify_db(ref string root_path)
{
	if (!root_path.exists())
	{
		writeln("Database does not exist.");
		root_path.mkdirRecurse();
		auto f_cat = File(root_path ~ "categories", "w");
		f_cat.close();
	}

	return 0;
}

int main(string[] args)
{
	immutable string root_path = expandTilde("~/.doit/");

	if (verify_db(root_path) != 0)
	{
		writeln("Problem validate database. Exiting.");
		return 1;
	}

	Options command_line;

	getopt(args,
		"categories", &command_line.m_list_categories,
		"add-category", &command_line.m_add_category
	);

	if (command_line.m_list_categories)
	{
		string list_path = root_path ~ "categories";

		if (!list_path.exists())
		{
			writeln("Unable to find categories. Datastore corrupted. Exiting.");
			return 1;
		}

		auto f_list = File(list_path, "r");

		string line;
		while (f_list.readln(line))
		{
			writeln(line);
		}

		writeln("Listed categories");
	}
	else if (command_line.m_add_category.length != 0)
	{
		string cat_path = root_path ~ command_line.m_add_category;

		if (!cat_path.exists())
		{
			auto f_cat = File(root_path ~ "categories", "a");
			f_cat.writeln(command_line.m_add_category);
			f_cat.close();

			auto f_new = File(cat_path, "w");
			f_new.close();
		}

		writeln("Added category: " ~ command_line.m_add_category);
	}

	return 0;
}
