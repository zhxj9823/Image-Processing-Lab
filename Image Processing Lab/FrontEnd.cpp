#define _CRT_SECURE_NO_WARNINGS

#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
//#include <cstdio>  // vsnprintf, sscanf, printf
//#include <cstdlib> // nullptr, malloc, free, atoi
#include <cstring>
#include <windows.h>
//#include <cstdlib>
#include <iostream>
#include <commdlg.h>
#include <vector>
#include <string>

static void GetLocation(char *open_file_direction);
static void ShowImage(char *open_file_direction, bool *p_open);
static void SaveImage(char *save_file_direction, bool *p_open);
std::string CreateTempImage(const char *file_direction, const char *new_file_direction);
static void SetGrayScale(std::string current_file, bool *p_open);
static void Change_YUV(std::string current_file);
static void Change_RGB(std::string current_file);
static void Change_HSL(std::string current_file);
static void AutoBinarization(std::string current_file);
std::string LocalBinarization(std::string current_file);
std::string ManuallyBinarization(std::string current_file);
static void Erose(std::string current_file);
static void Dilate(std::string current_file);
static void Close(std::string current_file);
static void Open(std::string current_file);
std::string Rotate(std::string current_file);
std::string Scale(std::string current_file);
std::string Translate(std::string current_file);
std::string Shear(std::string current_file);
static void RGBHistogram(std::string current_file);
static void HSLHistogram(std::string current_file);
static void RGBHistogram_E(std::string current_file);
static void HSLHistogram_E(std::string current_file);
std::string Logarithmic(std::string current_file);
std::string Mean(std::string current_file);
std::string Edge(std::string current_file);
std::string Lap(std::string current_file);
std::string Bilateral(std::string current_file);
static void ShowHelp();
static void ShowAbout();

