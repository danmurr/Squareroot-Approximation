#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

float abs(float x){
    if (x < 0) return (-1.0 * x);
    return x;
}

int power(int base, int exp){
    int ans = 1;
    if (exp < 1) return -1;
    for (int i = 0; i < exp; i++){
        ans *= base; 
    }
    return ans;
}

float power(float base, int exp){
    float ans = 1.0f;
    if (exp < 0) return 1.0f / power(base, -exp);
    for (int i = 0; i < exp; i++){
        ans *= base;
    }
    return ans;
}

int get_digits(float n){
    int m{static_cast<int>(n)};
    int digits{0};
    while (m > 0){m/=10;digits++;}

    return digits;
}

void tweak(float n, float &sqrt_n_approx){
    // std::cout << "Approx before tweak : " << sqrt_n_approx << "\n";
    //trying to get sqrt_n_approx closer to real sqrt_n;
    bool positive_direction = (power(sqrt_n_approx, 2) < n);
    float step{power(10.0f, get_digits(sqrt_n_approx))};
    for (int i = 0; i < 13; i++){
        step /= 10.0f;
        int max_counter{0};
        while (positive_direction ? (power(sqrt_n_approx, 2) < n) : (power(sqrt_n_approx, 2) > n)){
            max_counter++;
            sqrt_n_approx += positive_direction ? step : (-1.0f * step);
            // std::cout << sqrt_n_approx << " compared to CMath " << std::sqrt(n) << "\n";
            if (max_counter == 10) break;
        }
        sqrt_n_approx += positive_direction ? (-1.0f * step) : step;
    }
}

int find_companion(float n){
    int high_sqrt = get_digits(n);
    if (high_sqrt % 2 == 1)
        high_sqrt += 1;
    int low_sqrt = high_sqrt - 2;

    int low_squared = power(10, low_sqrt);
    int high_squared = power(10, high_sqrt);

    low_sqrt = power(10, low_sqrt/2); high_sqrt = power(10, high_sqrt/2);

    int middle_sqrt{0};
    int middle_squared{0};
    while (high_sqrt - low_sqrt > 1) {
        middle_sqrt = (low_sqrt + high_sqrt)/2;
        middle_squared = power(middle_sqrt, 2);
        
        if (middle_squared < n){
            low_squared = middle_squared;
            low_sqrt = middle_sqrt;
        }
        else{
            high_squared = middle_squared;
            high_sqrt = middle_sqrt;
        }
    }
    if (abs(high_squared - n) < abs(low_squared - n)){
        return high_sqrt;
    }
    return low_sqrt;
}  

float taylor_coeff(int n, int sqrt_a){
    float num = (n % 2 == 1) ? 1.0f : -1.0f;
    int prod = 1;
    for (int k = 0; k < n - 2; k++){
        prod *= (2*k + 1);
    }
    num *= static_cast<float>(prod);
    float denom = static_cast<float>(power(2, n));
    denom *= static_cast<float>(power(sqrt_a, (2*n - 1)));

    return (num / denom);
} 

bool is_infinite(float x) {
    return (x == x) && ((x - x) != 0.0f);
}

float taylor_expansion(float x){
    int sqrt_a = find_companion(x);
    int a = power(sqrt_a,2);

    float sqrt_x{static_cast<float>(sqrt_a)};
    float last_approx{sqrt_x};
    float distance{999};
    for (int n = 1; n < 100; n++){
        last_approx = sqrt_x;
        float tc = taylor_coeff(n, sqrt_a);
        if (is_infinite(tc)){
            break;
        }
        sqrt_x += (tc * power((x - static_cast<float>(a)), n));

        //next distance < last distance
        if (abs(sqrt_x - last_approx) < distance){
            distance = abs(sqrt_x - last_approx);
        }
        else{
            return last_approx;
        }
    }
    return sqrt_x;
}


float sqrt(float x){
    float sqrt_x = taylor_expansion(x);
    tweak(x, sqrt_x);
    return sqrt_x;
}

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
    float sqrt_user_value = sqrt(user_value);
    current_time = std::chrono::steady_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time);
    std::cout << "My Squareroot : " << sqrt_user_value << " in " << (elapsed_time.count() * power(10.0f, 9) )<< " nanoseconds.\n";

    return 0;
}