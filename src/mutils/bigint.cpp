#include <sstream>
#include <vector>

#include "bigint.h"

/**
 *
 * Pre-increment/decrement operators
 *
 */

auto BigInt::operator++() -> BigInt& {
  *this += 1;
  return *this;
}


auto BigInt::operator--() -> BigInt& {
  *this -= 1;
  return *this;
}


/**
 *
 * Post-increment/decrement operators
 *
 */

auto BigInt::operator++(int) -> BigInt {
  BigInt tmp = *this;
  *this += 1;
  return tmp;
}


auto BigInt::operator--(int) -> BigInt {
  BigInt tmp = *this;
  *this -= 1;
  return tmp;
}


/**
 *
 * Addition Operator: BigInt + BigInt
 *
 * Mimics addition by hand.
 *
 */
BigInt &BigInt::operator+=(const BigInt &bint) {
  if (bint == 0)  { return *this; }
  if (*this == 0) { *this = bint; return *this; }

  if (abs(bint) > abs(*this) && _positive != bint._positive) {
    _positive = !_positive;
    *this = bint - *this;
    return *this;
  }

  if (_positive == bint._positive) {
    add_reversed_vector_digits(_digits, bint._digits);
  } else {
    subtract_reverse_vector_digits(_digits, bint._digits);
  }

  _has_changes = true;
  update_value();
  return *this;
}


/**
 *
 * Subtraction Operator: BigInt - BigInt
 *
 * Mimics subtraction by hand.
 *
 */
BigInt &BigInt::operator-=(const BigInt &bint) {
  if (bint == 0)  { return *this; }
  if (*this == 0) { *this = bint; _positive = !_positive; return *this; }

  if (abs(bint) > abs(*this)) {
    _positive = !_positive;
    *this = bint + *this;
    _positive = !_positive;
    return *this;
  }

  if (_positive == bint._positive) {
    subtract_reverse_vector_digits(_digits, bint._digits);
  } else {
    add_reversed_vector_digits(_digits, bint._digits);
  }

  _has_changes = true;
  update_value();
  return *this;
}


/**
 *
 * Multiplication Operator: BigInt * BigInt
 *
 * Mimics multiplication by hand. Uses the concept of dynamic programming.
 *
 */
BigInt &BigInt::operator*=(const BigInt &bint) {
  if (_value == "0")      { return *this; }
  if (bint._value == "0") { *this = bint; return *this; }

  _positive = bint._positive == _positive;

  if (bint._value == "1") { return *this; }
  if (_value == "1") {
    *this = bint;
    if (_positive == bint._positive) { _positive = true; }
    return *this;
  }

  // Uses lhs as the multiplier, which therefore the slow ptr.
  auto lhsPtr = _digits.begin(); auto rhsPtr = bint._digits.begin();
  std::vector<int> buffer{};

  // Init buffer for addition of products
  for (size_t i = 0; i < _digits.size() + bint._digits.size(); ++i) {
    buffer.push_back(0);
  }

  auto bufPtrFast = buffer.begin(); auto bufPtrSlow = buffer.begin();
  int addOffset = 1; int prod = 1;
  int prodCarry = 0; int sumCarry = 0;

  // Multiply
  while (lhsPtr != _digits.end()) {
    if (rhsPtr != bint._digits.end()) {
      prod = *lhsPtr;
      prod *= *rhsPtr;
      ++rhsPtr;
    } else {
      // Append remaining carries to be included in next round
      if (bufPtrFast == buffer.end()) {
        buffer.push_back(prodCarry + sumCarry);
      } else {
        *bufPtrFast = prodCarry + sumCarry;
      }
      *lhsPtr = *bufPtrSlow;
      // Reset rhs
      rhsPtr = bint._digits.begin();
      // Reset buffer ptr with offset, just like adding the products by hand
      bufPtrFast = buffer.begin() + addOffset;

      ++lhsPtr; ++bufPtrSlow; ++addOffset;
      prodCarry = 0; sumCarry = 0;
      continue;
    }
    prod += prodCarry;

    // Dynamic addition subprocess
    if (bufPtrFast != buffer.end()) {
      int tmp = *bufPtrFast + (prod % BASE) + sumCarry;
      *bufPtrFast = tmp % BASE;
      sumCarry = tmp / BASE;
      ++bufPtrFast;
    }

    prodCarry = prod / BASE;
  }

  remove_lead_zeros(buffer);

  // Store the rest in the buffer to lhs
  while (bufPtrSlow != buffer.end()) {
    _digits.push_back(*bufPtrSlow);
    ++bufPtrSlow;
  }

  _has_changes = true;
  update_value();
  return *this;
}

