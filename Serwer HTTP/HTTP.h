#pragma once
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>
#include <json.hpp>
#include <thread>
#include <numeric>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <sstream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <string>
#include "tcp.h"
#include "response.h"
//namespace beast = boost::beast;
//namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
using namespace Web;
namespace Web{

	class HTTP
	{
	public:
		/*
		static http::response<http::dynamic_body> get_site_content(const char* url) {
			http::response<http::dynamic_body> return_response;
			try {
				auto const host = "www.google.com";
				auto const port = 80;
				auto const target = "/";
				net::io_context ioc;
				tcp::resolver resolver(ioc);
				beast::tcp_stream stream(ioc);
				auto const results = resolver.resolve(host, "80");
				stream.connect(results);

				http::request<http::string_body> req{ http::verb::get,target,11 };
				req.set(http::field::host, host);
				req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
				http::write(stream, req);
				beast::flat_buffer buffer;
				http::response<http::dynamic_body> response;
				http::read(stream, buffer, response);
				std::cout << response << std::endl;
				beast::error_code err;
				return_response = response;
				stream.socket().shutdown(tcp::socket::shutdown_both, err);
				if (err && err != beast::errc::not_connected)
					throw beast::system_error{ err };
			}
			catch (std::exception const& e) {
				std::cerr << "error: " << e.what() << std::endl;
			}
			return return_response;
		}*/

		
		static std::string send_raw(std::string body, std::string mime_type) {
			response* resp = new response();
			resp->status += "200 OK\n";
			resp->date += get_time();
			resp->last_modified += get_time();
			resp->etag = "ETag: '" + std::to_string(Random::randomint_range(0, 2137)) + "'\n";
			resp->accept_ranges += "bytes\n";
			resp->body += body;
			resp->content_type += mime_type + "\n";
			resp->content_length += std::to_string(body.size()+1)+"\n";
			resp->connection += "close\n";
			std::cout << resp->body;
			return response::to_string(*resp);
		}

		
	private:
		
		static std::string get_time() {
			std::time_t rawtime;
			time(&rawtime);
			return  ctime(&rawtime);
		}
	};

	class http_server {
	public:
		http_server(std::string host, const int PORT) {
			//TCP::tcp_server(*ioc, host, PORT);
		}

	private:
		boost::asio::io_context* ioc = new boost::asio::io_context();
	};

}