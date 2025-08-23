#include "FileDialog.h"

void FileDialog::Init(float& _maxWindowWidth, float& _maxWindowHeight)
{
	maxWindowWidth = _maxWindowWidth;
	maxWindowHeight = _maxWindowHeight;
}

void FileDialog::NavigateTo(const std::filesystem::path& path)
{
	if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) // after selecting path update state
	{
		fileChosen = false;
		currentDir = path;
		chosenPath.clear();
	}

}

void FileDialog::RefreshDir()
{
	entries.clear();
	for (auto& entry : std::filesystem::directory_iterator(currentDir))
	{
		entries.push_back(entry); // refresh dir list after each click
	}

	std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
		if (a.is_directory() != b.is_directory())
			return a.is_directory() > b.is_directory();
		return a.path().filename().string() < b.path().filename().string(); // sort directorys 
		});

}




void FileDialog::Render(float &_maxWindowWidth, float &_maxWindowHeight)
{
	maxWindowWidth = _maxWindowWidth;
	maxWindowHeight = _maxWindowHeight;
	fileDialogDims.update(maxWindowWidth, maxWindowHeight); // update widget dimensions

	ImGui::SetNextWindowSize(ImVec2(fileDialogDims.fileDialogVec.x, fileDialogDims.fileDialogVec.y), ImGuiCond_Always);
	ImGui::Begin("File Dialog");

	if(ImGui::Button("Parent Path"))
	{
		NavigateTo(currentDir.parent_path()); // ascend to parent dir
	}

	ImGui::Text("Current Directory : %s", currentDir.string().c_str()); 

	ImGui::Separator();

	RefreshDir();

	for (auto& entry : entries) // on click decides if it is a valid file or a dir 
	{
		std::string name = entry.path().filename().string();
		if (entry.is_directory())
		{
			if (ImGui::Selectable((name + "/").c_str()))
			{
				NavigateTo(entry.path()); // navigates to that dir
			}
		}
		else if (entry.path().extension() == ".db") // exists with file path
		{
			if (ImGui::Selectable(name.c_str()))
			{
				chosenPath = entry;
				fileChosen = true;

			}
		}
	}


	ImGui::End();
}