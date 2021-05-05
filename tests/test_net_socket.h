/*************************************************************************/
/*  test_net_socket.h                                                         */
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
#ifndef TEST_NET_SOCKET_H
#define TEST_NET_SOCKET_H
#include "core/io/net_socket.h"
#include "tests/test_macros.h"
namespace TestNetSocket {
class Mock : NetSocket {
	Error open(Type p_type, IP::Type &ip_type) override {
		return OK;
	}
	void close() override {
	};
	Error bind(IP_Address p_addr, uint16_t p_port) override{
		return OK;
	};
	Error listen(int p_max_pending) override {
		return OK;
	};
	Error connect_to_host(IP_Address p_addr, uint16_t p_port) override {
		return OK;
	};
	Error poll(PollType p_type, int timeout) const override {
		return OK;
	};
	Error recv(uint8_t *p_buffer, int p_len, int &r_read) override {
		return OK;
	};
	Error recvfrom(uint8_t *p_buffer, int p_len, int &r_read, IP_Address &r_ip, uint16_t &r_port, bool p_peek = false) override {
		return OK;
	};
	Error send(const uint8_t *p_buffer, int p_len, int &r_sent) override {
		return OK;
	};
	Error sendto(const uint8_t *p_buffer, int p_len, int &r_sent, IP_Address p_ip, uint16_t p_port) override {
		return OK;
	};
	Ref<NetSocket> accept(IP_Address &r_ip, uint16_t &r_port) override {
		return new Mock();
	};

	bool is_open() const override {
		return true;
	};
	int get_available_bytes() const override {
		return 0;
	};
	Error get_socket_address(IP_Address *r_ip, uint16_t *r_port) const override {
		return OK;
	};

	Error set_broadcasting_enabled(bool p_enabled) override {
		return OK;
	};
	void set_blocking_enabled(bool p_enabled) override{};
	void set_ipv6_only_enabled(bool p_enabled) override{};
	void set_tcp_no_delay_enabled(bool p_enabled) override{};
	void set_reuse_address_enabled(bool p_enabled) override{};
	Error join_multicast_group(const IP_Address &p_multi_address, String p_if_name) override {
		return OK;
	};
	Error leave_multicast_group(const IP_Address &p_multi_address, String p_if_name) override {
		return OK;
	};
};
} // namespace TestStreamPeerTCP
#endif
