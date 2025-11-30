#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

void find_primes_efficiently(long limit) {
    std::vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    
    for (long p = 2; p * p <= limit; p++) {
        if (is_prime[p]) {
            for (long i = p * p; i <= limit; i += p)
                is_prime[i] = false;
        }
    }
}

void process_data_efficiently(int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            matrix[i][j] = i + j;
        }
    }

    long long sum = 0;
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) { 
            sum += matrix[i][j];
        }
    }
    std::cout << "Efficient sum: " << sum << std::endl;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Starting efficient tasks..." << std::endl;
    
    find_primes_efficiently(2000000); 

    process_data_efficiently(35000); 

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Total time: " << diff.count() << " seconds" << std::endl;

    return 0;
}