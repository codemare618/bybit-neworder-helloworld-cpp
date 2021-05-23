#pragma once

#include <tchar.h>
#include "config.h"
#include <cpprest/http_client.h>
#include <iostream>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;

namespace bybitAPI {
	json::value bybitAPI(string_t url, method method, map<string_t, string_t>& data);
	bool setLeverage(string_t symbol, string_t leverage, string_t buyLeverage, string_t sellLeverage);
	bool openLimitOrder(string_t side, string_t symbol, string_t qty, string_t price);
	bool openMarketOrder(string_t side, string_t symbol, string_t qty);
}