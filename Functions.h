//
// Created by aleksandra on 10.12.2019.
//

#ifndef MONITOR_LAB4_FUNCTIONS_H
#define MONITOR_LAB4_FUNCTIONS_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <utility>

// priorytety wiadomosci
#define NORMAL_MSG 0
#define PRIORITY_MSG 1

//para przechowujaca wiadomosc oraz priorytet wiadomosci
typedef std:: pair <long, std::string> UserMessage;

// wielkosc wiadomosci
#define MIN_MEESAGE_SIZE 8
#define MAX_MESSAGE_SIZE 64


// funkcja pomocnicza zamieniajaca liczbe na priorytet
std::string getPriorityFromNumber(int number)
{
    if(number == PRIORITY_MSG)
    {
        return "PRIORITY MESSAGE";
    }
    else if (number == NORMAL_MSG)
    {
        return "NORMAL MESSAGE";
    }
}

// funkcja wyswietlajace wiadomosc na standardowe wyjscie
void printMessage(UserMessage message)
{
    std::cout << "Odczytywana wiadomosc: ** " << message.second << " ** " << std::endl;// " **, utworzono: "<< message.first << std::endl;
}


// funkcja pomocnicza sprawdzajaca czy dlugosc wiadomosci jest poprawna
bool checkMessage(std::string message)
{
    if(message.length() > MAX_MESSAGE_SIZE)
    {
        std:: cout << "Blad: wiadomosc jest zbyt dluga" << std::endl;
        return false;
    }
    else if (message.length() < MIN_MEESAGE_SIZE)
    {
        std:: cout << "Blad: wiadomosc jest zbyt krotka" << std::endl;
        return false;
    }

    return true;
}

int getPriority() {

    int number = rand() % 2;
    if(number == 0)
        return NORMAL_MSG;
    return PRIORITY_MSG;
}

std::string generateMessage(std::string userName, int priority) {

    std::string generatedMessage = "Uzytkownik: " + userName + ", priorytet: " + getPriorityFromNumber(priority);
    return generatedMessage;
}


#endif //MONITOR_LAB4_FUNCTIONS_H
