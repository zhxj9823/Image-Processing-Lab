#ifndef BITMAP_PIXEL_ACCESSOR_H
#define BITMAP_PIXEL_ACCESSOR_H

#include "bmp.h"

namespace bmp
{
	struct const_pixel_accessor_t {
		const bitmap_t &image;
		size_t x, y;
		const_pixel_accessor_t(const bitmap_t &image, size_t x, size_t y) noexcept :image(image), x(x), y(y) {	}
		std::pair<size_t, size_t> pair() {
			return { x,y };
		}
		operator color_t() const noexcept {
			return image.get_unchecked<color_t>(x, y);
		}
	};
	struct pixel_accessor_t {
		bitmap_t &image;
		size_t x, y;
		pixel_accessor_t(bitmap_t &image, size_t x, size_t y) noexcept :image(image), x(x), y(y) {	}
		std::pair<size_t, size_t> pair() {
			return { x,y };
		}
		operator color_t() const noexcept {
			return image.get_unchecked<color_t>(x, y);
		}

		void operator=(const color_t &color) noexcept {
			image.set(x, y, color);
		}
	};

}
#endif