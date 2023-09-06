#pragma once

#include <cstdio>
#include <cctype>
#include <stdexcept>

class BigInt128 {
private:
    unsigned long long low;
    unsigned long long high;

public:
    BigInt128();
    BigInt128(unsigned long long value);
    BigInt128(unsigned long long high, unsigned long long low);

    BigInt128 operator+(const BigInt128& other) const;
    BigInt128 operator-(const BigInt128& other) const;
    BigInt128 operator*(const BigInt128& other) const;
    BigInt128 operator/(const BigInt128& other) const;
    BigInt128 operator%(const BigInt128& other) const;

    bool operator>=(const BigInt128& other) const;
    bool operator<=(const BigInt128& other) const;
    bool operator>(const BigInt128& other) const;
    bool operator<(const BigInt128& other) const;


    bool operator==(const BigInt128& other) const;
    bool operator!=(const BigInt128& other) const { return !(*this == other); }

    // Convert to string for display
    char* to_string(char* buffer, unsigned int size) const;
    BigInt128 parse(const char* str) const;
};

// BigInt128 implementation

BigInt128::BigInt128() : low(0), high(0) {}

BigInt128::BigInt128(unsigned long long value) : low(value), high(0) {}

BigInt128::BigInt128(unsigned long long high, unsigned long long low) : low(low), high(high) {}

BigInt128 BigInt128::operator+(const BigInt128& other) const {
    BigInt128 result;
    unsigned long long carry = 0;

    result.low = low + other.low;
    if (result.low < low) {
        carry = 1;
    }

    result.high = high + other.high + carry;

    return result;
}

BigInt128 BigInt128::operator-(const BigInt128& other) const {
    BigInt128 result;
    unsigned long long borrow = 0;

    result.low = low - other.low;
    if (result.low > low) {
        borrow = 1;
    }

    result.high = high - other.high - borrow;

    return result;
}

BigInt128 BigInt128::operator*(const BigInt128& other) const {
    BigInt128 result;

    unsigned long long a = high;
    unsigned long long b = low;
    unsigned long long c = other.high;
    unsigned long long d = other.low;

    // Basic multiplication algorithm (ignoring carry)
    unsigned long long ac = a * c;
    unsigned long long ad = a * d;
    unsigned long long bc = b * c;
    unsigned long long bd = b * d;

    result.high = ac + (ad >> 64) + (bc >> 64);
    result.low = (ad << 64) + (bd & 0xFFFFFFFFFFFFFFFFULL);

    return result;
}

BigInt128 BigInt128::operator/(const BigInt128& other) const {
    if (other == BigInt128(0)) {
        // Handle division by zero
        // Return a special value or throw an exception
        throw std::invalid_argument("Division by zero");
    }

    BigInt128 quotient;
    BigInt128 remainder = *this;

    while (remainder >= other) {
        quotient = quotient + BigInt128(1);
        remainder = remainder - other;
    }

    return quotient;
}

BigInt128 BigInt128::operator%(const BigInt128& other) const {
    if (other == BigInt128(0)) {
        // Handle division by zero
        // Return a special value or throw an exception
    }

    BigInt128 remainder = *this;

    while (remainder >= other) {
        remainder = remainder - other;
    }

    return remainder;
}

bool BigInt128::operator==(const BigInt128& other) const {
    return (high == other.high) && (low == other.low);
}

bool BigInt128::operator<(const BigInt128& other) const {
    if (high < other.high) {
        return true;
    } else if (high > other.high) {
        return false;
    } else {
        return low < other.low;
    }
}

bool BigInt128::operator>(const BigInt128& other) const {
    if (high > other.high) {
        return true;
    } else if (high < other.high) {
        return false;
    } else {
        return low > other.low;
    }
}

bool BigInt128::operator<=(const BigInt128& other) const {
    return !(*this > other);
}

bool BigInt128::operator>=(const BigInt128& other) const {
    return !(*this < other);
}


char* BigInt128::to_string(char* buffer, unsigned int size) const {
    const unsigned int buffer_size = 33; // 32 hex digits + null terminator
    if (size < buffer_size) {
        return nullptr; // Insufficient buffer size
    }

    char temp[buffer_size];
    unsigned long long temp_high = high;
    unsigned long long temp_low = low;

    for (int i = buffer_size - 2; i >= 0; --i) {
        temp[i] = "0123456789ABCDEF"[temp_low & 0xF];
        temp_low >>= 4;
    }
    temp[buffer_size - 1] = '\0';

    snprintf(buffer, size, "%016llx%016s", temp_high, temp);

    return buffer;
}


BigInt128 BigInt128::parse(const char* str) const {
        if (!str) {
            throw std::invalid_argument("Input string is null");
        }

        // Skip leading whitespace
        while (*str && std::isspace(*str)) {
            ++str;
        }

        // Handle hexadecimal "0x" prefix if present
        if (*str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X')) {
            str += 2;
        }

        // Parse hexadecimal digits
        unsigned long long high = 0;
        unsigned long long low = 0;
        int digitCount = 0;

        while (*str) {
            char c = *str;
            if (std::isxdigit(c)) {
                unsigned int digit = std::isdigit(c) ? (c - '0') : (std::toupper(c) - 'A' + 10);
                if (digitCount < 16) {
                    low = (low << 4) | digit;
                } else if (digitCount < 32) {
                    high = (high << 4) | digit;
                } else {
                    break; // Ignore additional digits
                }
                ++digitCount;
            } else {
                break; // Stop parsing at non-hexadecimal character
            }
            ++str;
        }

        return BigInt128(high, low);
    }
