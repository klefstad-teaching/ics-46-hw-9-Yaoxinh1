#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <cctype>    
#include <algorithm>  
#include <cmath>     

using namespace std;


void error(string word1, string word2, string msg) {
    cerr << "ERROR with words \"" << word1 << "\" and \"" << word2 
         << "\": " << msg << endl;
}

static bool edit_distance_recursive(const string &s1, const string &s2, int d) {
    if (d < 0) return false;

   
    if (s1.empty() || s2.empty()) {
       
        return (int)std::abs((int)s1.size() - (int)s2.size()) <= d;
    }

 
    if (s1[0] == s2[0]) {
        return edit_distance_recursive(s1.substr(1), s2.substr(1), d);
    } else {

        return  edit_distance_recursive(s1.substr(1), s2,     d - 1) ||
                edit_distance_recursive(s1,         s2.substr(1), d - 1) ||
                edit_distance_recursive(s1.substr(1), s2.substr(1), d - 1);
    }
}


bool edit_distance_within(const string& str1, const string& str2, int d) {
    return edit_distance_recursive(str1, str2, d);
}


bool is_adjacent(const string& word1, const string& word2) {
    int len1 = (int)word1.size();
    int len2 = (int)word2.size();

    // If lengths differ by more than 1, can't be adjacent
    if (abs(len1 - len2) > 1) return false;

    // Case A: same length => up to 1 mismatch
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                diffCount++;
                if (diffCount > 1) return false;
            }
        }
        return true; // if diffCount <= 1 => distance <= 1
    }

    // Case B: differ by exactly 1 => insertion or deletion
    const string &longer  = (len1 > len2 ? word1 : word2);
    const string &shorter = (len1 > len2 ? word2 : word1);
    int i = 0, j = 0;
    bool foundDiff = false;
    while (i < (int)longer.size() && j < (int)shorter.size()) {
        if (longer[i] != shorter[j]) {
            if (foundDiff) {
                return false; 
            }
            foundDiff = true;
            i++;
        } else {
            i++;
            j++;
        }
    }
    // If we exit the loop, at most one mismatch => distance <= 1
    return true;
}


void load_words(set<string> & word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Failed to open " << file_name << endl;
        return;
    }
    string w;
    while (in >> w) {
        for (char &c : w) {
            c = static_cast<char>(tolower((unsigned char)c));
        }
        word_list.insert(w);
    }
    in.close();
}


vector<string> generate_word_ladder(const string& begin_word,
                                    const string& end_word,
                                    const set<string>& word_list)
{

    if (begin_word == end_word) {
        return { begin_word };
    }

    // BFS
    queue<vector<string>> paths;
    paths.push({ begin_word });

    // visited set
    set<string> visited;
    visited.insert(begin_word);

    while (!paths.empty()) {
        auto curr_path = paths.front();
        paths.pop();

        const string &last_word = curr_path.back();

        // Expand BFS by checking adjacency to all dictionary words
        for (const auto &candidate : word_list) {
            if (visited.find(candidate) == visited.end() &&
                is_adjacent(last_word, candidate))
            {
                visited.insert(candidate); // mark visited

                auto new_path = curr_path;
                new_path.push_back(candidate);

                if (candidate == end_word) {
                    return new_path; // Found a shortest path
                }

                paths.push(new_path);
            }
        }
    }

    // If BFS ends with no solution
    return {};
}


void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found.\n";
        return;
    }
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << " ";
    }
    cout << "\n";
}


void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    // Example 1: "apple"->"apple"
    auto ladder1 = generate_word_ladder("apple", "apple", word_list);
    print_word_ladder(ladder1);

    // Example 2: "cat"->"dog"
    auto ladder2 = generate_word_ladder("cat", "dog", word_list);
    print_word_ladder(ladder2);
}
