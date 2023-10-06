#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>
#include <exception>

using namespace std;

int main() {

    bool isValidFileName = false;
    bool isValidFilePath = false;
    string filePath;
    string fileName;

    // Defining my custom exception
    // Source: https://nscconline.brightspace.com/d2l/le/content/279260/viewContent/4020321/View
    struct MyException : public exception
    {
    public:
        const string what ()
        {
            return "Your file appears to still opened.";
        }

    };

    // Ask for the filePath
    do {
        // Regex based on source: https://stackoverflow.com/questions/24702677/regular-expression-to-match-a-valid-absolute-windows-directory-containing-spaces
        regex regPath("^[a-zA-Z]:\\\\(?!\\s)([^<>:\"/\\\\|?*]+\\\\?){0,256}$");

        //SOURCE: https://stackoverflow.com/questions/70924991/check-if-directory-exists-using-filesystem
        cout << "\nPlease insert a valid file path to save your file or press 'Q' to quit: \nExample: C:\\Users\\[USER_NAME]\\Desktop\\:" << endl;
        getline(cin, filePath);

        // Check if the user typed 'Q' to quit the program
        if (filePath == "Q") {
            cout << "Quitting.." << endl;
            return 0;
        }

        // Validate the file path
        if (regex_search(filePath, regPath)) {

            // If is valid, check if the path exists
            bool filepathExists = std::filesystem::is_directory(filePath);

            // If the file path exists this will include a / if the user miss it
            if (filepathExists) {
                cout << "\nYour path is valid!" << endl;

                if (filePath.back() != '/' && filePath.back() != '\\') {
                    filePath += '/';
                }

                isValidFilePath = true;

            } else {
                cout << "\nSorry, your path doesn't exists.\nPlease try it again.\n" << endl;
            }

        } else {
            cout << "\nThe path is invalid, please insert it again." << endl;
        }

    } while (!isValidFilePath);

    // Ask for the fileName
    do {
        // Regex source: https://www.experts-exchange.com/questions/22779026/Regex-for-a-valid-Windows-file-name.html
        regex regFileName("^(?!((con|prn|aux)((\\.[^\\\\/:\"*?<>|]{1,3}$)|$))|[\\s\\.])[^\\\\/:*?<>|]{1,254}$");

        cout << "\nPlease insert your file name: \nExample: myFile:" << endl;
        getline(cin, fileName);

        if (regex_search(fileName, regFileName)) {
            isValidFileName = true;
        } else {
            cout << "\nThe name is invalid, please insert it again." << endl;
        }
        cout << "\nYour new file " << fileName << ".html has been created!" << endl;

    } while (!isValidFileName);

    try {
        // Open the input file
        std::ifstream inputFile("OriginalCPP.cpp");

        // If didn't open, throw an error
        if (!inputFile.is_open()) {
            throw std::ios_base::failure("Failed to open input file");
        }

        // Create and open the output HTML file
        std::ofstream outputFile(filePath + fileName + ".html", std::ios::out);

        // If didn't open, throw an error
        if (!outputFile.is_open()) {
            throw std::ios_base::failure("Failed to open output file");
        }

        // SOURCE: https://www.udacity.com/blog/2021/05/how-to-read-from-a-file-in-cpp.html
        char mychar = inputFile.get(); // gets the first char

        outputFile << "<PRE>" << std::endl;
        // Check if it is the end of the file
        while(!inputFile.eof()) {
            if (mychar == '<') {
                outputFile << "&lt;";
            } else if (mychar == '>') {
                outputFile << "&gt;";
            } else {
                outputFile << mychar;
            }
            mychar = inputFile.get(); // gets the next char before loop it again
        }
        outputFile << "</PRE>" << std::endl;

        inputFile.close();
        outputFile.close();

        // If any of the files still open, throw my custom exception
        if (inputFile.is_open() || outputFile.is_open()) {
            throw MyException();
        }

    }
    // Programmer exception
    catch (MyException& customException) {
        std::cerr << "Custom exception occurred: " << customException.what() << std::endl;
        return 1;
    }
    // Library Exception (std::exception)
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    // Default exception
    catch (...) {
        std::cerr << "A default exception occurred." << std::endl;
        return 1;
    }

    return 0;
}