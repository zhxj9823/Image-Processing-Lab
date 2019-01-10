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

#include "AdjustColorWindow.h"
#include "ImmediateOperation.h"
#include "Middle.h"
static void GetOpenFileLocation(char *open_file_direction);
static void GetSaveFileLocation(char *open_file_direction);
static void ShowImage(bool *p_open);
static void SaveImage(char *save_file_direction, bool *p_open);
static void CreateTempImage(const char *file_direction, const char *new_file_direction);

static void LocalBinarization(bool *p_open);
static void ManuallyBinarization(bool *p_open);

static void Rotate(bool *p_open);
static void Scale(bool *p_open);
static void Translate(bool *p_open);
static void Shear(bool *p_open);

static void Mean(bool *p_open);
static void Edge(bool *p_open);
static void Lap(bool *p_open);
static void Bilateral(bool *p_open);
static void ShowHelp(bool *p_open);
static void ShowAbout(bool *p_open);

MiddleLayer middle;

void ImGui::MyShow(bool *show_window)
{

	static bool open_file = false;
	static bool save_file = false;
	static bool show_image = false;
	static bool save_image = false;
	
	static bool enable_local_binarization = false;
	static bool enable_manually_binarization = false;
	static bool enable_rotate = false;
	static bool enable_scale = false;
	static bool enable_translate = false;
	static bool enable_shear = false;
	static bool enable_mean = false;
	static bool enable_edge = false;
	static bool enable_lap = false;
	static bool enable_bilateral = false;
	static bool enable_help = false;
	static bool enable_about = false;

	
	static char save_file_direction[1024];

	static AdjustRGB RGBAdjuster(&middle);
	static AdjustHSL HSLAdjuster(&middle);
	static AdjustYUV YUVAdjuster(&middle);
	static EroseWindow EroseApplier(&middle);
	static DilateWindow DilateApplier(&middle);
	static OpeningWindow OpeningApplier(&middle);
	static ClosingWindow ClosingApplier(&middle);
	static GrayScaleWindow GrayScaleApplier(&middle);
	static AutoBinarizationWindow AutoBinarizationApplier(&middle);
	static HSLHistogramEqualizationWindow HSLHistogramEqualizationApplier(&middle);
	static RGBHistogramEqualizationWindow RGBHistogramEqualizationApplier(&middle);
	static LogarithmicWindow LogarithmicApplier(&middle);
	static HSLHistogramWindow HSLHistogramInfo(&middle);
	static RGBHistogramWindow RGBHistogramInfo(&middle);
	static int opened = 0;
	
	static int saved = 0;
	

	if (show_image)
		ShowImage(&show_image);
	if (save_image)
		SaveImage(save_file_direction, &save_image);

	GrayScaleApplier.render();

	YUVAdjuster.render();
	RGBAdjuster.render();
	HSLAdjuster.render();

	AutoBinarizationApplier.render();
	
	if (enable_local_binarization) LocalBinarization(&enable_local_binarization);
	if (enable_manually_binarization) ManuallyBinarization(&enable_manually_binarization);

	EroseApplier.render();
	DilateApplier.render();
	ClosingApplier.render();
	OpeningApplier.render();
	

	if (enable_rotate) Rotate(&enable_rotate);
	if (enable_scale) Scale(&enable_scale);
	if (enable_translate) Translate(&enable_translate);
	if (enable_shear) Shear(&enable_shear);

	RGBHistogramEqualizationApplier.render();
	HSLHistogramEqualizationApplier.render();
	
	HSLHistogramInfo.render();
	RGBHistogramInfo.render();
	LogarithmicApplier.render();
	
	if (enable_mean) Mean(&enable_mean);
	if (enable_edge) Edge(&enable_edge);
	if (enable_lap) Lap(&enable_lap);
	if (enable_bilateral) Bilateral(&enable_bilateral);

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
				static char open_file_direction[1024];
				GetOpenFileLocation(open_file_direction);

				if (open_file_direction[0] != 0)
				{
					std::cout << open_file_direction << std::endl;
					// current_file = CreateTempImage(open_file_direction, "temp.bmp");
					middle.load_image(open_file_direction);
					open_file = false;
					show_image = true;
					opened = 1;
				}
			}
			if (ImGui::MenuItem("Save As..", "Ctrl+S",&save_file))
			{
				GetSaveFileLocation(save_file_direction);
				if (save_file_direction[0] != 0)
				{
					middle.save(save_file_direction);
					std::cout << save_file_direction << std::endl;
				}
				save_file = false;
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
			if (ImGui::MenuItem("Cancle")) {
				middle.cancle();
			}
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
			if (ImGui::MenuItem("Grayscale", nullptr, &GrayScaleApplier.enable))
			{
			}			
			if (ImGui::BeginMenu("Adjust"))
			{
				ImGui::MenuItem("YUV", nullptr, &YUVAdjuster.enable);
				ImGui::MenuItem("RGB", nullptr, &RGBAdjuster.enable);
				ImGui::MenuItem("HSL", nullptr, &HSLAdjuster.enable);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Binarization"))
			{
				ImGui::MenuItem("Auto binarization", nullptr, &AutoBinarizationApplier.enable);
				ImGui::MenuItem("Auto local-adaptive binarization", nullptr, &enable_local_binarization);
				ImGui::MenuItem("Manually binarization", nullptr, &enable_manually_binarization);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Morphological Operation"))
			{
				ImGui::MenuItem("Erose", nullptr, &EroseApplier.enable);
				ImGui::MenuItem("Dilate", nullptr, &DilateApplier.enable);
				ImGui::MenuItem("Close", nullptr, &ClosingApplier.enable);
				ImGui::MenuItem("Open", nullptr, &OpeningApplier.enable);
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
				if (ImGui::MenuItem("RGB Histogram", nullptr, &RGBHistogramInfo.enable))
				{
				}
				if (ImGui::MenuItem("HSL Histogram", nullptr, &HSLHistogramInfo.enable))
				{
				}
				ImGui::Separator();
				if (ImGui::MenuItem("RGB Histogram Equalization",
					nullptr, &RGBHistogramEqualizationApplier.enable))
				{
				}
				if (ImGui::MenuItem("HSL Histogram Equalization",
					nullptr, &HSLHistogramEqualizationApplier.enable))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Image Logarithmic", nullptr, &LogarithmicApplier.enable))
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

static void ShowImage(bool *p_open)
{


	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::Begin("Image View",p_open);
	ImGui::Text("Image View");
	// Now that we have an OpenGL texture, assuming our imgui rendering function (imgui_impl_xxx.cpp file) takes GLuint as ImTextureID, we can display it:
	ImGui::Image(middle.texture(), 
		ImVec2(middle.width, middle.height)
		, ImVec2(0, 1), ImVec2(1, 0));
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


static void LocalBinarization(bool *p_open)
{
	int image_width, image_height;
	static int xnum, ynum;
	
	ImGui::Begin("LocalBinarization", p_open);
	ImGui::SliderInt("xnum", &xnum, 1, 32);
	ImGui::SliderInt("ynum", &ynum, 1, 32);
	PreviewAndApply(middle, [&]() {
		middle.local_adaptive_binarize(xnum, ynum);
	});

	ImGui::End();
}

static void ManuallyBinarization(bool *p_open)
{
	int image_width, image_height;
	static int threshold;
	
	ImGui::Begin("Manually Binarization", p_open);
	ImGui::SliderInt("threshold", &threshold, 0, 255 );
	PreviewAndApply(middle, [&]() {
		middle.binarize(threshold);
		});
	
	ImGui::End();
}


static void Rotate(bool *p_open)
{
	static float theta = 0;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	static int mode = 0;
	ImGui::Begin("Rotate", p_open);
	ImGui::SliderFloat("Theta", &theta, 0, 360);
	ImGui::DragInt("X0", &x0,0,4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("W", &x1, 0, 4096);
	ImGui::DragInt("H", &y1, 0, 4096);
	PreviewAndApply(middle, [&]() {
		middle.rotate(theta,x0,y0,x1,y1,mode);
		});

	ImGui::End();
}

static void Scale(bool *p_open)
{
	static float sx = 0.01, sy = 0.01;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	static int mode = 0;
	ImGui::Begin("Scale", p_open);
	ImGui::SliderFloat("sx", &sx, 0.01, 100);
	ImGui::SliderFloat("sy", &sy, 0.01, 100);
	ImGui::DragInt("X0", &x0, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("W", &x1, 0, 4096);
	ImGui::DragInt("H", &y1, 0, 4096);
	PreviewAndApply(middle, [&]() {
		middle.scale(sx,sy,x0,y0,x1,y1, mode);
		});

	ImGui::End();
}

static void Translate(bool *p_open)
{
	static float dx = 0,dy = 0;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	static int mode = 0;
	ImGui::Begin("Translate", p_open);
	ImGui::SliderFloat("dx", &dx, -2048, +2048);
	ImGui::SliderFloat("dy", &dy, -2048, +2048);
	ImGui::DragInt("X0", &x0, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("W", &x1, 0, 4096);
	ImGui::DragInt("H", &y1, 0, 4096);
	PreviewAndApply(middle, [&]() {
		middle.translate(dx,dy,x0,y0,x1,y1,mode);
		});

	ImGui::End();
}

static void Shear(bool *p_open)
{ 
	static float sx = 0.01, sy = 0.01;
	static int x0 = 0, x1 = 0, y0 = 0, y1 = 0;
	static int mode=0;
	ImGui::Begin("Shear", p_open);
	ImGui::SliderFloat("sx", &sx, 0.01, 100);
	ImGui::SliderFloat("sy", &sy, 0.01, 100);
	ImGui::DragInt("X0", &x0, 0, 4096);
	ImGui::DragInt("Y0", &y0, 0, 4096);
	ImGui::DragInt("W", &x1, 0, 4096);
	ImGui::DragInt("H", &y1, 0, 4096);
	PreviewAndApply(middle, [&]() {
		middle.shear(sx,sy,x0,y0,x1,y1, mode);
		});

	ImGui::End();
}



static void Mean(bool *p_open)
{
	static int windowSize;
	ImGui::Begin("Mean Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);
	PreviewAndApply(middle, [&]() {
		middle.mean(windowSize);
		});
	ImGui::End();
}

static void Edge(bool *p_open)
{
	static int windowSize;
	ImGui::Begin("Edge Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);
	PreviewAndApply(middle, [&]() {
		middle.edge(windowSize);
		});
	ImGui::End();
}

static void Lap(bool *p_open)
{
	static int windowSize;
	ImGui::Begin("Laplaician Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);
	PreviewAndApply(middle, [&]() {
		middle.lap(windowSize);
		});
	ImGui::End();
}

static void Bilateral(bool *p_open)
{
	static int windowSize;
	float sigma;
	ImGui::Begin("Bilateral Filter", p_open);
	ImGui::SliderInt("Window Size", &windowSize, 0, 10);
	ImGui::SliderFloat("Sigma", &sigma, 0, 10000);
	PreviewAndApply(middle, [&]() {
		middle.bilateral(windowSize, sigma);
		});
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