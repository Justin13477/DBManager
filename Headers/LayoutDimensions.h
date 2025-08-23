#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

// for control over app layout
struct fileDialogDims
{
	// file dialog

	float fileDialogWidth = 0;
	float fileDialogHeight = 0;
	ImVec2 fileDialogVec;
	void update(float maxWindowWidth, float maxWindowHeight)
	{
		fileDialogWidth = maxWindowWidth;
		fileDialogHeight = maxWindowHeight;
		fileDialogVec = { fileDialogWidth, fileDialogHeight };
	}
};

struct loggerUIDims
{
	// logger ui
	float loggerUIWidth = 0;
	float loggerUIHeight = 0;
	ImVec2 LoggerUIVec;
	void update(float maxWindowWidth, float maxWindowHeight)
	{
		loggerUIWidth = maxWindowWidth;
		loggerUIHeight = maxWindowHeight * 1/4;
		LoggerUIVec = { loggerUIWidth, loggerUIHeight };
	}
};

struct queryConsoleDims
{
	// query console
	// entire section
	float queryConsoleWidth;
	float queryConsoleHeight;
	ImVec2 queryConsoleVec;

	// results
	float qc_resultsWidth = queryConsoleWidth / 2.0f;
	float qc_resultsHeight = queryConsoleHeight * 2/3;
	ImVec2 qc_resultsVec;

	// console
	float qc_consoleWidth = queryConsoleWidth / 2.0f;
	float qc_consoleHeight = queryConsoleHeight * 1/3;
	ImVec2 qc_consoleVec;


	void update(float maxWindowWidth, float maxWindowHeight)
	{
		queryConsoleWidth = maxWindowWidth / 2.0f;
		queryConsoleHeight = maxWindowHeight * 2/3;

		qc_resultsWidth = queryConsoleWidth;
		qc_resultsHeight = queryConsoleHeight * 2/3;

		qc_consoleWidth = queryConsoleWidth;
		qc_consoleHeight = queryConsoleHeight * 1/3;

		queryConsoleVec = { queryConsoleWidth, queryConsoleHeight };
		qc_resultsVec = { qc_resultsWidth, qc_resultsHeight };
		qc_consoleVec = { qc_consoleWidth, qc_consoleHeight };

	}
};

struct schemaEditorDims
{
	// schema editor
	// entire section
	float schemaEditorWidth;
	float schemaEditorHeight;
	ImVec2 schemaEditorVec;

	// banner
	float se_bannerWidth = schemaEditorWidth;
	float se_bannerHeight = schemaEditorHeight / 8.0f;
	ImVec2 se_bannerVec;

	// table list
	float se_tableListWidth = schemaEditorWidth / 6.0f;
	float se_tableListHeight = schemaEditorHeight / 1.50f;
	ImVec2 se_tableListVec;

	// table view
	float se_tableViewWidth = schemaEditorWidth / 2.0f;
	float se_tableViewHeight = schemaEditorHeight / 2.0f;
	ImVec2 se_tableViewVec;

	// button dimensions
	float se_buttonWidth = se_bannerWidth * 0.25;
	float se_buttonHeight = se_bannerHeight;
	ImVec2 se_buttonVec;


	void update(float maxWindowWidth, float maxWindowHeight)
	{
		schemaEditorWidth = maxWindowWidth / 2.0f;
		schemaEditorHeight = maxWindowHeight * 2.0f / 3.0f;

		se_buttonWidth = se_bannerWidth * 0.25;
		se_buttonHeight = se_bannerHeight;
		se_buttonVec = { se_buttonWidth, se_buttonHeight };

		schemaEditorVec = { schemaEditorWidth, schemaEditorHeight };

		se_bannerHeight = schemaEditorHeight / 6.0f;
		se_bannerWidth = schemaEditorWidth;
		se_bannerVec = { se_bannerWidth, se_bannerHeight };

		// bottom height = remaining height after banner
		float bottomHeight = schemaEditorHeight - se_bannerHeight;

		se_tableListWidth = schemaEditorWidth * 0.25f;
		se_tableViewWidth = schemaEditorWidth - se_tableListWidth;

		se_tableListHeight = bottomHeight;
		se_tableViewHeight = bottomHeight;

		se_tableListVec = { se_tableListWidth, se_tableListHeight };
		se_tableViewVec = { se_tableViewWidth, se_tableViewHeight };

		se_buttonWidth = se_bannerWidth * 0.25;
		se_buttonHeight = se_bannerHeight;
	}
};