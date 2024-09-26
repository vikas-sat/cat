#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to decode the value in the given base
int decode_value(const string& value, int base) {
    int result = 0;
    int power = 1;
    for (int i = value.length() - 1; i >= 0; --i) {
        char digit = value[i];
        int digit_value;
        if (digit >= '0' && digit <= '9') {
            digit_value = digit - '0';
        } else {
            digit_value = digit - 'A' + 10;
        }
        result += digit_value * power;
        power *= base;
    }
    return result;
}

// Structure for handling fractions
struct fraction {
    long long num, den;

    // Constructor
    fraction(long long n, long long d) : num(n), den(d) {}

    // Reduce the fraction using GCD
    void reduce_fraction() {
        long long gcd = __gcd(num, den);
        num /= gcd;
        den /= gcd;
    }

    // Multiplication of two fractions
    fraction operator*(fraction f) {
        fraction temp(num * f.num, den * f.den);
        temp.reduce_fraction();
        return temp;
    }

    // Addition of two fractions
    fraction operator+(fraction f) {
        fraction temp(num * f.den + den * f.num, den * f.den);
        temp.reduce_fraction();
        return temp;
    }
};

// Lagrange interpolation to recover the constant term 'c'
long long generate_secret(int x[], int y[], int M) {
    fraction ans(0, 1);

    // Iterate through the given points
    for (int i = 0; i < M; ++i) {
        fraction l(y[i], 1); // Start with y[i]
        for (int j = 0; j < M; ++j) {
            if (i != j) {
                fraction temp(-x[j], x[i] - x[j]); // (x - xj) / (xi - xj)
                l = l * temp;
            }
        }
        ans = ans + l;
    }
    // Return the secret (the numerator of the final fraction)
    return ans.num / ans.den; // Since the answer should be an integer
}

// Main function with hardcoded input
int main() {
    // Hardcoded input test case
    int n = 9;  // Number of points
    int k = 6;  // Minimum number of points required

    // Corrected map of points (with base and encoded value)
    map<int, pair<int, string>> test_case = {
        {1, {10, "28735619723837"}},
        {2, {16, "1A228867F0CA"}},
        {3, {12, "32811A4AA0B7B"}},
        {4, {11, "917978721331A"}},
        {5, {16, "1A22886782E1"}},
        {6, {10, "28735619654702"}},
        {7, {14, "71AB5070CC4B"}},
        {8, {9, "122662581541670"}},
        {9, {8, "642121030037605"}}
    };

    vector<pair<int, int>> points;

    // Decode the values from the given base
    for (auto& entry : test_case) {
        int x = entry.first;  // The key of the map
        int base = entry.second.first;  // The base
        string encoded_value = entry.second.second;  // The encoded value

        // Decode the encoded value using the base
        int decoded_value = decode_value(encoded_value, base);
        points.push_back({x, decoded_value});
    }

    // We only need k points to recover the secret
    int* x = new int[k];
    int* y = new int[k];

    // Use the first k points for Lagrange interpolation
    for (int i = 0; i < k; ++i) {
        x[i] = points[i].first;
        y[i] = points[i].second;
    }

    // Get the secret constant term 'c'
    long long secret = generate_secret(x, y, k);

    cout << "The constant term (secret) is: " << secret << endl;

    // Clean up
    delete[] x;
    delete[] y;

    return 0;
}