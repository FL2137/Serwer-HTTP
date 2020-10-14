#pragma once
#include <json.hpp>
#include <regex>
#include <sstream>
#include <fstream>
#include "response.h"
#include <ctime>
#define LOG(X) std::cout<<X<<std::endl
using nlohmann::json;
namespace Web {
	class http_request {
	public:
		http_request() {
			this->method = "";
			this->connection_type = "";
			this->http_ver = "";
			this->url = "";
		};
		std::string data;
		std::string method;
		std::string url;
		std::string http_ver;
		std::string connection_type;
		std::vector<std::string> methodVec{
			"GET","POST","HEAD","DELETE","OPTIONS","PATCH","PUT"
		};
	};


	class request_parser {

	public:
		static std::string get_time() {
			std::time_t rawtime;
			time(&rawtime);
			return  ctime(&rawtime);
		}
		static Web::http_request parse_request(std::string request) {

			auto _req = new Web::http_request();
			
			
			
			std::vector<std::string> lineVec = {};
			
			
			
			
			
			std::stringstream ss(request);
			std::string out;
			while (std::getline(ss, out, '\n')) {
				lineVec.push_back(out);
			}

			//method
			for (std::string c : _req->methodVec) {
				std::regex r(c);
				if (std::regex_search(lineVec[0], r))
					_req->method = c;
			}
			//url
			auto r = new std::regex(R"(\s.*?\s)");
			std::smatch url;
			std::regex_search(lineVec[0], url, *r);
			for (auto u : url) {
				_req->url += u;
			}
			_req->url.erase(std::remove(_req->url.begin(), _req->url.end(), ' '), _req->url.end());
			




			//data
			bool dataFlag = false;
			for (std::size_t i = 0; i < lineVec.size(); i++) {
				if (lineVec[i].size() == 1) {
					for (auto c : lineVec[i]) {
						if ((int)c == 13) {
							dataFlag = true;
						}
					}
				}
				if (dataFlag)
					_req->data += lineVec[i];
			}
			for (auto c : lineVec) {
				LOG(c);
			}

			return *_req;





				

		}







	};



};