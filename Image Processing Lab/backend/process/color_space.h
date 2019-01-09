#ifndef PROCESS_COLOR_SPACE_H
#define PROCESS_COLOR_SPACE_H

#include <bitmap/bmp.h>
#include <utils/counter.h>

#include <algorithm>
#include <execution>

using utils::counter_2d;

namespace process
{
	inline void rgb_to_hsl(bmp::bitmap_t &image) {
		auto[lbegin, lend] = counter_2d<size_t>::make(0, 0, image.width, image.height);
		std::for_each(std::execution::par_unseq, lbegin, lend, [&image](auto pair) {
			auto[x, y] = pair;
			image.set(x, y, image.get_hsl_unchecked(x, y));
		});
	}
	inline void hsl_to_rgb(bmp::bitmap_t &image) {
		auto[lbegin, lend] = counter_2d<size_t>::make(0, 0, image.width, image.height);
		std::for_each(std::execution::par_unseq, lbegin, lend, [&image](auto pair) {
			auto[x, y] = pair;
			image.set_hsl_unchecked(x, y, image.get<bmp::color_t>(x, y));
		});
	}
}
#endif