void ImGui::MyShow(bool *show_window)
{
	static bool open_file = false;
	static bool save_file = false;
	static bool show_image = false;
	static bool save_image = false;
	static bool set_grayscale = false;
	static bool enable_YUV = false;
	static bool enable_RGB = false;
	static bool enable_HSL = false;
	static bool enable_auto_binarization = false;
	static bool enable_local_binarization = false;
	static bool enable_manually_binarization = false;
	static bool enable_erose = false;
	static bool enable_dilate = false;
	static bool enable_close = false;
	static bool enable_open = false;
	static bool enable_rotate = false;
	static bool enable_scale = false;
	static bool enable_translate = false;
	static bool enable_shear = false;
	static bool enable_RGB_Histogram = false;
	static bool enable_HSL_Histogram = false;
	static bool enable_RGB_Histogram_E = false;
	static bool enable_HSL_Histogram_E = false;
	static bool enable_logarithmic = false;
	static bool enable_mean = false;
	static bool enable_edge = false;
	static bool enable_lap = false;
	static bool enable_bilateral = false;
	static bool enable_help = false;
	static bool enable_about = false;

	static char open_file_direction[1024];
	static char save_file_direction[1024];
	std::string current_file;
	std::vector<std::string> temp_images;

	if (open_file)
	{
		GetLocation(open_file_direction);
		if (open_file_direction[0] != 0)
		{
			std::cout << open_file_direction << std::endl;
			current_file = CreateTempImage(open_file_direction, "temp.bmp");
			temp_images.push_back(current_file);
			open_file = false;
			show_image = true;
		}
	}

	if (save_file)
	{
		GetLocation(open_file_direction);
		std::cout << save_file_direction << std::endl;
		open_file = false;
		save_image = true;
	}

	if (show_image)
		ShowImage(open_file_direction, &show_image);
	if (save_image)
		SaveImage(save_file_direction, &save_image);
	if (set_grayscale)
		SetGrayScale(current_file, &set_grayscale);
	if (enable_YUV)
		Change_YUV(current_file);
	if (enable_RGB)
		Change_RGB(current_file);
	if (enable_HSL)
		Change_HSL(current_file);
	if (enable_auto_binarization)
		AutoBinarization(current_file);
	if (enable_local_binarization)
	{
		std::string temp_file_name = LocalBinarization(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_manually_binarization)
	{
		std::string temp_file_name = ManuallyBinarization(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_erose)
		Erose(current_file);
	if (enable_dilate)
		Dilate(current_file);
	if (enable_close)
		Close(current_file);
	if (enable_open)
		Open(current_file);
	if (enable_rotate)
	{
		std::string temp_file_name = Rotate(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_scale)
	{
		std::string temp_file_name = Scale(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_translate)
	{
		std::string temp_file_name = Translate(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_shear)
	{
		std::string temp_file_name = Shear(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_RGB_Histogram)
		RGBHistogram(current_file);
	if (enable_HSL_Histogram)
		HSLHistogram(current_file);
	if (enable_RGB_Histogram_E)
		RGBHistogram_E(current_file);
	if (enable_HSL_Histogram_E)
		HSLHistogram_E(current_file);
	if (enable_logarithmic)
	{
		std::string temp_file_name = Logarithmic(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_mean)
	{
		std::string temp_file_name = Mean(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_edge)
	{
		std::string temp_file_name = Edge(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_lap)
	{
		std::string temp_file_name = Lap(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_bilateral)
	{
		std::string temp_file_name = Bilateral(current_file);
		temp_images.push_back(temp_file_name);
	}
	if (enable_help)
		ShowHelp();
	if (enable_about)
		ShowAbout();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File", "Alt+F"))
		{
			ImGui::MenuItem("(File)", nullptr, false, false);
			if (ImGui::MenuItem("New..", "Ctrl+N"))
			{
			}
			if (ImGui::MenuItem("Open", "Ctrl+O", &open_file))
			{
			}
			if (ImGui::MenuItem("Save", "Ctrl+S", &save_file))
			{
			}
			if (ImGui::MenuItem("Save As..", "Ctrl+Shift+S"))
			{
			}
			if (ImGui::MenuItem("Quit", "Alt+F4"))
			{
				*show_window = 0;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("(Edit)", nullptr, false, false);
			if (ImGui::MenuItem("Undo", "CTRL+Z"))
			{
			}
			if (ImGui::MenuItem("Redo", "CTRL+Y"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X"))
			{
			}
			if (ImGui::MenuItem("Copy", "CTRL+C"))
			{
			}
			if (ImGui::MenuItem("Paste", "CTRL+V"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Image"))
		{
			ImGui::MenuItem("(Image)", nullptr, false, false);
			if (ImGui::MenuItem("Grayscale", nullptr, &set_grayscale))
			{
			}			
			if (ImGui::BeginMenu("Adjust"))
			{
				ImGui::MenuItem("YUV", nullptr, &enable_YUV);
				ImGui::MenuItem("RGB", nullptr, &enable_RGB);
				ImGui::MenuItem("HSL", nullptr, &enable_HSL);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Binarization"))
			{
				ImGui::MenuItem("Auto binarization", nullptr, &enable_auto_binarization);
				ImGui::MenuItem("Auto local-adaptive binarization", nullptr, &enable_local_binarization);
				ImGui::MenuItem("Manually binarization", nullptr, &enable_manually_binarization);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Morphological Operation"))
			{
				ImGui::MenuItem("Erose", nullptr, &enable_erose);
				ImGui::MenuItem("Dilate", nullptr, &enable_dilate);
				ImGui::MenuItem("Close", nullptr, &enable_close);
				ImGui::MenuItem("Open", nullptr, &enable_close);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Geometric Operation"))
			{
				ImGui::MenuItem("Rotate", nullptr, &enable_rotate);
				ImGui::MenuItem("Scale", nullptr, &enable_scale);
				ImGui::MenuItem("Translate", nullptr, &enable_translate);
				ImGui::MenuItem("Shear", nullptr, &enable_shear);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Histogram"))
			{
				if (ImGui::MenuItem("RGB Histogram", nullptr, &enable_RGB_Histogram))
				{
				}
				if (ImGui::MenuItem("HSL Histogram", nullptr, &enable_HSL_Histogram))
				{
				}
				ImGui::Separator();
				if (ImGui::MenuItem("RGB Histogram Equalization", nullptr, &enable_RGB_Histogram_E))
				{
				}
				if (ImGui::MenuItem("HSL Histogram Equalization", nullptr, &enable_HSL_Histogram_E))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Image Logarithmic", nullptr, &enable_logarithmic))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Filters"))
		{
			ImGui::MenuItem("(Filters)", nullptr, false, false);
			if (ImGui::MenuItem("Mean Filter", nullptr, &enable_mean))
			{
			}
			if (ImGui::MenuItem("Edge detection", nullptr, &enable_edge))
			{
			}
			if (ImGui::MenuItem("Laplaician Enhancement", nullptr, &enable_lap))
			{
			}
			if (ImGui::MenuItem("Bilateral Filter", nullptr, &enable_bilateral))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGui::MenuItem("(Options)", nullptr, false, false);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("(Window)", nullptr, false, false);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("(Help)", nullptr, false, false);
			if (ImGui::MenuItem("User Help", nullptr, &enable_help))
			{
			}
			if (ImGui::MenuItem("About", nullptr, &enable_about))
			{
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void GetLocation(char *open_file_direction)
{
	OPENFILENAME ofn;
	static char szFile[256];
	static char szFileTitle[256];
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = "Picture File(*.bmp)\0*.bmp;\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileName(&ofn))
	{
		strcpy(open_file_direction, szFileTitle);
	}
}

void ShowImage(char *open_file_direction, bool *p_open)
{
	int image_width, image_height;
	unsigned char *image_data = stbi_load(open_file_direction, &image_width, &image_height, nullptr, 4);

	// Turn the RGBA pixel data into an OpenGL texture:
	GLuint my_opengl_texture;
	glGenTextures(1, &my_opengl_texture);
	glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::Begin(open_file_direction);
	ImGui::Text(open_file_direction);
	// Now that we have an OpenGL texture, assuming our imgui rendering function (imgui_impl_xxx.cpp file) takes GLuint as ImTextureID, we can display it:
	ImGui::Image((void *)(intptr_t)my_opengl_texture, ImVec2(image_width, image_height));
	ImGui::End();
}

static void SetGrayScale(std::string current_file, bool *p_open)
{
	return;
}

static void SaveImage(char *save_file_direction, bool *p_open)
{
	return;
}

std::string CreateTempImage(const char *file_direction, const char *new_file_direction)
{
	return std::string();
}

static void Change_YUV(std::string current_file)
{
	float y, u, v;
	ImGui::Begin("YUV Change");

	ImGui::SliderFloat("Y", &y, -255.0f, 255.0f);
	ImGui::SliderFloat("U", &u, -255.0f, 255.0f);
	ImGui::SliderFloat("V", &v, -255.0f, 255.0f);

	//Do something--- parameter:YUV current_file

	ImGui::End();
}

static void Change_RGB(std::string current_file)
{	
	float r, g, b;
	ImGui::Begin("RGB Change");

	ImGui::SliderFloat("R", &r, 0.0f, 255.0f);
	ImGui::SliderFloat("G", &g, 0.0f, 255.0f);
	ImGui::SliderFloat("B", &b, 0.0f, 255.0f);

	//Do something

	ImGui::End();
}

static void Change_HSL(std::string current_file)
{	
	float h, s, l;
	ImGui::Begin("RGB Change");

	ImGui::SliderFloat("H", &h, 0.0f, 255.0f);
	ImGui::SliderFloat("S", &s, 0.0f, 255.0f);
	ImGui::SliderFloat("L", &l, 0.0f, 255.0f);	

	ImGui::End();
}

static void AutoBinarization(std::string current_file)
{
	
}

std::string LocalBinarization(std::string current_file)
{
	std::string temp = CreateTempImage(current_file.c_str(), "local_binarization_temp.bmp");

	// Do something

	int image_width, image_height;
	unsigned char *image_data = stbi_load(current_file.c_str(), &image_width, &image_height, nullptr, 4);

	// Turn the RGBA pixel data into an OpenGL texture:
	GLuint my_opengl_texture;
	glGenTextures(1, &my_opengl_texture);
	glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::Begin(("LocalBinarization:"+ current_file).c_str());
	ImGui::Text(("LocalBinarization:" + current_file).c_str());
	// Now that we have an OpenGL texture, assuming our imgui rendering function (imgui_impl_xxx.cpp file) takes GLuint as ImTextureID, we can display it:
	ImGui::Image((void *)(intptr_t)my_opengl_texture, ImVec2(image_width, image_height));
	ImGui::End();
	return std::string();
}

std::string ManuallyBinarization(std::string current_file)
{
	return std::string();
}

static void Erose(std::string current_file)
{
	
}

static void Dilate(std::string current_file)
{
	
}

static void Close(std::string current_file)
{
	
}

static void Open(std::string current_file)
{
	
}

std::string Rotate(std::string current_file)
{
	return std::string();
}

std::string Scale(std::string current_file)
{
	return std::string();
}

std::string Translate(std::string current_file)
{
	return std::string();
}

std::string Shear(std::string current_file)
{
	return std::string();
}

static void RGBHistogram(std::string current_file)
{

}

static void HSLHistogram(std::string current_file)
{

}

static void RGBHistogram_E(std::string current_file)
{

}

static void HSLHistogram_E(std::string current_file)
{

}

std::string Logarithmic(std::string current_file)
{
	return std::string();
}

std::string Mean(std::string current_file)
{
	return std::string();
}

std::string Edge(std::string current_file)
{
	return std::string();
}

std::string Lap(std::string current_file)
{
	return std::string();
}

std::string Bilateral(std::string current_file)
{
	return std::string();
}

static void ShowHelp()
{
	ImGui::SetNextWindowPos(ImVec2(640,360), ImGuiCond_FirstUseEver);
	ImGui::Begin("Help");
	ImGui::Text("This is a help");	
	ImGui::End();
}

static void ShowAbout()
{
	ImGui::SetNextWindowPos(ImVec2(640, 360), ImGuiCond_FirstUseEver);
	ImGui::Begin("About");
	ImGui::Text("About:");
	ImGui::End();
}
