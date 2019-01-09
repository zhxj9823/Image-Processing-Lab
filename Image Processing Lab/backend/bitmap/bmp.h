#ifndef BMP_H__
#define BMP_H__

#include <cstdint>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace bmp
{

#pragma pack(push, 1)
	struct file_header_t
	{
		uint16_t type;
		uint32_t size;
		uint32_t reserved;
		uint32_t offbit;

		file_header_t();
	};

	struct bitmap_header_t
	{
		uint32_t size;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bits_per_pixel;
		uint32_t compression;
		uint32_t size_of_image;
		int32_t x_pixels_per_meter;
		int32_t y_pixels_per_meter;
		uint32_t color_used;
		uint32_t color_important;

		bitmap_header_t();
	};
#pragma pack(pop)

	using color_t = std::tuple<uint8_t, uint8_t, uint8_t>;
	using yuv_t = std::tuple<double, double, double>;
	using hsl_t = std::tuple<double, double, double>;

	inline uint8_t clip(double x) noexcept {
		return static_cast<uint8_t>(std::clamp(x, 0.0, 255.0));
	}

	inline hsl_t HSL(double y, double u, double v) noexcept {
		return std::make_tuple(y, u, v);
	}

	inline yuv_t YUV(double y, double u, double v) noexcept {
		return std::make_tuple(y, u, v);
	}

	inline color_t make_RGB(double r, double g, double b) noexcept {
		return std::make_tuple(clip(r), clip(g), clip(b));
	}
	inline color_t make_RGB(uint8_t r, uint8_t g, uint8_t b) noexcept {
		return std::make_tuple(r, g, b);
	}


	struct bitmap_t
	{
		size_t width;
		size_t height;
		size_t line_byte_count;
		size_t byte_per_pixel;
		uint8_t *data;

		inline void set(size_t x, size_t y, color_t color) {
			if (x > width || y > height)
				throw "exceeding the border!";
			set_unchecked(x, y, color);
		}
		inline void set(size_t x, size_t y, yuv_t color) {
			if (x > width || y > height)
				throw "exceeding the border!";
			set_unchecked(x, y, color);
		}


		inline void set_unchecked(size_t x, size_t y, color_t color) noexcept {
			auto offset = y * line_byte_count + x * byte_per_pixel;
			data[offset + 2] = std::get<0>(color);
			data[offset + 1] = std::get<1>(color);
			data[offset] = std::get<2>(color);
		}

		inline void set_unchecked(size_t x, size_t y, yuv_t color) noexcept {
			auto offset = y * line_byte_count + x * byte_per_pixel;
			auto Y = std::get<0>(color), U = std::get<1>(color), V = std::get<2>(color);
			set_unchecked(x, y, make_RGB(Y + 1.13983 * (V - 128), Y - 0.39465 * (U - 128) - 0.58060 * (V - 128), Y + 2.03211 * (U - 128)));
		}

		template<typename T>
		inline T get(size_t x, size_t y) const {
			if (x > width || y > height)
				throw "exceeding the border!";
			return get_unchecked<T>(x, y);
		}

		template<typename T>
		inline T get_unchecked(size_t x, size_t y) const noexcept;

		template<>
		inline color_t get_unchecked<color_t>(size_t x, size_t y) const noexcept {
			auto offset = y * line_byte_count + x * byte_per_pixel;
			return std::make_tuple(data[offset + 2], data[offset + 1], data[offset]);
		}

		template<>
		inline uint8_t get_unchecked<uint8_t>(size_t x, size_t y) const noexcept {
			auto offset = y * line_byte_count + x * byte_per_pixel;
			return data[offset + 2];
		}
		template<>
		inline yuv_t get_unchecked<yuv_t>(size_t x, size_t y) const noexcept {
			auto offset = y * line_byte_count + x * byte_per_pixel;

			auto r = data[offset + 2], g = data[offset + 1], b = data[offset];

			auto Y = 0.299 * r + 0.587 * g + 0.114 * b;
			auto U = -0.169 * r - 0.331 * g + 0.5 * b + 128;
			auto V = 0.5 * r - 0.419 * g - 0.081 * b + 128;
			return std::make_tuple(Y, U, V);
		}



		hsl_t get_hsl_unchecked(size_t x, size_t y) const noexcept;

		inline hsl_t get_hsl(size_t x, size_t y) const {
			return get_hsl_unchecked(x, y);
		}
		void set_hsl_unchecked(size_t x, size_t y, hsl_t hsl) noexcept;


		bitmap_t() noexcept;
		bitmap_t(const bitmap_t &other);
		bitmap_t(const bitmap_t &other, size_t x0, size_t y0, size_t w, size_t h);
		bitmap_t(size_t width, size_t height);
		~bitmap_t() noexcept {
			delete[] data;
		}

		bitmap_t & operator=(const bitmap_t &other) & noexcept;
		bitmap_t & operator=(bitmap_t && other) & noexcept;


	};

	std::ifstream &operator>>(std::ifstream &input, bitmap_t &bitmap);
	std::ofstream &operator<<(std::ofstream &output, const bitmap_t &bitmap);
	inline void write(bitmap_t &bmp, std::string file) {
		std::ofstream fout(file, std::ios::binary);
		fout << bmp;
		fout.close();
	}
	inline bitmap_t load(std::string file) {
		bitmap_t bmp;
		std::ifstream fin(file, std::ios::binary);
		fin >> bmp;
		fin.close();
		return bmp;
	}
} // namespace bmp
#endif
