
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
		
		Iterator(T json);
		Iterator(T json, uint64_t id);
		~Iterator();
		
		void operator++(void);
		void operator++(int);
		
		bool operator==(const Iterator &other) const;
		bool operator!=(const Iterator &other) const;
		
		friend class Collection;
		
		JSON Value();
		std::string Name() const;
		Iterator<T> operator*();
		
	private:
		
		uint64_t id;
		T json;
	};
	
	friend class Iterator<JSON>;
	
	Iterator<JSON> begin();
	Iterator<JSON> end();
		
	
	
	enum ReferenceType {
		NONE,
		REFERENCE,
		PRIMARY
	};
	
	JSON();
	JSON(const JSON& other);
	JSON(void *ptr, ReferenceType referenceType);
	JSON(const std::string &jsonString);
	JSON(const char *jsonString);
	JSON(std::istream& input);
	
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
	JSON& operator=(double value);
	JSON& operator=(bool value);
	JSON& operator=(const char *value);
	JSON& operator=(const std::string &value);
	JSON& operator=(const JSON &value);
	
	
	JSON operator[](uint64_t id);
	void PushBack(const JSON& json);
	void PopBack();
	JSON Back();
	JSON Back() const;
	JSON Front();
	JSON Front() const;
	void Resize(uint64_t size);
	void Erase(uint64_t id);
	void InitArray();
	
	uint64_t Size() const;
	void Clear();
	
	JSON operator[](const std::string &name);
	JSON operator[](const char *name);
	void Erase(const std::string &name);
	void Erase(const char *name);
	bool HasKey(const std::string &name);
	bool HasKey(const char *name);
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

