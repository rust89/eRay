#ifndef _RCOMMONDIALOG_H_
#define _RCOMMONDIALOG_H_

#include "RObject.h"
#include "RCustomWindow.h"

namespace RClasses {

class RCommonDialog : public RObject
{	
	public:
		RCommonDialog();
		virtual ~RCommonDialog();

		virtual bool Show(RCustomWindow *pOwner) = 0;
};


class RFileDialog : public RCommonDialog
{	
	public:
		RFileDialog();
		virtual ~RFileDialog();

		RString FileName; // имя файла
		RString Filter; // фильтр
		RString DefExt; // расширение по умолчанию
};

class RSaveFileDialog : public RFileDialog
{	
	public:
		RSaveFileDialog();
		virtual ~RSaveFileDialog();

		virtual bool Show(RCustomWindow *pOwner);
};

} // namespace RClasses


#endif