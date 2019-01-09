#include "binarization.h"
#include "utils/counter.h"

#include<algorithm>
#include<vector>
#include<execution>
#include<array>
#include<iostream>
 

using utils::counter;
using utils::counter_2d;

namespace process
{
	
	void process::grayscale(bmp::bitmap_t & image) noexcept {
		std::for_each(std::execution::par_unseq,
			counter_2d<size_t>::begin(0, 0, image.width, image.height),
			counter_2d<size_t>::end(0, 0, image.width, image.height), [&image](auto p) {
			auto x = p.first, y = p.second;
			
			auto yuv = image.get_unchecked<bmp::yuv_t>(x, y);
			auto Y = std::get<0>(yuv);
			image.set_unchecked(x, y, bmp::make_RGB(Y, Y, Y));

		});
	}

	template<typename U>
	void binarize(bmp::bitmap_t &image, uint8_t threshold,
		size_t x0, size_t y0, size_t x1, size_t y1, U u) noexcept {
		x1 = std::min(x1, image.width - 1);
		y1 = std::min(y1, image.height - 1);


		std::for_each(std::execution::par_unseq, counter<size_t>(y0), counter<size_t>(y1 + 1), [&](auto y) {
			std::for_each(std::execution::par_unseq, counter<size_t>(x0), counter<size_t>(x1 + 1), [&](auto x) {
				auto Y = image.get<uint8_t>(x, y);

				if (u(Y, threshold)) {
					image.set_unchecked(x, y, bmp::make_RGB(uint8_t(0), uint8_t(0), uint8_t(0)));
				} else {
					image.set_unchecked(x, y, bmp::make_RGB(uint8_t(255), uint8_t(255), uint8_t(255)));
				}
			});
		});
	}

	



	uint8_t otsu(const std::array<double, 256>& histogram, size_t total) noexcept {
		std::array<double, 256> sigma;
		std::array<double, 256> presum;
		std::array<double, 256> i_his;
		std::array<double, 256> i_his_presum;

		std::inclusive_scan(std::execution::par_unseq,
			std::begin(histogram), std::end(histogram), std::begin(presum));

		std::transform(std::execution::par_unseq,
			std::begin(histogram), std::end(histogram), std::begin(i_his),
			[&histogram](auto &x) {
			size_t index = std::distance(&histogram[0], &x);
			return index * x;
		});

		std::inclusive_scan(std::execution::par_unseq,
			std::begin(i_his), std::end(i_his), std::begin(i_his_presum));

		std::transform(std::execution::par_unseq,
			std::begin(histogram), std::end(histogram), std::begin(sigma),
			[&histogram, &presum, &i_his_presum](auto &x) {
			size_t index = std::distance(&histogram[0], &x);
			auto q_l = presum[index];
			auto q_h = presum[255] - q_l;
			auto miu_l = static_cast<double>(i_his_presum[index]) / q_l;
			auto miu_h = static_cast<double>(i_his_presum[255] - i_his_presum[index]) / q_h;

			return std::abs(q_l) < 1e-8 || std::abs(q_h) < 1e-8 ? 0 : q_l * q_h*(miu_l - miu_h)*(miu_l - miu_h);
		});
		auto m = std::max_element(std::execution::par_unseq, std::begin(sigma), std::end(sigma));
		return std::distance(std::begin(sigma), m);
	}
	uint8_t get_threshold(const bmp::bitmap_t &image, size_t x0, size_t y0, size_t x1, size_t y1) noexcept {
		std::array<double, 256> histogram = {};
		x1 = std::min(x1, image.width - 1);
		y1 = std::min(y1, image.height - 1);


		for (size_t y = y0; y <= y1; y++) {
			for (size_t x = x0; x <= x1; x++) {
				auto Y = image.get_unchecked<uint8_t>(x, y);
				histogram[Y]+=1;
			}
		}
		auto total = (y1 - y0 + 1)*(x1 - x0 + 1);
		
		std::transform(std::execution::par_unseq,
			std::begin(histogram), std::end(histogram), std::begin(histogram), [total](auto x) {
			return x / total;
		});

		return otsu(histogram, total);
	}


	void binarize_window(bmp::bitmap_t & image, size_t x0, size_t y0, size_t x1, size_t y1) noexcept {

		auto threshold = get_threshold(image, x0, y0, x1, y1);
		return binarize(image, threshold, x0, y0, x1, y1);
	}

	void auto_binarize(bmp::bitmap_t & image) noexcept {
		auto x0 = size_t(0), y0 = size_t(0), x1 = image.width - 1, y1 = image.height - 1;
		auto threshold = get_threshold(image, x0, y0, x1, y1);
		binarize(image, threshold, x0, y0, x1, y1);
	}
	void auto_binarize(bmp::bitmap_t & image, size_t x_num, size_t y_num) noexcept {
		auto w = image.width / x_num;
		auto h = image.height / y_num;
		
		std::for_each(std::execution::par_unseq,
			counter<size_t>(0), counter<size_t>((x_num + 1)*(y_num + 1)), [&](auto n) {
			auto x = n % (x_num + 1), y = n / (x_num + 1);
			auto x0 = x * w, y0 = y * h, x1 = x * w + w - 1, y1 = y * h + h - 1;

			auto threshold = get_threshold(image, x0, y0, x1, y1);
			binarize(image, threshold, x0, y0, x1, y1);
		});
	}
}