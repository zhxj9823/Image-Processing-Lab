#ifndef MORPHOLOGICAL_OPERATE_H
#define MORPHOLOGICAL_OPERATE_H

#include "utils/counter.h"
#include "bitmap/bmp.h"


#include <type_traits>
#include <algorithm>
#include <execution>

using utils::counter;

namespace process {

	using morphological_operator = bool(const bmp::bitmap_t &image, size_t x, size_t y);
	using structure_element = std::vector<std::pair<int32_t, int32_t>>;

	const structure_element morphological_default_element = { {-1,0},{0,-1 },{0,0},{1,0},{0,1} };
	template<typename U>
	bmp::bitmap_t  morphological_operate(const bmp::bitmap_t &image,
		typename std::enable_if_t<
		std::is_invocable_r_v<bool, U, const bmp::bitmap_t &, size_t, size_t>,
		U> &&u) noexcept {

		bmp::bitmap_t output(image.width, image.height);

		std::for_each(std::execution::par_unseq, counter<size_t>(0), counter<size_t>(image.height),
			[&](auto y) {
			std::for_each(std::execution::par_unseq, counter<size_t>(0), counter<size_t>(image.width),
				[&](auto x) {
				if (std::forward<decltype(u)>(u)(image, x, y)) {
					output.set_unchecked(x, y, bmp::make_RGB(uint8_t(255), uint8_t(255), uint8_t(255)));
				}
			});
		});
		return output;
	}


	inline bmp::bitmap_t dilate(const bmp::bitmap_t & image,
		const structure_element & element = morphological_default_element) noexcept {
		auto f = [&](const bmp::bitmap_t &image, int32_t x, int32_t y) -> bool {
			for (auto &d : element) {
				auto nx = x + d.first, ny = y + d.second;
				if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
					if (image.get_unchecked<uint8_t>(nx, ny) > 0) return true;
				}
			}
			return false;
		};

		return morphological_operate<decltype(f)>(image, std::move(f));
	}
	inline bmp::bitmap_t erose(const bmp::bitmap_t & image,
		const structure_element & element = morphological_default_element) noexcept {
		auto f = [&](const bmp::bitmap_t &image, int32_t x, int32_t y) -> bool {
			for (auto &d : element) {
				auto nx = x + d.first, ny = y + d.second;
				if (nx >= 0 && nx < image.width && ny >= 0 && ny < image.height) {
					if (image.get_unchecked<uint8_t>(nx, ny) == 0) return false;
				}
			}
			return true;
		};

		return morphological_operate<decltype(f)>(image, std::move(f));
	}

	inline bmp::bitmap_t open(const bmp::bitmap_t & image,
		const structure_element & element = morphological_default_element)noexcept {
		return erose(dilate(image, element), element);
	}
	inline bmp::bitmap_t close(const bmp::bitmap_t & image,
		const structure_element & element = morphological_default_element) noexcept {
		return dilate(erose(image, element), element);
	}
}


#endif