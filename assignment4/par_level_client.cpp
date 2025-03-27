#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <algorithm>
#include <curl/curl.h>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

bool debug = false;
const string SERVICE_URL = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";
const int MAX_THREADS = 8;

mutex visited_mutex;
mutex next_level_mutex;

struct ParseException : std::runtime_error, rapidjson::ParseResult {
    ParseException(rapidjson::ParseErrorCode code, const char* msg, size_t offset) : 
        std::runtime_error(msg), 
        rapidjson::ParseResult(code, offset) {}
};

string url_encode(CURL* curl, string input) {
    char* out = curl_easy_escape(curl, input.c_str(), input.size());
    string s = out;
    curl_free(out);
    return s;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

string fetch_neighbors(CURL* curl, const string& node) {
    string url = SERVICE_URL + url_encode(curl, node);
    string response;

    if (debug)
        cout << "Sending request to: " << url << endl;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "User-Agent: C++-Client/1.0");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        cerr << "CURL error: " << curl_easy_strerror(res) << endl;
    } else if (debug) {
        cout << "CURL request successful!" << endl;
    }

    curl_slist_free_all(headers);

    if (debug) 
        cout << "Response received: " << response << endl;

    return (res == CURLE_OK) ? response : "{}";
}

vector<string> get_neighbors(const string& json_str) {
    vector<string> neighbors;
    Document doc;
    doc.Parse(json_str.c_str());
    
    if (doc.HasParseError()) {
        throw ParseException(doc.GetParseError(), "JSON parse error", doc.GetErrorOffset());
    }
    
    if (doc.HasMember("neighbors") && doc["neighbors"].IsArray()) {
        for (const auto& neighbor : doc["neighbors"].GetArray()) {
            neighbors.push_back(neighbor.GetString());
        }
    }
    return neighbors;
}

void process_node(CURL* curl, const string& node, unordered_set<string>& visited, vector<string>& next_level) {
    try {
        if (debug)
            cout << "Processing node: " << node << endl;

        vector<string> neighbors = get_neighbors(fetch_neighbors(curl, node));

        for (const auto& neighbor : neighbors) {
            unique_lock<mutex> visited_lock(visited_mutex);
            if (!visited.count(neighbor)) {
                visited.insert(neighbor);
                visited_lock.unlock();
                
                unique_lock<mutex> next_level_lock(next_level_mutex);
                next_level.push_back(neighbor);
            }
        }
    } catch (const ParseException& e) {
        cerr << "Error while fetching neighbors of: " << node << endl;
        throw e;
    }
}

vector<vector<string>> parallel_bfs(CURL* curl, const string& start, int depth) {
    vector<vector<string>> levels;
    unordered_set<string> visited;
    
    levels.push_back({start});
    visited.insert(start);

    for (int d = 0; d < depth; d++) {
        if (debug)
            cout << "Starting level: " << d << " with " << levels[d].size() << " nodes" << endl;

        levels.push_back({});
        vector<string>& current_level = levels[d];
        vector<string>& next_level = levels[d+1];
        
        int num_nodes = current_level.size();
        int num_threads = min(num_nodes, MAX_THREADS);
        
        vector<thread> threads;
        int nodes_per_thread = (num_nodes + num_threads - 1) / num_threads;
        
        for (int t = 0; t < num_threads; t++) {
            int start_idx = t * nodes_per_thread;
            int end_idx = min((t + 1) * nodes_per_thread, num_nodes);
            
            threads.emplace_back([&, start_idx, end_idx]() {
                CURL* thread_curl = curl_easy_init();
                if (!thread_curl) {
                    cerr << "Failed to initialize CURL in thread" << endl;
                    return;
                }
                
                for (int i = start_idx; i < end_idx; i++) {
                    process_node(thread_curl, current_level[i], visited, next_level);
                }
                
                curl_easy_cleanup(thread_curl);
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
    }
    
    return levels;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <node_name> <depth>\n";
        return 1;
    }

    string start_node = argv[1];
    int depth;
    try {
        depth = stoi(argv[2]);
    } catch (const exception& e) {
        cerr << "Error: Depth must be an integer.\n";
        return 1;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize CURL" << endl;
        return -1;
    }

    const auto start = chrono::steady_clock::now();
    
    auto levels = parallel_bfs(curl, start_node, depth);
    
    const auto finish = chrono::steady_clock::now();
    const chrono::duration<double> elapsed_seconds = finish - start;
    
    for (const auto& level : levels) {
        for (const auto& node : level) {
            cout << "- " << node << "\n";
        }
        cout << "Level size: " << level.size() << "\n";
    }
    
    cout << "Time to crawl: " << elapsed_seconds.count() << "s\n";
    
    curl_easy_cleanup(curl);
    
    return 0;
}