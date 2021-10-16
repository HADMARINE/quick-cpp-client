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

            json get(string url, request_config config);
            json post(string url, request_config config);
            json patch(string url, request_config config);
            json put(string url, request_config config);
            json delete(string url, request_config config);
            json primitive_requester(request_config config);
        }

        struct request_config {
            optional<web::http::methods> method,
            optional<unordered_map<string, json>> headers,
            optional<unordered_map<string, json>> query,
            optional<json> body,
            string url,
        };

    };
};