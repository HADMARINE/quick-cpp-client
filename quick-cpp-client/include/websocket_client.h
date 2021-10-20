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

			// url�� �����մϴ�.
			void set_url(utility::string_t url) { this->url = url; };

			// �������� ������ �����մϴ�.
			void connect();
			
			// receive ���� ����
			void set_recevie();

			// �������� ������ Ȯ���մϴ�.s
			bool is_connected() { return is_ws_ready; }

			// msg�� �����մϴ�.
			void send_message(string msg);
			void send_message(json::value msg);

			// ���� msg�� ���� ���� ������ ���� �������� vector���� �����մϴ�.
			string get_message();

			// �������� ������ �����մϴ�.
			void close();

		private:
			utility::string_t url;

			websocket_callback_client client;

			// �� ���� �غ� Ȯ��
			bool is_ws_ready;

			queue<string> messages;
		};
	}
}