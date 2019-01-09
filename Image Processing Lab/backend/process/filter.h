#ifndef LINEAR_FILTER_H
#define LINEAR_FILTER_H

#include "utils/counter.h"
#include "bitmap/bmp.h"
#include "bitmap/pixel_accessor.h"
#include <process/geometric.h>

#include <type_traits>
#include <algorithm>
#include <execution>

using bmp::pixel_accessor_t;
using bmp::const_pixel_accessor_t;
using utils::counter_2d;


namespace process
{
	bmp::color_t color_round2(const std::tuple<double, double, double> &c) {
		auto[r, g, b] = c;
		const double t = 128;
		return { std::clamp(lround(r + t),0l,255l),
			std::clamp(lround(g + t),0l,255l),std::clamp(lround(b + t),0l,255l) };
	}
	bmp::bitmap_t fuse_image(const bmp::bitmap_t &image1, const bmp::bitmap_t &image2, double sign,double offset) {
		if (image1.width != image2.width || image1.height != image2.height) {
			throw "Different size";
		}
		bmp::bitmap_t rval(image1);
		auto accessor_getter = [&rval](size_t x, size_t y) {
			return pixel_accessor_t(rval, x, y);
		};
		auto accessor_image1 = [&image1](size_t x, size_t y) {
			return const_pixel_accessor_t(image1, x, y);
		};
		auto accessor_image2 = [&image2](size_t x, size_t y) {
			return const_pixel_accessor_t(image2, x, y);
		};
		auto[lbegin, lend] = counter_2d<size_t,
			decltype(accessor_getter)>::make(0, 0,
				rval.width, rval.height, accessor_getter);
		auto[lbegin1, lend1] = counter_2d<size_t,
			decltype(accessor_image1)>::make(0, 0,
				rval.width, rval.height, accessor_image1);
		auto[lbegin2, lend2] = counter_2d<size_t,
			decltype(accessor_image2)>::make(0, 0,
				rval.width, rval.height, accessor_image2);
		auto it1 = lbegin1;
		auto it2 = lbegin2;
		for (auto it0 = lbegin; it0 != lend; ++it0,++it1,++it2) {
			*it0 = color_round(color_scale(*it1,1.0) + color_scale(*it2,sign))+
				std::make_tuple(offset,offset,offset);
		}
		return rval;
	}
	
	bmp::bitmap_t  mean_filter(const bmp::bitmap_t &image, size_t r) noexcept {
		bmp::bitmap_t rval(image);
		auto accessor_getter = [&rval](size_t x, size_t y) {
			return pixel_accessor_t(rval, x, y);
		};
		auto accessor_image = [&image](size_t x, size_t y) {
			return const_pixel_accessor_t(image, x, y);
		};
		auto[lbegin, lend] = counter_2d<size_t,
			decltype(accessor_getter)>::make(0, 0,
				rval.width, rval.height, accessor_getter);
		
		std::transform(lbegin, lend,lbegin,
			[r, accessor_image, &image](auto &&p) {
				auto[x, y] = p.pair();
				auto x0 = std::max(x - r, 0ull), y0 = std::max(y - r, 0ull),
					x1 = std::min(x + r + 1, image.width), y1 = std::min(y + r + 1, image.height);
				auto sum = std::make_tuple(0.0, 0.0, 0.0);

				auto[lbegin0, lend0] = counter_2d<size_t,
					decltype(accessor_image)>::make(x0, y0,
						x1, y1, accessor_image);
				
				for (auto it = lbegin0; it != lend0; ++it) {
					sum = sum + color_scale(*it, 1.0);
				}
				sum = sum - color_scale(image.get_unchecked<bmp::color_t>(x, y), 1.0);
				return color_round(color_scale(sum,1.0/( (x1-x0+1)*(y1-y0+1) - 1)));
			});
		return rval;
	}
	bmp::bitmap_t  laplaician_enhancement(const bmp::bitmap_t &image, size_t r) noexcept {
		bmp::bitmap_t rval(image);
		auto accessor_getter = [&rval](size_t x, size_t y) {
			return pixel_accessor_t(rval, x, y);
		};
		auto accessor_image = [&image](size_t x, size_t y) {
			return const_pixel_accessor_t(image, x, y);
		};
		auto[lbegin, lend] = counter_2d<size_t,
			decltype(accessor_getter)>::make(0, 0,
				rval.width, rval.height, accessor_getter);

		std::transform(lbegin, lend, lbegin,
			[r, accessor_image, &image](auto &&p) {
				auto[x, y] = p.pair();
				auto x0 = std::max(x - r, 0ull), y0 = std::max(y - r, 0ull),
					x1 = std::min(x + r + 1, image.width), y1 = std::min(y + r + 1, image.height);
				auto sum = std::make_tuple(0.0, 0.0, 0.0);

				auto[lbegin0, lend0] = counter_2d<size_t,
					decltype(accessor_image)>::make(x0, y0,
						x1, y1, accessor_image);

				for (auto it = lbegin0; it != lend0; ++it) {
					sum = sum + color_scale(*it, 1.0);
				}
				auto origin = image.get_unchecked<bmp::color_t>(x, y);
				sum = sum - color_scale(origin, (x1 - x0)*(y1 - y0));
				return color_round(origin-color_scale(color_round(sum),1.0));
			});
		return rval;
	}

