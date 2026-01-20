/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Tue, Jun, 2025                                                     *
 * Title           - mirrorZappy                                                        *
 * Description     -                                                                    *
 *     Protocol                                                                         *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include <functional>

class Protocol
{
public:
    Protocol(const std::string& host, int port);
    ~Protocol();

    void sendCommand(const std::string& cmd);

    bool pollResponse(std::string& response);

    void start();

    void stop();

private:
    int _sockfd;
    std::string _host;
    int _port;

    std::thread _recvThread;
    std::atomic<bool> _running;

    std::mutex _queueMutex;
    std::queue<std::string> _responses;

    void recvLoop();
    bool connectToServer();
    void closeConnection();
};