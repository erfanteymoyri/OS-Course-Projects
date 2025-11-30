#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

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
    for (long i = 0; i < limit; i++) {
        is_prime_naive(i);
    }
}

void process_data_inefficiently(int size) {
    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            matrix[i][j] = i + j;
        }
    }

    long long sum = 0;
    for(int j = 0; j < size; ++j) {
        for(int i = 0; i < size; ++i) {
            sum += matrix[i][j];
        }
    }
    std::cout << "Inefficient sum: " << sum << std::endl;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Starting inefficient tasks..." << std::endl;
    
    find_primes_slowly(2000000); 

    process_data_inefficiently(35000); 

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Total time: " << diff.count() << " seconds" << std::endl;

    return 0;
}