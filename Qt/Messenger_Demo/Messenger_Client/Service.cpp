#include "Service.hpp"
#include "NetworkDefinition.hpp"
#include "TCPClient.hpp"
#include "Utility.hpp"
#include "WinQuickWindow.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/json.hpp>
#include <chrono>
#include <ctime>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

Service::Service(WinQuickWindow &window, std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    : m_window{window}, m_sock{sock}
{
}

void Service::StartHandling()
{
    auto

        boost::asio::async_read(*m_sock,
                                m_server_request_buf.prepare(TCP_HEADER_SIZE),
                                [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {

                                });
}