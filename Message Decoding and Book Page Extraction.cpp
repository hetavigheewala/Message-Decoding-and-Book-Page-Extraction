// Message Decoding and Book Page Extraction

/// @brief This program ...
///        1. obtains a secret message file name from the user
///        2. reads the coded message into a string array
///        3. copies the array into a second string array
///        4. sorts the second string array
///        5. prints the two arrays side-by-side allowing 12 characters per col
///        6. obtains a page number from the user
///        7. opens a book and reads each line of text from that page into
///           an array.
///        8. prints the array to the screen.

// preprocessor directives
// library includes
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cassert>

// set namespace
using namespace std;

/// -------------------------------------------------------------------------
/// Function Prototype(s)
/// -------------------------------------------------------------------------
// for processing the secrete message
string getFileName(const string& prompt);
void getMessage(const string& fileName, string msg[], int& size, int maxSize);
void copyArray(const string a[], string b[], int size);
int extractPageNumber(const string codeStr);
void sortArray(string a[], int size);
void printSideBySide(const string a[], const string b[], int size);

// for processing the target page
int getPageNumber(const string& prompt);
int getPageLines(const string& FN, int pageNum, string a[], int MAX_LINES);
void printPageLines(const string a[], int size);

/// -------------------------------------------------------------------------
/// Main entry-point for this application
///
/// @returns Exit-code for the process - 0 for success, else an error code.
/// -------------------------------------------------------------------------
int main() {
    // Local constant definition(s)
    const int MAX_MESSAGE_SIZE = 100;       // max chars in msg
    const int MAX_PAGE_LINES = 25;          // max usable lines on a page

    // Variable declaration(s)
    int exitState;              // tracks the exit code for the process
    exitState = EXIT_SUCCESS;   // set state to successful run

    string messageFN;           // lab08-CodedMessage0.txt
    string bookKeyFN;           // Wizard_of_Oz.txt

    string message[MAX_MESSAGE_SIZE]{};         // host array for message
    string messageSorted[MAX_MESSAGE_SIZE]{};   // host array for sorted msg
    string bookPage[MAX_PAGE_LINES];            // host array for book page

    int messageSize = 0;        // actual size of message for virtual array
    int pageNumber;             // user supplied page number
    int lineCount;              // number of lines in the page for virtual array

    /* ******* Input Phase *********************************************** */
    messageFN = getFileName("Enter the file name of the coded message: ");
    // lab08-CodedMessage0.txt
    // lab08-CodedMessage1.txt

    bookKeyFN = getFileName("Enter the file name of the cipher text key: ");
    // Wizard_of_Oz.txt

    pageNumber = getPageNumber("Enter the number of the page to display: ");
    // 42

    cout << endl;

    /* ******* Processing Phase ****************************************** */
    // this section inputs the message, duplicates, and sorts the duplicate
    getMessage(messageFN, message, messageSize, MAX_MESSAGE_SIZE);
    copyArray(message, messageSorted, messageSize);
    sortArray(messageSorted, messageSize);

    // get the text from the user selected page into the array
    lineCount = getPageLines(bookKeyFN, pageNumber, bookPage, MAX_PAGE_LINES);


    /* ******* Output Phase ********************************************** */
    // print the unsorted and sorted cipher codes side-by-side
    printSideBySide(message, messageSorted, messageSize);
    cout << endl;

    // print the selected page of text
    printPageLines(bookPage, lineCount);
    cout << endl;


    return exitState;
}

/// -------------------------------------------------------------------------
/// Function Implementation(s)
/// -------------------------------------------------------------------------

/// This function gets a file name from the user
/// @param prompt message to user describing expected information
/// @return the name of the file
string getFileName(const string& prompt) {
    string fileName;        // holds the name of the file

    // prompt the user for and obtain the filename
    cout << prompt;
    cin >> fileName;

    return fileName;
}


/// This function reads the secrete message file and populates the msg array
/// @param fileName user supplied file name
/// @param [out] msg an array that will host the message lines
/// @param [out] size used to track the end of the virtual array
/// @param maxSize Maximum number of lines the host array can handle
void getMessage(const string& fileName, string msg[], int& size, int maxSize) {
    ifstream messageFin;    // file stream handler for message file
    string line;            // holds a single line of input
    int idx;                // array index used for populating arrays

    // attempt to open the message file
    messageFin.open(fileName.c_str());

    // populate the array if the file could be opened
    if (messageFin.is_open()) {
        idx = 0;
        while (getline(messageFin, line) && idx < maxSize) {
            msg[idx++] = line;
        }
        size = idx;          // save the number of elements used by msg
    } else {
        cout << "Unable to open: " << fileName << endl;
    }
    messageFin.close();
}

