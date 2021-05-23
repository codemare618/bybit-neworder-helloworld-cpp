#pragma once

#ifdef UNICODE
#define to_tstring to_wstring
#else
#define to_tstring to_string
#endif

#define BYBIT_ENDPOINT _T("https://api-testnet.bybit.com")
#define BYBIT_API_KEY "0QsHH13J0V5vS1ag1N"
#define BYBIT_API_SECRET "tRTWG5uIGzBirmLEBUmVkrk2F8VukrPItvW0"