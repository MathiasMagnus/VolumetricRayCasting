#pragma once

template <typename T, typename S>
struct Test
{
	Test(T t, S s,int x) : m_t(t), m_s(s), a(x){};

	void print() { m_t(); m_s(); };

	T m_t;
	S m_s;
	int a;
};