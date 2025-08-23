#include "Database.h"


// conditional ? if_true : if_false
// void print = [&]() { std::cout << x << std::endl; } 

bool Database::OpenDB(std::string &path) // opens db
{
	if (sqlite3_open(path.c_str(), &db) != SQLITE_OK)
	{
		err = sqlite3_errmsg(db);
		std::println("Failed to open Database"); // basic error handle
		
		Logger::GetInstance().LogMsg(Logger::ERR, err);

		return false;
	};
	Logger::GetInstance().LogMsg(Logger::INFO, "Opened DB");
	Logger::GetInstance().LogMsg(Logger::INFO, path.c_str());

	return true; // success
}




std::vector<std::vector<std::string>> Database::ExecuteQuery(const char* querySQL, std::vector<std::string>& colNames) // for querying data from database
{
	sqlite3_stmt* stmt;
	std::vector<std::vector<std::string>> results;

	if (sqlite3_prepare_v2(db, querySQL, -1, &stmt, nullptr) == SQLITE_OK && db != nullptr)
	{
		int cols = sqlite3_column_count(stmt);
		colNames.clear();
		for (int i = 0; i < cols; i++) // gives the option to extract colnames as well through a ref
		{
			colNames.push_back(sqlite3_column_name(stmt, i));
		}

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::vector<std::string> row; // makes a row and appends all data in said row

			for (int i = 0; i < cols; i++)
			{
				const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
				row.push_back(text ? text : "NULL");
				std::cout << row[i] << std::endl;
			}
			results.push_back(row);
			// follow general structure and update results vector with user data 
			// maybe return a vector of user data
			// vector of structs?

		}
		
		Logger::GetInstance().LogMsg(Logger::DEBUG, querySQL);
		Logger::GetInstance().LogMsg(Logger::INFO, "Query Executed successfully! "); 
		sqlite3_finalize(stmt);
		return results;
	}
	else
	{ // basic error handle
		err = sqlite3_errmsg(db);
		std::println("Failed to execute query {}", querySQL);
		Logger::GetInstance().LogMsg(Logger::ERR, err);
		return results;
	}
}

bool Database::ExecuteSQL(const char* sql) // for quick executions/commands
{
	if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
	{
		err = sqlite3_errmsg(db);
		std::println("Error executing SQL command : {}", sql);
		Logger::GetInstance().LogMsg(Logger::ERR, err);
		
		sqlite3_free(errMsg);
		return false;
	}
	Logger::GetInstance().LogMsg(Logger::INFO, "Executed SQL Successfully");

	return true;
}

std::string Database::getColumnType(sqlite3* db, const std::string& table, const std::string& column) { // to extract column type
	sqlite3_stmt* stmt = nullptr;                                                                       // legacy: was going to implement insert operations but its a skill issue
	std::string sql = "PRAGMA table_info(" + table + ");";

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
		return {};
	}

	std::string colType;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		const unsigned char* colName = sqlite3_column_text(stmt, 1);
		const unsigned char* typeName = sqlite3_column_text(stmt, 2);
		if (column == reinterpret_cast<const char*>(colName)) {
			colType = reinterpret_cast<const char*>(typeName);
			break;
		}
	}
	sqlite3_finalize(stmt);
	return colType;
}

std::vector<std::string> Database::GetCols(const std::string& tableName) // way to get cols without execute query
{
	sqlite3_stmt* stmt = nullptr;
	std::vector<std::string> cols;

	std::string sql = "PRAGMA table_info(" + tableName + ");";
	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		Logger::GetInstance().LogMsg(Logger::ERR, "Failed to prepare PRAGMA statement.");
		return cols;
	}

	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const char* colName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); // column name is at index 1
		if (colName)
		{
			cols.push_back(colName);
		}
	}

	sqlite3_finalize(stmt);
	return cols;
}

std::vector<std::string> Database::GetTables() // get tables
{
	std::vector<std::string> tables;
	std::vector<std::string> cols;

	const char* querySQL = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
	auto result = ExecuteQuery(querySQL, cols);
	for (auto& row : result)
	{
		if (!row.empty()) tables.push_back(row[0]);
	}
	return tables;
}


void Database::Close() 
{ 	
	if (db) // cleanup
	{
		sqlite3_close(db);
		db = nullptr;
		errMsg = nullptr;
		Logger::GetInstance().LogMsg(Logger::INFO, "Closed db");
	}
}