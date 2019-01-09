#ifndef PROCESS_LOGARITHMIC_H
#define PROCESS_LOGARITHMIC_H

#include <bitmap/bmp.h>
#include <utils/counter.h>
#include <execution>

using utils::counter_2d;

namespace process
{
	inline double max_luminance(bmp::bitmap_t &bitmap, size_t x0, size_t y0, size_t x1, size_t y1) {
		auto luminance_getter = [&bitmap](size_t x, size_t y) {
			return std::get<0>(bitmap.get_unchecked<bmp::yuv_t>(x, y));
		};
		auto[lbegin, lend] = utils::counter_2d<size_t, decltype(luminance_getter)>::make(
			x0, y0, x1 - 1, y1 - 1, luminance_getter);

		return *std::max_element(std::execution::par_unseq, lbegin, lend);
	}

	inline void logarithmic(bmp::bitmap_t &bitmap, size_t x0, size_t y0, size_t x1, size_t y1) {
		auto luminance_max = max_luminance(bitmap, x0, y0, x1, y1);
		auto[lbegin, lend] = utils::counter_2d<size_t>::make(
			x0, y0, x1 - 1, y1 - 1);
		std::for_each(std::execution::par_unseq, lbegin, lend,
			[&bitmap, luminance_max](auto pair) {
			auto[x, y] = pair;
			auto[Y, U, V] = bitmap.get<bmp::yuv_t>(x, y);
			bitmap.set_unchecked(x, y,
				bmp::YUV(std::log10(Y + 1) / std::log10(luminance_max + 1) * 255.0, U, V));
		});
	}
	inline void logarithmic(bmp::bitmap_t &bitmap) {
		logarithmic(bitmap, 0, 0, bitmap.width - 1, bitmap.height - 1);
	}
}

#endif