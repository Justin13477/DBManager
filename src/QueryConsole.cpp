#include <QueryConsole.h>

void QueryConsole::Init(float _windowWidth, float _windowHeight)
{
	maxWindowWidth = _windowWidth;
	maxWindowHeight = _windowHeight;


}
void QueryConsole::Render(std::shared_ptr<Database> db, float& _maxWindowWidth, float& _maxWindowHeight)
{
	maxWindowWidth = _maxWindowWidth;
	maxWindowHeight = _maxWindowHeight;
	queryConsoleDims.update(maxWindowWidth, maxWindowHeight); // update dimensions
	bool openQueryChild = ImGui::BeginChild("Query Console ##child", queryConsoleDims.queryConsoleVec, true); 
		
	if(openQueryChild)
	{
		bool openConsoleChild = ImGui::BeginChild("Console Panel", queryConsoleDims.qc_consoleVec); 
			
		if (openConsoleChild)

		{
			ImGui::Text("Query Console");
			ImGui::Separator();
			ImGui::InputTextMultiline("###Query Console", buffer, sizeof(buffer)); 

			if (ImGui::Button("Execute")) 
			{
				const char* sql = buffer;
				lastQueryResults.clear();
				lastQueryResults = db->ExecuteQuery(sql, colNames);

			}

			ImGui::EndChild();
		}
		else
		{
			ImGui::EndChild();
			Logger::GetInstance().LogMsg(Logger::ERR, "Console child failed to open");
		}
		bool openResultsChild = ImGui::BeginChild("Results Panel", queryConsoleDims.qc_resultsVec, true);
			
		if (openResultsChild)
		{
			ImGui::Text("Query Results");
			if (!lastQueryResults.empty() && !colNames.empty())
			{

				if (ImGui::BeginTable("Results", colNames.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
				{
					for (int i = 0; i < colNames.size(); i++)
					{
						ImGui::TableSetupColumn(colNames[i].c_str()); // displays vector<vecor<string>> of results from previous query

					}
					ImGui::TableHeadersRow();

					for (auto& row : lastQueryResults)
					{
						ImGui::TableNextRow();
						for (int col = 0; col < colNames.size(); col++)
						{
							ImGui::TableSetColumnIndex(col);

							const char* cellText = (col < (int)row.size()) ? row[col].c_str() : "";
							ImGui::TextUnformatted(cellText);

						}

					}
					ImGui::EndTable();
				}

			}
			ImGui::EndChild(); // end results
		}
		else
		{
			ImGui::EndChild(); // end results 
			Logger::GetInstance().LogMsg(Logger::ERR, "Failed to open results child ");
		}
		ImGui::EndChild(); // end query console
	}
	else // end query console
	{
		ImGui::EndChild();
		Logger::GetInstance().LogMsg(Logger::ERR, "Query Console child failed to open");

	}
}



