#ifndef _RIMAGELIST_H_
#define _RIMAGELIST_H_

#include "RObject.h"

#include "Commctrl.h"

namespace RClasses {

class RImageList : public RObject
{
	protected:
		HIMAGELIST m_hImageList;
	public:
		RImageList();
		~RImageList();
		const HIMAGELIST GetNativeImageList() const;
		bool Create(int cx, int cy, UINT flags, int cInitial, int cGrow);
		// добавляет в список изображение по маске
		// возвращает индекс вставленного изображения, иначе -1
		int AddMasked(HBITMAP hbmImage, COLORREF crMask);
		HICON GetIcon(int i,UINT flags);
		HBITMAP GetBitmap(int i);
		const int GetImageCount() const;
		void Destroy();
};

} // namespace RClasses
#endif
