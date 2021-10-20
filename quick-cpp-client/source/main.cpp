#include <http_client.h>
#include <websocket_client.h>

using namespace quick_client::web_http;
using namespace quick_client::web_socket;

int main(int argc, char* argv[])
{
    // http_instance 생성
    http_instance h_instance(L"https://httpbin.org/");

    // request_config로 request 설정
    request_config req_config;
    req_config.uri = L"/get";       // uri 설정 (앤드 포인트)
    req_config.method = GET;        // method 설정
    req_config.query.clear();       // 쿼리 설정
    req_config.headers.clear();     // 헤더 설정
    req_config.body = json::value::Null;    // body 설정

    // request 전송
    TOKEN req_token = h_instance.requester(req_config);

    while (true) {
        // response를 받았는지 대기
        if (h_instance.is_request_end(req_token)) {
            // 받은 데이터(json)를 가져온다. 이때 req_token은 만료된다.
            json::value value = h_instance.get_recived_value(req_token);

            // 받은 데이터를 출력한다.
            wcout << value.to_string() << endl;

            break;
        }
    }

    // websocket_instance 생성
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