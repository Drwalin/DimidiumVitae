
// This file is part of Drwalin's Dll Importer
// Copyright (C) 2018-2020 Marek Zalewski aka Drwalin aka DrwalinPCF

#ifndef DLL_IMPORTER_H
#define DLL_IMPORTER_H

#ifdef __cplusplus
extern "C" {
#endif

void *DllLoad(const char *file);
void DllRelease(void *handle);
void *DllGetObject(void *handle, const char *objectName);
char *DllGetErrorString();

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

class Dll {
public:
	
	Dll();
	Dll(const char *dllFileName);
	
	~Dll();
	
	bool IsValid() const;
	void *Open(const char *dllFileName);
	void Close();
	
	template <typename T >
	T Get(const char *objectName) {
		return reinterpret_cast<T>(DllGetObject(this->handle, objectName));
	}
	
private:
	
	void *handle;
};

#endif

#endif
