# Mini Search Engine

## Overview
This project is a C++ implementation of a basic search engine that demonstrates key Information Retrieval and Data Structures concepts. It supports document indexing, autocomplete suggestions, TF-IDF based ranking, and Boolean query processsing.

## Features
- Inverted Index for efficient document retrieval
- Trie-based Autocomplete
- TF-IDF Ranking for relevance scoring
- Boolean Query Support(AND, OR, NOT)
- Top-K Result Retrieval using Priority Queue(Haep)

## Technologies & Data Structures
- C++
- unordered_map(Hash Map)
- unordered_set
- Trie
- DFS Traversal
- Priority Queue(Heap)

 ## Compilation & Execution
 g++ search_engine.cpp -o search_engine  
 ./search_engine

 ## Sample Queries
 ### Search Query
 google search
 ### Boolean Queries
 google AND search  
 google OR amazon  
 NOT google
 ### Autocomplete
 **Prefix**: se  
 **Output**: search, searching

 ## Time Complexity
 | Operation | Complexity |
 | :--- | :--- |
 | Build Inverted Index | $O(T)$ |
 | Autocomplete | $O(P + K)$ |
 | TF-IDF Search | $O(M \times D)$ |
 | Heap Ranking | $O(R \log R)$ |
 
 **Where:**
 - **T** = Total words
 - **P** = Prefix length
 - **K** = Suggestions returned
 - **M** = Query terms
 - **D** = Matching documents
 - **R** = Ranked results

## Applications
- Search Engines
- E-commerce Search Systems
- Document Retrieval Platforms
- Digital Libraries

## Future Enhancements
- Phrase search
- Wildcard queries
- Web crawling
- PageRank integration
- Spell correction
- Stemming and lemmmatization
- Persistent storage using files or databases
- REST API support
- GUI/Web interface

## Conclusion
This project demonstrates how core data structures such as Hash Maps, Tries, and Heaps can be combined to build an efficient search engine capable of indexing, ranking, and retrieving relevant documents.

## Author
Varnika Shukla
