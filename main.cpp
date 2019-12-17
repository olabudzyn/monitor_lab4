#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
#include "Functions.h"
#include "MyMonitor.h"

// maksymalna wielkosc bufora
#define MAX_BUFFER_SIZE 10

//bufor wiadomosci
MyMonitor queue(MAX_BUFFER_SIZE);

int numberMessages, processedMessages;

// konsument wiadomosci - czytajacy
void reader(int work)
{
    std::cout << "\nINFO: Rozpoczecie czytania wiadomosci....\n" << std::endl;
    while(work) {
        if(queue.isEmpty())
        {
            sleep(1);
            if(queue.isEmpty()){
                work = 0;
                std::cout << "\nINFO: Watek czytenika zakonczony, lista wiadomosci do odczytania jest pusta"<< std::endl;
                return;
            }
        }

        UserMessage message = queue.removeFromQueue();
        printMessage(message);
        queue.printBuffer();
        ++processedMessages;

        if(processedMessages < numberMessages)
            work = 1;
        else
        {
            work = 0;
            std::cout << "\nINFO: Watek czytenika zakonczony, lista wiadomosci do odczytania jest pusta"<< std::endl;
        }
    }
}

// producent wiadomosci - uzytkownik
void user(const std::string &userName, int amount, int wait)
{
    numberMessages+=amount;
    for (int i = 0; i < amount ; ++i)
    {
        usleep(wait);
        int priority = getPriority();
        std::string message = generateMessage(userName, priority);

        if(!checkMessage(message)) {
            --numberMessages;
            return;
        }
        else
            queue.insertMessage(message, priority);
    }
}


// producent wiadomosci - uzytkownik, zla ilosc liter
void userSpecial(const std::string & userName, int priority, const std::string & message)
{
    ++numberMessages;
    if(!checkMessage(message)) {
        --numberMessages;
        return;
    }
    else
        queue.insertMessage(message, priority);
}


void testOrderMessage()
{
    std::cout << "\nTEST KOLEJNOSC WIADOMOSCI" << std:: endl;

    std::thread user1(user, "Meghan", 3, 1000000);
    std::thread user2(user, "John", 3, 500000);
    std::thread user3(user, "Maya", 3, 0);

    sleep(5);
    std::thread readerThread(reader, 1);

    user1.join();
    user2.join();
    user3.join();
    readerThread.join();
}

void testOverflowQueue()
{
    std::cout << "\nTEST PRZEPELNIENIA KOLEJKI" << std:: endl;

    std::thread user1(user, "Meghan", 3, 1000000);
    std::thread user2(user, "John", 3, 500000);
    std::thread user3(user, "Alex", 3, 200000);
    std::thread user4(user, "Tom", 3, 100000);
    std::thread user5(user, "Hull", 3, 100000);

    sleep(5);
    std::thread readerThread(reader, 1);

    user1.join();
    user2.join();
    user3.join();
    user4.join();
    user5.join();
    readerThread.join();
}

void testEmptyQueue()
{
    std::cout << "\nTEST CZYTANIA Z PUSTEJ KOLEJKI" << std:: endl;

    std::thread readerThread(reader, 1);
    queue.printBuffer();

    readerThread.join();
}

void testSameTimeAddMessage()
{
    std::cout << "\nTEST DODAWANIA WIADOMOSCI W TYM SAMYM CZASIE PRZEZ UZYTKOWNIKOW" << std:: endl;

    std::vector<std::thread> threads;

    threads.reserve(4);
    threads.emplace_back(user, "Meghan", MAX_BUFFER_SIZE, 0);
    sleep(1);
    threads.emplace_back(user, "Meghan", 1, 0);
    threads.emplace_back(user, "John", 1, 0);
    usleep(10000);

    threads.emplace_back(reader, 1);

    for(auto& thread : threads){
        thread.join();
    }
}

void testBadMessageLength()
{
    std::cout << "\nTEST DODAWANIA WIADOMOSCI O ZLEJ ILOSCI ZNAKOW" << std:: endl;

    std::thread user1(user, "Meghan", 3, 1000000);
    std::thread user2(userSpecial, "Alex", NORMAL_MSG, "krotka");
    std::thread user2b(userSpecial, "Alex2", PRIORITY_MSG, "krotka");
    std::thread user3(userSpecial, "Tom", NORMAL_MSG, "baaaaaaaaaaaaaaaaaaaaaaardzooooo dluuuuugaaaaaaaaaaaaaaaaaaaa wiaaaaaaadooomoooosc od Toooomaaa");
    std::thread user3b(userSpecial, "Tom2", PRIORITY_MSG, "baaaaaaaaaaaaaaaaaaaaaaardzooooo dluuuuugaaaaaaaaaaaaaaaaaaaa wiaaaaaaadooomoooosc od Toooomaaa");
    std::thread user4(user, "John", 3, 500000);

    sleep(5);
    std::thread readerThread(reader, 1);

    user1.join();
    user2.join();
    user2b.join();
    user3.join();
    user3b.join();
    user4.join();
    readerThread.join();
}

int main() {

    srand(time(NULL));
    numberMessages = 0, processedMessages = 0;
    //testBadMessageLength();
    //testOrderMessage();
    testOverflowQueue();
    //testEmptyQueue();
    //testSameTimeAddMessage();
    return 0;
}