#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <rapidjson/document.h>

// libcurl 回调函数，接收 API 数据
size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output) {
    size_t totalSize = size * nmemb;
    output->append((char *)contents, totalSize);
    return totalSize;
}

// URL 编码函数 (将 "Tom Hanks" 转换为 "Tom%20Hanks")
std::string url_encode(const std::string &value) {
    std::ostringstream encoded;
    for (char c : value) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            encoded << '%' << std::uppercase << std::hex << int((unsigned char)c);
        }
    }
    return encoded.str();
}

// 发送 HTTP 请求，获取邻居节点 JSON 数据
std::string fetch_neighbors(const std::string &node) {
    CURL *curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        std::string encoded_node = url_encode(node);  // **URL 编码**
        std::string url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/" + encoded_node;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    std::cout << "API Response for " << node << ": " << response << std::endl;  // **打印 API 返回数据**
    return response;
}

// 解析 JSON 数据，获取邻居节点
std::vector<std::string> parse_neighbors(const std::string &jsonStr) {
    std::vector<std::string> neighbors;
    rapidjson::Document doc;

    std::cout << "Parsing JSON: " << jsonStr << std::endl;  // **打印 JSON 数据**
    
    doc.Parse(jsonStr.c_str());

    if (doc.HasMember("neighbors") && doc["neighbors"].IsArray()) {
        for (auto &v : doc["neighbors"].GetArray()) {
            neighbors.push_back(v.GetString());
        }
    }

    return neighbors;
}

// 实现广度优先搜索 (BFS)
void bfs(const std::string &startNode, int maxDepth) {
    std::queue<std::pair<std::string, int>> q;
    std::unordered_set<std::string> visited;

    q.push({startNode, 0});
    visited.insert(startNode);

    while (!q.empty()) {
        auto [node, depth] = q.front();
        q.pop();

        std::cout << "Visiting: " << node << " at depth " << depth << std::endl;  // **打印当前访问的节点**

        if (depth >= maxDepth) continue;

        std::string jsonStr = fetch_neighbors(node);
        std::vector<std::string> neighbors = parse_neighbors(jsonStr);

        for (const std::string &neighbor : neighbors) {
            if (!visited.count(neighbor)) {
                visited.insert(neighbor);
                q.push({neighbor, depth + 1});
            }
        }
    }

    std::cout << "Final visited nodes:" << std::endl;
    for (const auto &node : visited) {
        std::cout << node << std::endl;
    }
}

// 主函数，处理输入参数并测量执行时间
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <startNode> <depth>" << std::endl;
        return 1;
    }

    std::string startNode = argv[1];
    int depth = std::stoi(argv[2]);

    auto start = std::chrono::high_resolution_clock::now();
    bfs(startNode, depth);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution Time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