/**
 *
 * Exponentiation Operator: BigInt ^ BigInt
 *
 */
BigInt &BigInt::operator^=(const BigInt &bint) {
  if (!bint._positive) { *this = BigInt(0); return *this; }  // Improve to consider rounding off
  if (bint._value == "0") { *this = BigInt(1); return *this; }
  if (bint._value == "1" || _value == "1" || _value == "0") { return *this; }

  BigInt lhsCopy{*this};
  std::vector<int> rhsCopy(bint._digits);
  auto rhsPtr = rhsCopy.begin();

  bool offsetOne = true;
  while (fill_nth_place(rhsPtr, rhsCopy.end())) {
    if (offsetOne) { --(*rhsPtr); offsetOne = false; }
    for (int i = 0; i < *rhsPtr; ++i) {
      *this *= lhsCopy;
    }
    *rhsPtr = 0;
  }

  _has_changes = true;
  update_value();
  return *this;
}


// /*
//  * TODO: This algorithm is still slower than brute force multipication if the
//  * division operator is slow. Unless division operator is optimized, this
//  * algorithm is useless.
//  *
//  * Uses Exponentiation by Squaring or Binary Exponentiation method.
//  */
// BigInt BigInt::fast_pow(BigInt base, BigInt pow) {
//   BigInt res = 1;
//   while (pow > 0) {
//     if (pow % 2 == 0) {
//       pow /= 2;
//       base *= base;
//     } else {
//       --pow;
//       res *= base;
//       pow /= 2;
//       base *= base;
//     }
//   }
//   return res;
// }
//
// /**
//  *
//  * Exponentiation Operator: BigInt - BigInt
//  *
//  */
// BigInt &BigInt::operator^=(const BigInt &bint) {
//   *this = fast_pow(*this, bint);
//   return *this;
// }


/**
 *
 * Division Operator: BigInt / BigInt
 *
 * Division by subtraction. Rounds up quotient if remainder is atleast half of
 * the divisor.
 *
 */
// TODO: Extremely slow even with digits less than 10. Look for better solution.
BigInt &BigInt::operator/=(const BigInt &bint) {
  if (bint._value == "0") {
    *this = bint;
    _value = "#DIV/0";
    _errors |= ERROR_DIV_ZERO;
    return *this;
  }
  if (_value == "0") { return *this; }

  _positive = _positive == bint._positive;

  if (bint._value == "1") { return *this; }

  BigInt lhsCopy = abs(*this);
  BigInt rhsCopy = abs(bint);

  BigInt quotient(0);
  while (lhsCopy > 0) {
    lhsCopy -= rhsCopy;
    if (lhsCopy._positive) {
      ++quotient;
    }
  }

  if (!lhsCopy._positive) {
    if ((lhsCopy + rhsCopy) * BigInt(2) >= rhsCopy) {
      ++quotient;
    }
  }

  if (!_positive) { quotient *= -1; }

  *this = quotient;
  return *this;
}

/**
 *
 * Modulo Operator: BigInt % BigInt
 *
 * Utilizes division by subtraction.
 *
 */
// TODO: Extremely slow even with digits less than 10. Look for better solution.
BigInt &BigInt::operator%=(const BigInt &bint) {
  if (!_positive || !bint._positive) {
    *this = BigInt(0);
    _value = "#DOMAIN";
    _errors |= ERROR_DOMAIN;
    return *this;
  }
  if (bint._value == "0") {
    *this = bint;
    _value = "#DIV/0";
    _errors |= ERROR_DIV_ZERO;
    return *this;
  }
  if (_value == "0")      { return *this; }
  if (bint._value == "1") {
    *this = BigInt(0);
    return *this;
  }
  if (*this < bint) {
    return *this;
  }


  _positive = _positive == bint._positive;

  BigInt lhsCopy = abs(*this);

  while (lhsCopy >= bint) {
    lhsCopy -= bint;
  }

  if (!_positive) {lhsCopy *= -1; }
  *this = lhsCopy;

  return *this;
}


/*
 *
 * Helper functions
 *
 */

void BigInt::update_value() {
  if (_digits.back() == 0 && !_positive) {
    _positive = true;
  }

  if (_has_changes) {
    auto it = _digits.rbegin();
    std::stringstream ss;

    while (it != _digits.rend()) {
      ss << *it;
      ++it;
    }

    _value.replace(0, _value.length(), ss.str());
    _has_changes = false;
  }
}


