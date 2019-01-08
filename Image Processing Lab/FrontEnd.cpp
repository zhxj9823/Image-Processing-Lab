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

class AdjustColor
{
public:
	void virtual render(std::string filename, bool *p_open)
	{
		ImGui::Begin((name1+name2+name3+"Change").c_str(), p_open);
		ImGui::SliderFloat(name1.c_str(), &a, -255.0f, 255.0f);
		ImGui::SliderFloat(name2.c_str(), &b, -255.0f, 255.0f);
		ImGui::SliderFloat(name3.c_str(), &c, -255.0f, 255.0f);

		apply(filename, a, b, c);

		ImGui::End();
	}
	void virtual apply(std::string filename, double a,double b,double c) = 0;
protected:
	float a, b, c;
	std::string name1,name2,name3;
};

class AdjustRGB:public AdjustColor
{
public:
	AdjustRGB(std::string n1, std::string n2, std::string n3)
	{
		name1 = n1;
		name2 = n2;
		name3 = n3;
		a = b = c = 0;
	}
	void apply(std::string filename, double a,double b,double c);
};

void AdjustRGB::apply(std::string filename, double a,double b,double c)
{
	
}

class AdjustHSL :public AdjustColor
{
public:
	AdjustHSL(std::string n1, std::string n2, std::string n3)
	{
		name1 = n1;
		name2 = n2;
		name3 = n3;
		a = b = c = 0;
	}
	void apply(std::string filename, double a, double b, double c);
};

void AdjustHSL::apply(std::string filename, double a, double b, double c)
{

}

class AdjustYUV :public AdjustColor
{
public:
	AdjustYUV(std::string n1, std::string n2, std::string n3)
	{
		name1 = n1;
		name2 = n2;
		name3 = n3;
		a = b = c = 0;
	}
	void apply(std::string filename, double a, double b, double c);
};

void AdjustYUV::apply(std::string filename, double a, double b, double c)
{

}

static void GetOpenFileLocation(char *open_file_direction);
static void GetSaveFileLocation(char *open_file_direction);
static void ShowImage(char *open_file_direction, bool *p_open);
static void SaveImage(char *save_file_direction, bool *p_open);
static void CreateTempImage(const char *file_direction, const char *new_file_direction);
static void GrayScale(std::string current_file, bool *p_open);
static void AutoBinarization(std::string current_file, bool *p_open);
static void LocalBinarization(std::string current_file, bool *p_open);
static void ManuallyBinarization(std::string current_file, bool *p_open);
static void Erose(std::string current_file, bool *p_open);
static void Dilate(std::string current_file, bool *p_open);
static void Close(std::string current_file, bool *p_open);
static void Open(std::string current_file, bool *p_open);
static void Rotate(std::string current_file, bool *p_open);
static void Scale(std::string current_file, bool *p_open);
static void Translate(std::string current_file, bool *p_open);
static void Shear(std::string current_file, bool *p_open);
static void RGBHistogram(std::string current_file, bool *p_open);
static void HSLHistogram(std::string current_file, bool *p_open);
static void RGBHistogram_E(std::string current_file, bool *p_open);
static void HSLHistogram_E(std::string current_file, bool *p_open);
static void Logarithmic(std::string current_file, bool *p_open);
static void Mean(std::string current_file, bool *p_open);
static void Edge(std::string current_file, bool *p_open);
static void Lap(std::string current_file, bool *p_open);
static void Bilateral(std::string current_file, bool *p_open);
static void ShowHelp(bool *p_open);
static void ShowAbout(bool *p_open);

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

	AdjustRGB RGBAdjuster("R", "G", "B");
	AdjustHSL HSLAdjuster("H", "S", "L");
	AdjustYUV YUVAdjuster("Y", "U", "V");

	static int opened = 0;
	if (!opened && open_file)
	{
		GetOpenFileLocation(open_file_direction);
		
		if (open_file_direction[0] != 0)
		{
			std::cout << open_file_direction << std::endl;
			// current_file = CreateTempImage(open_file_direction, "temp.bmp");
			temp_images.push_back(current_file);
			open_file = false;
			show_image = true;
			opened = 1;
		}
	}
	static int saved = 0;
	if (!saved && save_file)
	{
		GetSaveFileLocation(save_file_direction);		
		if (!saved && save_file_direction[0] != 0)
		{
			std::cout << save_file_direction << std::endl;			
			save_file = false;
			saved = 1;
		}
	}

	if (show_image)
		ShowImage(open_file_direction, &show_image);
	if (save_image)
		SaveImage(save_file_direction, &save_image);

	if (set_grayscale)
		GrayScale(current_file, &set_grayscale);

	if (enable_YUV)
		YUVAdjuster.render(current_file, &enable_YUV);
	if (enable_RGB)
		RGBAdjuster.render(current_file, &enable_RGB);
	if (enable_HSL)
		HSLAdjuster.render(current_file, &enable_HSL);

	if (enable_auto_binarization)
		AutoBinarization(current_file, &enable_auto_binarization);
	if (enable_local_binarization) LocalBinarization(current_file, &enable_local_binarization);
	if (enable_manually_binarization) ManuallyBinarization(current_file, &enable_manually_binarization);

	if (enable_erose)
		Erose(current_file, &enable_erose);
	if (enable_dilate)
		Dilate(current_file, &enable_dilate);
	if (enable_close)
		Close(current_file, &enable_close);
	if (enable_open)
		Open(current_file, &enable_open);

	if (enable_rotate) Rotate(current_file,&enable_rotate);
	if (enable_scale) Scale(current_file,&enable_scale);
	if (enable_translate) Translate(current_file,&enable_translate);
	if (enable_shear) Shear(current_file,&enable_shear);

	if (enable_RGB_Histogram)
		RGBHistogram(current_file, &enable_RGB_Histogram);
	if (enable_HSL_Histogram)
		HSLHistogram(current_file,&enable_HSL_Histogram);
	if (enable_RGB_Histogram_E)
		RGBHistogram_E(current_file,&enable_RGB_Histogram_E);
	if (enable_HSL_Histogram_E)
		HSLHistogram_E(current_file,&enable_HSL_Histogram_E);

	if (enable_logarithmic) Logarithmic(current_file,&enable_logarithmic);
	if (enable_mean) Mean(current_file,&enable_mean);
	if (enable_edge) Edge(current_file,&enable_edge);
	if (enable_lap) Lap(current_file,&enable_lap);
	if (enable_bilateral) Bilateral(current_file,&enable_bilateral);

	if (enable_help)
		ShowHelp(&enable_help);
	if (enable_about)
		ShowAbout(&enable_about);

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
			if (ImGui::MenuItem("Save As..", "Ctrl+S",&save_file))
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
				ImGui::MenuItem("Open", nullptr, &enable_open);
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

