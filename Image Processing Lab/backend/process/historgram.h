#ifndef PROCESS_HISTOGRAM_H
#define PROCESS_HISTOGRAM_H

#include <process/color_space.h>
#include <bitmap/bmp.h>
#include <array>
#include <algorithm>
#include <execution>
#include <tuple>



namespace process
{
	template<int HisNum, typename U>
	std::array<double, HisNum> get_histogram(
		const bmp::bitmap_t &image, size_t x0, size_t y0, size_t x1, size_t y1, U u) {

		std::array<double, HisNum> histogram = {};
		x1 = std::min(x1, image.width - 1);
		y1 = std::min(y1, image.height - 1);


		for (size_t y = y0; y <= y1; y++) {
			for (size_t x = x0; x <= x1; x++) {
				auto Y = u(image, x, y);
				histogram[Y] += 1;
			}
		}
		auto total = (y1 - y0 + 1)*(x1 - x0 + 1);

		std::transform(std::execution::par_unseq,
			std::begin(histogram), std::end(histogram), std::begin(histogram), [total](auto x) {
			return x / total;
		});
		return histogram;
	}

	template<int HisNum, typename U, typename V>
	void histogram_equalization(bmp::bitmap_t &image,
		size_t x0, size_t y0, size_t x1, size_t y1, U u, V v) {

		std::array<double, HisNum> histogram = get_histogram<HisNum>(image, x0, y0, x1, y1, u);
		std::array<double, HisNum> histogram_sum;
		std::inclusive_scan(std::execution::par_unseq, 
			std::begin(histogram), std::end(histogram), std::begin(histogram_sum));


		auto[lbegin, lend] = utils::counter_2d<size_t>::make(
			x0, y0, x1 - 1, y1 - 1);
		std::for_each(std::execution::par_unseq, lbegin, lend,
			[&image, &histogram_sum, v, u](auto pair) {
			auto[x, y] = pair;
			v(image, histogram_sum[u(image, x, y)]*255, x, y);
		});
	}
	template<int HisNum, typename U, typename V>
	inline void histogram_equalization(bmp::bitmap_t &image, U u, V v) {
		histogram_equalization<HisNum>(image, 0, 0, image.width - 1, image.height - 1, u, v);
	}


	template<int N>
	inline void histogram_equalization_Nth(bmp::bitmap_t &image) {
		
		histogram_equalization<256>(image, [](const bmp::bitmap_t &img, auto x, auto y) {
			return  std::get<N>(img.get_unchecked<bmp::color_t>(x, y));
		}, [](bmp::bitmap_t &img, auto new_value, auto x, auto y) {
			auto color = img.get_unchecked<bmp::color_t>(x, y);
			std::get<N>(color) = new_value;
			img.set_unchecked(x, y, color);
		});	
	}

	template<bool r, bool g, bool b>
	inline void histogram_equalization_color(bmp::bitmap_t &image) {
		if constexpr(r)
			histogram_equalization_Nth<0>(image);
		if constexpr (g)
			histogram_equalization_Nth<1>(image);
		if constexpr (b)
			histogram_equalization_Nth<2>(image);
	}	
}
#endif