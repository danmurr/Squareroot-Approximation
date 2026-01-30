#include <iostream>
#include <fstream> // Required for file operations
#include <cmath>   // For cmath sqrt comparison

#include <cstdlib> // Required for rand() and srand()
#include <ctime>   // Required for time()
#include <vector>
#include <string>
#include <filesystem>

/*

This code is not meant to be optimized or scrutinized.
It is merely intended to function.

*/

using namespace std;
namespace fs = std::filesystem;

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

string get_new_file_name(string path){
    vector<string> files = file_names(path);

    int fileNum{0};
    int fileNumMax{0};
    for (int i = 0; i < files.size(); i++){
        size_t pos1 = files[i].find('-'); // Find the position of '-'
        size_t pos2 = files[i].find('.'); // Find the position of '.'

        string number = files[i].substr(pos1 + 1, pos2 - pos1 - 1);
        fileNum = stoi(number);
        if (fileNum > fileNumMax)
            fileNumMax = fileNum; 
        cout << number << "\n";
    }
    
    try {
        fileNumMax++;
        return ("sequence-" + to_string(fileNumMax) + ".csv");
    } 
    catch (const invalid_argument& e) {
        cout << "Here 2" << "\n";
        cerr << "Error: The string is not a valid number.\n";
    }
    return "sequence-1.csv";
}

int main() {
    const string path = "../test/test-sequences/"; // Current directory
    string new_file_name = get_new_file_name(path);
    string new_file_path = path + new_file_name;

    ofstream myFile(new_file_path);
    myFile << "input,mysqrt,cmath sqrt" << "\n";
    for (int i = 1; i < 10; ++i) {
        for (int j = 0; j < 10; ++j){
            int max_val = static_cast<int>(pow(10, i) - 1);
            int min_val = static_cast<int>(pow(10, i - 1));

            int range_size = max_val - min_val + 1;
            double integer = static_cast<double>(rand() % range_size + min_val);
            if (max_val <= 0) max_val = 1; // Prevent modulo by zero
            double decimal = static_cast<double>(rand() % 1000001) / pow(10, 5);
            double random_val = integer + decimal;

            // Fix 7: Format the CSV line correctly
            myFile << random_val << "," << "\n";
        }

    }

    // 4. Close the file
    myFile.close();

    cout << "CSV file created successfully!" << endl;

    return 0;
}