#include <iostream>
#include "MenuController.h"

int main() {
    try{
    MenuController sistem67;
    sistem67.ejecutar();
} catch (const std::exception& e) {
    std::cerr << "[ERROR FATAL EN MAIN]: " << e.what() << std::endl;
    return 1;
}
    return 0;
}