#include "ladder.h"     
#include <iostream>
#include <string>
#include <cctype>      
using namespace std;

int main() {
   
    set<string> dict;
    load_words(dict, "words.txt");  

  
    cout << "Enter the start word: ";
    string start;
    cin >> start;

    cout << "Enter the end word: ";
    string end;
    cin >> end;

    
    for (char &c : start) {
        c = tolower(static_cast<unsigned char>(c));
    }
    for (char &c : end) {
        c = tolower(static_cast<unsigned char>(c));
    }


    vector<string> ladder = generate_word_ladder(start, end, dict);

  
    if (ladder.empty()) {
        cout << "No ladder found.\n";
    } else {
        cout << "Ladder from " << start << " to " << end << ":\n";
        for (size_t i = 0; i < ladder.size(); i++) {
            cout << ladder[i];
            if (i + 1 < ladder.size()) {
                cout << " -> ";
            }
        }
      
        cout << "\nLength: " << ladder.size() << " words\n";
    }

    return 0;
}
