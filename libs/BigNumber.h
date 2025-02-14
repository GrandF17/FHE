#include <algorithm>
#include <atomic>   // to use std::enable_if and std::is_arithmetic
#include <cstddef>  // to use std::enable_if and std::is_arithmetic
#include <iostream>
#include <string>
#include <type_traits>  // to use std::enable_if and std::is_arithmetic

#ifndef BIGNUMBER_LIB
#define BIGNUMBER_LIB

using namespace std;

class BigNumber {
    // === =============== ===
    // === private section ===
   private:
    // =========
    // variables

    string num;
    const BigNumber zero() { return BigNumber("0"); }

    // =========
    // functions
    bool isNeg(BigNumber a) { return a.num.at(0) == '-'; }

    /**
     * check if string is correct number
     */
    bool isNumber(const string& str) const {
        if (str.empty()) return false;
        /**
         * if str[0] != "-" AND str[0] is NaN --> string is not a number
         * for all i > 0 every symbol in string SHOULD BE a digit
         */
        for (int i = 0; i < str.size(); i++)
            if ((i == 0 && (!isdigit(str[i]) && str[i] != '-')) ||
                (i > 0 && !isdigit(str[i])))
                return false;

        return true;
    }

    int comparePos(BigNumber a, BigNumber b) {
        if (a.num.length() > b.num.length())
            return 1;
        else if (a.num.length() < b.num.length())
            return -1;
        else {
            // means that lengths of both BigNumbers are eq
            for (int i = 0; i < a.num.length(); i++) {
                if (a.num[i] > b.num[i]) return 1;
                if (a.num[i] < b.num[i]) return -1;
            }
            return 0;
        }
    }

    int compareNeg(BigNumber a, BigNumber b) {
        // deleting "-" from our negative BN
        BigNumber pos_a(a.num.substr(1));
        BigNumber pos_b(b.num.substr(1));
        return -1 * comparePos(pos_a, pos_b);
    }

    /**
     * if returns -1 --> a < b
     * if returns 0 --> a = b
     * if returns 1 --> a > b
     */
    int compare(BigNumber a, BigNumber b) {
        // -a > b
        if (isNeg(a) && !isNeg(b)) return -1;
        // a > -b
        if (!isNeg(a) && isNeg(b)) return 1;

        if (isNeg(a) && isNeg(b))
            return compareNeg(a, b);
        else
            return comparePos(a, b);
    }

    int compareByMod(BigNumber a, BigNumber b) {
        BigNumber aMod(isNeg(a) ? a.num.substr(1) : a.num);
        BigNumber bMod(isNeg(b) ? b.num.substr(1) : b.num);
        return comparePos(aMod, bMod);
    }

    BigNumber deleteZeroes(string a) {
        size_t first_non_zero = a.find_first_not_of('0');
        if (first_non_zero != string::npos) {
            a.erase(0, first_non_zero);
        } else {
            a.erase(1);
        }

        return BigNumber(a);
    }

    BigNumber deleteZeroes(BigNumber a) {
        return BigNumber(deleteZeroes(a.num));
    }

    // === ============== ===
    // === public section ===
   public:
    /**
     * default constructor
     */
    BigNumber() : num("0") {}

    /**
     * constructor for string
     */
    BigNumber(const string& digit) : num(digit) {
        if (!isNumber(digit) && digit.size() != 0)
            throw invalid_argument("Invalid argument: not a number");

        num = digit.size() > 0 ? digit : "0";
    }

    /**
     * constructor for generic type T, enabled for arithmetic types
     */
    template <typename T>
    BigNumber(const T& digit,
              typename enable_if<is_integral<T>::value>::type* = 0)
        : num(to_string(digit)) {}

    /**
     * basic operands overload for code simplicity
     */
    template <typename T>
    typename enable_if<is_arithmetic<T>::value, BigNumber&>::type operator=(
        const T& digit) {
        num = to_string(digit);
        return *this;
    }

    BigNumber operator+(const BigNumber& other) const {
        return BigNumber(num).add(other);
    }

    BigNumber& operator+=(const BigNumber& other) {
        *this = *this + other;
        return *this;
    }

    /**
     * prefix operator overload
     */
    BigNumber& operator++() {
        *this += BigNumber(1);
        // return current value (watch order of operations for prefix)
        return *this;
    }

    /**
     * postfix operator overload
     */
    BigNumber operator++(int) {
        // making a copy of current number
        BigNumber temp(*this);
        ++*this;
        // return current copy (watch order of operations for postfix)
        return temp;
    }

    BigNumber operator-(const BigNumber& other) const {
        return BigNumber(num).sub(other);
    }

    BigNumber& operator-=(const BigNumber& other) {
        *this = *this - other;
        return *this;
    }

