#include "websocket_client.h"

using namespace quick_client::web_socket;

websocket_instance::websocket_instance()
{
	is_ws_ready = false;
}

websocket_instance::~websocket_instance()
{
	close();
}

websocket_instance* quick_client::web_socket::websocket_instance::get_instance()
{
	static websocket_instance ins;
	return &ins;
}

void websocket_instance::connect() {
	client.connect(url).then([=]() { is_ws_ready = true; set_recevie(); });
}

void websocket_instance::set_recevie()
{
	if (!is_ws_ready) {
		cout << "websocket is not ready to recevie" << endl;
		return;
	}

	// receive handler ¼³Á¤
	client.set_message_handler([](websocket_incoming_message mes) {
		websocket_instance::get_instance()->messages.push(mes.extract_string().get());
		});
}

void websocket_instance::send_message(string msg)
{
	if (!is_ws_ready) {
		cout << "websocket is not ready to send" << endl;
		return;
	}

	websocket_outgoing_message o_mes;
	o_mes.set_utf8_message(msg);

	client.send(o_mes).then([] {
		cout << "done sending mes" << endl;
		});
}

void websocket_instance::send_message(json::value msg)
{
	if (!is_ws_ready) {
		cout << "websocket is not ready to send" << endl;
		return;
	}

	utility::string_t data = msg.to_string();
	string conv_str(data.begin(), data.end());

	websocket_outgoing_message o_mes;
	o_mes.set_utf8_message(conv_str);

	client.send(o_mes).then([] {
		cout << "done sending mes" << endl;
		});

}

void websocket_instance::close()
{
	client.close();
}

string websocket_instance::get_message()
{
	if (messages.size() <= 0)
		return "";

	string mes = messages.front();
	messages.pop();

	return mes;
}