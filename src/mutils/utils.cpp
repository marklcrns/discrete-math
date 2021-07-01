#include "utils.h"

#include <windows.h>

void mutils::sieve_of_eratosthenes(int n, std::set<int>& primes, bool verbose)
{
  // Colors
  HANDLE hStdout;
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

  for (int i = 1; i <= n; ++i) {

    if (i == 1) {
      if (verbose) {
        SetConsoleTextAttribute(hStdout, 0);
        std::cout << std::setw(10) << i;
      }
      continue;
    }

    // Sieve by multiple of primes already found
    bool newPrime = true;
    for (const auto& prime : primes) {
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
      if (newPrime) { // Colored
        SetConsoleTextAttribute(hStdout, 10);
      } else { // Gray color
        SetConsoleTextAttribute(hStdout, 8);
      }
      std::cout << std::setw(10) << i;
    }
  }
  // Reset color to white
  if (verbose) { SetConsoleTextAttribute(hStdout, 15); }
}


// Extended Euclidean algorithm
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


// https://www.techiedelight.com/find-general-solution-linear-diophantine-equation/
void mutils::linear_diophantine(int a, int b, int g)
{
  int a1, b1, g1;
  int x, y, x1, y1;

  std::printf("\nEquation: %dx + %dy = %d\n\n", a, b, g);

  int d = gcd(a, b, false);
  std::printf("GCD(%d, %d) = %d\n\n", a, b, d);

  if (d % g != 0) {
    std::cout << "The given equation has INFINITE SOLUTIONS\n" << std::endl;
  } else {
    std::cout << "The given equation has NO SOLUTION\n" << std::endl;
    return;
  }

  a1 = a / d;
  b1 = b / d;
  g1 = g / d;

  std::printf("Reduced Diophantine Equation: %dx + %dy = %d\n\n", a1, b1, g1);

  std::tie(x1, y1) = extended_gcd(a1, b1);
  x = (g / d) * x1;
  y = (g / d) * y1;

  std::cout << "General solution:" << std::endl;
  std::printf("\tx = %d + %dk\n", x, b / d);
  std::printf("\ty = %d - %dk   for any integer k\n\n", y, a / d);
}


// https://www.geeksforgeeks.org/generate-unique-partitions-of-an-integer/
auto mutils::partitions(int n) -> int
{
  int p[(size_t)n];
  int k = 0;
  p[k] = n;

  int parts = 0;
  // This loop first prints current partition then generates next
  // partition. The loop stops when the current partition has all 1s
  while (true)
  {
    print_array(p, k+1);
    ++parts;

    // Find the rightmost non-one value in p[]. Also, update the
    // rem_val to know how much value can be accommodated
    int rem_val = 0;
    while (k >= 0 && p[k] == 1) {
        rem_val += p[k];
        k--;
    }

    if (k < 0) break;

    // Decrease the p[k] found above and adjust the rem_val
    p[k]--;
    rem_val++;

    // If rem_val is more, then the sorted order is violated. Divide
    // rem_val in different values of size p[k] and copy these values at
    // different positions after p[k]
    while (rem_val > p[k]) {
      p[k+1] = p[k];
      rem_val = rem_val - p[k];
      k++;
    }

    // Copy rem_val to next position and increment position
    p[k+1] = rem_val;
    k++;
  }

  return parts;
}


// https://www.geeksforgeeks.org/bell-numbers-number-of-ways-to-partition-a-set/
auto mutils::partitions_bell(int n) -> BigInt
{
  BigInt bell[n + 1][n + 1];
  bell[0][0] = 1;
  for (int i = 1; i <= n; i++) {
    bell[i][0] = bell[i - 1][i-1];

    for (int j = 1; j <= i; j++) {
      bell[i][j] = bell[i - 1][j - 1] + bell[i][j - 1];
    }
  }
  return bell[n][0];
}


// GCD or GCF using Euclidian Algorithm
auto mutils::gcd(int a, int b, bool verbose) -> int
{
  int big;
  int small;
  if (a > b) {
    big = a; small = b;
  } else {
    small = a; big = b;
  }

  int remainder = big % small;
  if (verbose) {
    std::printf("%10d = %d(%d) + %d\n", big, small, big / small, remainder);
  }

  if (remainder == 0) { return small; }
  return gcd(small, remainder, verbose);
}


void mutils::divisors(int n, std::vector<int>& divisors)
{
  for (int i = 1; i <= n; ++i) {
    if (n % i == 0) { divisors.push_back(i); }
  }
}


void mutils::prime_factors(int n, std::vector<int>& primeFactors)
{
  std::set<int> primes;

  sieve_of_eratosthenes(n, primes, false);

  int temp = n;
  for (const auto& prime : primes) {
    if (temp % prime == 0) {
      while (temp % prime == 0) {
        temp /= prime;
        primeFactors.push_back(prime);
      }
    }
  }
}


auto mutils::is_string_ints(const std::string& str) -> bool
{
  return
    (!str.empty() && str.find_first_not_of("0123456789 ") == std::string::npos) ||
    (str.size() > 1 && str[0] == '-' && str.find_first_not_of("0123456789", 1) == std::string::npos);
}


auto mutils::prompt_input() -> std::string
{
  std::string str;
  std::getline(std::cin, str);
  return str;
}


bool mutils::prompt_confirm(const std::string& message)
{
  std::string confirm;
  do {
    std::cout << message;
    confirm = prompt_input();
  } while(confirm.empty() || confirm.find_first_not_of("yYnN ") != std::string::npos);

  return confirm == "y" || confirm == "Y";
}


auto mutils::prompt_int_input(const std::string& message) -> int
{
  int input;
  std::cout << message;
  while (!(std::cin >> input)) {
    std::cin.clear();
    while (std::cin.get() != '\n') continue;
    std::cout << "\nInvalid integer value! " << std::endl;
    std::cout << message;
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return input;
}


auto mutils::prompt_array_int_input(const std::string& message) -> std::vector<int>
{
  std::string rawInput;
  std::vector<int> input;

  std::cout << message;
  while(std::getline(std::cin, rawInput)) {
    if (rawInput.empty()) { break; }

    if (!mutils::is_string_ints(rawInput)) {
      std::cout << "Input contains non-integer!" << std::endl;
      std::cout << message;
      continue;
    }

    int tmp;
    std::stringstream ss(rawInput);
    while(ss >> tmp) { input.push_back(tmp); }

    break;
  }

  return input;
}


auto mutils::prompt_restart() -> bool
{
  char res;
  std::cout << "\nGo again? [Y/n]: ";
  while (!(std::cin >> res)
         || !(res == 'Y' || res == 'y' || res == 'N' || res == 'n'))
  {
    std::cout << "Go again? [Y/n]: ";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return res == 'Y' || res == 'y';
}

