#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctype.h>
#include <unistd.h>
#include <windows.h>

#include "mutils/utils.h"

void print_intro();
auto prompt_input() -> std::string;
bool prompt_confirm(const std::string& message);
auto prompt_int_input(const std::string& message) -> int;
auto prompt_array_int_input(const std::string& message) -> std::vector<int>;
auto prompt_restart() -> bool;

void print_console_colors(); // For Windows
void cls();


int main()
{
  print_intro();
  std::cout << "Press enter to continue";
  prompt_input();

  enum Operations {
    SOE,
    DIOPHANTINE,
    PARTITIONS,
    GCD,
    DIVISORS,
    PRIME_FACTORS,
  };

  std::map<int, std::string> opsName;
  std::map<int, bool> opsToggle;

  opsName[Operations::SOE] = "Sieve of Eratosthenes";
  opsName[Operations::DIOPHANTINE] = "Linear Diophantine Equation ax + by = g";
  opsName[Operations::PARTITIONS] = "Partitions p(n)";
  opsName[Operations::GCD] = "GCD/GCF (Euclidian)";
  opsName[Operations::DIVISORS] = "Divisors";
  opsName[Operations::PRIME_FACTORS] = "Prime Factors";

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
        std::cout << "[" << std::setw(2) << pair.first << "] "
                  << opsName[pair.first] << ": " << pair.second << std::endl;
      }

      if (!hasOp) {
        std::cout << "\nPlease select at least one operation." << std::endl;
      } else {
        std::cout << std::endl;
      }

      std::vector<int> inputs = prompt_array_int_input("Enter the id(s) separated by spaces to toggle: ");
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
          std::cout << "[" << std::setw(2) << pair.first << "] "
            << opsName[pair.first] << ": " << pair.second << std::endl;
        } else {
          std::cout << std::endl;
        }
      }
      std::cout << std::endl;

      if (prompt_confirm("Are you ok with these settings? (y/N): ")) {
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

      std::fprintf(stdout, "\n[%2d] %s\n\n", pair.first, opsName[pair.first].c_str());

      switch (pair.first) {

        case Operations::SOE:

          first = prompt_int_input("Enter an integer: ");
          std::cout << std::endl;
          mutils::sieve_of_eratosthenes(first, primes, true);
          std::cout << "\n\nAll primes:" << std::endl;
          print_set_by_column(primes, 10);
          std::cout << "\nTotal number of primes between 1 and "
                    << first << " are: " << primes.size() << std::endl;
          break;

        case Operations::DIOPHANTINE:

          first = prompt_int_input("Enter integer value of a: ");
          second = prompt_int_input("Enter integer value of b: ");
          third = prompt_int_input("Enter integer value of g: ");
          std::cout << std::endl;
          mutils::diophantine(first, second, third);
          break;

        case Operations::GCD:

          first = prompt_int_input("Enter first integer: ");
          second = prompt_int_input("Enter second integer: ");
          std::cout << std::endl;
          gcd = mutils::gcd(first, second, true);
          std::fprintf(stdout, "\nGCD(%d, %d) = %d\n", first, second, gcd);
          break;

        case Operations::DIVISORS:

          first = prompt_int_input("Enter first integer: ");
          std::cout << std::endl;
          mutils::divisors(first, integers);
          print_vector_by_column(integers, 9);
          std::cout << "\nThe sum of all divisors is: " << add_vector_values(integers) << std::endl;
          break;

        case Operations::PRIME_FACTORS:

          first = prompt_int_input("Enter an integer: ");
          mutils::prime_factors(first, integers);
          print_vector_by_column(integers, 9);
          std::cout << std::endl;
          break;

      }
    }

    // Reset options
    for (auto pair : opsToggle) {
      opsToggle[pair.first] = false;
    }
    hasOp = false;

    std::cout << "DONE!" << std::endl;
  } while (prompt_restart());

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


auto prompt_input() -> std::string
{
  std::string str;
  std::getline(std::cin, str);
  return str;
}


bool prompt_confirm(const std::string& message)
{
  std::string confirm;
  do {
    std::cout << message;
    confirm = prompt_input();
  } while(confirm.empty() || confirm.find_first_not_of("yYnN ") != std::string::npos);

  return confirm == "y" || confirm == "Y";
}


auto prompt_int_input(const std::string& message) -> int
{
  int input;
  std::cout << message;
  while (!(std::cin >> input))
  {
    std::cin.clear();
    while (std::cin.get() != '\n') continue;
    std::cout << "\nInvalid integer value! " << std::endl;
    std::cout << message;
  }

  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return input;
}


auto prompt_array_int_input(const std::string& message) -> std::vector<int>
{
  std::string rawInput;
  std::vector<int> input;

  std::cout << message;
  while(std::getline(std::cin, rawInput))
  {
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


auto prompt_restart() -> bool
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


void print_console_colors()
{
  HANDLE handle;
  handle = GetStdHandle(STD_OUTPUT_HANDLE);

  for (short unsigned int k = 0; k <= 255; ++k)
  {
    if (k % 10 == 1) { std::cout << std::endl; }
    SetConsoleTextAttribute(handle, k);
    std::cout << std::setw(10) << k;
  }
  // Reset color to white
  SetConsoleTextAttribute(handle, 15);
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
  if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
  {
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

