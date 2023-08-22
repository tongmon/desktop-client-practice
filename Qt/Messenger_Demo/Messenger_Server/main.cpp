#pragma region standard header
#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <coroutine>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#pragma endregion

#include <boost/system.hpp>

#include "TCPServer.hpp"

int main(int argc, char *argv[])
{
    unsigned short port_num = 3000;

    try
    {
        TCPServer server;

        unsigned int thread_pool_size = 1;

        server.Start(port_num, thread_pool_size);

        char a;
        std::cin >> a;

        server.Stop();
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "Error occured! Error code = "
                  << e.code() << ". Message: "
                  << e.what();
    }

    return 0;
}