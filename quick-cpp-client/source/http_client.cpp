#include "http_client.h"

using namespace quick_client::web_http;

http_instance::http_instance(utility::string_t url) : client(url)
{
	baseurl = url;

	// ��ū �ʱ�ȭ
	for (TOKEN token = 0; token < MAX_TOKENS; token++)
		available_tokens.push(token);
}

http_instance::http_instance(utility::string_t url, vector<pair<utility::string_t, utility::string_t>> preset_headers) : client(url)
{
	baseurl = url;
	this->preset_headers = preset_headers;

	// ��ū �ʱ�ȭ
	for (TOKEN token = 0; token < MAX_TOKENS; token++)
		available_tokens.push(token);
}

http_instance::~http_instance()
{
	baseurl.clear();
	preset_headers.clear();

	// �������̴� ��� �۾� ����
	cts.cancel();

	tasks.clear();
}

void quick_client::web_http::http_instance::check_requesters()
{
	for (auto task : tasks) {
		try {
			task.second.wait();
		}
		catch (exception error) {
			cout << error.what() << endl;
		}
	}
}

TOKEN http_instance::requester(request_config config)
{
	TOKEN token;

	// ��ū �� ó��
	living_token_count++;
	token = available_tokens.front();
	available_tokens.pop();

	auto cancle_token = cts.get_token();

	// uri ����
	uri_builder builder(config.uri);

	// �Է¹��� query ó��
	if (config.query.size() > 0)
		for (pair<utility::string_t, utility::string_t> query : config.query) {
			builder.append_query(query.first, query.second);
		}
	
	// �ۼ��� uri�� request ����
	http_request req;
	req.set_request_uri(builder.to_uri());

	// �޼ҵ� ����
	switch (config.method)
	{
	case POST:		req.set_method(methods::POST);  break;
	case GET:		req.set_method(methods::GET);	break;
	case PATCH:		req.set_method(methods::PATCH); break;
	case PUT:		req.set_method(methods::PUT);	break;
	case DELETE:	req.set_method(methods::DEL);	break;
	default :
		return NULL;
	}

	// �Է¹��� header ó��
	if (config.query.size() > 0)
		for (pair<utility::string_t, utility::string_t> header : config.headers) {
			req.headers().add(header.first, header.second);
		}
	else {
		if(preset_headers.size() > 0)
			for (pair<utility::string_t, utility::string_t> header : preset_headers) {
				req.headers().add(header.first, header.second);
			}
	}

	//�Է¹��� body ó��
	if(config.body != json::value::Null)
		req.set_body(config.body);

	// ĵ�� ��ū�� �Բ� ������ request ����
	tasks[token] = client.request(req, cancle_token);

	return token;
}

bool http_instance::is_request_end(TOKEN token)
{
	return tasks[token].is_done();
}

json::value http_instance::get_recived_value(TOKEN token)
{
	// ��û�� �Ϸ����� Ȯ����
	if (!is_request_end(token)) {
		cout << "��û�� �Ϸ���� �ʾҽ��ϴ�." << endl;
		return json::value::null();
	}

	// ���� ������ json���� �Ľ�
	http_response response = tasks[token].get();
	json::value data = response.extract_json().get();

	// ����� ��ū ����
	living_token_count--;
	available_tokens.push(token);

	return data;
}