void BigInt::remove_lead_zeros(std::vector<int> &vec) {
  while (vec.size() > 1 && vec[vec.size() - 1] == 0) {
    vec.pop_back();
  }
}


/**
 * WARNING: This functions assumes all values are positive and single digits.
 */
void BigInt::add_reversed_vector_digits(std::vector<int>& lhs, const std::vector<int>& rhs) {
  auto lhsPtr = lhs.begin();
  auto rhsPtr = rhs.begin();

  int sum = 0;
  while (lhsPtr != lhs.end() || rhsPtr != rhs.end()) {
    if (lhsPtr != lhs.end()) {
      sum += *lhsPtr;
    } else {  // Extend vector if rhs length > lhs length
      lhs.push_back(0);
      lhsPtr = lhs.end() - 1;
    }
    if (rhsPtr != rhs.end()) {
      sum += *rhsPtr;
      ++rhsPtr;
    }
    *lhsPtr = sum % BASE;   // Bring down
    sum /= BASE;            // Carry
    ++lhsPtr;
  }
  if (sum != 0) { _digits.push_back(sum); }

  remove_lead_zeros(this->_digits);
}



/**
 * WARNING: This functions assumes all values are positive and single digits.
 * Additionally, lhs should be greater than rhs.
 */
void BigInt::subtract_reverse_vector_digits(std::vector<int>& lhs, const std::vector<int>& rhs) {
  auto lhsPtr = lhs.begin();
  auto rhsPtr = rhs.begin();

  int diff = 0;
  while (lhsPtr != lhs.end() || rhsPtr != rhs.end()) {
    if (lhsPtr != lhs.end()) {
      diff = *lhsPtr;
    }
    if (rhsPtr != rhs.end()) {
      diff -= *rhsPtr;
      ++rhsPtr;
    }
    *lhsPtr = diff % BASE;   // Bring down
    if (*lhsPtr < 0) {
      take_away_one(lhsPtr, lhs.end());
    }
    ++lhsPtr;
  }

  remove_lead_zeros(this->_digits);
}


/**
 * Recursively look for the immediate non-zero nth place to take away one to be
 * carried down into nth place with appropriate base conversion.
 *
 *      from     end
 *        v       v
 * e.g. { 0, 0, 3 } -> { 0, 10, 2 } -> { 10, 9, 2 } -> return true
 *
 *      from     end
 *        v       v
 * e.g. { 9, 9, 9 } -> return true;
 *
 *      from     end
 *        v       v
 * e.g. { 0, 0, 0 } -> return false;
 *
 *         from  end
 *           v    v
 * e.g. { 8, 0, 9 } -> { 8, 10, 8 } -> return true
 */
bool BigInt::fill_nth_place(std::vector<int>::iterator nth, const std::vector<int>::iterator& end) {
  if (nth != end && *nth > 0) { return true; }

  while (nth != end) {
    if (fill_nth_place(++nth, end)) {
      --(*nth);
      --nth;
      *nth = BASE;
      return true;
    };
  }
  return false;
}

/**
 * Recursively look for the immediate non-zero nth place to take away one over
 * into `from` element converted into appropriate place value.
 *
 *      from     end
 *        v       v
 * e.g. { 1, 0, 3 } -> { 1, 10, 2 } -> { 11, 9, 2 } -> return true
 *
 *      from     end
 *        v       v
 * e.g. { 9, 9, 9 } -> { 19, 8, 9 } return true;
 *
 *      from     end
 *        v       v
 * e.g. { 0, 0, 0 } -> return false;
 *
 *         from  end
 *           v    v
 * e.g. { 1, 9, 3 } -> { 1, 19, 2 } -> return true
 */
bool BigInt::take_away_one(std::vector<int>::iterator from, const std::vector<int>::iterator& end) {
  if (fill_nth_place(++from, end)) {
    --(*from);
    --from;
    *from += BASE;
    return true;
  }
  return false;
}


auto BigInt::truncate_string(const std::string& str, size_t width, bool show_ellipsis) -> std::string {
  if (width > 0 && str.length() > width) {
    if (show_ellipsis) {
      return str.substr(0, width) + "...";
    }
    return str.substr(0, width);
  }
  return str;
}


auto BigInt::truncate_string(const std::string& str, size_t width, bool show_ellipsis) const -> std::string {
  if (width > 0 && str.length() > width) {
    if (show_ellipsis) {
      return str.substr(0, width) + "...";
    }
    return str.substr(0, width);
  }
  return str;
}

