#pragma once
#include <string>
#include "SafePtr.h"
#include <memory>
#include <type_traits>

template <typename T = void>
class Variant {
private:
	std::shared_ptr<T> m_data;
public:

	typedef const char CSTR;

	Variant(T* data) : m_data(data) {}

	template <typename Type, typename = std::enable_if<!std::is_same<T, void>::value>>
	Variant(Type data) : m_data(new T(data)) {}

	~Variant() {}

	/*inline Variant<int> GetInt() { return Variant<int>(m_data); }

	inline Variant<std::wstring> GetString() { return Variant<std::string>(m_data); }

	inline Variant<CSTR> GetCString() { return Variant<CSTR>(m_data); }

	inline Variant<float> GetFloat() { return Variant<float>(m_data); }*/

	Variant<T> operator=(T* data) { m_data = data; }
	Variant<T> operator=(T data) { m_data = new T(data); }

	template<typename Type> Variant<Type> GetVariant() { return Variant<Type>(m_data); }
	template<typename Type> Type GetValue() { return *Variant<Type>(m_data); }
	T* Get() { return m_data; }

};