#include <http_client.h>
#include <websocket_client.h>

using namespace quick_client::web_http;
using namespace quick_client::web_socket;

int main(int argc, char* argv[])
{
    // http_instance ����
    http_instance h_instance(L"https://httpbin.org/");

    // request_config�� request ����
    request_config req_config;
    req_config.uri = L"/get";       // uri ���� (�ص� ����Ʈ)
    req_config.method = GET;        // method ����
    req_config.query.clear();       // ���� ����
    req_config.headers.clear();     // ��� ����
    req_config.body = json::value::Null;    // body ����

    // request ����
    TOKEN req_token = h_instance.requester(req_config);

    while (true) {
        // response�� �޾Ҵ��� ���
        if (h_instance.is_request_end(req_token)) {
            // ���� ������(json)�� �����´�. �̶� req_token�� ����ȴ�.
            json::value value = h_instance.get_recived_value(req_token);

            // ���� �����͸� ����Ѵ�.
            wcout << value.to_string() << endl;

            break;
        }
    }

    // websocket_instance ����
    websocket_instance* ws_instance = websocket_instance::get_instance();
    ws_instance->set_url(L"ws://127.0.0.1:13254");
    ws_instance->connect();
    
    bool trigger;

    while (true) {
        if (ws_instance->is_connected()) {
            string data = ws_instance->get_message();
            
            if (data.size() > 0) {
                cout << data << endl;

                string mes_1 = "message1";
                string mes_2 = "message2";

                ws_instance->send_message(mes_1);
                ws_instance->send_message(mes_2);
            }
        }
    }

    return 0;
}