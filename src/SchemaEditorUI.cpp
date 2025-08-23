#include "SchemaEditorUI.h"

// Alot of the ui design is inconsistent
// i was free balling it tbh

void SchemaEditorUI::refresh()
{
	data.tableNames.clear();
	data.colNames.clear();
	data.tableNames = se.GetTables();
	if (!data.tableNames.empty() && data.selectedTableIndex < data.tableNames.size())
	{
		data.queryResults = se.GetTableData(data.tableNames[data.selectedTableIndex], data.colNames);
	}
	else
	{
		data.queryResults.clear();
		data.selectedTableIndex = 0;
	}
	
	//data.colNames = se.GetColNames(data.tableNames[data.selectedTableIndex]);
	data.refreshBuffers(se);
}

void SchemaEditorUI::Init(std::shared_ptr<Database> db, float& _maxWindowWidth, float& _maxWindowHeight)
{
	se.Init(db);
	data.maxWindowWidth = _maxWindowWidth;
	data.maxWindowHeight = _maxWindowHeight;
	
}


void SchemaEditorUI::RenderTable()
{
	if (data.chosenTable.empty()) // checks if a table is chosen
	{
		data.queryResults.clear();
		data.colNames.clear();
		if (!data.tableNames.empty()) // auto selectes first table if applicable 
		{
			data.chosenTable = data.tableNames[0];
			data.queryResults = se.GetTableData(data.chosenTable, data.colNames);

		}
	}

	if (data.chosenTable.empty() || data.colNames.empty())
		return;

	if (ImGui::BeginTable("Data Table", data.colNames.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		ImGui::TableNextRow();
		for (int col = 0; col < data.colNames.size(); col++) // displays a tables contents
		{
			ImGui::TableSetColumnIndex(col);
			ImGui::TextUnformatted(data.colNames[col].c_str());
		}

		for (auto& row : data.queryResults)
		{
			ImGui::TableNextRow();
			for (int col = 0; col < row.size(); col++)
			{
				ImGui::TableSetColumnIndex(col);
				ImGui::TextUnformatted(row[col].c_str());
			}
		}

		ImGui::EndTable();
	}
}

void SchemaEditorUI::RenderTableOpWindow()
{
	// sets size
	ImGui::SetNextWindowSize(ImVec2(data.maxWindowWidth * 1 / 4, data.maxWindowHeight * 1 / 4));
	if (ImGui::Begin("Table Operations##window", &data.openTableOpWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTabBar("Table Operations##tab bar"))
		{
			if (ImGui::BeginTabItem("Create Table##tab"))
			{ 
				data.currentTab = 0; // for tracking to reset buffers when swapping tabs
				ImGui::Text("New Table Name");  // for clean ui
				ImGui::SameLine();
				if (ImGui::InputText("##new table input", data.textBuff1, sizeof(data.textBuff1)))
				{
					data.newTableName = std::string(data.textBuff1); // converts buff to string 
				}

				ImGui::Text("New Column Name");
				ImGui::SameLine();
				if (ImGui::InputText("##new column input", data.textBuff2, sizeof(data.textBuff2)))
				{
					data.newColName = std::string(data.textBuff2); // converts buff to string
				} 

				ImGui::Text("Data Types");
				ImGui::SameLine();
				if (ImGui::Combo("##Data Types", &data.selectedTypeComboIndex, data.dataTypes, IM_ARRAYSIZE(data.dataTypes))) // select data for table to initialized with
				{
					if (data.dataTypes[data.selectedTypeComboIndex] == "Integer")
						data.SQLtype = SQLDataType::Integer;
					else if (data.dataTypes[data.selectedTypeComboIndex] == "Text")
						data.SQLtype = SQLDataType::Text;
					else if (data.dataTypes[data.selectedTypeComboIndex] == "Double")
						data.SQLtype = SQLDataType::Double;
					else if (data.dataTypes[data.selectedTypeComboIndex] == "Blob")
						data.SQLtype = SQLDataType::Blob;
				}

				if (ImGui::Button("Execute##newtable")) 
				{
					se.CreateTable(data.newTableName, data.SQLtype, data.newColName); // creates table and refreshes table list and text buffers
					refresh();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Rename Table##tab"))
			{
				data.currentTab = 1;
				ImGui::Text("Rename");
				ImGui::SameLine();
				if (ImGui::InputText("##rename table input", data.textBuff1, sizeof(data.textBuff1)))
				{
					data.newTableName = std::string(data.textBuff1); // convert buff to string
				}

				ImGui::Text("Select Table");
				ImGui::SameLine();
				ImGui::Combo("##Table Names combo1", &data.selectedTableComboIndex, data.items.data(), (int)data.items.size());

				if (ImGui::Button("Execute##rename table"))
				{
					std::string oldName = data.tableNames[data.selectedTableComboIndex];
					se.RenameTable(oldName, data.newTableName);
					refresh();
				}
				

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Delete Table##tab"))
			{
				data.currentTab = 2;
				ImGui::Text("Select Table");
				ImGui::SameLine();
				ImGui::Combo("##Table Names combo2", &data.selectedTableComboIndex, data.items.data(), (int)data.items.size());

				if (ImGui::Button("Execute##delete table"))
				{
					se.DropTable(data.items[data.selectedTableComboIndex]);
					refresh();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Delete Row"))
			{
				ImGui::Text("Select Table");
				ImGui::SameLine();
				ImGui::Combo("##select table", &data.selectedTableComboIndex, data.items.data(), (int)data.items.size());
				std::string table = data.items[data.selectedTableComboIndex];

				ImGui::Text("Clause");
				ImGui::SameLine();
				ImGui::InputText("##clause input", data.textBuff1, sizeof(data.textBuff1));
				std::string clause = data.textBuff1;

				if (ImGui::Button("Execute##delete row"))
				{
					se.DeleteData(table, clause);
					refresh();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		if (data.currentTab != data.lastTab)
		{
			data.refreshBuffers(se);
			data.lastTab = data.currentTab;
		}
		ImGui::End();
	}
}

void SchemaEditorUI::RenderColumnOpWindow()
{
	ImGui::SetNextWindowSize(ImVec2(data.maxWindowWidth * 1 / 4, data.maxWindowHeight * 1 / 4));

	if (ImGui::Begin("Column Operations##window", &data.openColumnOpWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::BeginTabBar("Column Operations##tab bar"))
		{
			// create column tab
			if (ImGui::BeginTabItem("Create Column##tab"))
			{
				ImGui::Text("Select Table");
				ImGui::SameLine();

				// Check if there are tables
				if (!data.items.empty())
				{
					ImGui::Combo("##Table Name rename cols", &data.selectedTableComboIndex, data.items.data(), (int)data.items.size());
					int safeIndex = std::clamp(data.selectedTableComboIndex, 0, (int)data.items.size() - 1);
					std::string table = data.items[safeIndex];

					ImGui::Text("Data Types");
					ImGui::SameLine();
					if (ImGui::Combo("##Data Types combo", &data.selectedTypeComboIndex, data.dataTypes, IM_ARRAYSIZE(data.dataTypes)))
					{
						if (data.dataTypes[data.selectedTypeComboIndex] == "Integer")
							data.SQLtype = SQLDataType::Integer;
						else if (data.dataTypes[data.selectedTypeComboIndex] == "Text")
							data.SQLtype = SQLDataType::Text;
						else if (data.dataTypes[data.selectedTypeComboIndex] == "Double")
							data.SQLtype = SQLDataType::Double;
						else if (data.dataTypes[data.selectedTypeComboIndex] == "Blob")
							data.SQLtype = SQLDataType::Blob;
					}

					ImGui::Text("New Column Name");
					ImGui::SameLine();
					if (ImGui::InputText("##new column1", data.textBuff1, sizeof(data.textBuff1)))
					{
						data.newColName = std::string(data.textBuff1);
					}

					if (ImGui::Button("Execute## create column"))
					{
						se.AddColumn(table, data.SQLtype, data.newColName);
						refresh();
					}
				}
				else
				{
					ImGui::Text("No tables available.");
				}

				ImGui::EndTabItem();
			}

			// rename column tab
			if (ImGui::BeginTabItem("Rename Column##tab"))
			{
				ImGui::Text("Select Table");
				ImGui::SameLine();

				if (!data.items.empty())
				{
					ImGui::Combo("##Table Name rename cols", &data.selectedTableComboIndex, data.items.data(), (int)data.items.size());
					int safeTableIndex = std::clamp(data.selectedTableComboIndex, 0, (int)data.items.size() - 1);
					std::string table = data.items[safeTableIndex];

					if (data.lastComboIndex != data.selectedTableComboIndex)
					{
						data.lastComboIndex = data.selectedTableComboIndex;
						data.colNames = se.GetColNames(table);
						data.refreshColNames();
					}

					ImGui::Text("Select Column");
					ImGui::SameLine();

					if (!data.colItems.empty())
					{
						ImGui::Combo("##Column Names rename cols", &data.selectedColComboIndex, data.colItems.data(), (int)data.colItems.size());
						int safeColIndex = std::clamp(data.selectedColComboIndex, 0, (int)data.colItems.size() - 1);
						std::string oldCol = data.colItems[safeColIndex];

						ImGui::Text("New Column Name");
						ImGui::SameLine();
						if (ImGui::InputText("##new column2", data.textBuff1, sizeof(data.textBuff1)))
						{
							data.newColName = std::string(data.textBuff1);
						}

						if (ImGui::Button("Execute##col rename"))
						{
							se.RenameColumn(table, oldCol, data.newColName);
							refresh();
						}
					}
					else
					{
						ImGui::Text("No columns available.");
					}
				}
				else
				{
					ImGui::Text("No tables available.");
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}


void SchemaEditorUI::Render(float& _maxWindowWidth, float& _maxWindowHeight)
{
	data.maxWindowWidth = _maxWindowWidth;
	data.maxWindowHeight = _maxWindowHeight;
	data.schemaEditorDims.update(data.maxWindowWidth, data.maxWindowHeight);

	// broke each panel in this module into its own render function and piece them together here
	data.openSchemaChild = ImGui::BeginChild("Schema Editor ##child", data.schemaEditorDims.schemaEditorVec, true);
	if (data.openSchemaChild)
	{
		// banner with buttons to open operations windows
		data.openBanner = ImGui::BeginChild("Banner", data.schemaEditorDims.se_bannerVec, true);
		if (data.openBanner)
		{
			// each button opens window with all related operations in tabs
			// add export option
			if (ImGui::Button("Table Operations", data.schemaEditorDims.se_buttonVec))
			{
				refresh();
				data.openTableOpWindow = !data.openTableOpWindow;
			}
			if(data.openTableOpWindow)
			{
				RenderTableOpWindow();
			}
			
			ImGui::SameLine();
			if (ImGui::Button("Column Operations", data.schemaEditorDims.se_buttonVec))
			{
				refresh();
				data.openColumnOpWindow = !data.openColumnOpWindow;
			}
			if (data.openColumnOpWindow)
			{
				RenderColumnOpWindow();
			}
			if (ImGui::Button("Empty Button"))
			{
			}
			
			ImGui::EndChild();
		}
		else
		{
			Logger::GetInstance().LogMsg(Logger::WARNING, "Banner bar child failed to open");
			ImGui::EndChild();
			
		}

		data.openTableData = ImGui::BeginChild("Table Data", data.schemaEditorDims.se_tableViewVec, true);
		if (data.openTableData)
		{
			RenderTable();
			ImGui::EndChild();
		}
		else
		{
			Logger::GetInstance().LogMsg(Logger::WARNING, "Table View child failed to open");
			ImGui::EndChild();
		}

		ImGui::SameLine();
		data.openTableList = ImGui::BeginChild("Table List", data.schemaEditorDims.se_tableListVec, true);
		if (data.openTableList)
		{
			// refresh tables on click instead of every frame
			if (ImGui::Button("Refresh Tables"))
			{
				data.tableNames.clear();
				data.colNames.clear();
				data.tableNames = se.GetTables();
				data.queryResults = se.GetTableData(data.tableNames[data.selectedTableIndex], data.colNames);
			}
			ImGui::Separator();
			for (int i = 0; i < data.tableNames.size(); ++i)
			{
				data.tableSelected = (data.chosenTable == data.tableNames[i]);
				// loads selected tables contents when clicked
				if (ImGui::Selectable(data.tableNames[i].c_str(), data.tableSelected))
				{
					data.chosenTable = data.tableNames[i]; // Update selection
					data.selectedTableIndex = i;
					data.tableNames.clear();
					data.colNames.clear();
					data.tableNames = se.GetTables();
					data.queryResults = se.GetTableData(data.tableNames[data.selectedTableIndex], data.colNames);
				}
			}
			ImGui::EndChild();
		}
		else
		{
			Logger::GetInstance().LogMsg(Logger::WARNING, "table list child failed to open");
			ImGui::EndChild();
		}

		ImGui::EndChild();
	}
	else
	{
		Logger::GetInstance().LogMsg(Logger::WARNING, "Schema window failed to open");
		ImGui::EndChild();
	}

}
	
