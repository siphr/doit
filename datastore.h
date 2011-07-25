#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

class _datastore
{
	public:

	_datastore();
	~_datastore();


	struct _stats_summary
	{
		size_t m_number_of_categories;
		size_t m_done_items;
		size_t m_todo_items;
	};

	struct _stats_category
	{
		size_t m_done_items;
		size_t m_todo_items;
		std::string m_current_item;
		float m_rate_of_discovery;
		float m_rate_of_completion;
	};

	void get_categories(std::vector<std::string>& categories) const;
	void add_category(char const *category) const;

	void get_content(char const *category, std::vector< std::vector<std::string> >& content) const;
	void add_content(char const *category, char const *content) const;

	void done_content(char const *category) const;

	void get_stats_category(char const *category, _stats_category& stats) const;

	private:

	void ensure_schema() const;
	void execute_query(const char* query, std::vector< std::vector<std::string> >* results00=NULL) const;

	bool category_exists(char const *category) const;

	sqlite3* m_db;
	static const std::string m_db_path;
};
