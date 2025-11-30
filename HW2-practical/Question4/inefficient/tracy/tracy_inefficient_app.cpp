#include <iostream>
#include <vector>
#include <cmath>

#include "tracy/public/tracy/Tracy.hpp" 

bool is_prime_naive(long n) {
    if (n <= 1) return false;
    for (long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

void find_primes_slowly(long limit) {
    ZoneScopedN("Inefficient Primes"); 
    for (long i = 0; i < limit; i++) {
        is_prime_naive(i);
    }
}

void process_data_inefficiently(int size) {
    ZoneScopedN("Inefficient Matrix");

    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    
    {
        ZoneScopedN("Matrix Population");
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                matrix[i][j] = i + j;
            }
        }
    }

    {
        ZoneScopedN("Matrix Processing (Columnar)");
        long long sum = 0;
        
        for(int j = 0; j < size; ++j) {
            for(int i = 0; i < size; ++i) {
                sum += matrix[i][j]; 
            }
        }
        std::cout << "Inefficient sum: " << sum << std::endl;
    }
}

int main() {
    ZoneScoped; 

    std::cout << "Starting inefficient tasks (with Tracy)..." << std::endl;
    
    find_primes_slowly(2000000); 
    process_data_inefficiently(35000); 

    std::cout << "Inefficient tasks finished." << std::endl;

    FrameMark; 
    return 0;
}