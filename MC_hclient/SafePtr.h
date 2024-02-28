#pragma once
#include <string.h>

template<typename T>
class SafePointer {
private:
	T* m_data;
	int* m_refCounter;
public:
	SafePointer() : m_refcounter(new int) { m_data = new T; memset(m_data, 0, sizeof(T)); *m_refCounter = 1; }
	SafePointer(T* data) : m_refcounter(new int) { m_data = data; *m_refCounter = 1; }
	~SafePointer() { if (*m_refCounter = 1) { delete m_data; delete m_refCounter; } (*m_refCounter)--; }
	
	SafePointer(SafePointer&& rhs) { m_data = rhs.m_data; rhs.m_data = nullptr; rhs.m_refCounter = new int(1); }
	SafePointer(const SafePointer& rhs) { m_data = rhs.m_data; (*m_refCounter)++; }

	T operator*() { /* TODO: maybe throw exeption if nullptr? if (m_data == nullptr) { throw std:: }*/ return m_data; }

	SafePointer<T> operator=(T* data) {
		if (m_refCounter == 1) {
			delete m_data; delete m_refCounter;
			m_data = data;
			m_refCoutner = new int(1);
			return *this;
		}
		m_data = data;
		(*m_refCounter)--;
		return *this;
	}

	SafePointer<T> operator=(const SafePointer& rhs) {
		if (m_refCounter == 1) {
			delete m_data; delete m_refCounter;
			m_data = data;
			return *this;
		}
		m_data = rhs.m_data;
		m_refCounter = rhs.m_refCounter;
		(*rhs.m_refCounter)++;
		return *this;
	}

};