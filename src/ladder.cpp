#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cctype>


bool is_adjacent(const std::string &word1, const std::string &word2) {
    int len1 = (int) word1.size();
    int len2 = (int) word2.size();
  
    if (std::abs(len1 - len2) > 1) {
        return false;
    }

   
    if (len1 == len2) {
        int diffCount = 0;
        for (int i = 0; i < len1; ++i) {
            if (word1[i] != word2[i]) {
                diffCount++;
                if (diffCount > 1) return false;
            }
        }
        return (diffCount == 1);
    }


    const std::string &longer  = (len1 > len2) ? word1 : word2;
    const std::string &shorter = (len1 > len2) ? word2 : word1;

    int i = 0, j = 0; 
    bool foundDifference = false;
    while (i < (int)longer.size() && j < (int)shorter.size()) {
        if (longer[i] != shorter[j]) {
            if (foundDifference) {
                return false; 
            }
            foundDifference = true;
            i++;
        } else {
            i++;
            j++;
        }
    }

    return true;
}


void load_words(std::set<std::string> &word_list, const std::string &file_name) {
    std::ifstream in(file_name);
    if (!in) {
        std::cerr << "Error: Could not open file '" << file_name << "'\n";
        return;
    }
    std::string w;
    while (in >> w) {
    
        for (char &c : w) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
        word_list.insert(w);
    }
    in.close();
}


std::vector<std::string> generate_word_ladder(const std::string &begin_word,
                                              const std::string &end_word,
                                              const std::set<std::string> &word_list)
{
    
    if (begin_word == end_word) {
        
        return {};
    }

  
    std::queue<std::vector<std::string>> ladder_queue;
 
    ladder_queue.push({begin_word});

 
    std::unordered_set<std::string> visited;
    visited.insert(begin_word);

   
    while (!ladder_queue.empty()) {
   
        std::vector<std::string> current_ladder = ladder_queue.front();
        ladder_queue.pop();

      
        const std::string &last_word = current_ladder.back();

       
        for (auto &candidate : word_list) {
        
            if (visited.find(candidate) == visited.end() && is_adjacent(last_word, candidate)) {
             
                std::vector<std::string> new_ladder = current_ladder;
                new_ladder.push_back(candidate);

               
                visited.insert(candidate);

               
                if (candidate == end_word) {
                    return new_ladder;
                }

                
                ladder_queue.push(new_ladder);
            }
        }
    }

   
    return {};
}


void print_word_ladder(const std::vector<std::string> &ladder) {
    if (ladder.empty()) {
        std::cout << "No ladder found\n";
        return;
    }
    for (size_t i = 0; i < ladder.size(); i++) {
        std::cout << ladder[i];
        if (i + 1 < ladder.size()) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";
}


void verify_word_ladder() {
    std::set<std::string> word_list;
    load_words(word_list, "words.txt");

    
    std::vector<std::string> ladder = generate_word_ladder("cat", "dog", word_list);
    std::cout << "Ladder from 'cat' to 'dog':\n";
    print_word_ladder(ladder);

}
