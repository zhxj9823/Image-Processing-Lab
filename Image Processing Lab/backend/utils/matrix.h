#ifndef UTILS_MATRIX_H
#define UTILS_MATRIX_H

#include <array>


namespace utils
{


	using vector2_t = std::pair<double, double>;

	inline vector2_t operator-(const vector2_t &a, const vector2_t &b) {
		return vector2_t(a.first - b.first, a.second - b.second);
	}
	struct transform2D_t {
		double a, b, c, d, e, f;
		transform2D_t(double a, double b, double d, double e, double c = 0, double f = 0)
			:a(a), b(b), c(c), d(d), e(e), f(f) {}
		
		transform2D_t inverse()const {
			return transform2D_t(e / (a*e - b * d),
				b / (b*d - a * e), d / (b*d - a * e), a / (a*e - b * d),
				(b*f - c * e) / (a*e - b * d), (c*d - a * f) / (a*e - b * d));
		}
		static transform2D_t translate(double x, double y) {
			return transform2D_t(1, 0, 0, 1, x, y);
		}
		static transform2D_t scale(double x, double y) {
			return transform2D_t(x, 0.0, 0.0, y);
		}
		static transform2D_t rotate(double theta) {
			return transform2D_t(std::cos(theta), std::sin(theta),
				-std::sin(theta), std::cos(theta));
		}
		static transform2D_t shear(double dx, double dy) {
			return transform2D_t(1.0, dx, dy, 1.0);
		}

		template<bool MX, bool MY>
		static transform2D_t mirror() {
			if constexpr (MX && MY) {
				return transform2D_t(-1, 0.0, 0.0, -1);
			}
			if constexpr (!MX && MY) {
				return transform2D_t(1, 0.0, 0.0, -1);
			}
			if constexpr (MX && !MY) {
				return transform2D_t(-1, 0.0, 0.0, 1);
			}
			if constexpr (!MX && !MY) {
				return transform2D_t(1, 0.0, 0.0, 1);
			}
		}

	};
	inline transform2D_t operator*(const transform2D_t &t1, const transform2D_t &t2) {
		return transform2D_t{
			t1.a*t2.a+t1.b*t2.d, t1.a*t2.b+t1.b*t2.e,
			t2.a*t1.d+t2.d*t1.e, t2.b*t1.d+t1.e*t2.e,
			t1.c + t1.a*t2.c + t1.b*t2.f, t2.c*t1.d+t1.f+t1.e*t2.f
		};
	}
	inline vector2_t operator*(const transform2D_t &trans, const vector2_t&vec) {
		return vector2_t(trans.a*vec.first + trans.b*vec.second + trans.c,
			trans.d*vec.first + trans.e*vec.second + trans.f);
	}
}

#endif