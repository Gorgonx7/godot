/*************************************************************************/
/*  test_image.h                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef TEST_HTTP_CLIENT_H
#define TEST_HTTP_CLIENT_H

#include "tests/test_macros.h"
#include "core/io/http_client.h"
namespace TestHttpClient {
enum Port {
	PORT_HTTP = 80,
	PORT_HTTPS = 443,

};
	class TestHTTPClient : public HTTPClient {
public:
	bool get_ssl(){
		return ssl;
	};
	void set_ssl(bool p_ssl){
		ssl = p_ssl;
	};
	String test_get_host_protocol(String host) {
		return get_host_protocol(host);
	};
	int test_set_connection_port(int port) {
		return set_connection_port(port);
	}
	};
	TEST_CASE("[http_client] Get Host Protocol") {
		struct Test { 
			String conn_string, want_conn_string;
			bool want_ssl;
		};
		const Test tt [2] = {
			{ "http://test", "test", false },
			{ "https://testNotSSL", "testNotSSL", true }
		};
		for (int x = 0; x < 2; x++) {
			TestHTTPClient test_client;
			CHECK(test_client.test_get_host_protocol(tt[x].conn_string) == tt[x].want_conn_string);
			CHECK(test_client.get_ssl() == tt[x].want_ssl);
		}
	}

	TEST_CASE("[http_client] Set Connection Port"){
		struct Test {
			int conn_port, want_port;
			bool is_ssl;
		};
		const Test tt[4] = {
			{ -1, PORT_HTTP, false },
			{ -1, PORT_HTTPS, true },
			{ 44, 44, false},
			{ 44, 44, true }
		};
		for (int x = 0; x < 4; x++) {
			TestHTTPClient test_client;
			test_client.set_ssl(tt[x].is_ssl);
			int got = test_client.test_set_connection_port(tt[x].conn_port);
			CHECK(got == tt[x].want_port);
		}
	}

	TEST_CASE("[http_client] Close Connection") {
		struct Test {
			int body_size, want_body_size, body_left, want_body_left, chunk_left, want_chunk_left, response_num, want_response_num;
			bool chunk_trailer_part, want_chunk_trailer_part, read_until_eof, want_read_until_eof, handshaking, want_handshaking;
			Ref<StreamPeerTCP> tcp_connection;
		};
		const Test tt[2] = {

		};
		for (int x = 0; x < 2; x++) {

		}
		TestHTTPClient test_client;
	}
	} // namespace TestHttpClient
#endif // TEST_HTTP_CLIENT_H
