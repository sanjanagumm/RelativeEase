#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <string>
#include <thread>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

constexpr int PORT = 8080;

void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    res.version(req.version());
    res.keep_alive(req.keep_alive());
    res.set(http::field::content_type, "application/json");
    std::string json_data = "{\"name\": \"John\", \"age\": 30, \"isStudent\": true}";
    res.body() = json_data;
    res.prepare_payload();
}

void do_session(tcp::socket& socket) {
    // This buffer is required to read data from the socket
    beast::flat_buffer buffer;
    http::request<http::string_body> req;
    http::read(socket, buffer, req);
    http::response<http::string_body> res;
    handle_request(req, res);

    http::write(socket, res);
}

int main() {

    net::io_context ioc{1};

 
    tcp::acceptor acceptor{ioc, {tcp::v4(), PORT}};

    while (true) {

        tcp::socket socket{ioc};

        acceptor.accept(socket);
        std::thread{[&socket] { do_session(socket); }}.detach();
    }

    return 0;
}