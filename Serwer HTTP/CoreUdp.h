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

#include "CoreUtility.h"

using boost::asio::ip::udp;
namespace Web{
	class UdpServer
	{
	public:
		UdpServer(boost::asio::io_context& io_context, boost::asio::ip::address_v4 address, int port,
			std::function<std::string(std::string)> lambd)
			:_socket(io_context, udp::endpoint(address, port))
		{
			this->handle_request = lambd;
			start_receive();
		}
		static UdpServer Run(std::string address, int port,
			std::function < std::string(std::string)> request_function) {
			try {
				boost::asio::io_context ioc;
				UdpServer server(ioc, boost::asio::ip::make_address_v4(address), port,
					request_function);
				ioc.run();
				return server;
			}
			catch (std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}

		static std::string Client(std::string address, int port,
			std::string _req) {
			try {
				boost::asio::io_context ioc;
				udp::resolver resolver(ioc);
				boost::asio::ip::address_v4 ipaddr = boost::asio::ip::make_address_v4(address);
				udp::endpoint ep = *resolver.resolve(udp::endpoint(ipaddr, port)).begin();

				boost::array<char, 1024> recv_buffer;
				udp::socket _socket(ioc);
				_socket.open(udp::v4());
				boost::shared_ptr<std::string> request = boost::shared_ptr<std::string>(new std::string(_req));
				_socket.send_to(boost::asio::buffer(*request), ep);
				udp::endpoint remote_ep;
				std::size_t len = _socket.receive_from(boost::asio::buffer(recv_buffer), remote_ep);
				return CoreUtility::convert_buffer(recv_buffer, len);
			}
			catch (std::exception& e) {
				return e.what();
			}
		}




	private:


		void start_receive() {
			_socket.async_receive_from(
				boost::asio::buffer(_data), _ep,
				boost::bind(&UdpServer::handle_receive, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
			if (!error) {
				auto rsp = this->handle_request(CoreUtility::convert_buffer(_data, bytes_transferred));
				boost::shared_ptr<std::string> response = boost::shared_ptr<std::string>(
					new std::string(rsp));
				_socket.async_send_to(boost::asio::buffer(*response), _ep,
					boost::bind(&UdpServer::handle_send, this, response,
						boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				start_receive();
			}
		}
		void handle_send(boost::shared_ptr<std::string> response, const boost::system::error_code& error, std::size_t length) {}

		std::function<std::string(std::string)> handle_request;
		udp::socket _socket;
		udp::endpoint _ep;
		boost::array<char, 1024> _data;
	};

}

