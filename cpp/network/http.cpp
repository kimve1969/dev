#include<iostream>
#include<boost/beast/core.hpp>
#include<boost/beast/http.hpp>
#include<boost/beast/version.hpp>
#include<boost/asio/connect.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<cstdlib>
#include<string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main(int argc, char *argv[])
{
  try{
    if(argc !=4 && argc != 5)
    {
      std::cerr<<"http www.example.com 80 / 1.0\n";
      return EXIT_FAILURE;
    }

    auto const host = argv[1];
    auto const port = argv[2];
    auto const target = argv[3];
    int version = argc==5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

    // The io_context is requred for all I/O
    net::io_context ioc;

    // These objects perfom our I/O
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);

    // Look up domane name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from lookup
    stream.connect(results);

    // Set up an HTTP GET request message
    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Reciver the HTTP response
    http::read(stream, buffer, res);

    // Write the message to standart output
    std::cout<< res << std::endl;

    // Gracefulle close the socket
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes
    // so don't bother reportin it
    if(ec && ec != beast::errc::not_connected)
      throw beast::system_error{ec};
    
    // if we get here then the connection is closed gracefully
  }
  catch(std::exception const &e){
    std::cerr<<"ERROR: "<<e.what()<<std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