	bmp::bitmap_t  laplaician_operator(const bmp::bitmap_t &image, size_t r) noexcept {
		bmp::bitmap_t rval(image);
		auto accessor_getter = [&rval](size_t x, size_t y) {
			return pixel_accessor_t(rval, x, y);
		};
		auto accessor_image = [&image](size_t x, size_t y) {
			return const_pixel_accessor_t(image, x, y);
		};
		auto[lbegin, lend] = counter_2d<size_t,
			decltype(accessor_getter)>::make(0, 0,
				rval.width, rval.height, accessor_getter);

		std::transform(lbegin, lend, lbegin,
			[r, accessor_image, &image](auto &&p) {
				auto[x, y] = p.pair();
				auto x0 = std::max(x - r, 0ull), y0 = std::max(y - r, 0ull),
					x1 = std::min(x + r + 1, image.width), y1 = std::min(y + r + 1, image.height);
				auto sum = std::make_tuple(0.0, 0.0, 0.0);

				auto[lbegin0, lend0] = counter_2d<size_t,
					decltype(accessor_image)>::make(x0, y0,
						x1, y1, accessor_image);

				for (auto it = lbegin0; it != lend0; ++it) {
					sum = sum + color_scale(*it, 1.0);
				}
				auto origin = image.get_unchecked<bmp::color_t>(x, y);
				sum = sum - color_scale(origin, (x1 - x0)*(y1 - y0));
				return color_round2(sum);
			});
		return rval;
	}
	inline double square(double x) noexcept {
		return x * x;
	}
	
	inline double omega(size_t x, size_t y, size_t k, size_t l,
		float sigma_d, float sigma_s, const bmp::bitmap_t &image)  noexcept {
		bmp::color_t c1 = image.get_unchecked<bmp::color_t>(x, y);
		bmp::color_t c2 = image.get_unchecked<bmp::color_t>(k, l);
		auto delta = c1 - c2;
		double r2 = square(std::get<0>(delta));
		double g2 = square(std::get<1>(delta));
		double b2 = square(std::get<2>(delta));
		return std::exp(-(square(x-k)+square(y-l))/(2*square(sigma_d))- 
			(r2+g2+b2) / (2 * square(sigma_s)));
	}
	//
	bmp::bitmap_t  bilateral_filter(const bmp::bitmap_t &image, size_t r, double sigma_r) noexcept {
		bmp::bitmap_t rval(image);
		
		auto[clbegin, clend] = counter_2d<size_t>::make(0, 0,
				rval.width, rval.height);
		
		std::for_each(std::execution::par_unseq, clbegin, clend,
			[&r, &image, &sigma_r, &rval](auto &&p) {
				auto[x, y] = p;
				auto x0 = std::max(x - r, 0ull), y0 = std::max(y - r, 0ull),
					x1 = std::min(x + r + 1, image.width), y1 = std::min(y + r + 1, image.height);
				auto sum = std::make_tuple(0.0, 0.0, 0.0);

				
				double sigma_d = (x1 - x0 + 1)*(y1 - y0 + 1) - 1;
				double tot=0.0;
				for (auto yy = y0; yy < y1; yy++) {
					for (auto xx = x0; xx < x1; xx++) {
						tot = tot + omega(x, y, xx, yy, sigma_d, sigma_r, image);
					}
				}
				std::tuple<double, double, double> tot2 = { 0,0,0 };
				for (auto yy = y0; yy < y1; yy++) {
					for (auto xx = x0; xx < x1; xx++) {
						tot2 = tot2 + 
							color_scale(image.get_unchecked<bmp::color_t>(xx,yy),
								omega(x, y, xx, yy, sigma_d, sigma_r, image));
					}
				}
				
				rval.set_unchecked(x,y,color_round(color_scale(tot2,1.0 / tot)));
			});
		return rval;
	}
}

#endif