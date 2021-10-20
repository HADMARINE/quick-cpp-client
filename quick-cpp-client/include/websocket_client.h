#pragma once

#include <string>
#include <queue>
#include <cpprest/ws_client.h>
#include <cpprest/json.h>


namespace quick_client {
	namespace web_socket {
		using namespace std;
		using namespace web;
		using namespace web::websockets::client;
		using namespace concurrency;

		class websocket_instance {
		public:
			websocket_instance();
			~websocket_instance();

			static websocket_instance* get_instance();

			// url을 설정합니다.
			void set_url(utility::string_t url) { this->url = url; };

			// 서버와의 접속을 진행합니다.
			void connect();
			
			// receive 설정 진행
			void set_recevie();

			// 서버와의 접속을 확인합니다.s
			bool is_connected() { return is_ws_ready; }

			// msg를 전송합니다.
			void send_message(string msg);
			void send_message(json::value msg);

			// 받은 msg중 가장 먼저 도착한 값을 가져오고 vector에서 삭제합니다.
			string get_message();

			// 서버와의 접속을 종료합니다.
			void close();

		private:
			utility::string_t url;

			websocket_callback_client client;

			// 웹 소켓 준비 확인
			bool is_ws_ready;

			queue<string> messages;
		};
	}
}