#include "InputHandler.h"
#include <iostream>

int InputHandler::getUserInput() {
    int choice;
    std::cout << "Choose method for calculating grades:" << std::endl;
    std::cout << "1. Standard Deviation" << std::endl;
    std::cin >> choice;
    return choice;
}