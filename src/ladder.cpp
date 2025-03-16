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


bool edit_distance_within(const string& str1, const string& str2, int d) {
    int n = (int)str1.size();
    int m = (int)str2.size();
    if (abs(n - m) > d) {
        return false;
    }
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    for (int i = 0; i <= n; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= n; i++) {
        int rowMin = dp[i][0];
        for (int j = 1; j <= m; j++) {
            int cost = (str1[i-1] == str2[j-1]) ? 0 : 1;
            dp[i][j] = min({
                dp[i-1][j] + 1,     
                dp[i][j-1] + 1,     
                dp[i-1][j-1] + cost 
            });
            rowMin = min(rowMin, dp[i][j]);
        }
        if (rowMin > d) {
            return false;
        }
    }
    return (dp[n][m] <= d);
}


bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
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
            c = (char)tolower((unsigned char)c);
        }
        word_list.insert(w);
    }
    in.close();
}


static vector<string> get_neighbors(const string &word,
                                    const set<string> &word_list)
{
    vector<string> neighbors;
    neighbors.reserve(64);

    // (1) Substitutions
    for (int i = 0; i < (int)word.size(); i++) {
        char original = word[i];
        for (char c = 'a'; c <= 'z'; c++) {
            if (c == original) continue;
            string new_word = word;
            new_word[i] = c;
            if (word_list.count(new_word)) {
                neighbors.push_back(new_word);
            }
        }
    }
    // (2) Insertions
    for (int i = 0; i <= (int)word.size(); i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            string new_word = word.substr(0,i) + c + word.substr(i);
            if (word_list.count(new_word)) {
                neighbors.push_back(new_word);
            }
        }
    }
    // (3) Deletions
    for (int i = 0; i < (int)word.size(); i++) {
        string new_word = word.substr(0, i) + word.substr(i+1);
        if (word_list.count(new_word)) {
            neighbors.push_back(new_word);
        }
    }
    // Sort + unique so BFS always expands neighbors in alphabetical order
    sort(neighbors.begin(), neighbors.end());
    neighbors.erase(unique(neighbors.begin(), neighbors.end()), neighbors.end());
    return neighbors;
}


vector<string> generate_word_ladder(const string& begin_word,
                                    const string& end_word,
                                    const set<string>& word_list)
{
    // Fix #1: If identical, return single-word ladder immediately
    if (begin_word == end_word) {
        return { begin_word };
    }

    // BFS queue of partial paths
    queue<vector<string>> paths;
    paths.push({ begin_word });

    // visited
    set<string> visited;
    visited.insert(begin_word);

    while (!paths.empty()) {
        auto curr_path = paths.front();
        paths.pop();
        const string &last = curr_path.back();

        // Generate neighbors in alphabetical order
        auto nbrs = get_neighbors(last, word_list);
        for (auto &candidate : nbrs) {
            if (!visited.count(candidate)) {
                visited.insert(candidate);
                // Extend the path
                vector<string> new_path = curr_path;
                new_path.push_back(candidate);
                if (candidate == end_word) {
                    return new_path; // Found the ladder
                }
                paths.push(new_path);
            }
        }
    }
    // No path found
    return {};
}


void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found.\n";
        return;
    }
    cout << "Word ladder found: ";
    for (const string &w : ladder) {
        cout << w << " ";
    }
    cout << "\n";
}


void verify_word_ladder() {
    set<string> dict;
    load_words(dict, "words.txt");

    // #1 "were" -> "were" should return a ladder of size=1: ["were"]
    auto ladderA = generate_word_ladder("were", "were", dict);
    print_word_ladder(ladderA);

    // #2 "awake" -> "sleep" might follow the path from the test 
    auto ladderB = generate_word_ladder("awake", "sleep", dict);
    print_word_ladder(ladderB);
}
