#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include <chrono>
#include <format>
#include <algorithm> // Required for replace
#include <cmath>

#include "functions.hpp"

using namespace std;
namespace fs = filesystem;

/*

This code is not meant to be optimized or scrutinized.
It is merely intended to function.

*/
bool areEqual(double a, double b, double epsilon = 1e-2) {
    return abs(a - b) < epsilon;
}

vector<string> file_names(string path){
    vector<string> return_vector;
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            // Iterate through the directory
            for (const auto& entry : fs::directory_iterator(path)) {
                // entry.path().filename() gives just the name (e.g., "data.csv")
                return_vector.push_back(entry.path().filename());
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error: " << e.what() << "\n";
    }
    return return_vector;
}

vector<string> file_lines(string filename){
    ifstream inputFile(filename);
    vector<string> lines_read;

    // Check if the file was opened successfully
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return lines_read; // Return with an error code
    }

    string line;
    int lineNumber = 1;

    // Read the file line by line using getline
    // The loop continues as long as a line is successfully read
    while (getline(inputFile, line)) {
        lines_read.push_back(line); // Store the line
        lineNumber++;
    }

    // Close the file stream (optional, the destructor does this automatically)
    inputFile.close();

    return lines_read;
}

int main() {
    const string path = "../test/test-sequences/";
    // The name of the file you want to read
    vector<string> test_files = file_names(path);
    
    const string write_path = "../test/past-tests/";
    const auto now = chrono::system_clock::now();
    // Format the time point directly into a string (adjust format specifiers as needed)
    string date_time_str = format("{:%Y-%m-%d %H:%M:%S}", now);
    
    replace(date_time_str.begin(), date_time_str.end(), ':', '_');
    replace(date_time_str.begin(), date_time_str.end(), ' ', '_');
    string new_directory = write_path + date_time_str;
    if (fs::create_directory(new_directory)) {
        cout << "Successfully created single directory: " << new_directory << endl;
    } else {
        cout << "Directory already exists or could not be created: " << new_directory << endl;
        return 1;
    }


    for (const string& test_file : test_files) {
        ofstream outputFile(new_directory + "/" + test_file);
        if (!outputFile.is_open()){
            std::cerr << "Error: Unable to create a new file." << std::endl;
            return 1; // Indicate an error
        }

        string read_file = path + test_file;
        vector<string> f_lines = file_lines(read_file);
        
         outputFile << f_lines[0] << "\n";
        // Start at 0 unless you intentionally want to skip the first line
        for (int i = 1; i < f_lines.size(); i++) {
            string test_number = f_lines[i].substr(0, f_lines[i].size()-1);
            try {
                double n = stod(test_number);
                chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
                float cmath_sqrt_val = sqrt(n);
                chrono::steady_clock::time_point current_time = chrono::steady_clock::now();
                chrono::duration<double> elapsed_time = chrono::duration_cast<chrono::nanoseconds>(current_time - start_time);
                float cmath_nanoseconds = (elapsed_time.count() * power(10.0f, 9));
                // cout << "CMath Squareroot : " << cmath_sqrt_val << " in " << (elapsed_time.count() * power(10.0f, 9) )<< " nanoseconds.\n";
                start_time = chrono::steady_clock::now();
                float sqrt_user_value = my_sqrt(n);
                current_time = chrono::steady_clock::now();
                elapsed_time = chrono::duration_cast<chrono::nanoseconds>(current_time - start_time);
                float my_nanoseconds = (elapsed_time.count() * power(10.0f, 9));
                // cout << "My Squareroot : " << sqrt_user_value << " in " << (elapsed_time.count() * power(10.0f, 9) )<< " nanoseconds.\n";

                if (areEqual(cmath_sqrt_val, sqrt_user_value)){
                    // cout << n << ", " << my_nanoseconds << ", " << cmath_nanoseconds << "\n";
                    outputFile << n << ", " << my_nanoseconds << ", " << cmath_nanoseconds << "\n";
                }
                else{
                    outputFile << n << ", -100, " << cmath_nanoseconds << "\n";
                    // cout << cmath_sqrt_val << " is not equal to " << sqrt_user_value <<"\n";
                }
            } catch (const invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << endl;
            } catch (const out_of_range& oor) {
                cerr << "Out of range: " << oor.what() << endl;
            }
            
        }
    }

    return 0; // Return successfully
}
