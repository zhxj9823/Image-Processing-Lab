#ifndef COUNTER_H_
#define COUNTER_H_

#include<iterator>

namespace utils {

	template<typename T>
	class counter
	{
	public:
		using value_type = T;
		using reference = const T&;
		using pointer = const T*;
		using difference_type = ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;
		counter() = default;
		explicit counter(T x) { inc = x; }

		size_t const& base() const { return inc; }
		reference operator*() const { return inc; }
		counter& operator++() { ++inc; return *this; }
		counter& operator--() { --inc; return *this; }

		counter& operator+=(size_t i) { inc += i; return *this; }
		counter operator+(size_t i) const { return counter(inc + i); }
		counter operator-(size_t i) const { return counter(inc - i); }
		difference_type operator-(const counter<T> o) { return inc - o.inc; };
		counter& operator-=(size_t i) { inc -= i; return *this; }

		bool operator!=(const counter<T> &other) { return inc != other.inc; }
	private:
		T inc;
	};

	template<typename T, typename U>
	struct make_pair_t {
		make_pair_t() = default;
		inline std::pair<T, U> operator()(T &&t, U &&u) const {
			return std::make_pair(std::forward<T &&>(t), std::forward<T &&>(u));
		}
	};

	template<typename T, typename U = make_pair_t<T, T>>
	class counter_2d
	{
	public:
		using value_type = std::invoke_result_t<U, T, T>;
		using reference = const value_type&;
		using pointer = const value_type*;
		using difference_type = ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;
		counter_2d(U uu = U()) : u(uu) { inc = 0; };

		counter_2d(const counter_2d<T>& o) : u(o.u) {
			inc = o.inc;
			start_x = o.start_x;
			start_y = o.start_y;
			end_x = o.end_x;
			end_y = o.end_y;
		}
		counter_2d(T i, T sx, T ex, T sy, T ey, U uu = U()) :u(uu) {
			inc = i; start_x = sx; end_x = ex; start_y = sy; end_y = ey;

		}


		static counter_2d<T, U> begin(T x0, T y0, T x1, T y1, U uu = U()) {
			return counter_2d(0, x0, x1, y0, y1, uu);
		}

		static counter_2d<T, U> end(T x0, T y0, T x1, T y1, U uu = U()) {
			return counter_2d((x1 - x0)*(y1 - y0), x0, x1, y0, y1, uu);
		}
		inline static std::pair<counter_2d<T, U>, counter_2d<T, U>>
			make(T x0, T y0, T x1, T y1, U uu = U()) {
			return std::make_pair(begin(x0, y0, x1, y1, uu), end(x0, y0, x1, y1, uu));
		}

		std::pair<T, T> pair()const {
			return std::makr_pair(inc % (end_x - start_x) + start_x, inc / (end_x - start_x) + start_y);
		}
		value_type operator*() const {
			return u(inc % (end_x - start_x)+start_x, inc / (end_x - start_x)+start_y);
		}
		counter_2d& operator++() {
			++inc;

			return *this;
		}
		counter_2d& operator--() {
			--inc;
			return *this;
		}

		counter_2d& operator+=(size_t i) {
			inc += i;
			return *this;
		}
		counter_2d operator+(size_t i) const {

			return counter_2d(i + inc, start_x, end_x, start_y, end_y, u);
		}
		counter_2d operator-(size_t i) const {
			return counter_2d(inc - i, start_x, end_x, start_y, end_y, u);
		}
		difference_type operator-(const counter_2d<T,U>& o) const { return inc - o.inc; };
		counter_2d& operator-=(size_t i) {
			inc -= i;
			return *this;
		}
		utils::counter_2d<T, U> & operator=(const utils::counter_2d<T, U> & o) & {
			inc = o.inc;
			start_x = o.start_x;
			start_y = o.start_y;
			end_x = o.end_x;
			end_y = o.end_y;
			return *this;
		}
		bool operator!=(const counter_2d<T, U> &other) { return inc != other.inc; }
		// and loads of others
	private:
		T start_x, end_x, start_y, end_y;
		T inc;
		U u;
		
	};




}
#endif