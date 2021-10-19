#include <http_client.h>

using namespace quick_client::web_http;

int main(int argc, char* argv[])
{
    // http_instance ����
    http_instance instance(L"https://httpbin.org/");

    // request_config�� request ����
    request_config req_config;
    req_config.uri = L"/get";
    req_config.method = GET;
    req_config.query.clear();
    req_config.headers.clear();
    req_config.body = json::value::Null;

    // request ����
    TOKEN req_token = instance.requester(req_config);

    while (true) {
        // response�� �޾Ҵ��� ���
        if (instance.is_request_end(req_token)) {
            // ���� ������(json)�� �����´�. �̶� req_token�� ����ȴ�.
            json::value value = instance.get_recived_value(req_token);

            // ���� �����͸� ����Ѵ�.
            wcout << value.to_string() << endl;

            break;
        }
    }

    return 0;
}