#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include<cstdlib>
#include <functional>
#include "CoreUtility.h"
#include "request_parser.h"
#define LOG(X) std::cout<<X<<std::endl
using boost::asio::ip::tcp;

namespace Web {
	namespace TCP {

		class tcp_connection
			: public boost::enable_shared_from_this<tcp_connection>
		{
		public:
			typedef boost::shared_ptr<tcp_connection> pointer;

			static pointer create(boost::asio::io_context& io_context, std::function<void(std::string,std::string&)> handler_function)
			{
				return pointer(new tcp_connection(io_context,handler_function));
			}

			tcp::socket& socket()
			{
				return socket_;
			}

			void start()
			{

				boost::asio::async_read(socket_, read_buffer, boost::asio::transfer_at_least(1),
					boost::bind(&tcp_connection::handle_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

			}

		private:
			char data[1024];

			std::size_t c_c(const boost::system::error_code& error, std::size_t bytes_transferred) {
				if (!error)return 0;
				else return 1;
			}

			std::string bufmake_string(boost::asio::streambuf& streambuf)
			{
				return { boost::asio::buffers_begin(streambuf.data()),
						boost::asio::buffers_end(streambuf.data()) };
			}


			boost::asio::streambuf read_buffer;
			tcp_connection(boost::asio::io_context& io_context, std::function<void(std::string,std::string&)> handler_function)
				: socket_(io_context)
			{
				request_handler = handler_function;
			}
			
			std::function<void(std::string,std::string&)> request_handler;
			std::string message_;

			void handle_read(const boost::system::error_code& error, std::size_t size) {
				if (!error) {
					std::string request_url = bufmake_string(read_buffer);
					//LOG(request_url);
					request_handler(request_url,message_);
					boost::asio::async_write(socket_, boost::asio::buffer(message_,(int)message_.size()),
						boost::bind(&tcp_connection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
				}
				else {
					std::cerr << "error: " << error.message() << std::endl;
					socket_.close();
				}
			}


			void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
			{
				//puts("sending");
				if (!error) {}
				else {
					std::cerr << "error: " << error.message() << std::endl;
					socket_.close();
				}
			}

			tcp::socket socket_;
		};



		//SERVER
		class tcp_server
		{
		public:
			std::function<void(std::string, std::string&)> request_handler = std::function<void(std::string, std::string&)>();


			tcp_server(boost::asio::io_context& io_context,std::string address, int port,std::function<void(std::string,std::string&)> f)
				: io_context_(io_context),
				acceptor_(io_context, tcp::endpoint(boost::asio::ip::make_address_v4(address),port))
			{
				request_handler = f;
				start_accept(request_handler);
			}
			static void async_server(std::string address, int port, void (*f)(std::string,std::string&)) {
				std::cout << "started" << std::endl;
				boost::asio::io_context ioc;
				tcp_server tcp(ioc,address,port,f);
				ioc.run();
			}
		private:
			void start_accept(std::function<void(std::string,std::string&)> f)
			{
				tcp_connection::pointer new_connection =
					tcp_connection::create(io_context_, f);

				acceptor_.async_accept(new_connection->socket(),
					boost::bind(&tcp_server::handle_accept, this, new_connection,
						boost::asio::placeholders::error));
			}

			void handle_accept(tcp_connection::pointer new_connection,
				const boost::system::error_code& error)
			{
				if (!error)
				{
					new_connection->start();
				}

				start_accept(request_handler);
			}

			boost::asio::io_context& io_context_;
			tcp::acceptor acceptor_;
		};

		class tcp_client {
		public:

			tcp_client(std::string addr, const int port) {
				//try{
				//	boost::asio::io_context ioc;
				//	tcp::resolver resolver(ioc);
				//	tcp::endpoint _ep(boost::asio::ip::make_address_v4(addr), port);
				//	tcp::resolver::results_type _rt = resolver.resolve(_ep);
				//	tcp::socket _socket(ioc);
				//	boost::asio::connect(_socket, _rt);

				//	for (;;) {
				//		boost::system::error_code error;

				//		_socket.write_some(boost::asio::buffer("/uniform"), error);
				//		
				//		//boost::asio::async_read(_socket, buffer, boost::asio::transfer_at_least(1),
				//			//boost::bind(&tcp_client::handle_read,NULL,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred ));
				//		
				//		
				//		
				//		if (error == boost::asio::error::eof)
				//			break;
				//		else if (error)
				//			throw boost::system::system_error(error);
				//	}
				//}
				//catch (std::exception& e) {
				//	std::cerr << e.what() << std::endl;
				//}


				//connection
				boost::asio::io_context ioc;
				tcp::socket _socket(ioc);
				_socket.connect(tcp::endpoint(boost::asio::ip::make_address_v4(addr), port));
				
				//write to server
				const std::string message = "/uniform";
				boost::system::error_code error;
				boost::asio::write(_socket, boost::asio::buffer(message), error);


				//receive from server
				boost::asio::streambuf recvBuffer;
				boost::asio::read(_socket, recvBuffer, boost::asio::transfer_all(), error);

				const char* data = boost::asio::buffer_cast<const char*>(recvBuffer.data());
				puts(data);
				_socket.close();
				delete this->buffer;
			}
			static std::string tcp_request(std::string addr, const int port, const std::string message) {
				boost::asio::io_context ioc;
				tcp::socket _socket(ioc);
				_socket.connect(tcp::endpoint(boost::asio::ip::make_address_v4(addr), port));

				//write to server
				boost::system::error_code error;
				boost::asio::write(_socket, boost::asio::buffer(message), error);


				//receive from server
				boost::asio::streambuf recvBuffer;
				boost::asio::read(_socket, recvBuffer, boost::asio::transfer_all(), error);

				const char* data = boost::asio::buffer_cast<const char*>(recvBuffer.data());
				_socket.close();

				return data;
			}
		private:
			boost::array<char, 1024>* buffer = new boost::array<char,1024>;

		};
	}
}

