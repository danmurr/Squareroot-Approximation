#include <iostream>
#include <cmath>

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
    if (high_squared == n){
        return high_sqrt;
    }
    return low_sqrt;
}  

float taylor_coeff(int n, int sqrt_a){
    float num = (n % 2 == 1) ? 1.0 : -1.0;
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

float sqrt(float x){
    int sqrt_a = find_companion(x);
    int a = power(sqrt_a,2);

    float sqrt_x{static_cast<float>(sqrt_a)};
    for (int n = 1; n < 3; n++){
        float tc = taylor_coeff(n, sqrt_a);
        if (is_infinite(tc)){
            break;
        }
        sqrt_x += (tc * power((x - static_cast<float>(a)), n));
    }
    return sqrt_x;
}

int main(){
    float user_value;

    std::cout << "Enter the number you wish to squareroot: ";

    if (!(std::cin >> user_value)) {
        std::cout << "Invalid input." << std::endl;
        return 1;  
    }
    
    std::cout << "Approximate Squareroot : " << sqrt(user_value) << "\n"; 
    std::cout << "CMath Squareroot : " << std::sqrt(user_value) << "\n";
    return 0;
}