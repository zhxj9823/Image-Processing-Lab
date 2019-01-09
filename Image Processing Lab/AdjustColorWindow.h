#ifndef _SUB_WINDOWS_H
#define _SUB_WINDOWS_H

#include "imgui.h"
#include "Middle.h"

class AdjustColor
{
protected:
	MiddleLayer *middle;
public:
	
	bool enable;
	void virtual render()
	{
		if (!enable) return;
		ImGui::Begin((name1 + name2 + name3 + "Change").c_str(), &enable);
		ImGui::SliderFloat(name1.c_str(), &a, -255.0f, 255.0f);
		ImGui::SliderFloat(name2.c_str(), &b, -255.0f, 255.0f);
		ImGui::SliderFloat(name3.c_str(), &c, -255.0f, 255.0f);
		if (ImGui::Button("Apply"))
		{
			middle->apply([&]() {
				apply(a, b, c);
			});
		}
		if (ImGui::Button("Preview")) 
		{
			apply(a, b, c);
		}


		ImGui::End();
	}
	virtual void apply(double a, double b, double c) = 0;
protected:
	float a, b, c;
	std::string name1, name2, name3;
};

class AdjustRGB :public AdjustColor
{
public:
	AdjustRGB(MiddleLayer *m)
	{
		name1 = "R";
		name2 = "G";
		name3 = "B";
		a = b = c = 0;
		middle = m;
		enable = false;
	}
	virtual void apply(double a, double b, double c) {
		middle->rgb_adjust(a, b, c);
	}
};


class AdjustHSL :public AdjustColor
{
public:
	AdjustHSL(MiddleLayer *m)
	{
		name1 = "H";
		name2 = "S";
		name3 = "L";
		a = b = c = 0;
		middle = m;
		enable = false;
	}
	virtual void apply(double a, double b, double c) {
		middle->hsl_adjust(a, b, c);
	}
};


class AdjustYUV :public AdjustColor
{
public:
	AdjustYUV(MiddleLayer *m)
	{
		name1 = "Y";
		name2 = "U";
		name3 = "V";
		a = b = c = 0;
		middle = m;
		enable = false;
	}
	virtual void apply(double a, double b, double c) {

	}
};

template<typename U>
inline void PreviewAndApply(MiddleLayer &middle,U u) {
	if (ImGui::Button("Apply"))
	{
		middle.apply([&]() {
			u();
		});
	}
	if (ImGui::Button("Preview"))
	{
		u();
	}
}


#endif
