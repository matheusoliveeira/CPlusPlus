#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>

// path: C:\Users\mathe\Documents\teste

using namespace std;

int main() {

    bool isValidFileName = false;
    bool isValidFilePath = false;
    string filePath;
    string fileName;

    do {
        // Ask for the filePath
        //SOURCE: https://stackoverflow.com/questions/70924991/check-if-directory-exists-using-filesystem
        cout << "\nPlease insert a valid file path to save your file: \nExample: (C:\\Users\\[USER NAME]\\Desktop\\):" << endl;
        getline(cin, filePath);
        bool filepathExists = std::filesystem::is_directory(filePath);

        // If the file path exists this will include a / if the user miss it
        if (filepathExists) {
            cout << "\nYour file path is GOOD: " << filePath << endl;
            if (filePath.back() != '/' && filePath.back() != '\\') {
                filePath += '/';
            }
            isValidFilePath = true;

        } else {
            cout << "\nSorry, it is an invalid file path.\nPlease try it again.\n" << endl;
        }

    } while (!isValidFilePath);

    // Regex source: https://www.experts-exchange.com/questions/22779026/Regex-for-a-valid-Windows-file-name.html
    regex reg("^(?!((con|prn|aux)((\\.[^\\\\/:\"*?<>|]{1,3}$)|$))|[\\s\\.])[^\\\\/:*?<>|]{1,254}$");

    do {
        // Ask for the fileName
        cout << "\nPlease insert your file name: \nExample: (myFile):" << endl;
        getline(cin, fileName);
        if (regex_search(fileName, reg)) {
            cout << "\nIt is a valid name, thank you!" << endl;
            isValidFileName = true;
        } else {
            cout << "\nThe name is invalid, please insert it again." << endl;
        }

        cout << "Your full file name is: " << fileName << ".html" << endl;

    } while (!isValidFileName);

    std::ifstream inputFile("OriginalCPP.cpp");

    // Create and open the output HTML file
    std::ofstream outputFile(filePath + fileName + ".html", std::ios::out);

    // SOURCE: https://www.udacity.com/blog/2021/05/how-to-read-from-a-file-in-cpp.html
    if(!inputFile.fail() && !outputFile.fail()) {
        char mychar = inputFile.get(); // gets the first char

        outputFile << "<PRE>" << std::endl;
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
    }

    return 0;
}