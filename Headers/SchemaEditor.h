#pragma once
#define FMT_HEADER_ONLY
#include "Logger.h"
#include "Database.h"
#include <vector>
#include <map>
#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include "core_types.h"



class SchemaEditor
{
public:
	void Init(std::shared_ptr<Database> _db);
	// enums
	// for mapping
	

private:

	std::shared_ptr<Database> db;

	// for gui to read
	std::vector<std::string> TableNames;
	std::vector<std::string> colNames;
	// for displaying crud operations
	std::vector<std::vector<std::string>> QueryResults;

	std::string ToSQLType(SQLDataType type); // to pass enum to string format
	std::string OpToString(SQLOperation op); // util function
	

	// to retrieve easily
	// grab copy and modify it
	std::map<SQLOperation, std::string> sqlTemplates = // for mapping what sql operation to use useful abstraction when interacting with the db
	{
		// set up all functions to accept the correct parameters
		{SQLOperation::CreateTable, "CREATE TABLE IF NOT EXISTS {} ({} {} PRIMARY KEY AUTOINCREMENT);"},
		{SQLOperation::DropTable, "DROP TABLE IF EXISTS {};"}, 
		{SQLOperation::RenameTable, "ALTER TABLE {} RENAME TO {};"},
		{SQLOperation::AddColumn, "ALTER TABLE {} ADD COLUMN {} {};"},
		{SQLOperation::RenameColumn, "ALTER TABLE {} RENAME COLUMN {} TO {};"},
		{SQLOperation::InsertData, "INSERT INTO {} ({}) VALUES ({});"}, // not implemented
		{SQLOperation::UpdateData, "UPDATE {} SET {} = {} WHERE {};"}, // not implemented
		{SQLOperation::DeleteData, "DELETE FROM {} WHERE {};"}, // not implemented
		{SQLOperation::GetTables, "SELECT name FROM sqlite_master WHERE type='table';"},
		{SQLOperation::GetTableData, "SELECT * FROM {}"}
	};



public:
	
	// only bind literal raw data via sql_bind functions
	// prone to sql injection
	std::vector<std::vector<std::string>> GetTableResults(std::string& tableName);

	bool CreateTable(const std::string& tableName, SQLDataType columnDataType, const std::string& columnName);
	bool DropTable(const std::string& tableName);
	bool RenameTable(const std::string& oldTableName, const std::string& newTableName);


	bool AddColumn(const std::string& tableName, SQLDataType columnDataType, const std::string& columnName);

	// not real need a work around
	//int RemoveColumn(std::string& tableName, std::string& columnName);
	bool RenameColumn(const std::string& tableName, const std::string& oldColumnName, const std::string& newColumnName);

	// new to pass values by binding


	bool DeleteData(const std::string& tableName, std::string& clause);

	std::vector<std::vector<std::string>> GetTableData(const std::string& tableName, std::vector<std::string> &_colNames);

	std::vector<std::string> GetTables();

	std::vector<std::string> GetColNames(const std::string _tableName);

	std::string getColumnType(sqlite3* _db, const std::string& table, const std::string& column);

	sqlite3* getDatabase() { return db->getDatabase(); }

	std::vector<std::vector<std::string>> GetQueryResults() { return QueryResults;  }

};