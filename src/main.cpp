#include <map>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include <windows.h>

#include "mutils/utils.h"
#include "mutils/bigint.h"


void print_intro();
void cls();
void print_console_colors(); // For Windows

int main()
{
  print_intro();
  std::cout << "Press enter to continue";
  mutils::prompt_input();

  enum Operations {
    SOE,
    DIOPHANTINE,
    PARTITIONS,
    PARTITIONS_BELL,
    GCD,
    LCM,
    DIVISORS,
    PRIME_FACTORS,
    FACTORIAL,
  };

  std::map<int, std::string> opsName;
  std::map<int, bool> opsToggle;

  opsName[Operations::SOE] = "Sieve of Eratosthenes";
  opsName[Operations::DIOPHANTINE] = "Linear Diophantine Equation ax + by = g";
  opsName[Operations::PARTITIONS] = "Partitions";
  opsName[Operations::PARTITIONS_BELL] = "Partitions (Bell Numbers)";
  opsName[Operations::GCD] = "GCD/GCF (Euclidian)";
  opsName[Operations::LCM] = "LCM";
  opsName[Operations::DIVISORS] = "Divisors";
  opsName[Operations::PRIME_FACTORS] = "Prime Factors";
  opsName[Operations::FACTORIAL] = "Factorial n!";

  // Initialize all operations to false
  for (auto ops : opsName) { opsToggle[ops.first] = false; }

  bool hasOp = false;
  do
  {
    // Operation options
    while (true) {
      cls();

      std::cout << std::endl;
      for (auto pair : opsToggle) {
        std::printf("[%2d] %s : %d\n", pair.first, opsName[pair.first].c_str(), pair.second);
      }

      if (!hasOp) {
        std::cout << "\nPlease select at least one operation." << std::endl;
      } else {
        std::cout << std::endl;
      }

      std::vector<int> inputs = mutils::prompt_array_int_input("Enter the id(s) separated by spaces to toggle: ");
      for (auto input : inputs) {
        if (opsToggle.find(input) != opsToggle.end()) {
          bool toggle = opsToggle[input];
          opsToggle[input] = !toggle;
        }
      }

      cls();
      std::cout << std::endl;
      for (auto pair : opsToggle) {
        if (pair.second) {
          std::printf("[%2d] %s : %d\n", pair.first, opsName[pair.first].c_str(), pair.second);
        } else {
          std::cout << std::endl;
        }
      }
      std::cout << std::endl;

      if (mutils::prompt_confirm("Are you ok with these settings? (y/N): ")) {
        for (auto pair : opsToggle) {
          if (pair.second) { hasOp = true; break; }
        }
        if (hasOp) { break; }
        else { hasOp = false; }
      }
    }

    cls();
    for (auto pair : opsToggle) {
      if (!pair.second) { continue; }

      std::set<int> primes;
      std::vector<int> integers;
      int first, second, third;
      int gcd;
      int lcm;
      int parts;
      BigInt partsBell;
      BigInt factorial;

      std::printf("\n[%2d] %s\n\n", pair.first, opsName[pair.first].c_str());

      switch (pair.first) {

        case Operations::SOE:

          first = mutils::prompt_int_input("Enter integer value of n: ");
          std::cout << std::endl;
          mutils::sieve_of_eratosthenes(first, primes, true);
          std::cout << "\n\nAll primes:" << std::endl;
          mutils::print_set_by_column(primes, 10);
          std::printf("\nTotal number of primes between (1, %d): %llu\n", first, primes.size());
          break;

        case Operations::DIOPHANTINE:

          first = mutils::prompt_int_input("Enter integer value of a: ");
          second = mutils::prompt_int_input("Enter integer value of b: ");
          third = mutils::prompt_int_input("Enter integer value of g: ");
          std::cout << std::endl;
          mutils::linear_diophantine(first, second, third);
          break;

        case Operations::PARTITIONS:

          first = mutils::prompt_int_input("Enter integer value of n: ");
          std::cout << std::endl;
          parts = mutils::partitions(first);
          std::printf("\np(%d) = %d\n", first, parts);
          break;

        case Operations::PARTITIONS_BELL:

          first = mutils::prompt_int_input("Enter integer value of n: ");
          std::cout << std::endl;
          for (int i = 1; i <= first; ++i) {
            partsBell = mutils::partitions_bell(i);
            std::printf("p(%2d) = %s\n", i, partsBell.to_string().c_str());
          }
          break;

        case Operations::GCD:

          first = mutils::prompt_int_input("Enter first integer: ");
          second = mutils::prompt_int_input("Enter second integer: ");
          std::cout << std::endl;
          gcd = mutils::gcd(first, second, true);
          std::printf("\nGCD(%d, %d) = %d\n", first, second, gcd);
          break;

        case Operations::LCM:

          first = mutils::prompt_int_input("Enter first integer: ");
          second = mutils::prompt_int_input("Enter second integer: ");
          std::cout << std::endl;
          lcm = mutils::lcm(first, second, true);
          std::printf("\nLCM(%d, %d) = %d\n", first, second, lcm);
          break;

        case Operations::DIVISORS:

          first = mutils::prompt_int_input("Enter integer value of n: ");
          std::cout << std::endl;
          mutils::divisors(first, integers);
          mutils::print_vector_by_column(integers, 9);
          std::cout << "\nThe sum of all divisors is: "
                    << mutils::add_vector_values(integers) << std::endl;
          break;

        case Operations::PRIME_FACTORS:

          first = mutils::prompt_int_input("Enter integer value of n: ");
          mutils::prime_factors(first, integers);
          mutils::print_vector_by_column(integers, 9);
          std::cout << std::endl;
          break;

        case Operations::FACTORIAL:

          first = mutils::prompt_int_input("Enter integer value of n: ");
          factorial = mutils::factorial(first);
          std::cout << "Factorial of " << first << ": " << factorial << std::endl;
          break;
      }
    }

    // Reset options
    for (auto pair : opsToggle) {
      opsToggle[pair.first] = false;
    }
    hasOp = false;

    std::cout << "DONE!" << std::endl;
  } while (mutils::prompt_restart());

  std::cout << "\nBye." << std::endl;
  sleep(1);
  return 0;
}


