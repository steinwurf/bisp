// Copyright (c) Steinwurf ApS 2018.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <bisp/spin_flipper.hpp>
#include <bisp/spin_mirror.hpp>

#include <iostream>
#include <thread>
#include <memory>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <iostream>

void wait(std::chrono::milliseconds ms)
{
    auto start = std::chrono::high_resolution_clock::now();

    while (std::chrono::high_resolution_clock::now() - start < ms)
    {}
}

int main(int argc, char* argv[])
{
    namespace bpo = boost::program_options;

    std::string remote_ip_string;
    uint16_t port;
    std::string interface_ip_string;

    // Parse the prorgram options
    bpo::options_description options("Bisp Example Options");

    options.add_options()
    ("port,p", bpo::value<uint16_t>(&port)->default_value(12820),
     "UDP Port to use.")
    ("remote_ip,r", bpo::value<std::string>(&remote_ip_string),
     "Remote IPv4 address, leave unset when running as flipper.")
    ("interface_ip,i", bpo::value<std::string>(&interface_ip_string),
     "Interface IPv4 address.")
    ("help,h", "Print this help message");

    bpo::variables_map opts;

    // Verify all required options are present
    try
    {
        bpo::store(bpo::parse_command_line(argc, argv, options), opts);

        if (opts.count("help"))
        {
            std::cout << options << std::endl;
            return 0;
        }

        bpo::notify(opts);
    }
    catch (const std::exception& e)
    {
        std::cout << "Error when parsing commandline options: " << e.what()
                  << std::endl;
        std::cout << options << std::endl;
        return 1;
    }

    boost::asio::io_service io;
    boost::asio::ip::udp::socket socket(io);

    boost::asio::ip::address_v4 interface_ip = interface_ip_string.empty() ?
        boost::asio::ip::address_v4::any() :
        boost::asio::ip::address_v4::from_string(interface_ip_string);

    boost::asio::ip::udp::endpoint local(interface_ip, port);

    std::shared_ptr<bisp::bit_spinner> bit_spinner;

    socket.open(local.protocol());
    socket.bind(local);

    boost::asio::ip::udp::endpoint remote;
    if (!remote_ip_string.empty())
    {
        std::cout << "Starting flipper" << std::endl;
        remote = boost::asio::ip::udp::endpoint(
            boost::asio::ip::address_v4::from_string(remote_ip_string), port);
        bit_spinner = std::make_shared<bisp::spin_flipper>();
        auto out = bit_spinner->outgoing();
        socket.send_to(boost::asio::buffer(&out, 1), remote, 0);
    }
    else
    {
        std::cout << "Starting mirror" << std::endl;
        bit_spinner = std::make_shared<bisp::spin_mirror>();
    }

    uint8_t receive_buffer = 0;

    std::function<void(const boost::system::error_code&,std::size_t)> callback;
    callback = std::bind(
        [&](const auto& error, auto bytes) {
            if (error)
            {
                std::cout << error.message() << std::endl;
                return;
            }
            assert(bytes == 1);
            bit_spinner->incoming(receive_buffer);
            auto out = bit_spinner->outgoing();
            auto rtt = bit_spinner->rtt();
            if (rtt)
                std::cout << remote << ": " <<  rtt.get().count() << " ms" << std::endl;

            wait(std::chrono::milliseconds(30));
            socket.send_to(boost::asio::buffer(&out, 1), remote, 0);
            socket.async_receive(boost::asio::buffer(&receive_buffer, 1), callback);
        },
        std::placeholders::_1,
        std::placeholders::_2);

    socket.async_receive_from(boost::asio::buffer(&receive_buffer, 1), remote, callback);
    std::thread io_thread([&io]{
        io.run();
    });

    std::cout << "Press Enter to Exit" << '\n';
    std::cin.get();
    socket.close();
    io.stop();
    io_thread.join();

    return 0;
}
