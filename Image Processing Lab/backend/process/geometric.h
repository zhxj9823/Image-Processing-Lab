#ifndef PROCESS_GEOMETRIC_H
#define PROCESS_GEOMETRIC_H

#include <utils/counter.h>
#include <utils/matrix.h>
#include <bitmap/pixel_accessor.h>
#include <bitmap/bmp.h>
#include <execution>

using bmp::pixel_accessor_t;
using utils::counter_2d;
using utils::transform2D_t;
using utils::vector2_t;

namespace process
{
	inline void fill(bmp::bitmap_t &image, size_t x, size_t y, size_t w, size_t h, bmp::color_t color) {
		auto accessor_getter = [&image](size_t x, size_t y) {
			return pixel_accessor_t(image, x, y);
		};

		auto[lbegin, lend] = counter_2d<size_t,
			decltype(accessor_getter)>::make(x, y,
				std::min(x + w, image.width), std::min(y + h, image.height), accessor_getter);

		std::fill(std::execution::par_unseq, lbegin, lend, color);
	}
	inline std::tuple<double, double, double> color_scale(const std::tuple<double, double, double> &color, double scaler) {
		auto[r, g, b] = color;
		return { r*scaler, g*scaler, b*scaler };
	}
	inline std::tuple<double, double, double> color_scale(const bmp::color_t &color, double scaler) {
		auto[r, g, b] = color;
		return { r*scaler, g*scaler, b*scaler };
	}
	inline bmp::color_t color_round(const std::tuple<double, double, double> &c) {
		auto[r, g, b] = c;
		return { std::clamp(lround(r),0l,255l),
			std::clamp(lround(g),0l,255l),std::clamp(lround(b),0l,255l) };
	}
	
	inline std::tuple<double, double, double> operator+(const std::tuple<double, double, double> &x,
		const std::tuple<double, double, double>&y) {
		auto[r1, g1, b1] = x;
		auto[r2, g2, b2] = y;
		return { r1 + r2, g1 + g2, b1 + b2 };
	}
	inline std::tuple<double, double, double> operator-(const std::tuple<double, double, double> &x,
		const std::tuple<double, double, double>&y) {
		auto[r1, g1, b1] = x;
		auto[r2, g2, b2] = y;
		return { r1 - r2, g1 - g2, b1 - b2 };
	}
	inline std::tuple<double, double, double> operator/(const std::tuple<double, double, double> &x,
		const std::tuple<double, double, double>&y) {
		auto[r1, g1, b1] = x;
		auto[r2, g2, b2] = y;
		return { r1 / r2, g1 / g2, b1 / b2 };
	}
	inline std::tuple<double, double, double> operator*(const std::tuple<double, double, double> &x,
		const std::tuple<double, double, double>&y) {
		auto[r1, g1, b1] = x;
		auto[r2, g2, b2] = y;
		return { r1 * r2, g1 * g2, b1 * b2 };
	}
	struct bilinear_t {
		bmp::color_t operator()(const bmp::bitmap_t &img, double x, double y) {
			size_t x0 = x, y0 = y;
			auto&& f00 = img.get<bmp::color_t>(x0, y0);
			auto&& f01 = img.get<bmp::color_t>(x0, y0 + 1);
			auto&& f10 = img.get<bmp::color_t>(x0 + 1, y0);
			auto&& f11 = img.get<bmp::color_t>(x0 + 1, y0 + 1);
			if (x0 == img.width - 1) {
				f10 = f00;
				f11 = f01;
			}
			if (y0 == img.height - 1) {
				f01 = f00;
				f11 = f10;
			}
			double xx = x - floor(x), yy = y - floor(y);
			return color_round(color_scale(f00, (1 - xx)*(1 - yy)) + color_scale(f01, (1 - xx)*yy) +
				color_scale(f10, xx*(1 - yy)) + color_scale(f11, xx*yy));
		}
	};

	struct nearest_neighbor_t {
		bmp::color_t operator()(const bmp::bitmap_t &img, double x, double y) {
			return img.get<bmp::color_t>(std::lround(x), std::lround(y));
		}
	};


	using utils::operator-;

	template<typename Interpolate = nearest_neighbor_t>
	inline void transform(bmp::bitmap_t &image, size_t x, size_t y, size_t w, size_t h,

		const transform2D_t &trans, Interpolate u = Interpolate()) {

		bmp::bitmap_t area(image, x, y, w, h);
		fill(image, x, y, w, h, bmp::make_RGB(uint8_t(0), uint8_t(0), uint8_t(0)));
		std::array<vector2_t, 4> pos = { { { x,y },{x + w,y },  { x,y + h },  { x + w,y + h } } };
		std::transform(std::execution::par_unseq, std::begin(pos), std::end(pos), std::begin(pos),
			[&trans](auto &&p) {return trans * p; });
		auto xm = 1e10, xM = 0.0, ym = 1e10, yM = 0.0;
		for (auto && p : pos) {
			auto[x, y] = p;
			xm = std::min(xm, x);
			xM = std::max(xM, x);
			ym = std::min(ym, y);
			yM = std::max(yM, y);
		}
		xm = std::max(xm, 0.0);
		ym = std::max(ym, 0.0);
		xM = std::min(xM, double(image.width));
		yM = std::min(yM, double(image.height));
		auto accessor_getter = [&image](size_t x, size_t y) {
			return pixel_accessor_t(image, x, y);
		};

		auto[lbegin, lend] =
			counter_2d<size_t, decltype(accessor_getter)>::make(xm, ym, xM, yM, accessor_getter);

		auto inv = trans.inverse();
		std::transform(lbegin, lend, lbegin, [&](auto &&p) {
			auto[ox, oy] = p.pair();
			auto[x, y] = inv * (p.pair()) - vector2_t(x, y);
			if (x < 0 || x + 0.5 >= area.width || y < 0 || y + 0.5 >= area.height)  return image.get<bmp::color_t>(ox, oy);
			return u(area, x, y);
			});
	}

}

#endif // !PROCESS_GEOMETRIC_H
