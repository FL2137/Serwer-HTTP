#include "Header.h"
using namespace Web;

int main() {
	Web::TCP::tcp_server::async_server(std::string("192.168.1.103"), 66, [](std::string request_string,std::string& resp) {
		http_request request = request_parser::parse_request(request_string);
		

		//LOG(request.url);
		
		if (request.method == "GET"){

			if (request.url == "/") {

				let res = Web::response("index.html", "text/html");
				resp = response::to_string(res);
			}
			else if (request.url == "/index.html") {
				auto res = response("index.html", "text/html");
				resp = response::to_string(res);
			}
			else if (request.url == "/xxx.js") {

				auto res = response("xxx.js", "text/javascript");
				resp = response::to_string(res);
			}
			else if (request.url == "/bimbex.html") {
				auto res = response("bimbex.html", "text/html");
				resp = response::to_string(res);
			}
			else if (request.url == "/style.css") {
				auto res = response("style.css", "text/css");
				resp = response::to_string(res);
			}
			else if (request.url == "/obraz") {
				auto res = response("vibe.jpg", "image/jpeg");
				resp = response::to_string(res);
			}
			else if (request.url == "/favicon.ico") {
				auto res = response("favicon.ico", "image/x-icon");
				resp = response::to_string(res);
			}


			else if (request.url == "/kpopout.png") {
				auto res = response("kpopout.png", "image/png");
				resp = response::to_string(res);
			}
			
		}
		else if (request.method == "POST") {
			if (request.url == "/xml") {
				auto res = response("terx.xml", "text/xml");
				resp = response::to_string(res);
			}
			else if (request.url == "/jsontest") {
				resp = HTTP::send_raw(R"({"r":"json","c":"empty"})","application/json)");
			}
			
		}

	});


	return 0;
}