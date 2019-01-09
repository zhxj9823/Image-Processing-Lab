#ifndef _MIDDLE_H_
#define _MIDDLE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"

#include <stack>
#include <string>
#include <bitmap/bmp.h>
#include <process/binarization.h>
#include <process/logarithmic.h>
#include <process/geometric.h>
#include <process/morphological_operation.h>
#include <process/historgram.h>
#include <process/color_space.h>

class MiddleLayer {
	bmp::bitmap_t current_image;

	
	
	GLuint my_opengl_texture;
public :
	float HisR[255], HisG[255], HisB[255];
	float maxR, maxG, maxB;
	bmp::bitmap_t preview_image;
	MiddleLayer() {
		
	}
	void init() {
		glGenTextures(1, &my_opengl_texture);
		glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void load_image(std::string path) {
		current_image = bmp::load(path);
		load_texture(current_image);
		init_histogram();
	}
	ImTextureID texture() const {
		return (void*)(intptr_t)(my_opengl_texture);
	}
	template<typename U>
	void apply(U u) {
		u();
		current_image = preview_image;
		init_histogram();
	}
	int width, height;
	
private:
	void load_texture(bmp::bitmap_t &image) {
		glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			image.width, image.height, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
		width = image.width;
		height = image.height;
	}
	void init_histogram() {
		auto rgetter = [&](auto&& image, auto x, auto y) {
			return std::get<0>(image.get<bmp::color_t>(x, y));
		};
		auto ggetter = [&](auto&& image, auto x, auto y) {
			return std::get<1>(image.get<bmp::color_t>(x, y));
		};
		auto bgetter = [&](auto&& image, auto x, auto y) {
			return std::get<2>(image.get<bmp::color_t>(x, y));
		};
		maxR = maxG = maxB = 0;
		auto r = process::get_histogram<256, decltype(rgetter)>(current_image,
			0, 0, current_image.width - 1, current_image.height - 1, rgetter);
		auto g = process::get_histogram<256, decltype(ggetter)>(current_image,
			0, 0, current_image.width - 1, current_image.height - 1, ggetter);
		auto b = process::get_histogram<256, decltype(bgetter)>(current_image,
			0, 0, current_image.width - 1, current_image.height - 1, bgetter);
		for (int i = 0; i < 256; i++) {
			HisR[i] = r[i];
			HisG[i] = g[i];
			HisB[i] = b[i];
			maxR = std::max(maxR, HisR[i]);
			maxG = std::max(maxG, HisG[i]);
			maxB = std::max(maxB, HisB[i]);
		}
	}
public:
	void grayscale() {
		preview_image = bmp::bitmap_t(current_image);
		process::grayscale(preview_image);
		load_texture(preview_image);
		
	}
	void auto_binarize() {
		preview_image = bmp::bitmap_t(current_image);
		process::auto_binarize(preview_image);
		load_texture(preview_image);
	}
	void binarize(int threshold) {
		preview_image = bmp::bitmap_t(current_image);
		process::binarize(preview_image, threshold);
		load_texture(preview_image);
	}
	void local_adaptive_binarize(int x, int y) {
		preview_image = bmp::bitmap_t(current_image);
		process::auto_binarize(preview_image, x,y);
		load_texture(preview_image);
	}
	void erose() {
		preview_image = bmp::bitmap_t(current_image);
		process::erose(preview_image);
		load_texture(preview_image);
	}
	void dialte() {
		preview_image = bmp::bitmap_t(current_image);
		process::dilate(preview_image);
		load_texture(preview_image);
	}
	void opening() {
		preview_image = bmp::bitmap_t(current_image);
		process::open(preview_image);
		load_texture(preview_image);
	}
	void closing() {
		preview_image = bmp::bitmap_t(current_image);
		process::close(preview_image);
		load_texture(preview_image);
	}
	void logarithmic() {
		preview_image = bmp::bitmap_t(current_image);
		process::logarithmic(preview_image);
		load_texture(preview_image);
	}
	void rgb_histogram_equalization() {
		preview_image = bmp::bitmap_t(current_image);
		process::histogram_equalization_color<true,true,true>(preview_image);
		load_texture(preview_image);
	}
	
	void hsl_histogram_equalization() {
		preview_image = bmp::bitmap_t(current_image);
		process::rgb_to_hsl(preview_image);
		process::histogram_equalization_color<true, true, true>(preview_image);
		process::hsl_to_rgb(preview_image);
		load_texture(preview_image);
	}
	
};
#endif