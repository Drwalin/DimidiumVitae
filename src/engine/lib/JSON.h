
//	This file is a wrapper for RapidJSON and stl
// Copyright (C) 2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef JSON_WRAPPER_FOR_RAPIDJSON_H
#define JSON_WRAPPER_FOR_RAPIDJSON_H

#if defined JSON_WRAPPER_FOR_RAPIDJSON_CPP
typedef GenericValue<UTF8<> > RAPIDJSON_TYPE;
#else
typedef void RAPIDJSON_TYPE;
#endif

#include <string>
#include <iostream>

#include <cinttypes>

class JSON {
public:
	
	template<typename T>
	class Iterator {
	public:
		
		Iterator(T &json);
		Iterator(T &json, uint64_t id);
		~Iterator();
		
		void operator++(void);
		void operator++(int);
		
		bool operator==(const Iterator &other) const;
		bool operator!=(const Iterator &other) const;
		
		JSON Value();
		std::string Name() const;
		Iterator<T> operator*();
		
	private:
		
		uint64_t id;
		T json;
	};
	
	template<typename T>
	class ConstIterator {
	public:
		
		ConstIterator(const T &json);
		ConstIterator(const T &json, uint64_t id);
		~ConstIterator();
		
		void operator++(void);
		void operator++(int);
		
		bool operator==(const ConstIterator &other) const;
		bool operator!=(const ConstIterator &other) const;
		
		const JSON Value() const;
		std::string Name() const;
		ConstIterator<T> operator*();
		
	private:
		
		uint64_t id;
		const T json;
	};
	
	friend class Iterator<JSON>;
	friend class ConstIterator<JSON>;
	
	Iterator<JSON> begin();
	Iterator<JSON> end();
	ConstIterator<JSON> begin() const;
	ConstIterator<JSON> end() const;
		
	
	
	enum ReferenceType {
		NONE,
		REFERENCE,
		PRIMARY
	};
	
	JSON();
	JSON(const JSON& other);
	JSON(JSON&& other);
	explicit JSON(void *ptr, ReferenceType referenceType);
	explicit JSON(const std::string &jsonString);
	explicit JSON(const char *jsonString);
	explicit JSON(std::istream& input);
	
	~JSON();
	void Destroy();
	
	
	void Parse(const char *jsonString);
	void Parse(const std::string &jsonString);
	void Parse(std::istream &input);
	
	std::string Write() const;
	std::string PrettyWrite() const;
	void Write(std::ostream &output) const;
	void PrettyWrite(std::ostream &output) const;
	
	
	JSON& operator=(uint64_t value);
	JSON& operator=(int64_t value);
	JSON& operator=(uint32_t value) {return this->operator=((uint64_t)value);}
	JSON& operator=(int32_t value) {return this->operator=((int64_t)value);}
	JSON& operator=(uint16_t value) {return this->operator=((uint64_t)value);}
	JSON& operator=(int16_t value) {return this->operator=((int64_t)value);}
	JSON& operator=(double value);
	JSON& operator=(float value) {return this->operator=((double)value);}
	JSON& operator=(long double value) {return this->operator=((double)value);}
	JSON& operator=(bool value);
	JSON& operator=(const char *value);
	JSON& operator=(const std::string &value);
	JSON& operator=(const JSON &value);		// copy value to current pointer, or make PRIMARY pointer
	JSON& operator<<=(const JSON &value);	// make reference to value.ref
	
	
	JSON operator[](uint64_t id);
	const JSON operator[](uint64_t id) const;
	JSON operator[](int64_t id) {return this->operator[]((uint64_t)id);}
	const JSON operator[](int64_t id) const {return this->operator[]((uint64_t)id);}
	JSON operator[](uint32_t id) {return this->operator[]((uint64_t)id);}
	const JSON operator[](uint32_t id) const {return this->operator[]((uint64_t)id);}
	JSON operator[](int32_t id) {return this->operator[]((uint64_t)id);}
	const JSON operator[](int32_t id) const {return this->operator[]((uint64_t)id);}
	JSON operator[](uint16_t id) {return this->operator[]((uint64_t)id);}
	const JSON operator[](uint16_t id) const {return this->operator[]((uint64_t)id);}
	JSON operator[](int16_t id) {return this->operator[]((uint64_t)id);}
	const JSON operator[](int16_t id) const {return this->operator[]((uint64_t)id);}
	void PushBack(const JSON& json);
	void PopBack();
	JSON Back();
	const JSON Back() const;
	JSON Front();
	const JSON Front() const;
	void Resize(uint64_t size);
	void Erase(uint64_t id);
	void InitArray();
	
	uint64_t Size() const;
	void Clear();
	
	JSON operator[](const std::string &name);
	JSON operator[](const char *name);
	const JSON operator[](const std::string &name) const;
	const JSON operator[](const char *name) const;
	void Erase(const std::string &name);
	void Erase(const char *name);
	bool HasKey(const std::string &name) const;
	bool HasKey(const char *name) const;
	void InitObject();
	
	
	operator int64_t() const;
	operator uint64_t() const;
	operator int32_t() const;
	operator uint32_t() const;
	operator int16_t() const;
	operator uint16_t() const;
	operator float() const;
	operator double() const;
	operator std::string() const;
	operator bool() const;
	
	int64_t GetInt() const;
	uint64_t GetUint() const;
	double GetFloat() const;
	std::string GetString() const;
	bool GetBool() const;
	
	void SetInt(int64_t value);
	void SetUint(uint64_t value);
	void SetFloat(double value);
	void SetString(const std::string &value);
	void SetString(const char *value);
	void SetBool(bool value);
	
	bool IsBool() const;
	bool IsNumber() const;
	bool IsInt() const;
	bool IsUint() const;
	bool IsFloat() const;
	bool IsString() const;
	bool IsTrue() const;
	bool IsFalse() const;
	bool IsNull() const;
	bool IsArray() const;
	bool IsObject() const;
	
private:
	
	RAPIDJSON_TYPE *ref;
	ReferenceType type;
};

std::ostream& operator<<(std::ostream &output, const JSON &json);
std::istream& operator>>(std::istream &input, JSON &json);

#endif

