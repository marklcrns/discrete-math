#pragma once

#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <vector>

/**
 * BigInt class that stores arbitrary amout of integer that supports basic
 * arithmetic and comparison operators.
 *
 * Private vector _digits stores digits in reverse order for convenience as most
 * of the arithemic operators mimics by hand techniques when calculating two
 * arbitrary numbers.
 *
 * Exponent       (^) - Space O(n + m), Time O(nM)
 * Multiplication (*) - Space O(n + m), Time O(nm)
 * Division       (/) - Space O(2(n + m)), Time O(n^2)
 * Modulus        (%) - Space O(2(n + m)), Time O(n^2)
 * Addition       (+) - Space O(1), Time O(max(n,m))
 * Subtraction    (-) - Space O(1), Time O(max(n,m))
 *
 * where n and m are the respective number of digits of lhs and rhs, and M as
 * the arithmetic value of rhs.
 */
class BigInt {
  private:
    const unsigned char ERROR_DIV_ZERO = 1;
    const unsigned char ERROR_DOMAIN = 2;

    const int BASE = 10;
    std::string _value{"0"};
    std::vector<int> _digits{0};
    bool _positive = true;
    bool _has_changes = false;
    unsigned char _errors = 0;

    void update_value();
    void add_reversed_vector_digits(std::vector<int>& lhs, const std::vector<int>& rhs);
    void subtract_reverse_vector_digits(std::vector<int>& lhs, const std::vector<int>& rhs);
    bool fill_nth_place(std::vector<int>::iterator nth, const std::vector<int>::iterator& end);
    bool take_away_one(std::vector<int>::iterator from, const std::vector<int>::iterator& end);
    void remove_lead_zeros(std::vector<int> &vec);
    auto fast_pow(BigInt base, BigInt pow) -> BigInt;
    auto truncate_string(const std::string& str, size_t width, bool show_ellipsis = false) -> std::string;
    auto truncate_string(const std::string& str, size_t width, bool show_ellipsis = false) const -> std::string;

  public:
    // Constructors
    BigInt() = default;
    BigInt(const BigInt&) = default;
    BigInt(BigInt&&) = default;
    ~BigInt() = default;
    BigInt(const std::string& num) {
      if (num[0] == '-') {
        if (num.size() <= 1) { return; }
        if (!(num.size() == 2 && num[1] == '0')) {
          _positive = false;
        }
        _value.assign(num.begin() + 1, num.end());
      } else {
        _value = num;
      }
      // Remove leading whitespace
      _value.erase(0, std::min(_value.find_first_not_of('0'), _value.size()-1));

      _digits.clear();
      for (auto it = _value.crbegin(); it != _value.crend(); ++it) {
        _digits.push_back(*it - '0');
      }
    }

    BigInt(const int& num) {
      int num_copy = static_cast<int>(num);
      if (num < 0) { _positive = false; num_copy *= -1; }
      _value = std::to_string(num_copy);

      _digits.clear();
      while (num_copy != 0) {
        _digits.push_back(num_copy % BASE);
        num_copy /= BASE;
      }
    }

    BigInt(const long int& num) {
      int num_copy = static_cast<int>(num);
      if (num < 0) { _positive = false; num_copy *= -1; }
      _value = std::to_string(num_copy);

      _digits.clear();
      while (num_copy != 0) {
        _digits.push_back(num_copy % BASE);
        num_copy /= BASE;
      }
    }

    BigInt(const long long int& num) {
      int num_copy = static_cast<int>(num);
      if (num < 0) { _positive = false; num_copy *= -1; }
      _value = std::to_string(num_copy);

      _digits.clear();
      while (num_copy != 0) {
        _digits.push_back(num_copy % BASE);
        num_copy /= BASE;
      }
    }

    BigInt(int&& num) {
      if (num < 0) { _positive = false; num *= -1; }
      _value = std::to_string(num);

      _digits.clear();
      while (num != 0) {
        _digits.push_back(num % BASE);
        num /= BASE;
      }
    }

    BigInt(long int&& num) {
      if (num < 0) { _positive = false; num *= -1; }
      _value = std::to_string(num);

      _digits.clear();
      while (num != 0) {
        _digits.push_back(static_cast<int>(num % BASE));
        num /= BASE;
      }
    }

    BigInt(long long int&& num) {
      if (num < 0) { _positive = false; num *= -1; }
      _value = std::to_string(num);

      _digits.clear();
      while (num != 0) {
        _digits.push_back(static_cast<int>(num % BASE));
        num /= BASE;
      }
    }

    // Copy assignment
    BigInt& operator=(const BigInt& bint) {
      if (&bint != this) {
        _value = bint._value;
        _digits = bint._digits;
        _positive = bint._positive;
        _has_changes = bint._has_changes;
        _errors = bint._errors;
      }
      return *this;
    }

    // Move assignment
    BigInt& operator=(BigInt&& bint) noexcept {
      if (&bint != this) {
        _value = std::move(bint._value);
        _digits = std::move(bint._digits);
        _positive = bint._positive;
        _has_changes = bint._has_changes;
        _errors = bint._errors;
      }
      return *this;
    }

