#ifndef PROCESS_H_
#define PROCESS_H_

#include "bitmap/bmp.h"


namespace process
{
	void grayscale(bmp::bitmap_t &image) noexcept;

	void auto_binarize(bmp::bitmap_t &image) noexcept;
	void auto_binarize(bmp::bitmap_t &image, size_t x_num, size_t y_num) noexcept;

	template<typename U = std::less<uint8_t>>
	void binarize(bmp::bitmap_t & image, uint8_t threshold,
		size_t x0, size_t y0, size_t x1, size_t y1, U u = U()) noexcept;

	template<typename U = std::less<uint8_t>>
	void binarize(bmp::bitmap_t & image, uint8_t threshold, U u = U()) noexcept;

	uint8_t get_threshold(const bmp::bitmap_t &image, size_t x0, size_t y0, size_t x1, size_t y1) noexcept;
}

#endif