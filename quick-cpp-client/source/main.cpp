#include <http_client.hh>

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

    TOKEN req_token = instance.requester(req_config);

    while (true) {
        if (instance.is_request_end(req_token)) {
            json::value value = instance.get_recived_value(req_token);

            wcout << value.to_string() << endl;

            break;
        }
    }

    return 0;
}
