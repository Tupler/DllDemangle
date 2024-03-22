#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>
class PEParser {

private:
	PVOID PEbuffer;
	PVOID pNtHeader;
	int iSectionCounts;
	PIMAGE_SECTION_HEADER pSectionHeader;
public:
	int CheckPE();
	

	std::vector<char*> GetImportTable(char* dllname);
	DWORD RVA2RAW(DWORD RVA);
	PEParser(std::string path);
	
	~PEParser();




};