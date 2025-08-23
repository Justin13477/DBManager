#pragma once
#include "SchemaEditor.h"
#include "Database.h"
#include "Window.h"
#include "LayoutDimensions.h"
#include "core_types.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class SchemaEditorUI
{
private:
	SchemaEditor se;


	struct data
	{
		// include everyting
		// this is a skill issue 
		// extremely inconsistent and hard to make since of
		// need to absolutely refactor this and make consistent

		std::vector<std::string> tableNames;
		std::vector<std::string> colNames;
		std::vector<std::vector<std::string>> queryResults;
		std::vector<SQLValue> insertValues;
		const char* dataTypes[4] = { "Integer", "Text", "Double", "Blob" };
		std::vector<const char*> tablesCombo;

		int currentTab = 0;
		int lastTab = 0;

		int selectedTableComboIndex = 0;
		int selectedColComboIndex = 0;
		int selectedTypeComboIndex = 0;

		int selectedIndex = 0;
		int selectedIndex2 = 0;

		int lastComboIndex = -1;

		std::string newTableName;
		std::string newColName;
		SQLDataType SQLtype;

		static const size_t buffSize = 256;
		char textBuff1[buffSize] = "";
		char textBuff2[buffSize] = "";



		std::vector<const char*> items; // temp c array
		std::vector<const char*> colItems;

		std::string chosenTable;

		float maxWindowWidth;
		float maxWindowHeight;


		schemaEditorDims schemaEditorDims;

		// open master child window
		bool openSchemaChild;

		// open child windows
		bool openBanner;
		bool openTableList;
		bool openTableData;

		// open operation windows
		bool openTableOpWindow;
		bool openColumnOpWindow;
		bool openInsertOpWindow;

		// for selecting 
		bool tableSelected;
		bool typeSelected;

		// to keep table index
		int selectedTableIndex;

		
		void refreshColNames()
		{
			selectedColComboIndex = 0;
			colItems.clear();
			for (const auto& str : colNames)
			{
				colItems.push_back(str.c_str());
			}
		}

		void refreshBuffers(SchemaEditor &se)
		{
			
			newTableName.clear();
			newColName.clear();
			memset(textBuff1, 0, sizeof(textBuff1));
			memset(textBuff2, 0, sizeof(textBuff2));

			items.clear();
			colItems.clear();

			//colNames = se.GetColNames(tableNames[selectedTableIndex]);
			items.reserve(tableNames.size()); // reserve space

			for (const auto& str : tableNames) // fills temp array for combo
			{
				items.push_back(str.c_str());
			}

			for (const auto& str : colNames)
			{
				colItems.push_back(str.c_str());
			}


			selectedIndex = (items.empty() ? -1 : 0);
			selectedIndex2 = (colItems.empty() ? -1 : 0);

			lastComboIndex = 0;

			selectedTableComboIndex = (items.empty() ? -1 : 0);
			selectedColComboIndex = (colItems.empty() ? -1 : 0);
			selectedTypeComboIndex = 0;

			Logger::GetInstance().LogMsg(Logger::DEBUG, "Buffers refreshed");
		}
	} data;
	

public:
	void Init(std::shared_ptr<Database> db, float &_maxWindowWidth, float &_maxWindowHeight);
	void Render(float &_maxWindowWidth, float &_maxWindowHeight);
	void refresh();

	void RenderTable();
	void RenderTableOpWindow();
	void RenderColumnOpWindow();
	

};