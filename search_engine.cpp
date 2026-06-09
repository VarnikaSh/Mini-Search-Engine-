/*
=============================================
            MINI SEARCH ENGINE
=============================================

Features:
- Inverted Index(unordered_map)
- Trie for Autocomplete
- DFS Traversal
- TF-IDF Ranking
- Top-K Results using Priority Queue(Heap)
- Boolean Queries(AND/OR/NOT)

Compile:
g++ search_engine.cpp -o search_emgine

Run:
./search_engine
=============================================
*/

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <cmath>
#include <sstream>
#include <algorithm>

using namespace std;

//=============================================
// DOCUMENTS
//=============================================

unordered_map<int, string> documents = {
    {1, "google search engine uses inverted index"},
    {2, "amazon search supports autocomplete using trie"},
    {3, "search engine ranking with tf idf algorithm"},
    {4, "google and amazon both use search systems"},
    {5, "trie data structure supports prefix searching"},
    {6, "hashmap and heap are important data structures"}
};

//=============================================
// TOKENIZER
//=============================================

vector<string> tokenize(string text)
{
    vector<string> words;
    stringstream ss(text);
    string word;

    while(ss >> word)
    {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }
    return words;
}

//=============================================
// TRIE
//=============================================

class TrieNode
{
    public:
    unordered_map<char, TrieNode*> child;
    bool isEnd;

    TrieNode()
    {
        isEnd = false;
    }
};

class Trie
{
    private:
    TrieNode* root;
    void DFS(TrieNode* node, string current, vector<string>& result)
    {
        if(node->isEnd)
        result.push_back(current);

        for(auto x : node->child)
        {
            DFS(x.second, current + x.first, result);
        }
    }

    public:
    Trie()
    {
        root = new TrieNode();
    }

    void insert(string word)
    {
        TrieNode* curr = root;
        for(char c : word)
        {
            if(curr->child.find(c) == curr->child.end())
            curr->child[c] = new TrieNode();

            curr = curr->child[c];
        }
        curr->isEnd = true;
    }

    vector<string> autocomplete(string prefix)
    {
        TrieNode* curr = root;
        for(char c : prefix)
        {
            if(curr->child.find(c) == curr->child.end())
            return {};

            curr = curr->child[c];
        }

        vector<string> result;
        DFS(curr, prefix, result);

        return result;
    }
};

//==============================================
// SEARCH ENGINE
//==============================================

class SearchEngine
{
    private:
    unordered_map<string, unordered_set<int>> invertedIndex;
    Trie trie;
    int totalDocs;

    public:
    SearchEngine()
    {
        totalDocs = documents.size();
        buildIndex();
    }

    //------------------------------------------

    void buildIndex()
    {
        unordered_set<string> uniqueWords;
        for(auto doc : documents)
        {
            vector<string> words = tokenize(doc.second);
            for(string word : words)
            {
                invertedIndex[word].insert(doc.first);
                uniqueWords.insert(word);
            }
        }
        for(string word : uniqueWords)
        trie.insert(word);
    }

    //-----------------------------------------

    void showIndex()
    {
        cout << "\n===== INVERTED INDEX ====\n\n";

        for(auto item : invertedIndex)
        {
            cout << item.first << "->";
            for(int id : item.second)
               cout << id << " ";
            
            cout << endl;   

        }
    }

    void autocomplete()
    {
        string prefix;

        cout << "\nEnter Prefix: ";
        cin >> prefix;

        vector<string> words = trie.autocomplete(prefix);

        if(words.empty())
        {
            cout << "\nNo Suggestions Found.\n";
            return;
        }
        cout << "\nSuggestions:\n";

        for(string w : words)
          cout << w << endl;
    }

    //------------------------------------------

    unordered_map<int, double> tfidf(string query)
    {
        unordered_map<int, double> score;
        vector<string> queryWords = tokenize(query);
        for(string word : queryWords)
        {
            if(invertedIndex.find(word) == invertedIndex.end())
               continue;
            
            int df = invertedIndex[word].size();
            double idf = log((double)totalDocs / df);
            for(int docID : invertedIndex[word])
            {
                vector<string> words = tokenize(documents[docID]);
                int count = 0;
                for(string w : words)
                  if(w == word)
                    count++;
                double tf = (double)count / words.size();
                score[docID] += tf * idf;    
            }
        }
        return score;
    }

    void search()
    {
        cin.ignore();
        string query;
        cout << "\nEnter Search Query: ";
        getline(cin, query);
        auto scores = tfidf(query);
        priority_queue<pair<double, int>> pq;
        for(auto x : scores)
        {
            pq.push({x.second, x.first});
        }
        if(pq.empty())
        {
            cout << "\nNo Results Found.\n";
            return;
        }
        cout << "\n==== SEARCH RESULTS ====\n\n";

        while(!pq.empty())
        {
            auto top = pq.top();
            pq.pop();

            cout << "Document ID : " << top.second << endl;
            cout << "TF-IDF Score : " << top.first << endl;
            cout << "Content    : " << documents[top.second] << endl;
            cout << "-----------------------------\n";
        }
    }
    
    //----------------------------------------------

    void booleanQuery()
    {
        cin.ignore();
        string query;
        cout << "\nExamples:\n";
        cout << "google AND search\n";
        cout << "google OR amazon\n";
        cout << "NOT google\n";

        cout << "Enter Query: ";
        getline(cin, query);

        stringstream ss(query);

        vector<string> token;
        string x;

        while(ss >> x)
          token.push_back(x);
        
        unordered_set<int> result;
        
        if(token.size() == 2 && token[0] == "NOT")
        {
            unordered_set<int> allDocs;
            for(auto d : documents)
               allDocs.insert(d.first);
            
            result = allDocs;

            for(int id : invertedIndex[token[1]])
              result.erase(id);
        }
        else if(token.size() == 3)
        {
            string left = token[0];
            string op = token[1];
            string right = token[2];

            if(op == "AND")
            {
                for(int id : invertedIndex[left])
                {
                    if(invertedIndex[right].count(id))
                       result.insert(id);
                }
            }

            else if(op == "OR")
            {
                result = invertedIndex[left];
                for(int id : invertedIndex[right])
                   result.insert(id);
            }
        }
        else
        {
            result = invertedIndex[query];
        }

        cout << "\nMatching Documents:\n\n";
        if(result.empty())
        {
            cout << "No Match Found.\n";
            return;
        }

        for(int id : result)
        {
            cout << "[" << id << "]"
                 << documents[id] << endl;
        }
    }
};

//==================================================
// MAIN
//==================================================

int main()
{
    SearchEngine engine;
    int choice;

    while(true)
    {
        cout << "\n===============================\n";
        cout << "       MINI SEARCH ENGINE\n ";
        cout << "===============================\n";

        cout << "1. Search using TF-IDF\n";
        cout << "2. Autocomplete\n";
        cout << "3. Boolean Query\n";
        cout << "4. Show Inverted Index\n";
        cout << "5. Exit\n";

        cout << "\nEnter choice: ";
        cin >> choice;

        switch(choice)
        {
            case 1:
               engine.search();
               break;
            
            case 2:
               engine.autocomplete();
               break;
            
            case 3:
               engine.booleanQuery();
               break;
            
            case 4:
               engine.showIndex();
               break;
               
            case 5:
               cout << "\nThank You!\n";
               return 0;
            
            default:
               cout << "\nInvalid Choice!\n";   
        }
    }
    return 0;
};