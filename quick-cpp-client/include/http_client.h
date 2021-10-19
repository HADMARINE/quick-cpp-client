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

        // 요청을 진행하기 위한 설정값
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

            // 보낸 요청들이 완료될 때 까지 대기한다. (프로그램이 멈출 수 있음)
            void check_requesters();

            // config 넣어주면 주어진 값으로 request를 진행한다.
            TOKEN requester(request_config config);

            // token값을 통해 해당 요청이 종료되었는지 확인한다.
            bool is_request_end(TOKEN token);

            // 요청에 대한 token값을 통해 수신받은 값을 가져온다. 이후 토큰을 파기한다
            json::value get_recived_value(TOKEN token);

        private:
            utility::string_t baseurl;
            vector<pair<utility::string_t, utility::string_t>> preset_headers;

            // baseurl로 생성된 http 클라이언트 객체
            http_client client;

            // 토큰 발급 관리 전용
            uint32_t        living_token_count;
            queue<TOKEN>    available_tokens;

            //map<TOKEN, >
            map<TOKEN, pplx::task<http_response>> tasks;
            cancellation_token_source cts;
        };
    };
};