#ifndef IMM_OP_H
#define IMM_OP_H
#include "Middle.h"
#include <cstdio>

class ImmOpWindow 
{
protected:
	MiddleLayer *middle;
	const char *name;
public:
	bool enable;
	ImmOpWindow(MiddleLayer *m, const char*n):middle(m), name(n), enable(false){}
	void virtual render()
	{
		if (!enable) return;
		ImGui::Begin(name, &enable);
		if (ImGui::Button("Apply"))
		{
			middle->apply([&]() {
				apply();
			});
		}
		if (ImGui::Button("Preview"))
		{
			apply();
		}
		ImGui::End();
	}
	virtual void apply() = 0;
};
class EroseWindow : public ImmOpWindow {
public:
	EroseWindow(MiddleLayer *m) :ImmOpWindow(m, "Erose"){}
	virtual void apply() {
		middle->erose();
	}
};
class DilateWindow : public ImmOpWindow {
public:
	DilateWindow(MiddleLayer *m) :ImmOpWindow(m, "Dilate") {}
	virtual void apply() {
		middle->dialte();
	}
};

class OpeningWindow : public ImmOpWindow {
public:
	OpeningWindow(MiddleLayer *m) :ImmOpWindow(m, "Opening") {}
	virtual void apply() {
		middle->opening();
	}
};

class ClosingWindow : public ImmOpWindow {
public:
	ClosingWindow(MiddleLayer *m) :ImmOpWindow(m, "Closing") {}
	virtual void apply() {
		middle->closing();
	}
};

class AutoBinarizationWindow : public ImmOpWindow {
public:
	AutoBinarizationWindow(MiddleLayer *m) :ImmOpWindow(m, "Auto Binarization") {}
	virtual void apply() {
		middle->auto_binarize();
	}
};

class GrayScaleWindow : public ImmOpWindow {
public:
	GrayScaleWindow(MiddleLayer *m) :ImmOpWindow(m, "GrayScale") {}
	virtual void apply() {
		middle->grayscale();
	}
};

class InfoWindow
{
protected:
	MiddleLayer *middle;
	const char *name;
public:
	bool enable;
	InfoWindow(MiddleLayer *m, const char*n) :middle(m), name(n), enable(false) {}
	void virtual render()
	{
		if (!enable) return;
		ImGui::Begin(name, &enable);
		
			apply();
		
		ImGui::End();
	}
	virtual void apply() = 0;
};
class HSLHistogramWindow : public InfoWindow {
public:
	HSLHistogramWindow(MiddleLayer *m) :InfoWindow(m, "HSL Histogram") {}
	virtual void apply() {
		
	}
};

class RGBHistogramWindow : public InfoWindow {
public:
	RGBHistogramWindow(MiddleLayer *m) :InfoWindow(m, "RGB Histogram") {}
	virtual void apply() {
		
		
		ImGui::PlotHistogram("R", middle->HisR, 255, 0, NULL, 0.0f, middle->maxR, ImVec2(200, 70));
		ImGui::PlotHistogram("G", middle->HisG, 255, 0, NULL, 0.0f, middle->maxG, ImVec2(200, 70));
		ImGui::PlotHistogram("B", middle->HisB, 255, 0, NULL, 0.0f, middle->maxB, ImVec2(200, 70));
	}
};

class HSLHistogramEqualizationWindow : public ImmOpWindow {
public:
	HSLHistogramEqualizationWindow(MiddleLayer *m) :ImmOpWindow(m, "HSL Histogram Equalization") {}
	virtual void apply() {
		middle->hsl_histogram_equalization();
	}
};

class RGBHistogramEqualizationWindow : public ImmOpWindow {
public:
	RGBHistogramEqualizationWindow(MiddleLayer *m) :ImmOpWindow(m, "RGB Histogram Equalization") {}
	virtual void apply() {
		middle->rgb_histogram_equalization();
//		ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f);
	}
};

class LogarithmicWindow : public ImmOpWindow {
public:
	LogarithmicWindow(MiddleLayer *m) :ImmOpWindow(m, "Logarithmic") {}
	virtual void apply() {
		middle->logarithmic();
		//ImGui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f);
	}
};


#endif