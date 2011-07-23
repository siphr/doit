#include "datastore.h"
#include <iostream>
#include <assert.h>
#include <sstream>

const std::string _datastore::m_db_path = ".doit.db";

_datastore::_datastore()
{
	assert(sqlite3_open(".doit.db", &m_db) == SQLITE_OK);
	ensure_schema();
}

_datastore::~_datastore()
{
	sqlite3_close(m_db);
}

void _datastore::execute_query(const char* query, std::vector< std::vector<std::string> >* results00) const
{
	sqlite3_stmt *stmt;
	if (sqlite3_prepare_v2(m_db, query, -1, &stmt, 0) != SQLITE_OK)
	{
		std::cout << sqlite3_errmsg(m_db) << std::endl;
		assert(false);
	}

	int const column_count = sqlite3_column_count(stmt);

	if (results00) results00->clear();

	while (int const result = sqlite3_step(stmt) == SQLITE_ROW)
	{
		if (results00)
		{
			std::vector<std::string> values;

			for (int column_index=0; column_index < column_count; ++column_index)
			{
				values.push_back((char*)sqlite3_column_text(stmt, column_index));	
			}
				
			results00->push_back(values);
		}
	}

	assert(sqlite3_finalize(stmt) == SQLITE_OK);
}

void _datastore::ensure_schema() const
{
	char const qry[] = "create table if not exists categories (name STRING);";
	execute_query(qry);

	std::vector<std::string> categories;
	get_categories(categories);

	for (std::vector<std::string>::const_iterator cat = categories.begin(); cat != categories.end(); ++cat)
	{
		std::ostringstream ss;
		ss << "create table if not exists " << *cat << " (content STRING);";
		execute_query(ss.str().c_str());
	}
}

void _datastore::get_categories(std::vector<std::string>& categories) const
{
	char const *qry = "select name from categories order by name asc;";
	std::vector< std::vector<std::string> > results;
	execute_query(qry, &results);

	for (std::vector< std::vector<std::string> >::const_iterator res = results.begin(); res != results.end(); ++res)
	{
		categories.push_back(res->at(0));
	}
}

void _datastore::add_category(char const *category) const
{
	std::ostringstream ss;
	ss << "insert into categories values('" << category << "');";

	execute_query(ss.str().c_str());
}

void _datastore::add_content(char const *category, char const *content) const
{
	std::ostringstream ss;
	ss << "insert into " << category << " values('" << content << "');";

	execute_query(ss.str().c_str());
}
