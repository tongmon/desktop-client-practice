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

// stb는 header-only library이기에 매크로를 한번 정의하고 포함해줘야 한다.
// 매크로는 프로젝트에 단 한번만 쓰여야 됨
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "DBConnectionPool.hpp"
#include "MessengerService.hpp"
#include "NetworkDefinition.hpp"
#include "TCPServer.hpp"

int main(int argc, char *argv[])
{
    try
    {
        // DB 연결 정보 초기화
        DBConnectionPool::Get(4, {"localhost", "3000", "messenger_db", "tongstar", "@Lsy12131213"});

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