    // Arithmetic operators
    BigInt& operator+=(const BigInt& bint);
    BigInt& operator-=(const BigInt& bint);
    BigInt& operator*=(const BigInt& bint);
    BigInt& operator^=(const BigInt& bint);
    BigInt& operator/=(const BigInt& bint);
    BigInt& operator%=(const BigInt& bint);
    BigInt& operator++();
    BigInt& operator--();
    BigInt operator++(int);
    BigInt operator--(int);

    friend
    bool operator<(const BigInt& lhs, const BigInt& rhs);

    // Member functions
    auto abs() noexcept -> std::string { return _value; }
    auto to_string() noexcept -> std::string {
      if (_positive) { return _value; }
      return "-" + _value;
    }
    auto to_scientific(const size_t& width) noexcept -> std::string {
      size_t len = _value.length();
      if (width > 0 && len > width) {
        if (_positive) {
          return truncate_string(_value, width) +
            "e+" + std::to_string(len - width);
        }
        return "-" + truncate_string(_value, width) +
          "e+" + std::to_string(len - width);
      }
      return to_string();
    }

    // Const member functions
    auto abs() const noexcept -> std::string { return _value; }
    bool is_positive() const noexcept { return _positive; }
    bool is_valid() const noexcept { return _errors == 0; }
    auto to_string() const noexcept -> std::string {
      if (_positive) { return _value; }
      return "-" + _value;
    }
    auto to_scientific(const size_t& width) const noexcept -> std::string {
      size_t len = _value.length();
      if (width > 0 && len > width) {
        if (_positive) {
          return truncate_string(_value, width) +
            "e+" + std::to_string(len - width);
        }
        return "-" + truncate_string(_value, width) +
          "e+" + std::to_string(len - width);
      }
      return to_string();
    }

    // Static member functions
    static BigInt abs(BigInt bint) {
      bint._positive = true;
      return bint;
    }

}; // end of BigInt

// Non-member function

// String operations
inline
std::string operator+(std::string lhs, const BigInt& rhs) {
  lhs.append(rhs.to_string());
  return lhs;
}
inline
std::string operator+(const BigInt& lhs, const std::string& rhs) {
  return "" + lhs + rhs;
}
inline
std::ostream &operator<<(std::ostream &os, const BigInt& num) {
  os << num.to_string();
  return os;
}

// Relational operators
inline
bool operator==(const BigInt& lhs, const BigInt& rhs) {
  return lhs.to_string() == rhs.to_string();
}

inline
bool operator==(const BigInt& lhs, const std::string& rhs) {
  return lhs.to_string() == rhs;
}

inline
bool operator==(const BigInt& lhs, const int& rhs) {
  return lhs.to_string() == std::to_string(rhs);
}

inline
bool operator==(const BigInt& lhs, const long int& rhs) {
  return lhs.to_string() == std::to_string(rhs);
}

inline
bool operator==(const BigInt& lhs, const long long int& rhs) {
  return lhs.to_string() == std::to_string(rhs);
}

inline
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
  return !(lhs.to_string() == rhs.to_string());
}

inline
bool operator< (const BigInt& lhs, const BigInt& rhs) {
  if (lhs._positive && rhs._positive) {
    if (lhs._digits.size() < rhs._digits.size()) { return true; }
    if (lhs._digits.size() > rhs._digits.size()) { return false; }
  }
  if (!lhs._positive && !rhs._positive) {
    if (lhs._digits.size() < rhs._digits.size()) { return false; }
    if (lhs._digits.size() > rhs._digits.size()) { return true; }
  }
  if (lhs._positive && !rhs._positive) { return false; }
  if (!lhs._positive && rhs._positive) { return true; }

  auto lhs_it = lhs._digits.rbegin(); auto rhs_it = rhs._digits.rbegin();
  while (lhs_it != lhs._digits.rend() || rhs_it != rhs._digits.rend()) {
    if (*lhs_it == *rhs_it) {
      ++lhs_it; ++rhs_it; continue;
    }
    break;
  }
  if (lhs_it == lhs._digits.rend() || rhs_it == rhs._digits.rend()) {
    return false;
  }

  if (!lhs._positive && !rhs._positive) {
    return *lhs_it > *rhs_it;
  }
  return *lhs_it < *rhs_it;
}

inline
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
  return lhs < rhs || lhs == rhs;
}

inline
bool operator> (const BigInt& lhs, const BigInt& rhs) {
  return !(lhs < rhs) && lhs != rhs;
}

inline
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
  return lhs > rhs || lhs == rhs;
}

// Arithmetic operations

inline
BigInt operator+(BigInt lhs, const BigInt& rhs) {
  lhs += rhs;
  return lhs;
}

inline
BigInt operator-(BigInt lhs, const BigInt& rhs) {
  lhs -= rhs;
  return lhs;
}

inline
BigInt operator*(BigInt lhs, const BigInt& rhs) {
  lhs *= rhs;
  return lhs;
}

inline
BigInt operator^(BigInt lhs, const BigInt& rhs) {
  lhs ^= rhs;
  return lhs;
}

inline
BigInt operator/(BigInt lhs, const BigInt& rhs) {
  lhs /= rhs;
  return lhs;
}

inline
BigInt operator%(BigInt lhs, const BigInt& rhs) {
  lhs %= rhs;
  return lhs;
}

