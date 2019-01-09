#include "bmp.h"
#include <fstream>
#include <execution>
#include <utils/counter.h>
#include <algorithm>

using utils::counter;

namespace bmp
{

	file_header_t::file_header_t() {
		type = 19778;
		reserved = 0;
		offbit = 54;
	}

	bitmap_header_t::bitmap_header_t() {
		size = 40;
		planes = 1;
		bits_per_pixel = 24;
		compression = 0;
		x_pixels_per_meter = 4723;
		y_pixels_per_meter = 4723;
		color_used = 0;
		color_important = 0;
	}
	std::ifstream &operator>>(std::ifstream &input, bitmap_t &bitmap) {
		file_header_t file_header;
		bitmap_header_t bitmap_header;

		input.read(reinterpret_cast<char *>(&file_header), sizeof(file_header_t));
		input.read(reinterpret_cast<char *>(&bitmap_header), sizeof(bitmap_header_t));
		bitmap.line_byte_count = ((bitmap_header.width * bitmap_header.bits_per_pixel + 31) >> 5) << 2;

		auto data_size = bitmap.line_byte_count * bitmap_header.height;
		if (bitmap.data != nullptr)
			delete bitmap.data;

		bitmap.byte_per_pixel = 3;
		bitmap.data = new uint8_t[data_size];
		input.read(reinterpret_cast<char *>(bitmap.data), data_size);
		bitmap.width = bitmap_header.width;
		bitmap.height = bitmap_header.height;

		return input;
	}

	std::ofstream &operator<<(std::ofstream &output, const bitmap_t &bitmap) {
		file_header_t file_header;
		bitmap_header_t bitmap_header;
		file_header.size = sizeof(file_header_t) +
			sizeof(bitmap_header_t) + bitmap.line_byte_count * bitmap.height;
		bitmap_header.size_of_image = bitmap.line_byte_count * bitmap.height;
		bitmap_header.width = bitmap.width;
		bitmap_header.height = bitmap.height;
		output.write(reinterpret_cast<const char *>(&file_header), sizeof(file_header_t));
		output.write(reinterpret_cast<const char *>(&bitmap_header), sizeof(bitmap_header_t));
		output.write(reinterpret_cast<const char *>(bitmap.data), bitmap_header.size_of_image);
		return output;
	}
	


	 hsl_t bitmap_t::get_hsl_unchecked(size_t x, size_t y) const noexcept {
		auto offset = y * line_byte_count + x * byte_per_pixel;

		auto r = data[offset + 2] / 255.0, g = data[offset + 1] / 255.0, b = data[offset] / 255.0;

		double M = std::max(r, std::max(g, b)), m = std::min(r, std::min(g, b));
		double H = 0;

		if (M == r and g >= b) {
			H = 60.0 * (g - b) / (M - m);
		} else if (M == r and g < b) {
			H = 60.0 * (g - b) / (M - m) + 360;
		} else if (M == g) {
			H = 60.0 * (b - r) / (M - m) + 120;
		} else if (M == b) {
			H = 60.0 * (r - g) / (M - m) + 240;
		}

		double L = (M + m) / 2;
		double S = 0;
		if (L < 1e-6 || M == m) {
			S = 0;
		} else if (L < 0.5) {
			S = (M - m) / (2 * L);
		} else {
			S = (M - m) / (2 * 1 - 2 * L);
		}

		return make_RGB(H*255.0 / 360, S*255.0, L*255.0);
	}

	 void bitmap_t::set_hsl_unchecked(size_t x, size_t y, hsl_t hsl) noexcept {
		auto H = std::clamp(std::get<0>(hsl),0.0,255.0) / 255.0,
			S = std::clamp(std::get<1>(hsl),0.0,255.0) / 255.0,
			L = std::clamp(std::get<2>(hsl),0.0,255.0) / 255.0;
		if (S == 0) {
			set(x, y, make_RGB(L*255.0, L*255.0, L*255.0));
		} else {
			double q = 0;
			if (L < 0.5) {
				q = L * (1 + S);
			} else {
				q = L + S - (L*S);
			}
			double P = 2 * L - q;
			double t[] = { H + 1 / 3.0, H,  H - 1 / 3.0 };
			if (t[0] > 1) t[0] -= 1;
			if (t[2] < 0) t[2] += 1;
			double r[3];
			for (auto i = 0; i < 3; i++) {
				if (t[i] < 1 / 6.0) r[i] = P + (q - P) * 6 * t[i];
				else if (t[i] >= 1 / 6.0 && t[i] < 1 / 2.0) r[i] = q;
				else if (t[i] >= 1 / 2.0 && t[i] < 2 / 3.0) r[i] = P + ((q - P) * 6 * (2 / 3.0 - t[i]));
				else r[i] = P;
			}
			set(x, y, make_RGB(r[0] * 255, r[1] * 255, r[2] * 255));

		}
	}

	bitmap_t::bitmap_t() noexcept {
		byte_per_pixel = 3;
		data = nullptr;
	}
	bitmap_t::bitmap_t(const bitmap_t & other) {
		width = other.width;
		height = other.height;
		byte_per_pixel = 3;
		line_byte_count = ((width * 24 + 31) >> 5) << 2;
		data = new uint8_t[line_byte_count * height];
		std::copy(std::execution::par_unseq, other.data, other.data + line_byte_count * height, data);
	}
	bitmap_t::bitmap_t(const bitmap_t & other, size_t x0, size_t y0, size_t w, size_t h) {
		w = std::min(w, other.width - x0);
		h = std::min(h, other.height - y0);
		width = w;
		height = h;
		byte_per_pixel = 3;
		line_byte_count = ((width * 24 + 31) >> 5) << 2;
		data = new uint8_t[line_byte_count * height];
		std::for_each(counter(y0), counter(y0+h), [&](auto y) {
			std::copy(std::execution::par_unseq, 
				other.data + other.line_byte_count * y + x0 * byte_per_pixel,
				other.data + other.line_byte_count * y + (x0 + w)* byte_per_pixel, data + (y - y0)*line_byte_count);
		});
	
	}
	bitmap_t::bitmap_t(size_t w, size_t h) {
		width = w;
		height = h;
		byte_per_pixel = 3;
		line_byte_count = ((width * 24 + 31) >> 5) << 2;
		data = new uint8_t[line_byte_count * height];
		std::fill(data, data + line_byte_count * height, 0);
	}
	bitmap_t & bitmap_t::operator=(const bitmap_t &other) & noexcept {
		*this = bitmap_t(other);
		return *this;
	}
	bitmap_t & bitmap_t::operator=(bitmap_t &&other) & noexcept {
		byte_per_pixel = other.byte_per_pixel;
		width = other.width;
		line_byte_count = other.line_byte_count;
		height = other.height;
		if (data != nullptr) delete[] data;
		data = other.data;
		other.data = nullptr;
		return *this;
	}

} // namespace bmp
