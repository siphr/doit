module DoIt;

import std.stdio;
import std.getopt;
import Options;

int main(string[] args)
{
	Options command_line;

	getopt(args,
		"categories", &command_line.m_list_categories);

	if (command_line.m_list_categories)
	{
		writeln("Listing categories");
	}

	writeln("lets do this.");
	return 0;
}
