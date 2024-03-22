
#include "PEParser.h"
#include <Windows.h>
#include <fstream>

int PEParser::CheckPE()
{
	PIMAGE_DOS_HEADER pdosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(this->PEbuffer);
	if (pdosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		std::cout << "[-] 错误的DOS头Magic值" << std::endl;
		return -1;
	}

	PIMAGE_NT_HEADERS32 pntHeader = reinterpret_cast<PIMAGE_NT_HEADERS32>((DWORD)(this->PEbuffer) + pdosHeader->e_lfanew);
	
	if (pntHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		std::cout << "[-] 错误的NT头Magic值" << std::endl;
		return -1;
	}
	this->pNtHeader = pntHeader;
	if (pntHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_I386)
	{
		std::cout << "[+] 平台:x86" << std::endl;
		return 0;
	}
	else if(pntHeader->FileHeader.Machine == IMAGE_FILE_MACHINE_AMD64) {
		std::cout << "[+] 平台:x64" << std::endl;
		return 1;
	}
	
}

std::vector<char*> PEParser::GetImportTable(char* dllname)
{
	int machine = CheckPE();
	if (machine == -1)
	{
		
		return std::vector<char*>();
	}
	std::vector<char*> NameVector;
	// x32
	if (machine == 0)
	{
		
		PIMAGE_NT_HEADERS32 pNtHeader32 = reinterpret_cast<PIMAGE_NT_HEADERS32>(this->pNtHeader);

		IMAGE_FILE_HEADER FileHeader = pNtHeader32->FileHeader;

		this->iSectionCounts = FileHeader.NumberOfSections;

		IMAGE_OPTIONAL_HEADER32 optional_Header = (IMAGE_OPTIONAL_HEADER32)pNtHeader32->OptionalHeader;

		this->pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeader32 + 24 + FileHeader.SizeOfOptionalHeader);

		if (optional_Header.DataDirectory[1].VirtualAddress != 0) {
			IMAGE_DATA_DIRECTORY ImPortAddr = optional_Header.DataDirectory[1];
			PIMAGE_IMPORT_DESCRIPTOR ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)(this->RVA2RAW(ImPortAddr.VirtualAddress) + (DWORD)(this->PEbuffer));
			//导入表会有多个，导入表只要不为空，则它的字段也一定不为空，如果它的字段为空，则此导入表遍历到了最后
			while (ImportTable->OriginalFirstThunk)
			{
				//RVA Name
				char* dllName = (char*)(this->RVA2RAW(ImportTable->Name) + (DWORD)(this->PEbuffer));
				if (_stricmp(dllName, dllname) == 0)
				{

				
				//得到导入名称表 INT
				PIMAGE_THUNK_DATA pThunkData = (PIMAGE_THUNK_DATA)(this->RVA2RAW(ImportTable->OriginalFirstThunk) + (DWORD)(this->PEbuffer));

				while (pThunkData->u1.Function)
				{
					//判断是否按序号导入
					if (pThunkData->u1.Ordinal & IMAGE_ORDINAL_FLAG32)	//0x80000000
					{
					}
					else
					{
						//否则就是按名称导入
						PIMAGE_IMPORT_BY_NAME importName = (PIMAGE_IMPORT_BY_NAME)(this->RVA2RAW(pThunkData->u1.AddressOfData) + (DWORD)(this->PEbuffer));
						//printf("按名称导入: %s\n", importName->Name);
						NameVector.push_back(importName->Name);
					}
					pThunkData++;
				}

				}	
				ImportTable++;
			}
		}
		return NameVector;
	}
	// x64
	if (machine == 1) {
		
		PIMAGE_NT_HEADERS64 pNtHeader64 = reinterpret_cast<PIMAGE_NT_HEADERS64>(this->pNtHeader);
		IMAGE_FILE_HEADER FileHeader = pNtHeader64->FileHeader;

		this->iSectionCounts = FileHeader.NumberOfSections;

		IMAGE_OPTIONAL_HEADER64 optional_Header = (IMAGE_OPTIONAL_HEADER64)pNtHeader64->OptionalHeader;

		this->pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeader64 + 24 + FileHeader.SizeOfOptionalHeader);

		if (optional_Header.DataDirectory[1].VirtualAddress != 0) {
			IMAGE_DATA_DIRECTORY ImPortAddr = optional_Header.DataDirectory[1];
			PIMAGE_IMPORT_DESCRIPTOR ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)(this->RVA2RAW(ImPortAddr.VirtualAddress) + (DWORD)(this->PEbuffer));
			//导入表会有多个，导入表只要不为空，则它的字段也一定不为空，如果它的字段为空，则此导入表遍历到了最后
			while (ImportTable->OriginalFirstThunk)
			{
	
				//RVA Name
				char* dllName = (char*)(this->RVA2RAW(ImportTable->Name) + (DWORD)(this->PEbuffer));
				if (_stricmp(dllName, dllname) == 0)
				{
					//得到导入名称表 INT
					PIMAGE_THUNK_DATA pThunkData = (PIMAGE_THUNK_DATA)(this->RVA2RAW(ImportTable->OriginalFirstThunk) + (DWORD)(this->PEbuffer));

					while (pThunkData->u1.Function)
					{
						//判断是否按序号导入
						if (pThunkData->u1.Ordinal & IMAGE_ORDINAL_FLAG32)	//0x80000000
						{
						}
						else
						{
							//否则就是按名称导入
							PIMAGE_IMPORT_BY_NAME importName = (PIMAGE_IMPORT_BY_NAME)(this->RVA2RAW(pThunkData->u1.AddressOfData) + (DWORD)(this->PEbuffer));
							//printf("按名称导入: %s\n", importName->Name);
							NameVector.push_back(importName->Name);
						}
						pThunkData++;
					}

				}
				ImportTable++;
			}
		}
		return NameVector;
	}
}





DWORD PEParser::RVA2RAW(DWORD RVA)
{
	for (int i = 0; i < this->iSectionCounts; i++)
	{
		if (this->pSectionHeader[i].VirtualAddress <= RVA && RVA <= this->pSectionHeader[i + 1].VirtualAddress) {
			return RVA - this->pSectionHeader[i].VirtualAddress + this->pSectionHeader[i].PointerToRawData;
		}
	}
	return -1;
}

PEParser::PEParser(std::string path)
{
	std::cout << "[+] 加载成功 path:"<<path.c_str() << std::endl;

	std::ifstream ifs;
	ifs.open(path.c_str(), std::ios::binary);

	if (ifs.is_open()) {
		ifs.seekg(0, ifs.end);
		int iLength = ifs.tellg();
		ifs.seekg(0, ifs.beg);

	
		this->PEbuffer = (char*)malloc(iLength);
		ifs.read((char*)this->PEbuffer, iLength);
		
		ifs.close();
		
	}

}

PEParser::~PEParser()
{
	if (this->PEbuffer) {
		free(this->PEbuffer);
	}
}