////////////////////////// Function Declarations ///////////////////////////////


void print_intro()
{
  std::cout << "Collection of Positive Integer Algorithm Solutions\n"
            << "by Mark Lucernas <https://github.com/marklcrns>\n" << std::endl;
}


// Clear screen, works for Windows and Unix
// Ref: https://docs.microsoft.com/en-us/windows/console/clearing-the-screen
void cls()
{
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  SMALL_RECT scrollRect;
  COORD scrollTarget;
  CHAR_INFO fill;

  // Get the number of character cells in the current buffer.
  if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
    return;
  }

  // Scroll the rectangle of the entire buffer.
  scrollRect.Left = 0;
  scrollRect.Top = 0;
  scrollRect.Right = csbi.dwSize.X;
  scrollRect.Bottom = csbi.dwSize.Y;

  // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
  scrollTarget.X = 0;
  scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

  // Fill with empty spaces with the buffer's default text attribute.
  fill.Char.UnicodeChar = TEXT(' ');
  fill.Attributes = csbi.wAttributes;

  // Do the scroll
  ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

  // Move the cursor to the top left corner too.
  csbi.dwCursorPosition.X = 0;
  csbi.dwCursorPosition.Y = 0;

  SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}


void print_console_colors()
{
  HANDLE handle;
  handle = GetStdHandle(STD_OUTPUT_HANDLE);

  for (short unsigned int k = 0; k <= 255; ++k) {
    if (k % 10 == 1) { std::cout << std::endl; }
    SetConsoleTextAttribute(handle, k);
    std::cout << std::setw(10) << k;
  }
  // Reset color to white
  SetConsoleTextAttribute(handle, 15);
}

