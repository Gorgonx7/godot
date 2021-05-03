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
	class TestHTTPClient : public HTTPClient {
public:
		
		String test_get_host_protocol(String host) {
			return get_host_protocol(host);
		}
	};
	TEST_CASE("[Http_Client] Get Host Protocol") {
		struct Test { 
			String conn_string, want_conn_string;
			bool want_ssl;
		};
		const Test tt [2] = {
			{ "http://", "", false },
			{ "https://", "", true }
		};
		for (int x = 0; x < sizeof(tt); x++) {
			TestHTTPClient test_client;
			CHECK(test_client.test_get_host_protocol(tt[x].conn_string) == tt[x].want_conn_string);
		}
	}
} // namespace TestHttpClient
#endif // TEST_HTTP_CLIENT_H
