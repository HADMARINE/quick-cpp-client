#include "http_client.h"

using namespace quick_client::web_http;

http_instance::http_instance(utility::string_t url) : client(url)
{
	baseurl = url;

	// 토큰 초기화
	for (TOKEN token = 0; token < MAX_TOKENS; token++)
		available_tokens.push(token);
}

http_instance::http_instance(utility::string_t url, vector<pair<utility::string_t, utility::string_t>> preset_headers) : client(url)
{
	baseurl = url;
	this->preset_headers = preset_headers;

	// 토큰 초기화
	for (TOKEN token = 0; token < MAX_TOKENS; token++)
		available_tokens.push(token);
}

http_instance::~http_instance()
{
	baseurl.clear();
	preset_headers.clear();

	// 진행중이던 모든 작업 종료
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

	// 토큰 값 처리
	living_token_count++;
	token = available_tokens.front();
	available_tokens.pop();

	auto cancle_token = cts.get_token();

	// uri 설정
	uri_builder builder(config.uri);

	// 입력받은 query 처리
	if (config.query.size() > 0)
		for (pair<utility::string_t, utility::string_t> query : config.query) {
			builder.append_query(query.first, query.second);
		}
	
	// 작성된 uri로 request 설정
	http_request req;
	req.set_request_uri(builder.to_uri());

	// 메소드 설정
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

	// 입력받은 header 처리
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

	//입력받은 body 처리
	if(config.body != json::value::Null)
		req.set_body(config.body);

	// 캔슬 토큰과 함께 설정한 request 진행
	tasks[token] = client.request(req, cancle_token);

	return token;
}

bool http_instance::is_request_end(TOKEN token)
{
	return tasks[token].is_done();
}

json::value http_instance::get_recived_value(TOKEN token)
{
	// 요청이 완료됬는지 확인함
	if (!is_request_end(token)) {
		cout << "요청이 완료되지 않았습니다." << endl;
		return json::value::null();
	}

	// 받은 데이터 json으로 파싱
	http_response response = tasks[token].get();
	json::value data = response.extract_json().get();

	// 만료된 토큰 정리
	living_token_count--;
	available_tokens.push(token);

	return data;
}