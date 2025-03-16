#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>    
#include <cctype>     
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
    int len1 = (int)word1.size();
    int len2 = (int)word2.size();


    if (abs(len1 - len2) > 1) {
        return false;
    }


    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                diffCount++;
                if (diffCount > 1) return false;
            }
        }
        return (diffCount == 1);
    }


    const string &longer  = (len1 > len2) ? word1 : word2;
    const string &shorter = (len1 > len2) ? word2 : word1;
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
   
    return true;
}


void load_words(set<string> &word_list, const string &file_name) {
    ifstream in(file_name);
    if (!in) {
        cerr << "Failed to open file: " << file_name << endl;
        return;
    }
    string w;
    while (in >> w) {
       
        for (char &c : w) {
            c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        }
        word_list.insert(w);
    }
    in.close();
}


vector<string> generate_word_ladder(const string &begin_word,
                                    const string &end_word,
                                    const set<string> &word_list)
{
    
    if (begin_word == end_word) {
        return {};
    }

   
    queue<vector<string>> ladder_queue;
    ladder_queue.push({ begin_word });

  
    set<string> visited;
    visited.insert(begin_word);


    while (!ladder_queue.empty()) {
       
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();

  
        const string &last_word = current_ladder.back();

      
        for (const auto &dict_word : word_list) {
            if (visited.find(dict_word) == visited.end()
                && is_adjacent(last_word, dict_word))
            {
             
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(dict_word);

             
                visited.insert(dict_word);

              
                if (dict_word == end_word) {
                    return new_ladder;
                }

                
                ladder_queue.push(new_ladder);
            }
        }
    }
  
    return {};
}


void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No ladder found" << endl;
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        if (i + 1 < ladder.size()) {
            cout << " -> ";
        }
    }
    cout << endl;
}


void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");

    {
        auto ladder = generate_word_ladder("cat", "dog", word_list);
        cout << "Ladder from cat to dog:\n";
        print_word_ladder(ladder);
    }
    {
        auto ladder = generate_word_ladder("work", "play", word_list);
        cout << "Ladder from work to play:\n";
        print_word_ladder(ladder);
    }
   
}
