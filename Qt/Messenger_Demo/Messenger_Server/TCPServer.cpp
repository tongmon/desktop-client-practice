#include "TCPServer.hpp"
#include "NetworkDefinition.hpp"

// void Service::OnRequestReceived(const boost::system::error_code &ec, std::size_t bytes_transferred)
// {
//     if (ec != boost::system::errc::success)
//     {
//         // std::cout << "Error occured! Error code = "
//         //           << ec.value()
//         //           << ". Message: " << ec.message();
//
//         OnFinish();
//         return;
//     }
//
//     // Process the request.
//     m_response = ProcessRequest(m_request);
//
//     // Initiate asynchronous write operation.
//     boost::asio::async_write(*m_sock.get(),
//                              boost::asio::buffer(m_response),
//                              [this](const boost::system::error_code &ec, std::size_t bytes_transferred) {
//                                  OnResponseSent(ec, bytes_transferred);
//                              });
// }
//
// void Service::OnResponseSent(const boost::system::error_code &ec, std::size_t bytes_transferred)
// {
//     if (ec != boost::system::errc::success)
//     {
//         // 보냄 처리 안된 경우 로직
//     }
//
//     OnFinish();
// }
//
// void Service::OnFinish()
// {
//     delete this;
// }
//
// // 클라이언트에서 전달된 request를 활용하는 실제 로직은 여기에 위치한다.
// std::string Service::ProcessRequest(boost::asio::streambuf &request)
// {
//     std::string req((std::istreambuf_iterator<char>(&request)), std::istreambuf_iterator<char>());
//     std::string response;
//
//     // 실제 로직
//     // std::cout << req << "\n";
//     // std::string response = "datadatahavehaveshowshowtaketake";
//
//     return response;
// }
//
// Service::Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
//     : m_sock(sock)
// {
// }
//
// // 변경 필요
// void Service::StartHandling()
// {
//     boost::asio::async_read_until(*m_sock.get(),
//                                   m_request,
//                                   '\n',
//                                   [this](
//                                       const boost::system::error_code &ec, std::size_t bytes_transferred) {
//                                       OnRequestReceived(ec, bytes_transferred);
//                                   });
// }
//
// void Acceptor::InitAccept()
// {
//     std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
//
//     m_acceptor.async_accept(*sock.get(),
//                             [this, sock](const boost::system::error_code &error) {
//                                 OnAccept(error, sock);
//                             });
// }
//
// void Acceptor::OnAccept(const boost::system::error_code &ec,
//                         std::shared_ptr<boost::asio::ip::tcp::socket> sock)
// {
//     if (ec == boost::system::errc::success)
//         (new Service(sock))->StartHandling();
//     else
//     {
//         // Accept 실패시 동작 정의
//     }
//
//     // 서버가 멈추지 않았다면 InitAccept() 계속 수행
//     m_is_stopped.load() ? m_acceptor.close() : InitAccept();
// }
//
// Acceptor::Acceptor(boost::asio::io_service &ios, unsigned short port_num)
//     : m_ios(ios),
//       m_acceptor(m_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num)),
//       m_is_stopped(false)
// {
// }
//
// void Acceptor::Start()
// {
//     m_acceptor.listen();
//     InitAccept();
// }
//
// void Acceptor::Stop()
// {
//     m_is_stopped.store(true);
// }
//
// TCPServer::TCPServer()
// {
//     m_work.reset(new boost::asio::io_service::work(m_ios));
// }
//
// void TCPServer::Start(unsigned short port_num, unsigned int thread_pool_size)
// {
//     assert(thread_pool_size > 0);
//
//     // 수신기 작동 시작
//     acc.reset(new Acceptor(m_ios, port_num));
//     acc->Start();
//
//     // 쓰레드 풀 생성
//     for (int i = 0; i < thread_pool_size; i++)
//     {
//         std::unique_ptr<std::thread> th(
//             new std::thread([this]() {
//                 m_ios.run();
//             }));
//
//         m_thread_pool.push_back(std::move(th));
//     }
// }
//
// void TCPServer::Stop()
// {
//     acc->Stop();
//     m_ios.stop();
//
//     for (auto &th : m_thread_pool)
//         th->join();
// }

// template <typename U>

// void TCPServer<Service>::InitAcceptor(unsigned short port_num)
// {
//     // 소켓 연결 설정
//     m_acceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(m_ios,
//                                                                   boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(),
//                                                                                                  port_num));
//     m_is_stopped.store(false);
//     m_acceptor->listen();
//
//     std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
//     m_acceptor->async_accept(*sock.get(),
//                              [this, sock, port_num](const boost::system::error_code &error) {
//                                  if (error == boost::system::errc::success)
//                                      (new Service(sock))->StartHandling();
//                                  else
//                                  {
//                                      // Accept 실패시 동작 정의
//                                  }
//
//                                  // 서버가 멈추지 않았다면 InitAccept() 계속 수행
//                                  m_is_stopped.load() ? m_acceptor->close() : InitAcceptor(port_num);
//                              });
// }

// TCPServer<Service>::TCPServer(unsigned short port_num, unsigned int thread_pool_size)
// {
//     m_acceptor.reset(nullptr);
//     m_work.reset(new boost::asio::io_service::work(m_ios));
//
//     // acceptor 연결 설정
//     InitAcceptor(port_num);
//
//     // 쓰레드 풀 생성
//     for (int i = 0; i < thread_pool_size; i++)
//     {
//         std::unique_ptr<std::thread> th(
//             new std::thread([this]() {
//                 m_ios.run();
//             }));
//         m_thread_pool.push_back(std::move(th));
//     }
// }

// TCPServer<Service>::~TCPServer()
// {
// }
