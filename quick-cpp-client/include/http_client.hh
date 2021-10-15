#include <nlohmann/json.hpp>
#include <string>
#include <optional>
#include <cpprest/http_client.h>


namespace quick_client {
    namespace http {
        using json = nlohmann::json;
        using namespace std;
        json http_get(string url, unordered_map<string, string>);
        json http_post();
        json http_patch();
        json http_put();
        json http_delete();
        json primitive_requester();

        struct request_options {
            optional<web::http::methods> method,
            optional<unordered_map<string, string>> headers,
            optional<unordered_map<string, string>> query,
            optional<string> body,
            optional<string> url,
        };

    };
};