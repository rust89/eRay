#include "RCommonDialog.h"

using namespace RClasses;

RCommonDialog::RCommonDialog()
	: RObject()
{

}

RCommonDialog::~RCommonDialog()
{

}

////////////////////////////////////////////////////

RFileDialog::RFileDialog()
	: RCommonDialog()
{
	FileName = TEXT('\0');
}

RFileDialog::~RFileDialog()
{

}

////////////////////////////////////////////////////

RSaveFileDialog::RSaveFileDialog()
{

}

RSaveFileDialog::~RSaveFileDialog()
{

}

bool RSaveFileDialog::Show(RCustomWindow *pOwner)
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];
	for (std::size_t i = 0; i < FileName.size() && i < MAX_PATH; i++)
		szFile[i] = FileName[i];

	TCHAR *szFilter = new TCHAR[Filter.size()];
	for (std::size_t i = 0; i < Filter.size(); i++)
		szFilter[i] = Filter[i];

	TCHAR *szDefExt = new TCHAR[DefExt.size()];
	for (std::size_t i = 0; i < DefExt.size(); i++)
		szDefExt[i] = DefExt[i];	

	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	

	ofn.lStructSize = sizeof(OPENFILENAME); 
	ofn.hwndOwner = pOwner->Handle(); 
	ofn.lpstrFilter = szFilter; 
	ofn.lpstrFile= szFile; 
	ofn.lpstrDefExt = szDefExt;
	ofn.nMaxFile = sizeof(szFile)/ sizeof(*szFile); 
	ofn.Flags = OFN_OVERWRITEPROMPT; 
 
	BOOL Result = GetSaveFileName(&ofn);

	FileName.assign(ofn.lpstrFile);

	delete [] szFilter;
	delete [] szDefExt;

	return Result ? true : false;
}