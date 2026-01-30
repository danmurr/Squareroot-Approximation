#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "functions.hpp"

int main(){
    float user_value;

    std::cout << "Enter the number you wish to squareroot: ";

    if (!(std::cin >> user_value)) {
        std::cout << "Invalid input." << std::endl;
        return 1;  
    }

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    float cmath_sqrt_val = std::sqrt(user_value);
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time);
    std::cout << "CMath Squareroot : " << cmath_sqrt_val << " in " << (elapsed_time.count() * power(10.0f, 9) )<< " nanoseconds.\n";
    start_time = std::chrono::steady_clock::now();
    float sqrt_user_value = my_sqrt(user_value);
    current_time = std::chrono::steady_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time);
    std::cout << "My Squareroot : " << sqrt_user_value << " in " << (elapsed_time.count() * power(10.0f, 9) )<< " nanoseconds.\n";

    return 0;
}