#ifndef VARIANT_H
#define VARIANT_H
#include <string>
#include "SafePtr.h"
#include <memory>
#include <type_traits>

#define NULL_VARIANT Variant<>(nullptr)

/// <summary>
/// To future me and other devs:
///  I would REALLY try to avoid using this class beacuse it is not only confusing but can also be dangerous since it acts like a regular pointer.
///	 If you do use this MAKE SURE TO CHECK ALL INSTANCES IN THE DEBUGGER check the value of m_data AND the value it points to AND the reference counter
///  this class can hide BIG problems caused by the object or other code, so PLEASE PLEASE PLEASE PLEASE PLEASE be careful and very much try to limit its
///	 use to the parsing code
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T = void>
class Variant {
private:
	Variant(T* data, int* refcounter) : m_data(data), m_refcounter(refcounter) {}

	T* m_data;
	int* m_refcounter;

	template<typename Type>
	friend class Variant;
public:

	Variant(T* data) : m_data(data), m_refcounter(new int(1)) {}

	Variant(const Variant<T>& rhs) { m_data = rhs.m_data; m_refcounter = rhs.m_refcounter; (*m_refcounter)++; }

	/*template <typename Type = T, typename = typename std::enable_if<!std::is_same<T, void>::value>>
	Variant(Type data) : m_data(new T(data)) {}*/

	~Variant() { if (CheckVariantNull()) { return; } (*m_refcounter)--; if (m_refcounter == 0) { delete m_data; } }


	/*inline Variant<int> GetInt() { return Variant<int>(m_data); }

	inline Variant<std::wstring> GetString() { return Variant<std::string>(m_data); }

	inline Variant<CSTR> GetCString() { return Variant<CSTR>(m_data); }

	inline Variant<float> GetFloat() { return Variant<float>(m_data); }*/

	//Variant<T>& operator=(T* data) { m_data = data; }
	
	// C++ template witch craft BE CAREFUL USING FUNCTION IS DANGEROUS BECEAUSE IT CASTS TO TYPE FROM T
	template <typename Type = T, typename = typename std::enable_if<!std::is_same<T, void>::value>>
	inline const Variant<T>& operator=(Type data) {
		if (CheckVariantNull()) {
			m_data = static_cast<T*>(new Type(data));
			*m_refcounter = 1;
			return *this;
		}
		
		// this is dangerous and dumb. fix.
		memcpy(m_data, &data, sizeof(Type));
		return *this;
	}

	inline const Variant<T>& operator=(const Variant<T>& rhs) {
		m_data = rhs.m_data;
		if (m_refcounter == 1) { delete m_data; delete m_refcounter; } rhs.IncRefCounter(); m_refcounter = rhs.m_refcounter; 
		return *this;
	}

	template<typename Type>
	inline bool operator==(const Variant<Type>& rhs);

	inline T* GetPtr() const { return m_data; }

	bool CheckVariantNull() const { return (m_data == nullptr); }

	// the returned variant may be a copy, but the copied object still points to the same memory, so it is valid
	template<typename Type = void> inline Variant<Type> GetVariant() {
		(*m_refcounter)++;  return Variant<Type>(static_cast<Type*>(m_data), m_refcounter); }

	// this will only get a copy of the stored variable, use the assignment operator if you want to change the value the variant points to
	template<typename Type> inline Type GetValue() const { return *static_cast<Type*>(m_data); }

	//std::shared_ptr<T>& Get() { return m_data; }

};

#endif