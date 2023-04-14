#pragma once
#include "request_parser.h"
#include <string>
namespace Web{
	struct response {
	public:
		static std::string get_time() {
			std::time_t rawtime;
			time(&rawtime);
			return  ctime(&rawtime);
		}
		response() {
			this->status = "HTTP/1.1 ";
			this->date = "DATE: ";
			this->last_modified = "Last-Modified: ";
			this->etag = "ETag: ";
			this->accept_ranges = "Accept-Ranges: ";
			this->content_length = "Content-Length: ";
			this->connection = "Connection: ";
			this->content_type = "Content-Type: ";
			this->body = "\n";
		};
		response(std::string data_url, std::string content_type) {
			std::string data_string = "";
			std::ifstream file(data_url);
			std::string t;

			if (content_type == "image/jpeg" || content_type == "image/png" ||
				content_type =="imgae/x-icon" || content_type=="image/gif" ||
				content_type=="image/bmp") {
				puts("OBRAZ");
				if (file.is_open()) {
					puts("XDD");
					int l;
					std::ostringstream os;
					char buf[65536];
					while ((l = file.readsome(buf, 65536) > 0)){
						os.write(buf, l);
					}
					data_string = os.str();
					file.close();
				}
				else {
					puts("ERROR KURWA");
				}
			}
			else if (content_type == "application/json") {

			}
			else {
				if (file.is_open()) {
					while (std::getline(file, t)) {
						data_string += t + "\n";
					}
					file.close();
				}
			}
			
			
			this->status = "HTTP/1.1 200 OK\n";
			this->date = "DATE: "+get_time();
			this->last_modified = "Last-Modified: " + get_time();
			this->etag = "ETag: '"+std::to_string(Random::randomint_range(0, 2137)) + "'\n";
			this->accept_ranges += "Accept-Ranges: bytes\n";
			this->content_length = "Content-Length: " + std::to_string(data_string.size()) + "\n";
			this->connection = "Connection: close\n";
			this->content_type = "Content-Type: " + content_type+"\n";
			this->body =data_string;
		}
		static std::string to_string(response r) {
			std::string ret = "";
			return r.status
				+= r.date
				+= r.server
				+= r.last_modified
				+= r.etag
				+= r.accept_ranges
				+= r.content_length
				+= r.connection
				+= r.content_type
				+= std::string("\n")
				+= r.body;
		}
		std::string status;
		std::string date;
		std::string server = "Server: wcpp\n";
		std::string etag;
		std::string accept_ranges;
		std::string content_length;
		std::string connection;
		std::string content_type;
		std::string body;
		std::string last_modified;
		std::string test_response = std::string("HTTP/1.1 200 OK\n"
			"DATE: Sun, 20 Sep 2020 15 : 23 : 12 GMT\n"
			"Server: cppTemp\n"
			"Last-Modified : Sun, 20 Sep 2020\n"
			"ETag : 'guwno'\n"
			"Accept-Ranges : bytes\n"
			"Content-Length : 21\n"
			"Connection : close\n"
			"Content-Type : text/html\n"
			"\n"
			"<h1>My page </h1>");

		enum status_type {
			Continue =100,
			switching_protocol=101,
			processing=102,
			early_hints=103,
			ok = 200,
			created=201,
			accepted=202,
			non_authoritative_information=203,
			no_content=204,
			reset_content=205,

			bad_request=400,
			unauthorized=401,
			payment_required=402,
			forbidden=403,
			not_found=404,
			method_not_allowed=405,
			not_acceptable=406,
			request_timeout=408,


			internal_server_error=500
			
		};



	};


}