static void GetOpenFileLocation(char *open_file_direction)
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
	ofn.lpstrTitle = TEXT("Please Open a File");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;

	if (GetOpenFileName(&ofn))
	{
		strcpy(open_file_direction, szFileTitle);
	}
}

static void GetSaveFileLocation(char *save_file_direction)
{
	OPENFILENAME ofn;
	static char szFile[256];
	static char szFileTitle[256];
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrDefExt = TEXT("bmp");
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = sizeof(szFileTitle);
	ofn.lpstrTitle = TEXT("Please Enter a Filename");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn))
	{
		strcpy(save_file_direction, szFileTitle);
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

	stbi_image_free(image_data);
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::Begin(open_file_direction,p_open);
	ImGui::Text(open_file_direction);
	// Now that we have an OpenGL texture, assuming our imgui rendering function (imgui_impl_xxx.cpp file) takes GLuint as ImTextureID, we can display it:
	ImGui::Image((void *)(intptr_t)my_opengl_texture, ImVec2(image_width, image_height));
	ImGui::End();
}

static void SaveImage(char *save_file_direction, bool *p_open)
{
	return;
}

static void CreateTempImage(const char *file_direction, const char *new_file_direction)
{
	;
}

static void GrayScale(std::string current_file, bool *p_open)
{
	ImGui::Begin("GrayScale", p_open);
	if (ImGui::Button("Preview"))
	{

	}
	ImGui::End();
}

static void AutoBinarization(std::string current_file, bool *p_open)
{
	ImGui::Begin("Auto Binarization", p_open);
	if (ImGui::Button("Preview"))
	{

	}
	ImGui::End();
}

static void LocalBinarization(std::string current_file, bool *p_open)
{
	int image_width, image_height;
	static int xnum, ynum;
	
	ImGui::Begin(("LocalBinarization:"+ current_file).c_str(), p_open);
	ImGui::SliderInt("xnum", &xnum, 1, 32);
	ImGui::SliderInt("ynum", &ynum, 1, 32);
	// std::string temp = CreateTempImage(current_file.c_str(), "local_binarization_temp.bmp");

	// Do something

	
	unsigned char *image_data = stbi_load(current_file.c_str(), &image_width, &image_height, nullptr, 4);

	// Turn the RGBA pixel data into an OpenGL texture:
	GLuint my_opengl_texture;
	glGenTextures(1, &my_opengl_texture);
	glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	
	// Now that we have an OpenGL texture, assuming our imgui rendering function (imgui_impl_xxx.cpp file) takes GLuint as ImTextureID, we can display it:
	ImGui::Image((void *)(intptr_t)my_opengl_texture, ImVec2(image_width, image_height));
	ImGui::End();
}

static void ManuallyBinarization(std::string current_file, bool *p_open)
{
	int image_width, image_height;
	static int threshold;
	
	ImGui::Begin(("Manually Binarization:"+ current_file).c_str(), p_open);
	ImGui::SliderInt("threshold", &threshold, 0, 255 );
	// std::string temp = CreateTempImage(current_file.c_str(), "local_binarization_temp.bmp");

	// Do something
	
	unsigned char *image_data = stbi_load(current_file.c_str(), &image_width, &image_height, nullptr, 4);

	// Turn the RGBA pixel data into an OpenGL texture:
	GLuint my_opengl_texture;
	glGenTextures(1, &my_opengl_texture);
	glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	
	// Now that we have an OpenGL texture, assuming our imgui rendering function (imgui_impl_xxx.cpp file) takes GLuint as ImTextureID, we can display it:
	ImGui::Image((void *)(intptr_t)my_opengl_texture, ImVec2(image_width, image_height));
	ImGui::End();
}

