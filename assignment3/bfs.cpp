#include <iostream>
#include <queue>
#include <unordered_set>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {

        return 0;
    }
    return newLength;
}

std::string encode_url(const std::string& node) {
    std::string encoded_node;
    for (char ch : node) {
        if (ch == ' ') {
            encoded_node += "%20";
        } else {
            encoded_node += ch;
        }
    }
    return encoded_node;
}

std::unordered_set<std::string> get_neighbors(const std::string& node) {
    std::unordered_set<std::string> neighbors;
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/" + encode_url(node);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {

            std::cout << "API Response: " << response << std::endl;

            rapidjson::Document doc;
            doc.Parse(response.c_str());
            if (doc.HasParseError()) {
                std::cerr << "JSON Parse Error: " << doc.GetParseError() << std::endl;
                return neighbors;
            }
            if (doc.HasMember("error")) {
                std::cerr << "API Error: " << doc["error"].GetString() << std::endl;
                return neighbors;
            }
            if (doc.HasMember("neighbors")) {
                const rapidjson::Value& neighborsArray = doc["neighbors"];
                for (rapidjson::SizeType i = 0; i < neighborsArray.Size(); i++) {
                    neighbors.insert(neighborsArray[i].GetString());
                }
            }
        } else {
            std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return neighbors;
}

std::unordered_set<std::string> bfs(const std::string& start_node, int depth) {
    std::unordered_set<std::string> visited;
    std::queue<std::pair<std::string, int>> queue;
    queue.push({start_node, 0});
    visited.insert(start_node);

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        std::string node = current.first;
        int current_depth = current.second;

        if (current_depth < depth) {
            for (const auto& neighbor : get_neighbors(node)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    queue.push({neighbor, current_depth + 1});
                }
            }
        }
    }

    return visited;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <start_node> <depth>" << std::endl;
        return 1;
    }

    std::string start_node = argv[1];
    int depth = std::stoi(argv[2]);

    auto result = bfs(start_node, depth);

    for (const auto& node : result) {
        std::cout << node << std::endl;
    }

    return 0;
}