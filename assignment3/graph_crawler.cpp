#include <iostream>
#include <queue>
#include <unordered_set>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <rapidjson/document.h>

using namespace std;

//Function to handle HTTP response
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

//Function to get neighbors from API
vector<string> getNeighbors(string node) {
    CURL* curl = curl_easy_init();
    if (!curl) return {};

    //Correctly replace spaces with "%20", learned from the discussion board
    size_t pos = 0;
    while ((pos = node.find(' ', pos)) != string::npos) {
        node.replace(pos, 1, "%20");
        pos += 3;  // Move past the replaced "%20"
    }

    string url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/" + node;
    string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) return {};  //If API call fails, return empty list

    //Parse JSON response
    rapidjson::Document doc;
    doc.Parse(response.c_str());

    vector<string> neighbors;
    if (doc.HasMember("neighbors") && doc["neighbors"].IsArray()) {
        for (const auto& neighbor : doc["neighbors"].GetArray()) {
            neighbors.push_back(neighbor.GetString());
        }
    }
    return neighbors;
}

//BFS traversal
vector<string> bfs(const string& startNode, int maxDepth) {
    queue<pair<string, int>> q;
    unordered_set<string> visited;  //Store visited nodes
    vector<string> result;  //Store the results

    q.push(make_pair(startNode, 0));
    visited.insert(startNode);

    while (!q.empty()) {
        pair<string, int> current = q.front();
        string node = current.first;
        int depth = current.second;
        q.pop();

        if (depth > maxDepth) continue;
        result.push_back(node);

        vector<string> neighbors = getNeighbors(node);
        for (const string& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(make_pair(neighbor, depth + 1));
            }
        }
    }
    return result;
}

//Main function
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <start_node> <depth>" << endl;
        return 1;
    }

    string startNode = argv[1];
    int depth = stoi(argv[2]);

    vector<string> nodes = bfs(startNode, depth);

    cout << "Nodes found:" << endl;
    for (const string& node : nodes) {
        cout << node << endl;
    }

    return 0;
}