    /**
     * prefix operator overload
     */
    BigNumber& operator--() {
        *this -= BigNumber(1);
        // return current value (watch order of operations for prefix)
        return *this;
    }

    /**
     * postfix operator overload
     */
    BigNumber operator--(int) {
        // making a copy of current number
        BigNumber temp(*this);
        --*this;
        // return current copy (watch order of operations for postfix)
        return temp;
    }

    BigNumber operator*(const BigNumber& other) const {
        return BigNumber(num).mul(other);
    }

    BigNumber& operator*=(const BigNumber& other) {
        *this = *this * other;
        return *this;
    }

    BigNumber operator/(const BigNumber& other) const {
        return BigNumber(num).div(other);
    }

    BigNumber& operator/=(const BigNumber& other) {
        *this = *this / other;
        return *this;
    }

    BigNumber operator%(const BigNumber& other) const {
        return BigNumber(num).mod(other);
    }

    BigNumber& operator%=(const BigNumber& other) {
        *this = *this % other;
        return *this;
    }

    // ========== ========= ========
    // comparison operators overload
    bool operator==(const BigNumber& other) const {
        return BigNumber(num).eq(other);
    }

    bool operator!=(const BigNumber& other) const {
        return !BigNumber(num).eq(other);
    }

    bool operator<=(const BigNumber& other) const {
        return BigNumber(num).leq(other);
    }

    bool operator<(const BigNumber& other) const {
        return BigNumber(num).le(other);
    }

    bool operator>=(const BigNumber& other) const {
        return BigNumber(num).geq(other);
    }

    bool operator>(const BigNumber& other) const {
        return BigNumber(num).ge(other);
    }

