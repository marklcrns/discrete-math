#include "utils.h"
#include <windows.h>

void mutils::sieve_of_eratosthenes(int n, std::set<int>& primes, bool verbose)
{
  // Colors
  HANDLE hStdout;
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

  for (int i = 1; i <= n; ++i)
  {

    if (i == 1) {
      if (verbose)
      {
        SetConsoleTextAttribute(hStdout, 0);
        std::cout << std::setw(10) << i;
      }
      continue;
    }

    // Sieve by multiple of primes already found
    bool newPrime = true;
    for (const auto& prime : primes)
    {
      // Assuming the set is sorted, only check if within the range of num
      if (i < prime) { break; }

      if (i % prime == 0)
      {
        newPrime = false;
        break;
      }
    }

    if (newPrime) { primes.insert(i); }

    if (verbose)
    {
      // New line after 10 numbers in a row
      if (i % 10 == 0) { std::cout << std::endl; }

      if (newPrime) { SetConsoleTextAttribute(hStdout, 10); } // Colored
      else { SetConsoleTextAttribute(hStdout, 8); } // No color

      std::cout << std::setw(10) << i;
    }
  }

  if (verbose) { SetConsoleTextAttribute(hStdout, 15); }
}


void mutils::prime_factors(int n, std::vector<int>& primeFactors)
{
  std::set<int> primes;

  sieve_of_eratosthenes(n, primes, false);

  int temp = n;
  for (const auto& prime : primes)
  {
    if (temp % prime == 0)
    {
      while (temp % prime == 0)
      {
        temp /= prime;
        primeFactors.push_back(prime);
      }
    }
  }
}


void mutils::divisors(int n, std::vector<int>& divisors)
{
  for (int i = 1; i <= n; ++i)
  {
    if (n % i == 0) { divisors.push_back(i); }
  }
}


// GCD or GCF in Euclidian Algorithm
auto mutils::gcd(int a, int b, bool verbose) -> int
{
  int big;
  int small;
  if (a > b)
  {
    big = a;
    small = b;
  }
  else
  {
    small = a;
    big = b;
  }

  int remainder = big % small;
  if (verbose) {
    std::cout << std::setw(10) << big << " = " << small << "(" << big / small << ")" << " + " << remainder << std::endl;
  }

  if (remainder == 0) { return small; }
  return gcd(small, remainder, verbose);
}


// Function for the extended Euclidean algorithm
// It returns multiple values using tuple in C++
auto mutils::extended_gcd(int a, int b) -> std::tuple<int, int>
{
  if (a == 0) {
    return std::make_tuple(0, 1);
  }

  int x, y;

  // unpack tuple returned by function into variables
  std::tie(x, y) = extended_gcd(b % a, a);

  return std::make_tuple((y - (b/a) * x), x);
}


void mutils::diophantine(int a, int b, int g)
{
  int a1, b1, g1;
  int x, y, x1, y1;

  std::fprintf(stdout, "\nDiophantine Equation: %dx + %dy = %d\n\n", a, b, g);

  int d = gcd(a, b, false);
  std::fprintf(stdout, "GCD(%d, %d) = %d\n\n", a, b, d);

  if (d % g != 0) {
    std::cout << "The given equation has infinite solutions\n" << std::endl;
  }
  else {
    std::cout << "The given equation has no solution\n" << std::endl;
    return;
  }

  a1 = a / d;
  b1 = b / d;
  g1 = g / d;

  std::fprintf(stdout, "Reduced Diophantine Equation: %dx + %dy = %d\n\n", a1, b1, g1);

  std::tie(x1, y1) = extended_gcd(a1, b1);
  x = (g / d) * x1;
  y = (g / d) * y1;

  std::cout << "General solution:" << std::endl;
  std::fprintf(stdout, "\tx = %d + %dk\n", x, b / d);
  std::fprintf(stdout, "\ty = %d - %dk   for any integer k\n\n", y, a / d);
}


auto mutils::is_string_ints(const std::string& str) -> bool
{
  return
    (!str.empty() && str.find_first_not_of("0123456789 ") == std::string::npos) ||
    (str.size() > 1 && str[0] == '-' && str.find_first_not_of("0123456789", 1) == std::string::npos);
}
