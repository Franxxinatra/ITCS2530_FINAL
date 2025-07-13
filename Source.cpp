// Full Name: Zachary Schultz
// Class: ITCS-2530
// GitHub Repo URL: https://github.com/Franxxinatra/ITCS2530_FINAL
// One-Sentence Summary: A program that takes chords and puts them in a readable, learnable format.
// Your posted hobby or interest: Guitar chord teacher.
// AI Tool(s) Used: ChatGPT.

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

const int MAX_SECTIONS = 20;
const int MAX_CHORDS = 50;

// Define a struct to hold individual chords and potential metadata
struct Chord {
    string text; // The chord as input by the user, formatted appropriately
    // You can add more fields here, e.g., chord duration, type, etc.
};

// Represents a section of a song, like a verse or chorus
class Section {
public:
    string name; // Name of the section
    Chord chords[MAX_CHORDS]; // Array of chords in the section
    int chordCount = 0; // Number of chords added

    // Prints formatted chords for the section
    void print() const {
        const int spacing = 10;
        cout << "\nFormatted Chords for: " << name << "\n";
        cout << string(50, '-') << "\n";
        for (int i = 0; i < chordCount; ++i) {
            cout << setw(spacing) << left << chords[i].text;
        }
        cout << "\n" << string(50, '-') << "\n";
    }
};

// Validates and gets the number of chords from the user
int getValidChordCount() {
    int count;
    while (true) {
        cout << "Enter the number of chords you will type: ";
        cin >> count;

        if (cin.fail() || count <= 0 || count > MAX_CHORDS) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a positive integer (1 - " << MAX_CHORDS << ").\n";
        }
        else {
            cin.ignore();
            return count;
        }
    }
}

// Formats the chord string for consistency
string formatChord(const string& chordRaw) {
    string formatted;
    bool afterSlash = false;

    for (size_t i = 0; i < chordRaw.size(); ++i) {
        char c = chordRaw[i];
        if (i == 0 || afterSlash) {
            formatted += toupper(c); // Capitalize root and chord after slash
            afterSlash = false;
        }
        else if (c == '/') {
            formatted += c;
            afterSlash = true;
        }
        else {
            formatted += tolower(c);
        }
    }
    return formatted;
}

// Gets and formats chords for a given section
void getChords(Section& section) {
    string chordInput;
    for (int i = 0; i < section.chordCount; ++i) {
        cout << "Chord " << (i + 1) << ": ";
        getline(cin, chordInput);
        section.chords[i].text = formatChord(chordInput);
    }
}

// Allows the user to edit an existing section
void editSection(Section song[], int sectionCount) {
    if (sectionCount == 0) {
        cout << "\nNo sections to edit.\n";
        return;
    }

    cout << "\nWhich section do you want to edit?\n";
    for (int i = 0; i < sectionCount; ++i) {
        cout << (i + 1) << ". " << song[i].name << "\n";
    }

    int index;
    cout << "Enter the section number: ";
    cin >> index;
    cin.ignore();

    if (index < 1 || index > sectionCount) {
        cout << "Invalid section number.\n";
        return;
    }
    index--;

    int choice;
    cout << "What do you want to edit? (1: Section name, 2: Chords): ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        cout << "Enter the new section name: ";
        getline(cin, song[index].name);
    }
    else if (choice == 2) {
        song[index].chordCount = getValidChordCount();
        getChords(song[index]);
    }
    else {
        cout << "Invalid choice.\n";
    }
}

// Prints the entire formatted song
void printFullSong(const Section song[], int sectionCount) {
    cout << "\n======= Full Song Chord Layout =======\n";
    for (int i = 0; i < sectionCount; ++i) {
        song[i].print();
    }
}

// Saves the song structure to a text file
void saveToFile(const Section song[], int sectionCount, const string& filename) {
    ofstream outFile(filename.c_str());
    if (!outFile) {
        cout << "Failed to open file for writing.\n";
        return;
    }
    outFile << sectionCount << "\n";
    for (int i = 0; i < sectionCount; ++i) {
        outFile << song[i].name << "\n";
        outFile << song[i].chordCount << "\n";
        for (int j = 0; j < song[i].chordCount; ++j) {
            outFile << song[i].chords[j].text << "\n";
        }
    }
    outFile.close();
    cout << "Song saved to " << filename << "\n";
}

// Loads the song structure from a text file
int loadFromFile(Section song[], const string& filename) {
    ifstream inFile(filename.c_str());
    if (!inFile) {
        cout << "Failed to open file.\n";
        return 0;
    }
    int sectionCount;
    inFile >> sectionCount;
    inFile.ignore();
    for (int i = 0; i < sectionCount && i < MAX_SECTIONS; ++i) {
        getline(inFile, song[i].name);
        inFile >> song[i].chordCount;
        inFile.ignore();
        for (int j = 0; j < song[i].chordCount && j < MAX_CHORDS; ++j) {
            getline(inFile, song[i].chords[j].text);
        }
    }
    inFile.close();
    cout << "Song loaded from " << filename << "\n";
    return sectionCount;
}

int main() {
    cout << "--- Guitar Chord Formatter for Entire Song ---\n\n";

    Section song[MAX_SECTIONS];
    int sectionCount = 0;
    char choice;

    // Main menu loop
    do {
        cout << "\nChoose an action:\n";
        cout << "1. Add Section\n";
        cout << "2. Edit Section\n";
        cout << "3. View All Sections\n";
        cout << "4. Load Song from File\n";
        cout << "5. Save Song to File\n";
        cout << "6. Quit\n";
        cout << "> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case '1':
            if (sectionCount >= MAX_SECTIONS) {
                cout << "Maximum number of sections reached.\n";
                break;
            }
            cout << "Which part of the song are you typing chords for (e.g., Verse, Chorus, Bridge)?\n> ";
            getline(cin, song[sectionCount].name);
            song[sectionCount].chordCount = getValidChordCount();
            getChords(song[sectionCount]);
            sectionCount++;
            break;
        case '2':
            editSection(song, sectionCount);
            break;
        case '3':
            printFullSong(song, sectionCount);
            break;
        case '4': {
            string filename;
            cout << "Enter filename to load from: ";
            getline(cin, filename);
            sectionCount = loadFromFile(song, filename);
            break;
        }
        case '5': {
            string filename;
            cout << "Enter filename to save to: ";
            getline(cin, filename);
            saveToFile(song, sectionCount, filename);
            break;
        }
        case '6':
            cout << "\nExiting program. Final formatted song:\n";
            printFullSong(song, sectionCount);
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != '6');

    return 0;
}
