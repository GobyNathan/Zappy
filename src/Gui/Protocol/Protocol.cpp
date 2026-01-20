/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Jun, 2025                                                     *
 * Title           - mirrorZappy                                                        *
 * Description     -                                                                    *
 *     Protocol                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "Protocol.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Protocol::Protocol(const std::string& host, int port)
    : _sockfd(-1), _host(host), _port(port), _running(false)
{
    if (connectToServer()) {
        sendCommand("GRAPHIC");
        _running = true;
        _recvThread = std::thread(&Protocol::recvLoop, this);
    } else {
        std::cerr << "Failed to connect to server " << host << ":" << port << std::endl;
    }
}

Protocol::~Protocol()
{
    stop();
    closeConnection();
}

void Protocol::start()
{
    if (!_running && _sockfd != -1) {
        _running = true;
        _recvThread = std::thread(&Protocol::recvLoop, this);
    }
}

void Protocol::stop()
{
    _running = false;
    if (_recvThread.joinable())
        _recvThread.join();
}

bool Protocol::connectToServer()
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
        return false;

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);

    if (inet_pton(AF_INET, _host.c_str(), &serv_addr.sin_addr) <= 0)
        return false;

    if (connect(_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        return false;

    return true;
}

void Protocol::closeConnection()
{
    if (_sockfd != -1) {
        close(_sockfd);
        _sockfd = -1;
    }
}

void Protocol::sendCommand(const std::string& cmd)
{
    if (_sockfd != -1) {
        std::string toSend = cmd + "\n";
        send(_sockfd, toSend.c_str(), toSend.size(), 0);
    }
}

bool Protocol::pollResponse(std::string& response)
{
    std::lock_guard<std::mutex> lock(_queueMutex);
    if (_responses.empty())
        return false;
    response = _responses.front();
    _responses.pop();
    return true;
}

void Protocol::recvLoop()
{
    char buffer[1024];
    while (_running) {
        ssize_t n = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            buffer[n] = '\0';
            std::string data(buffer);
            // Découpe en lignes si plusieurs réponses d'un coup
            size_t pos = 0;
            while ((pos = data.find('\n')) != std::string::npos) {
                std::string line = data.substr(0, pos);
                {
                    std::lock_guard<std::mutex> lock(_queueMutex);
                    std::cout << "DEBUG: " << line << std::endl;
                    _responses.push(line);
                }
                data.erase(0, pos + 1);
            }
        } else if (n == 0) {
            // Déconnexion serveur
            _running = false;
        } else {
            // Erreur
            _running = false;
        }
    }
}