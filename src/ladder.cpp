#include "ladder.h"
#include <queue>
#include <iostream>
#include <unordered_set>
#include <cctype>
#include <algorithm>
using namespace std;


bool is_adjacent(const string& word1, const string& word2) 
{
  
    int len1 = (int)word1.size();
    int len2 = (int)word2.size();
    if (abs(len1 - len2) > 1) return false;

   
    if (len1 == len2) {
        int countDiff = 0;
        for (int i = 0; i < len1; i++) {
            if (word1[i] != word2[i]) {
                countDiff++;
                if (countDiff > 1) return false;
            }
        }
        return (countDiff == 1);
    }


    const string& longer = (len1 > len2) ? word1 : word2;
    const string& shorter= (len1 > len2) ? word2 : word1;
    int i = 0, j = 0;
    bool foundDifference = false;
    while (i < (int)longer.size() && j < (int)shorter.size()) {
        if (longer[i] != shorter[j]) {
            if (foundDifference) return false; 
            foundDifference = true;
            i++;
        } else {
            i++;
            j++;
        }
    }
    
    return true;
}


void load_words(set<string>& dict, const string& filename)
{
    ifstream in(filename);
    if (!in) {
        cerr << "Could not open " << filename << "\n";
        return;
    }
    string w;
    while (in >> w) {
       
        for (char &c : w) {
            c = tolower(static_cast<unsigned char>(c));
        }
        dict.insert(w);
    }
    in.close();
}


vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list)
{
    
    if (begin_word == end_word) {
        return {}; 
    }


    queue<vector<string>> ladder_queue;

    unordered_set<string> visited;

    ladder_queue.push({begin_word});
    visited.insert(begin_word);

   
    while (!ladder_queue.empty()) {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();

      
        const string& last_word = ladder.back();

        
        for (auto &w : word_list) {
            if (visited.find(w) == visited.end() && is_adjacent(last_word, w)) {
            
                vector<string> new_ladder = ladder;
                new_ladder.push_back(w);
                visited.insert(w);

                
                if (w == end_word) {
                    return new_ladder;
                }
              
                ladder_queue.push(new_ladder);
            }
        }
    }


    return {};
}