/// This function makes a copy of an array
/// @param a immutable array that will be copied
/// @param b target array receiving the data
/// @param size size of virtual array
void copyArray(const string a[], string b[], int size) {
    for (int i = 0; i < size; i++) {
        b[i] = a[i];
    }
}

/// This function extracts the page number from the cipher code
/// @param codeStr cipher code string
/// @return the page number from the code string.
int extractPageNumber(const string codeStr) {
    int pageNum;    // holds the derived page number

    const size_t pos1 = codeStr.find('.');
    pageNum = stoi(codeStr.substr(0, pos1));

    return pageNum;
}


/// This function performs a selection sort on the virtual portion of the array
/// @param a is the virtual array to be sorted
/// @param size is the end of the virtual array
void sortArray(string a[], int size) {
    int i;          // outer loop index
    int j;          // inner loop index
    int minIdx;     // index of min val
    int p;          // extracted page number from a[j]
    int pMin;       // extracted page number from a[minIdx]
    string temp;    // temp holder for string swap

    // adjust boundary between sorted and unsorted portions
    for (i = 0; i < size - 1; i++) {
        // Find the smallest element in unsorted portion of the array
        // so that it can be swapped
        minIdx = i;
        for (j = i + 1; j < size; j++) {
            // extract the page number and compare the integer values instead
            // of the strings.
            p = extractPageNumber(a[j]);
            pMin = extractPageNumber(a[minIdx]);
            if (p < pMin) {
                //            if (a[j] < a[minIdx]) {
                minIdx = j;
            }
        }

        // Swap the smallest element with the first unsorted element
        temp = a[minIdx];
        a[minIdx] = a[i];
        a[i] = temp;
    }
}

/// This function prints two arrays side-by-side using a column width of 12
/// @param a the unsorted array
/// @param b the sorted array
/// @param size the virtual size of the two arrays
void printSideBySide(const string a[], const string b[], int size) {
    const int COLW = 12;                // the column width for table
    for (int i = 0; i < size; i++) {
        cout << left << "    "
        << setw(COLW) << a[i]
        << setw(COLW) << b[i]
        << endl;
    }
}

/// This function obtains a page number from a friendly user
/// If the user isn't friendly, an assertion is executed on a stream failure
/// @param prompt message to user describing expected information
/// @return the integer page number
int getPageNumber(const string& prompt) {
    int page;           // holds page number

    cout << prompt;     // prompt the user for the page number
    cin >> page;        // and save to page

    assert(!cin.fail());    // fail on bad input
    assert(page > 0);       // fail on negative numbers

    return page;
}

/// This function retrieves the lines of text on a particular page
/// @param [in] FN The name of the book (cipher text) file
/// @param pageNum the page number to retrieve
/// @param [out] a the array that will receive the lines from the page
int getPageLines(const string& FN, int pageNum, string a[], int MAX_LINES) {
    int pageCount = -3;             // set to -3 to skip front matter
    int lineCount = 0;              // current line in page
    bool done = false;              // LCV
    string line;                    // holds line read from file

    ifstream fin;                   // input file handler

    // open the cipher text
    fin.open(FN.c_str());

    // read the lines on the page, even the blank ones
    if (fin.is_open()) {
        // ignore all pages and information up to the target page
        while (pageCount != pageNum && getline(fin, line)) {
            if (line.find("page") != std::string::npos) {
                pageCount++;
            }
        }

        // read the lines after the page number of the target page
        // while tracking track the line count
        done = false;
        while (getline(fin, line) && !done) {
            // "page" is no where in the document other than identifying page
            // numbers. So we can use the presence of "page" to indicate we are
            // done reading the page.
            if (line.find("page") != std::string::npos) {
                done = true;
            } else {
                // place into the array and increment the line count
                // we didn't see "page" because we're not done
                a[lineCount++] = line;
                if (lineCount > MAX_LINES) {
                    done = true;
                }
            }
        }
    } else {
        cout << "Unable to open: " << FN << endl;
    }
    // done reading page
    fin.close();
    // return the number of lines that were read
    return lineCount;
}

/// This function prints the lines from the page we captured
/// @param a the array holding the page lines
/// @param size the virtual size of the page
void printPageLines(const string a[], int size) {
    for (int i = 0; i < size; i++) {
        cout << a[i] << endl;
    }
}