#include <http_client.h>

using namespace quick_client::web_http;

int main(int argc, char* argv[])
{
    // http_instance 생성
    http_instance instance(L"https://httpbin.org/");

    // request_config로 request 설정
    request_config req_config;
    req_config.uri = L"/get";
    req_config.method = GET;
    req_config.query.clear();
    req_config.headers.clear();
    req_config.body = json::value::Null;

    // request 전송
    TOKEN req_token = instance.requester(req_config);

    while (true) {
        // response를 받았는지 대기
        if (instance.is_request_end(req_token)) {
            // 받은 데이터(json)를 가져온다. 이때 req_token은 만료된다.
            json::value value = instance.get_recived_value(req_token);

            // 받은 데이터를 출력한다.
            wcout << value.to_string() << endl;

            break;
        }
    }

    return 0;
}