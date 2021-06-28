#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <set>
#include <iomanip>
#include <unistd.h>

namespace mutils {
  void sieve_of_eratosthenes(int n, std::set<int>& primes, bool verbose);
  void divisors(int n, std::vector<int>& divisors);
  auto gcd(int m, int n, bool verbose) -> int;
  auto extended_gcd(int m, int n) -> std::tuple<int, int>;
  void prime_factors(int n, std::vector<int>& primeFactors);
  void diophantine(int a, int b, int c);

  bool is_string_ints(const std::string& str);
}

template<typename T>
void print_set_by_column(const std::set<T>& set, int width)
{
  int i = 0;
  for (const auto& val : set)
  {
    if (i % 10 == 0) { std::cout << std::endl; }
    std::cout << std::setw(width) << val;
    ++i;
  }
  std::cout << std::endl;
}

template<typename T>
void print_vector_by_column(const std::vector<T>& vec, int width)
{
  int i = 0;
  for (const auto& val : vec)
  {
    if (i % 10 == 0) { std::cout << std::endl; }
    std::cout << std::setw(width) << val;
    ++i;
  }
  std::cout << std::endl;
}

template<typename T>
T add_vector_values(const std::vector<T>& vec)
{
  T sum = 0;
  for (const auto& val : vec)
  {
    sum += val;
  }
  return sum;
}

template<typename T>
T mult_vector_values(const std::vector<T>& vec)
{
  T prod = 0;
  for (const auto& val : vec) { prod *= val; }
  return prod;
}

template<typename T>
T add_set_values(const std::set<T>& set)
{
  T sum = 0;
  for (const auto& val : set) { sum += val; }
  return sum;
}

template<typename T>
T mult_set_values(const std::set<T>& set)
{
  T prod = 0;
  for (const auto& val : set) { prod += val; }
  return prod;
}
