// bybitcpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "bybitapi.h"

int main()
{
    std::cout << "Hello World!\n";

    std::cout << "Setting Leverage to 10x for BTCUSD Perpetual...\n";
    bool result = bybitAPI::setLeverage(_T("BTCUSD"), _T("0"), _T("10"), _T("10"));  // Leverage 0 means cross margin mode.
    if (result) {
        result = bybitAPI::openLimitOrder(_T("Buy"), _T("BTCUSD"), _T("10"), _T("10000"));
    }
    if (result) {
        std::cout << "Successfully created order\n";
    }

    cout << "Press any key to exit...\n";
    std::getchar();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
