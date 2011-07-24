#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

class _datastore
{
	public:

	_datastore();
	~_datastore();


	void get_categories(std::vector<std::string>& categories) const;
	void add_category(char const *category) const;
	void get_content(char const *category, std::vector< std::vector<std::string> >& content) const;
	void add_content(char const *category, char const *content) const;

	private:

	void ensure_schema() const;
	void execute_query(const char* query, std::vector< std::vector<std::string> >* results00=NULL) const;

	bool category_exists(char const *category) const;

	sqlite3* m_db;
	static const std::string m_db_path;
};
