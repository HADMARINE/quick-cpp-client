#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <queue>
#include <cpprest/http_client.h>
#include <cpprest/http_headers.h>

namespace quick_client {
    namespace web_http {
        using namespace std;
        using namespace web;
        using namespace web::http;
        using namespace web::http::client;
        using namespace concurrency;

        using TOKEN = uint32_t;

#define MAX_TOKENS 1000

        typedef enum request_method {
            POST        = 0,
            GET         = 1,
            PATCH       = 2,
            PUT         = 3,
            DELETE      = 4
        } request_method;

        // ��û�� �����ϱ� ���� ������
        struct request_config {
            utility::string_t uri;
            request_method method;
            vector<pair<utility::string_t, utility::string_t>> headers;
            vector<pair<utility::string_t, utility::string_t>> query;
            json::value body;
        };

        class http_instance {
        public:
            http_instance(utility::string_t url);
            http_instance(utility::string_t url, vector<pair<utility::string_t, utility::string_t>> preset_headers);
            ~http_instance();

            // ���� ��û���� �Ϸ�� �� ���� ����Ѵ�. (���α׷��� ���� �� ����)
            void check_requesters();

            // config �־��ָ� �־��� ������ request�� �����Ѵ�.
            TOKEN requester(request_config config);

            // token���� ���� �ش� ��û�� ����Ǿ����� Ȯ���Ѵ�.
            bool is_request_end(TOKEN token);

            // ��û�� ���� token���� ���� ���Ź��� ���� �����´�. ���� ��ū�� �ı��Ѵ�
            json::value get_recived_value(TOKEN token);

        private:
            utility::string_t baseurl;
            vector<pair<utility::string_t, utility::string_t>> preset_headers;

            // baseurl�� ������ http Ŭ���̾�Ʈ ��ü
            http_client client;

            // ��ū �߱� ���� ����
            uint32_t        living_token_count;
            queue<TOKEN>    available_tokens;

            //map<TOKEN, >
            map<TOKEN, pplx::task<http_response>> tasks;
            cancellation_token_source cts;
        };
    };
};