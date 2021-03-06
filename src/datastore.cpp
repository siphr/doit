#include "datastore.h"

#include <assert.h>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BOOST_FILESYSTEM_VERSION 2

#include <boost/filesystem.hpp>

_datastore::_datastore()
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;

	assert(sqlite3_open((std::string(homedir) + std::string("/.doit.db")).c_str(), &m_db) == SQLITE_OK);
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
		ss << "create table if not exists " << *cat << " (content STRING, date_added DATE, done BOOL, date_done DATE);";
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

bool _datastore::category_exists(char const *category) const
{
	std::ostringstream ss;
	ss << "select name from categories where name like '" << category << "';";

	std::vector< std::vector<std::string> > results;
	execute_query(ss.str().c_str(), &results);
	return !results.empty();
}

void _datastore::add_category(char const *category) const
{
	if (category_exists(category)) return;

	std::ostringstream ss;
	ss << "insert into categories values('" << category << "');";
	execute_query(ss.str().c_str());

	ss.str("");

	ss << "create table if not exists " << category << " (content STRING, date_added DATE, done BOOL, date_done DATE);";
	execute_query(ss.str().c_str());
}

void _datastore::get_content(char const *category, std::vector< std::vector<std::string> >& contents) const
{
	std::ostringstream ss;
	ss << "select content,date_added,done,date_done from " << category << " where not done order by date_added asc;";
	execute_query(ss.str().c_str(), &contents);
}

void _datastore::add_content(char const *category, char const *content) const
{
	if (!category_exists(category))
	{
		add_category(category);
	}
	
	std::ostringstream ss;
	ss << "insert into " << category << " values('" << content << "', datetime(), 0, '');";

	execute_query(ss.str().c_str());
}

void _datastore::done_content(char const *category) const
{
	assert(category_exists(category));

	std::ostringstream ss;
	ss << "update " << category << " set done=1,date_done=datetime() where content=(select content from " << category << " where done=0 order by date_added asc limit 1);";

	execute_query(ss.str().c_str());
}

void _datastore::get_stats_category(char const *category, _stats_category& stats) const
{
	std::ostringstream ss;
	std::vector< std::vector<std::string> > results;

	ss << "select count(rowid) from " << category << " where done=0;";
	execute_query(ss.str().c_str(), &results);
	stats.m_todo_items = atoi(results.at(0).at(0).c_str());

	ss.str("");
	ss << "select count(rowid) from " << category << " where done=1;";
	execute_query(ss.str().c_str(), &results);
	stats.m_done_items = atoi(results.at(0).at(0).c_str());

	if (stats.m_todo_items != 0)
	{
		ss.str("");
		ss << "select content,date_added from " << category << " where done=0 order by date_added asc limit 1;";
		execute_query(ss.str().c_str(), &results);
		stats.m_current_item = results.at(0).at(0) + " | " + results.at(0).at(1);
	}
	else
	{
		stats.m_current_item = "";
	}

	ss.str("");

	ss << "select count(rowid) from " << category << ";";
	execute_query(ss.str().c_str(), &results);
	stats.m_rate_of_discovery = atof(results.at(0).at(0).c_str());

	if (stats.m_rate_of_discovery > 0)
	{
		ss.str("");
		ss << "select max(julianday(date_added)) - min(julianday(date_added)) from " << category << ";";
		execute_query(ss.str().c_str(), &results);
		stats.m_rate_of_discovery /= atof(results.at(0).at(0).c_str()) + 1;
	}

	ss.str("");

	ss << "select count(rowid) from " << category << " where done=1;";
	execute_query(ss.str().c_str(), &results);
	stats.m_rate_of_completion = atof(results.at(0).at(0).c_str());

	if (stats.m_rate_of_completion > 0)
	{
		ss.str("");
		ss << "select 1/avg(julianday(date_done) - julianday(date_added)) from " << category << " where done=1;";
		execute_query(ss.str().c_str(), &results);
		stats.m_rate_of_completion = atof(results.at(0).at(0).c_str());
	}
}
