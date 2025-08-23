#pragma once
#include <variant>
#include <string>


// no longer needed 
// was going to use this for inserting data but I suck
using SQLValue = std::variant<int, double, std::string, std::uintptr_t>;

enum class SQLOperation
{
	CreateTable, DropTable, RenameTable,
	AddColumn, RemoveColumn, RenameColumn,
	InsertData, UpdateData, DeleteData,
	GetTables, GetTableData
};
// to be converted to string
enum class SQLDataType
{
	Integer, Text, Double, Blob
};