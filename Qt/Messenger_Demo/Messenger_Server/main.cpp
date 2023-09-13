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

#include "DBConnectionPool.hpp"
#include "MessengerService.hpp"
#include "NetworkDefinition.hpp"
#include "TCPServer.hpp"

int main(int argc, char *argv[])
{
    try
    {
        // DB 연결 정보 초기화
        DBConnectionPool::get(4, {"localhost", "3000", "messenger_db", "tongstar", "@Lsy12131213"});

        // TCP 서버 생성
        TCPServer<MessengerService> server(SERVER_PORT, 2);

        char a;
        std::cin >> a;
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "Error occured! Error code = "
                  << e.code() << ". Message: "
                  << e.what();
    }

    return 0;
}