#include "bybitapi.h"
#include <openssl/hmac.h>

string apiSecretKey(BYBIT_API_SECRET);

#define PATH_ACTIVE_ORDER _T("/v2/private/order/create")
#define PATH_SET_LEVERAGE _T("/v2/private/position/leverage/save")


typedef std::pair<string_t, string_t> Param;

char binary_to_hex_digit(unsigned a) {
    return a + (a < 10 ? '0' : 'a' - 10);
}

string binary_to_hex(unsigned char const* binary, unsigned binary_len) {
    string r(binary_len * 2, '\0');
    for (unsigned i = 0; i < binary_len; ++i) {
        r[i * 2] = binary_to_hex_digit(binary[i] >> 4);
        r[i * 2 + 1] = binary_to_hex_digit(binary[i] & 15);
    }
    return r;
}

std::string hmacEncode(string& input) {
	unsigned char result[EVP_MAX_MD_SIZE];
	unsigned result_len = 0;
	HMAC(EVP_sha256(), apiSecretKey.data(), apiSecretKey.size(), reinterpret_cast<unsigned char const*>(input.data()), input.size(), result, &result_len);
    return binary_to_hex(result, result_len);    
}

string_t qsEncode(map<string_t, string_t>& data) {
    if (data.empty()) {
        return _T("");
    }
    // sort by alphabetically.
    std::vector<Param> params(data.begin(), data.end());
    std::sort(params.begin(), params.end(), [&](const Param& a, const Param & b) -> bool {
        return a.first < b.first;
    });

    string_t result = _T("");
    for (auto pair = params.begin(); pair != params.end(); pair++) {
        if (!(pair == params.begin())) {
            result += _T("&");
        }
        result += (pair->first + _T("=") + uri::encode_uri(pair->second));
    }
    return result;
}

namespace bybitAPI {
    json::value bybitAPI(string_t url, method method, map<string_t, string_t>& data) {
        http_client client(BYBIT_ENDPOINT);

        // add timestamp parameter to data map
        data[_T("timestamp")] = to_tstring(std::time(0) * 1000);
        data[_T("api_key")] = _T(BYBIT_API_KEY);

        auto queryString = qsEncode(data);

        std::string queryStringA;
#ifdef UNICODE
        // convert to ansi string for hmac
        queryStringA.assign(queryString.begin(), queryString.end());
#else
        queryStringA = queryString;
#endif
        auto encodedA = hmacEncode(queryStringA);
        string_t encoded;
#ifdef UNICODE
        encoded.assign(encodedA.begin(), encodedA.end());
#else
        encoded = encodedA;
#endif
        queryString += (_T("&sign=") + encoded);
        auto json = client.request(method, url + _T("?") + queryString).get().extract_json().get();
#if 0
        std::string serializedA;
        auto serialized = json.serialize();
        serializedA.assign(serialized.begin(), serialized.end());
        cout << serializedA << "\n";
#endif
        return json;
    }

    bool setLeverage(string_t symbol, string_t leverage, string_t buyLeverage, string_t sellLeverage) {
        map<string_t, string_t> data = {
            {_T("symbol"), symbol},
            {_T("leverage"), leverage },
            {_T("buy_leverage"), buyLeverage},
            {_T("sell_leverage"), sellLeverage}
        };
        try {
            auto response = bybitAPI(PATH_SET_LEVERAGE, methods::POST, data);
            int retCode = response[_T("ret_code")].as_integer();
            if (retCode == 0 || retCode == 34036) { // leverage not changed
                return true;
            }
            cout << "Set Leverage failed with ret code : " << retCode << "\n";
        }
        catch (std::exception const& ex) {
            cout << "Error while setting leverage : " << ex.what() << "\n";
        }
        return false;
    }

    bool openLimitOrder(string_t side, string_t symbol, string_t qty, string_t price) {
        map<string_t, string_t> data = {
            {_T("side"), side},
            {_T("symbol"), symbol},
            {_T("order_type"), _T("Limit")},
            {_T("qty"), qty},
            {_T("price"), price},
            {_T("close_on_trigger"), _T("false")},
            {_T("reduce_only"), _T("false")},
            {_T("time_in_force"), _T("GoodTillCancel")}
        };

        try {
            auto response = bybitAPI(PATH_ACTIVE_ORDER, methods::POST, data);
            int retCode = response[_T("ret_code")].as_integer();
            if (retCode == 0) {
                return true;
            }
            cout << "Create order failed with ret code : " << retCode << "\n";
        }
        catch (std::exception const& ex) {
            cout << "Error occurred while creating the order : " << ex.what() << "\n";
        }
        return false;
    }

    bool openMarketOrder(string_t side, string_t symbol, string_t qty) {
        map<string_t, string_t> data = {
            {_T("side"), side},
            {_T("symbol"), symbol},
            {_T("order_type"), _T("Market")},
            {_T("qty"), qty},
            {_T("close_on_trigger"), _T("false")},
            {_T("reduce_only"), _T("false")},
            {_T("time_in_force"), _T("GoodTillCancel")},
        };

        try {
            auto response = bybitAPI(PATH_ACTIVE_ORDER, methods::POST, data);
            int retCode = response[_T("ret_code")].as_integer();
            if (retCode == 0) {
                return true;
            }
            cout << "Create order failed with ret code : " << retCode << "\n";
        }
        catch (std::exception const& ex) {
            cout << "Error occurred while creating the order : " << ex.what() << "\n";
        }
        return false;
    }
}