static void Erose(std::string current_file, bool *p_open)
{
	ImGui::Begin("Erose", p_open);
	if(ImGui::Button("Preview"))
	{
		
	}
	ImGui::End();
}

static void Dilate(std::string current_file, bool *p_open)
{
	ImGui::Begin("Dilate", p_open);
	if (ImGui::Button("Preview"))
	{
		
	}
	ImGui::End();
}

static void Close(std::string current_file, bool *p_open)
{
	ImGui::Begin("Close", p_open);
	if (ImGui::Button("Preview"))
	{
		
	}
	ImGui::End();
}

static void Open(std::string current_file, bool *p_open)
{
	ImGui::Begin("Open", p_open);
	if (ImGui::Button("Preview"))
	{
		
	}
	ImGui::End();
}

static void Rotate(std::string current_file, bool *p_open)
{
	static float theta = 0;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	ImGui::Begin("Rotate", p_open);
	ImGui::SliderFloat("Theta", &theta, 0, 360);
	ImGui::DragInt("X0", &x0,0,4096);
	ImGui::DragInt("X1", &x1, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("Y1", &y1, 0, 4096);


	ImGui::End();
}

static void Scale(std::string current_file, bool *p_open)
{
	static float sx = 0.01, sy = 0.01;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	ImGui::Begin("Scale", p_open);
	ImGui::SliderFloat("sx", &sx, 0.01, 100);
	ImGui::SliderFloat("sy", &sy, 0.01, 100);
	ImGui::DragInt("X0", &x0, 0, 4096);
	ImGui::DragInt("X1", &x1, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("Y1", &y1, 0, 4096);


	ImGui::End();
}

static void Translate(std::string current_file, bool *p_open)
{
	static float dx = 0,dy = 0;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	ImGui::Begin("Translate", p_open);
	ImGui::SliderFloat("dx", &dx, -2048, +2048);
	ImGui::SliderFloat("dy", &dy, -2048, +2048);
	ImGui::DragInt("X0", &x0, 0, 4096);
	ImGui::DragInt("X1", &x1, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("Y1", &y1, 0, 4096);


	ImGui::End();
}

static void Shear(std::string current_file, bool *p_open)
{ 
	static float sx = 0.01, sy = 0.01;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	ImGui::Begin("Shear", p_open);
	ImGui::SliderFloat("sx", &sx, 0.01, 100);
	ImGui::SliderFloat("sy", &sy, 0.01, 100);
	ImGui::DragInt("X0", &x0, 0, 4096);
	ImGui::DragInt("X1", &x1, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("Y1", &y1, 0, 4096);


	ImGui::End();
}

static void RGBHistogram(std::string current_file, bool *p_open)
{
	static float arr[1000];
	ImGui::Begin("RGB Histogram", p_open);

	ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f);
	ImGui::End();
}

static void HSLHistogram(std::string current_file, bool *p_open)
{
	static float arr[1000];
	ImGui::Begin("HSL Histogram", p_open);

	ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f);
	ImGui::End();
}

static void RGBHistogram_E(std::string current_file, bool *p_open)
{
	ImGui::Begin("RGB Histogram Equalization", p_open);
	if (ImGui::Button("Preview"))
	{

	}
	ImGui::End();
}

static void HSLHistogram_E(std::string current_file, bool *p_open)
{
	ImGui::Begin("HSL Histogram Equalization", p_open);
	if (ImGui::Button("Preview"))
	{

	}
	ImGui::End();
}

static void Logarithmic(std::string current_file, bool *p_open)
{
	ImGui::Begin("Logarithmic", p_open);
	if (ImGui::Button("Preview"))
	{

	}
	ImGui::End();
}

static void Mean(std::string current_file, bool *p_open)
{
	static int windowSize;
	ImGui::Begin("Mean Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);

	ImGui::End();
}

static void Edge(std::string current_file, bool *p_open)
{
	static int windowSize;
	ImGui::Begin("Edge Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);

	ImGui::End();
}

static void Lap(std::string current_file, bool *p_open)
{
	static int windowSize;
	ImGui::Begin("Laplaician Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);

	ImGui::End();
}

static void Bilateral(std::string current_file, bool *p_open)
{
	static int windowSize;
	float sigma;
	ImGui::Begin("Bilateral Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);
	ImGui::SliderFloat("Sigma", &sigma, 0, 10000);

	ImGui::End();
}

static void ShowHelp(bool *p_open)
{
	ImGui::SetNextWindowPos(ImVec2(640,360), ImGuiCond_FirstUseEver);
	ImGui::Begin("Help",p_open);
	ImGui::Text("This is a help");	
	ImGui::End();
}


static void ShowAbout(bool *p_open)
{
	ImGui::SetNextWindowPos(ImVec2(640, 360), ImGuiCond_FirstUseEver);
	ImGui::Begin("About",p_open);
	ImGui::Text("About:");
	ImGui::End();
}