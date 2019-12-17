//
// Created by aleksandra on 10.12.2019.
//
#ifndef MONITOR_LAB4_MYMONITOR_H
#define MONITOR_LAB4_MYMONITOR_H

#include <list>
#include <utility>
#include <iterator>
#include <string>
#include <chrono>
#include <iostream>
#include "monitor.h"
#include "Functions.h"


class MyMonitor : public Monitor
{
public:
    MyMonitor(int size) : Monitor(), SIZE_(size), count_(0)
    {
        normalHead_ = queue_.end();
        begin_ = std::chrono::steady_clock::now();
    }

    void insertMessage( std::string message, int priority)
    {
        std::cout << "\nUzytkownik rozpoczyna wysylanie wiadomosci: ** " << message <<  std:: endl;

        enter();

        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        std::chrono::microseconds time (std::chrono::duration_cast<std::chrono::microseconds>(end-begin_));

        message = message + " , dodano do kolejki: " + std::to_string(time.count()) + " us";

        if(count_ == SIZE_)
            wait(full_queue_);

        if(priority == PRIORITY_MSG)
            pushPriority(std::make_pair(time.count(), message));
        else if(priority == NORMAL_MSG)
            pushNormal(std::make_pair(time.count(), message));

        ++count_;

        std::cout << "Wiadomosc: ** " << message << " ** zostala dodana do kolejki!\n" << std:: endl;

        if(count_ == 1)
            signal(empty_queue_);

        leave();
    }

    UserMessage removeFromQueue()
    {
        UserMessage message;

        enter();

        if(count_ == 0)
            wait(empty_queue_);

        message = popBufferHead();
        --count_;

        if(count_ == SIZE_-1)
            signal(full_queue_);

        leave();
        return message;
    }

    bool isEmpty(){
        return queue_.empty();
    }


    // funkcja pomocnocza wyswietlajaca zawartosc bufora
    void printBuffer()
    {
        if(queue_.empty())
        {
            std:: cout << "\nBufor wiadomosci jest pusty." << std::endl;
            return;
        }
        std::list<UserMessage> :: iterator i;

        std::cout << "\n\n------------------------------------------" << std::endl;
        std::cout << "Bufor wiadomosci: " << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        for(i = queue_.begin() ; i != queue_.end() ; ++i)
        {
            UserMessage message = *i;
            std::cout << "Wiadomosc: ** " << message.second << " ** " << std:: endl;
        }

        std::cout << "\n\n";
    }


private:
    const int SIZE_;
    int count_;
    Condition empty_queue_, full_queue_;
    std::list<UserMessage> queue_;
    std::list<UserMessage> :: const_iterator normalHead_;
    std::chrono::time_point<std::chrono::steady_clock> begin_;

    void pushNormal(const UserMessage& userMessage)
    {
        queue_.push_back(userMessage);

        if(normalHead_ == queue_.end())
            normalHead_ = std::next(queue_.end(), -1);
    }

    void pushPriority(const UserMessage& userMessage)
    {
        queue_.insert(normalHead_, userMessage);
    }

    UserMessage popBufferHead()
    {
        UserMessage message = queue_.front();

        if(message == *normalHead_)
        {
            normalHead_ = std::next(normalHead_,1);
        }
        queue_.pop_front();
        return message;
    }

};

#endif //MONITOR_LAB4_MYMONITOR_H
