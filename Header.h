#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define let auto

#include "CoreUtility.h"
#include <iostream>
#include <boost/asio.hpp>
#include "CoreUdp.h"
#include "json.hpp"
#include "response.h"
#include "request_parser.h"
#include <cstdlib>
#include "tcp.h"
#include <functional>
#include "HTTP.h"
