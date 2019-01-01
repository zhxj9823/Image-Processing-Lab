#include "imgui.h"
#include <cstdio>          // vsnprintf, sscanf, printf
#include <cstdlib>         // NULL, malloc, free, atoi
#include <cstring>
#include <windows.h>
#include <cstdlib>
#include <iostream>

static void GetLocation(char* open_file_direction);
static void ShowGrayScale(bool* p_open);

void ImGui::MyShow(bool *show_window)
{
	static bool open_file = false;
	static bool show_grayscale = false;

	static char open_file_direction[1024];

	if (open_file)
	{
		GetLocation(open_file_direction);
		std::cout << open_file_direction << std::endl;
		open_file = false;
	}
	if (show_grayscale) ShowGrayScale(&show_grayscale);
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File","Alt+F"))
		{
			if (ImGui::MenuItem("New..","Ctrl+N")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O",&open_file)) {}			
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..","Ctrl+Shift+S")) {}			
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				*show_window = 0;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y")) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Image"))
		{
			bool show = true;
			if (ImGui::MenuItem("Grayscale", NULL, &show_grayscale)){}
			if (ImGui::MenuItem("Binarization"))
			{
				
			}
			if (ImGui::MenuItem("Adjust"))
			{
				
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("User Help")) {}
			if (ImGui::MenuItem("About", "CTRL+Y")) {} 
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void GetLocation(char* open_file_direction)
{
	wchar_t  szFilePath[1024] = { 0 };
	OPENFILENAMEW openFileName = { 0 };
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.nMaxFile = 1024;//这个必须设置，不设置的话不会出现打开文件对话框
	openFileName.lpstrFilter = L"文本文件（*.txt*）\0*.txt\0任何文件（*.*）\0*.*\0";
	openFileName.lpstrInitialDir = L"./";
	openFileName.lpstrFile = szFilePath;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//Conversion
	size_t i;
	wcstombs_s(&i, open_file_direction, (size_t)1024, szFilePath, (size_t)1024);
}

static void ShowGrayScale(bool* p_open)
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::Begin("Grayscale");
	ImGui::Text("Grayscale");
	ImGui::End();
}