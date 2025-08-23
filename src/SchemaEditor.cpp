#include "SchemaEditor.h"

void SchemaEditor::Init(std::shared_ptr<Database> _db)
{
	db = _db;
}

std::string SchemaEditor::ToSQLType(SQLDataType type) 
{
	switch (type)
	{
	case SQLDataType::Blob: return "BLOB";

	case SQLDataType::Integer: return "INTEGER";
		 
	case SQLDataType::Text: return "TEXT";

	case SQLDataType::Double: return "DOUBLE";

	default: return "UNKNOWN";
	}
}

std::string SchemaEditor::OpToString(SQLOperation op) // utility function. only needs to return string for debug
{
	switch (op) // learn all commands/keyword phrases to finish this. for now use only create table
	{
	case SQLOperation::CreateTable: return "CREATE TABLE";

	case SQLOperation::DropTable: return "DROP TABLE";

	case SQLOperation::RenameTable: return "RENAME TABLE";
	}

}


std::vector<std::vector<std::string>> SchemaEditor::GetTableResults(std::string& tableName) // just a get tables 
{
	std::string sql = sqlTemplates[SQLOperation::GetTables];
	QueryResults = db->ExecuteQuery(sql.c_str(), colNames);
	return QueryResults;
		
}
// make each return a value to a globally reference data structure so it auto updates after each crud op
// TODO: Add error handling 
bool SchemaEditor::CreateTable(const std::string& tableName, SQLDataType columnDataType, const std::string& columnName) // creates tables
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::CreateTable];
	std::string _columnDataType = ToSQLType(columnDataType);
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(tableName, columnName, _columnDataType));
	colNames.clear();

	if (!db->ExecuteQuery(sql.c_str(), colNames).empty()) // change to execute sql instead of query
		return true; 
	return false;
}

bool SchemaEditor::DropTable(const std::string& tableName) 
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::DropTable];
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(tableName));
	colNames.clear();
	if (!db->ExecuteQuery(sql.c_str(), colNames).empty())
		return true;
	return false;
}

bool SchemaEditor::RenameTable(const std::string& oldTableName, const std::string& newTableName)
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::RenameTable];
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(oldTableName, newTableName));
	colNames.clear();
	if (!db->ExecuteQuery(sql.c_str(), colNames).empty())
		return true;
	return false;
}


bool SchemaEditor::AddColumn(const std::string& tableName, SQLDataType columnDataType, const std::string& columnName)
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::AddColumn];
	std::string _columnDataType = ToSQLType(columnDataType);
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(tableName, columnName, _columnDataType));
	colNames.clear();
	if (!db->ExecuteQuery(sql.c_str(), colNames).empty())
		return true;
	return false;

}

bool SchemaEditor::RenameColumn(const std::string& tableName, const std::string& oldColumnName, const std::string& newColumnName)
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::RenameColumn];
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(tableName, oldColumnName, newColumnName));
	colNames.clear();
	if (!db->ExecuteQuery(sql.c_str(), colNames).empty())
		return true;
	return false;

}

std::vector<std::vector<std::string>>  SchemaEditor::GetTableData(const std::string& tableName, std::vector<std::string> &_colNames)
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::GetTableData];
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(tableName));
	std::vector<std::vector<std::string>> queryResults;

	queryResults = db->ExecuteQuery(sql.c_str(), _colNames);
	return queryResults;
}

std::vector<std::string> SchemaEditor::GetTables()
{
	return db->GetTables(); 
}

std::vector<std::string> SchemaEditor::GetColNames(const std::string _tableName)
{
	std::vector<std::string> cols = db->GetCols(_tableName);
	return cols;
}

std::string SchemaEditor::getColumnType(sqlite3* _db, const std::string& table, const std::string& column)
{
	std::string type = db->getColumnType(_db, table, column);
	return type;
}

bool SchemaEditor::DeleteData(const std::string& tableName, std::string& clause)
{
	std::string sqlTemplate = sqlTemplates[SQLOperation::DeleteData];
	std::string sql = fmt::vformat(sqlTemplate, fmt::make_format_args(tableName, clause));
	if(db->ExecuteSQL(sql.c_str()))
		return true;
	return false;
}





