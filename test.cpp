#include <iostream>
#include <stdio.h>
#include <string>

void func(int x, int y = 0) {
    std::cout << "X = " << x << std::endl;
    std::cout << "y = " << y << std::endl;
}

int main() {
    int is_admin = 1;
    char operation = 'A';
    switch(operation) {
        if(is_admin) {
            case 'A' : std::cout << "item added";
        }
        case 'L' : std::cout << "Logged in successfuly"; 
        default : std::cout << "default";
    }
}

