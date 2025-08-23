#pragma once
#include "Database.h"
#include <sqlite3.h>
#include <print>
#include <vector>
#include <string>
#include <filesystem>
#include "core_types.h"

#include "Logger.h"

class Database
{
public:

	
private:
	std::string path;
	sqlite3* db = nullptr;
	const char* err = errMsg;
	char* errMsg = nullptr;
	

public:
	Database() = default;
	~Database() { Close(); }

	void Close();

	bool OpenDB(std::string &path);

	bool isOpen() { return db != nullptr; }

	bool ExecuteSQL(const char* sql);

	sqlite3* getDatabase() { return db; }

	int ExecuteAndBind(const char* sql, std::vector<std::string>& colNames, std::vector<SQLValue> &values)
	{
		sqlite3_stmt* stmt;

		if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
		{
			int cols = sqlite3_column_count(stmt);
			colNames.clear();
			for (int i = 0; i < cols; i++)
			{
				colNames.push_back(sqlite3_column_name(stmt, i));
			}
			for (int i = 0; i < values.size(); i++)
			{
				
				std::visit([&](auto&& val) {
					using T = std::decay_t<decltype(val)>;
					if constexpr (std::is_same_v<T, int>)
					{
						sqlite3_bind_int(stmt, static_cast<int>(i + 1), val);
					}
					else if constexpr (std::is_same_v<T, double>)
					{
						sqlite3_bind_double(stmt, static_cast<int>(i + 1), val);
					}
					else if constexpr (std::is_same_v<T, std::string>)
					{
						sqlite3_bind_text(stmt, static_cast<int>(i + 1), val.c_str(), -1, SQLITE_TRANSIENT);
					}
					}, values[i]);
			}


			Logger::GetInstance().LogMsg(Logger::DEBUG, sql);
			Logger::GetInstance().LogMsg(Logger::INFO, "Query Executed successfully! ");
			sqlite3_finalize(stmt);
			return 1;
		}
		else
		{
			err = sqlite3_errmsg(db);
			std::println("Failed to execute query {}", sql);
			Logger::GetInstance().LogMsg(Logger::ERR, err);
			return 2;
		}
	}

	std::string getColumnType(sqlite3* db, const std::string& table, const std::string& column);

	std::vector<std::vector<std::string>> ExecuteQuery(const char* querySQL, std::vector<std::string>& colNames);
	std::vector<std::string> GetCols(const std::string& tableName);

	// schema editor functions
	std::vector<std::string> GetTables();
	// for columns
	// getting data from columns up to x amount
	// search for x column or table
	// drop table
	// delete col
	// delete data


	// add a modify method that takes table and where and value
	// write methods that are grand in granularity then abstract them into different methods for more specific features for the app
	
	
private:

};