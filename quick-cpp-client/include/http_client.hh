#include <nlohmann/json.hpp>
#include <string>
#include <optional>
#include <cpprest/http_client.h>


namespace quick_client {
    namespace http {
        using namespace std;
        using json = nlohmann::json;
        
        class http_instance {
            string baseurl;
            unordered_map<string, json> preset_headers;

            http_instance(string url, unordered_map<string, json> preset_headers);
            ~http_instance();

            json requester(request_config config);
        };

        struct request_config {
            string url,
            optional<web::http::methods> method,
            optional<unordered_map<string, json>> headers,
            optional<unordered_map<string, json>> query,
            optional<json> body,
        };
    };
};