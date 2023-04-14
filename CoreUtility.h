#pragma once
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <random>
class CoreUtility {
public:
	static int ut(int b) {
		return b+1;
	}
	static std::string convert_buffer(boost::array<char, 1024> buffer, size_t length) {
		std::string data = buffer.data();
		std::string out = "";
		for (std::size_t i = 0; i < length; i++) {
			out += data[i];
		}return out;
	}
	static std::string streambuffer_to_string(boost::asio::streambuf& streambuf) {
		return { boost::asio::buffers_begin(streambuf.data()),
			boost::asio::buffers_end(streambuf.data()) };
	}

};
class Random {
private:
	std::mt19937 seed_eng() {
		std::random_device r;
		std::seed_seq seed{ r(),r(), r(), r() };
		return std::mt19937(seed);
	}
	auto operator()(int a, int b) {
		std::mt19937 eng = this->seed_eng();
		std::uniform_int_distribution<int> dst(a, b);
		return dst(eng);
	}
	
public:
	static int randomint_range(int min, int max) {
		Random r;
		return r(min, max);
	}
	static int random_int() {
		Random r;
		return r(0, 100000000);
	}
};