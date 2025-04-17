#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include "include/json.hpp"

using json = nlohmann::json;
using namespace std;

// Convert number from any base to decimal
long long convertToDecimal(const string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else continue; // Skip invalid characters
        if (digit >= base) {
            cerr << "Error: Digit " << c << " is not valid for base " << base << endl;
            return -1; // Return an error value
        }
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation at x = 0 to get the constant term c
long long lagrangeInterpolation(const vector<pair<int, long long>>& points, int k) {
    long double result = 0;

    // Loop over each point to compute the Lagrange basis polynomial
    for (int j = 0; j < k; ++j) {
        long double xj = points[j].first;
        long double yj = points[j].second;
        long double term = yj;

        // Calculate the Lagrange basis polynomial for each point
        for (int m = 0; m < k; ++m) {
            if (m != j) {
                long double xm = points[m].first;
                term *= (-xm) / (xj - xm);  // Evaluate at x = 0
            }
        }
        result += term;  // Add up the contribution of each term
    }

    return llround(result);  // Round to nearest integer
}

// Function to process the input and find the secret (constant term)
void solve(const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    json j;
    inFile >> j;
    inFile.close();

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<int, long long>> points;

    // Parse the input and decode the values
    for (auto& [key, val] : j.items()) {
        if (key == "keys") continue;
        if (!all_of(key.begin(), key.end(), ::isdigit)) continue;

        int x = stoi(key);  // Convert the key (which is a string) to an integer
        int base = stoi(val["base"].get<string>());
        string value = val["value"].get<string>();

        long long y = convertToDecimal(value, base);
        if (y == -1) return; // Handle conversion error
        points.push_back({x, y});
    }

    // Sort the points based on the x values
    sort(points.begin(), points.end(), [](const pair<int, long long>& a, const pair<int, long long>& b) {
        return a.first < b.first;
    });

    // Ensure there are enough points
    if (points.size() < k) {
        cerr << "Not enough points for interpolation in file: " << filename << endl;
        return;
    }

    // Take the first k points for interpolation
    vector<pair<int, long long>> subset(points.begin(), points.begin() + k);
    long long secret = lagrangeInterpolation(subset, k);

    cout << "Secret (c) for " << filename << ": " << secret << endl;
}

int main() {
    // Call the function for both test cases
    solve("C:/Users/suren/Desktop/possibleworks/testcase1.json");
    solve("C:/Users/suren/Desktop/possibleworks/testcase2.json");
    return 0;
}