    /**
     * other public functions
     */
    BigNumber add(BigNumber b) {
        // if [-a + -b] => [-(a + b)]
        if (isNeg(num) && isNeg(b)) {
            BigNumber pos_num(num.substr(1));
            BigNumber pos_b(b.num.substr(1));
            BigNumber result(pos_num.add(pos_b));

            return BigNumber("-" + result.num);
        }

        // if [a + -b] => [a - b]
        if (!isNeg(num) && isNeg(b)) return BigNumber(num).sub(b.num.substr(1));

        // if [-a + b] => [b - a]
        if (isNeg(num) && !isNeg(b)) return b.sub(num.substr(1));

        // if numbers are positive:
        string result;
        int carry = 0;
        int i = num.length() - 1;
        int j = b.num.length() - 1;

        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) {
                sum += num[i] - '0';
                i--;
            }
            if (j >= 0) {
                sum += b.num[j] - '0';
                j--;
            }
            carry = sum / 10;
            sum %= 10;
            result += to_string(sum);
        }

        reverse(result.begin(), result.end());
        return result;
    }

    BigNumber sub(BigNumber b) {
        if (BigNumber(num).eq(b)) return zero();

        // if [-a - -b] => [b - a]
        if (isNeg(num) && isNeg(b))
            return BigNumber(num.substr(1)).sub(num.substr(1));

        // if [a - -b] => [a + b]
        if (!isNeg(num) && isNeg(b)) return BigNumber(num).add(b.num.substr(1));

        // if [-a - b] => [-(a + b)]
        // we got the same logic in "add"
        if (isNeg(num) && !isNeg(b))
            return BigNumber("-" + b.add(num.substr(1)).num);

        // if a < b
        if (BigNumber(num).le(b)) return BigNumber("-" + b.sub(num).num);

        // if numbers are positive and a >= b:
        string result;
        int borrow = 0;
        int i = num.length() - 1;
        int j = b.num.length() - 1;

        while (i >= 0 || j >= 0) {
            int diff = borrow;
            if (i >= 0) {
                diff += num[i] - '0';
                i--;
            }
            if (j >= 0) {
                diff -= b.num[j] - '0';
                j--;
            }
            if (diff < 0) {
                diff += 10;
                borrow = -1;
            } else {
                borrow = 0;
            }
            result += to_string(diff);
        }

        reverse(result.begin(), result.end());
        return deleteZeroes(result);
    }

    BigNumber mul(BigNumber b) {
        BigNumber result("0");

        // if [-a * -b] => [a * b]
        if (isNeg(num) && isNeg(b))
            return BigNumber(num.substr(1)).mul(b.num.substr(1));

        // if [-a * b] => [-(a * b)]
        if (isNeg(num))
            return BigNumber("-" + BigNumber(num.substr(1)).mul(b).num);

        // if [a * -b] => [-(a * b)]
        if (isNeg(b))
            return BigNumber("-" + BigNumber(num).mul(b.num.substr(1)).num);

        for (int i = num.length() - 1; i >= 0; i--) {
            string temp;
            int carry = 0;

            for (int j = b.num.length() - 1; j >= 0; j--) {
                int product = (num[i] - '0') * (b.num[j] - '0') + carry;
                carry = product / 10;
                product %= 10;
                temp += to_string(product);
            }
            if (carry > 0) {
                temp += to_string(carry);
            }

            reverse(temp.begin(), temp.end());
            temp += string(num.length() - i - 1, '0');
            result = result.add(BigNumber(temp));
        }

        return deleteZeroes(result);
    }

    BigNumber powBN(BigNumber degree) {
        if (BigNumber(num).eq(zero())) return zero();
        if (degree.eq(zero())) return BigNumber("1");
        return BigNumber(num).mul(
            BigNumber(num).powBN(degree.sub(BigNumber("1"))));
    }

    BigNumber div(BigNumber divisor) {
        if (divisor.eq(zero())) throw "Division by zero!";
        if (BigNumber(num).eq(zero())) return zero();
        if (compareByMod(num, divisor) == -1) return zero();

        string index = "";
        if (!(isNeg(num) && isNeg(divisor)) &&
            !(!isNeg(num) && !isNeg(divisor))) {
            index += "-";
        }

        //////////////////////////////////////////////

        string quotient = "";
        size_t nextIndex = divisor.num.size();
        BigNumber tmp = BigNumber(num.substr(0, divisor.num.size()));

        size_t extraDigits = 0;

        while (nextIndex < num.size()) {
            if (tmp < divisor) {
                tmp = BigNumber(deleteZeroes(tmp.num + num[nextIndex]));
                extraDigits++;
                nextIndex++;
            }
            if (tmp < divisor) {
                quotient += "0";
                continue;
            }

            BigNumber q = 0;
            while (tmp >= divisor) {
                tmp -= divisor;
                q++;
            }
            quotient += q.num;
            extraDigits = 0 + (tmp == 0 ? 1 : 0);
        }

        if (divisor.num.size() == num.size()) {
            BigNumber q = 0;
            while (tmp >= divisor) {
                tmp -= divisor;
                q++;
            }
            quotient += q.num;
        }

        return BigNumber(index + quotient);
    }

    /**
     * EXPERIMENTAL!!!
     *
     * #Начальное приближение
     * x = 1.0 / b
     *
     * #Итерационный процесс
     * for i in range(max_iterations):
     *     x = x * (2 - b * x)
     *
     *     #Проверка условия остановки
     *     if abs(a * x - 1) < tolerance:
     *        break
     *
     * #Результат деления
     * return a * x
     */
    BigNumber fastDiv(BigNumber divisor, size_t maxIterations = 100) {
        BigNumber scale("10000000");  // mul all numbers by 10_000_000
        BigNumber tolerance("1");
        BigNumber divider = BigNumber(num).mul(scale);

        BigNumber x = scale / divisor;

        for (size_t i = 0; i < maxIterations; i++) {
            x = x * (scale * 2 - (divisor * x) / scale) / scale;
            if (compareByMod(divider * x / scale - scale, tolerance) == -1)
                break;
        }

        return divider * x / scale;
    }

    /**
     * a narrowly focused function that accepts only a positive modulus and if
     * num is negative, then returns a positive number NOT GREATER than modulus
     * Examples:
     * 123 mod -2 --> ERROR
     * -5 mod 2 --> 1
     * 20 mod 3 --> 2
     */
    BigNumber mod(BigNumber modulus) {
        if (isNeg(modulus))
            throw invalid_argument("Invalid argument: modulus is negative");
        /**
         * -n mod m = (-n * -1 + (n - m)) mod m
         */
        if (isNeg(BigNumber(num))) {
            BigNumber revretedNum(BigNumber(num).mul(BigNumber("-1")));
            BigNumber subResult(BigNumber(num).sub(modulus));

            return revretedNum.add(subResult.mod(modulus));
        }

        BigNumber factorOne = BigNumber(num).div(modulus);
        return BigNumber(num).sub(factorOne.mul(modulus));
    }
    // ================
    // compare methods:

    /**
     * shows true if num = second
     */
    bool eq(BigNumber second) { return compare(num, second) == 0; }

    /**
     * shows true if num <= second
     */
    bool leq(BigNumber second) {
        int res = compare(num, second);
        return res == 0 || res == -1;
    }

    /**
     * shows true if num >= second
     */
    bool geq(BigNumber second) {
        int res = compare(num, second);
        return res == 0 || res == 1;
    }

    /**
     * shows true if num < second
     */
    bool le(BigNumber second) { return compare(num, second) == -1; }

    /**
     * shows true if num > second
     */
    bool ge(BigNumber second) { return compare(num, second) == 1; }

    // =======
    // friends

    /**
     * work book electronic extract
     */
    friend ostream& operator<<(ostream& os, const BigNumber& bigNum) {
        return os << bigNum.num;
    }
};

#endif