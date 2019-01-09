#ifndef PROCESS_H_
#define PROCESS_H_

#include "bitmap/bmp.h"
#include <execution>
#include <utils/counter.h>

using utils::counter_2d;

namespace process
{
	inline void add_rgb(bmp::bitmap_t & image, double r, double g, double b) noexcept {
		std::for_each(std::execution::par_unseq,
			counter_2d<size_t>::begin(0, 0, image.width, image.height),
			counter_2d<size_t>::end(0, 0, image.width, image.height), [&image,r,g,b](auto p) {
				auto x = p.first, y = p.second;
				auto c = image.get_unchecked<bmp::color_t>(x, y);
				auto rr = std::get<0>(c), gg = std::get<1>(c), bb = std::get<2>(c);
				image.set_unchecked(x, y, bmp::make_RGB(rr + r, gg + g, bb + b));
			});
	}
	inline void add_hsl(bmp::bitmap_t & image, double r, double g, double b) noexcept {
		std::for_each(std::execution::par_unseq,
			counter_2d<size_t>::begin(0, 0, image.width, image.height),
			counter_2d<size_t>::end(0, 0, image.width, image.height), [&image,r,g,b](auto p) {
				auto x = p.first, y = p.second;
				auto c = image.get_hsl_unchecked(x, y);
				auto rr = std::get<0>(c), gg = std::get<1>(c), bb = std::get<2>(c);
				image.set_hsl_unchecked(x, y, bmp::HSL(rr + r, gg + g, bb + b));
			});
	}
	void grayscale(bmp::bitmap_t &image) noexcept;

	void auto_binarize(bmp::bitmap_t &image) noexcept;
	void auto_binarize(bmp::bitmap_t &image, size_t x_num, size_t y_num) noexcept;

	template<typename U = std::less<uint8_t>>
	void binarize(bmp::bitmap_t & image, uint8_t threshold,
		size_t x0, size_t y0, size_t x1, size_t y1, U u = U()) noexcept;

	template<typename U = std::less<uint8_t>>
	inline void binarize(bmp::bitmap_t & image, uint8_t threshold, U u = U()) noexcept {
		binarize(image, threshold, 0, 0, image.width - 1, image.height - 1, u);
	}

	uint8_t get_threshold(const bmp::bitmap_t &image, size_t x0, size_t y0, size_t x1, size_t y1) noexcept;
}

#endif