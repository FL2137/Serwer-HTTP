#pragma once
#include <string>
#include <vector>
namespace Web {

	struct mime{
		std::vector<std::string> mime_types{
			"application/octet-stream",
			"application/x-bzip",
			"application/x-bzip2",
			"text/css",
			"text/html",
			"text/csv",
			"application/msword",
			"application/gzip",
			"image/gif",
			"image/bmp",
			"image/png",
			"image/jpeg",
			"application/javascript",
			"application/json",
			"audio/mpeg",
			"video/mpeg",
		};
	};
};