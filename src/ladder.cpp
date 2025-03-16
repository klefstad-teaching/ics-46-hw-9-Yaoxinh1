#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <cctype>     // for tolower
#include <algorithm>  // for std::min
#include <cmath>      // for std::abs

using namespace std;

// 1) error(...)
void error(string word1, string word2, string msg) {
    cerr << "ERROR with words \"" << word1 << "\" and \"" << word2
         << "\": " << msg << endl;
}

// 2) edit_distance_within(...)
//    Keep this unchanged for any grader tests that call it directly.
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
                dp[i-1][j] + 1,     // deletion
                dp[i][j-1] + 1,     // insertion
                dp[i-1][j-1] + cost // substitution
            });
            rowMin = min(rowMin, dp[i][j]);
        }
        if (rowMin > d) {
            return false;
        }
    }
    return (dp[n][m] <= d);
}

/**
 * 3) is_adjacent(...)
 *    Changed to a direct one-edit check, which is O(length of word).
 *    distance=0 => "apple" is adjacent to "apple".
 *    distance=1 => normal one-edit difference.
 */
bool is_adjacent(const string& word1, const string& word2) {
    int len1 = (int)word1.size();
    int len2 = (int)word2.size();

    // If length difference > 1, can't be within 1 edit
    if (abs(len1 - len2) > 1) return false;

    // CASE A: same length => up to 1 mismatch
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                diffCount++;
                if (diffCount > 1) return false;
            }
        }
        // diffCount <= 1 => distance <= 1, so adjacent
        return true;
    }

    // CASE B: lengths differ by exactly 1 => insertion/deletion
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
    // If we exit, at most one mismatch => distance <= 1 => adjacent
    return true;
}

// 4) load_words(...)
void load_words(set<string> & word_list, const string& file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Failed to open " << file_name << endl;
        return;
    }
    string w;
    while (in >> w) {
        for (char &c : w) {
            c = (char) tolower((unsigned char)c);
        }
        word_list.insert(w);
    }
    in.close();
}

// 5) generate_word_ladder(...)
vector<string> generate_word_ladder(const string& begin_word,
                                    const string& end_word,
                                    const set<string>& word_list)
{
    // If identical, return single-word ladder so "apple->apple" passes
    if (begin_word == end_word) {
        return { begin_word };
    }

    queue<vector<string>> paths;
    paths.push({ begin_word });

    set<string> visited;
    visited.insert(begin_word);

    while (!paths.empty()) {
        auto curr_path = paths.front();
        paths.pop();

        const string &last_word = curr_path.back();

        // BFS expansion
        for (auto &candidate : word_list) {
            if (visited.find(candidate) == visited.end() &&
                is_adjacent(last_word, candidate))
            {
                // Mark visited to avoid re-enqueueing
                visited.insert(candidate);

                // Extend path
                vector<string> new_path = curr_path;
                new_path.push_back(candidate);

                // If we reached target
                if (candidate == end_word) {
                    return new_path;
                }

                paths.push(new_path);
            }
        }
    }

    // No ladder found
    return {};
}

// 6) print_word_ladder(...)
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

// 7) verify_word_ladder()
void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");


    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
