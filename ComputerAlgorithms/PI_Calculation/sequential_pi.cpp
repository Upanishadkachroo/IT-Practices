#include <iostream>
#include <cmath>
#include <iomanip> // For setting precision
#include <chrono>  // For time measurement

using namespace std;
using namespace std::chrono;

// Function to estimate pi sequentially
double estimate_pi_sequential(int n) {
    // Initialize the total area
    double total_area = 0.0;
    
    // Width of each strip
    double delta_x = 1.0 / n;
    
    // Sum the areas of the strips
    for (int i = 1; i <= n; ++i) {
        // x_i is the right endpoint of the i-th strip
        double x_i = i / static_cast<double>(n);
        // Height of the rectangle at x_i
        double y_i = std::sqrt(1.0 - x_i * x_i);
        // Area of the rectangle (y_i * delta_x)
        total_area += y_i * delta_x;
    }
    
    // The estimate of pi is 4 times the area
    double pi_estimate = 4 * total_area;
    return pi_estimate;
}

int main() {
    // Number of strips (subintervals) to divide the range [0,1] into
    int n = 1000000; // You can change this number to improve the accuracy
    cout << "Enter the number of strips you want to create between [0,1]: ";
    cin >> n;

    // Start the clock before calculation
    auto start = high_resolution_clock::now();

    // Calculate pi estimate using numerical integration
    double pi_estimate = estimate_pi_sequential(n);

    // Stop the clock after calculation
    auto stop = high_resolution_clock::now();

    // Actual value of pi
    double pi_actual = M_PI; // In C++, M_PI is defined in <cmath>

    // Calculate the error from the actual value of pi
    double error = std::fabs(pi_estimate - pi_actual);

    // Calculate the duration of the calculation in milliseconds
    auto duration = duration_cast<milliseconds>(stop - start);
    
    // Output the results
    cout << "Estimated value of Pi: " << std::setprecision(15) << pi_estimate << endl;
    cout << "Actual value of Pi: " << std::setprecision(15) << pi_actual << endl;
    cout << "Error from actual value of Pi: " << std::setprecision(15) << error << endl;
    cout << "Time taken for calculation: " << duration.count() << " milliseconds" << endl;

    return 0;
}
