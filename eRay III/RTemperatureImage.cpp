#include "RTemperatureImage.h"
#include "resource.h"
/*
C, lambda, Ro брать в среднем
*/

RTemperatureImage::RTemperatureImage() 
{
	m_nSightX = 0;
	m_bDrawSight = false;
	hSigthAxesPen = CreatePen(PS_SOLID,1,RGB(255,0,0));
	hBalloonPen = CreatePen(PS_SOLID,1,RGB(0xc0,0xc0,0xc0));
	hBalloonBrush = CreateSolidBrush(RGB(0xc0,0xc0,0xc0));

	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfHeight = 14;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = DEFAULT_QUALITY;
	hBalloonFont = CreateFontIndirect(&lf);

	AddMessage(WM_CREATE,MSG_EVENT_CAST(RTemperatureImage::OnCreate),this);
	AddMessage(WM_SIZE,MSG_EVENT_CAST(RTemperatureImage::OnSize),this);
	AddMessage(WM_PAINT,MSG_EVENT_CAST(RTemperatureImage::OnPaint),this);
	AddMessage(WM_MOUSEMOVE,MSG_EVENT_CAST(RTemperatureImage::OnMouseMove),this);
	AddMessage(WM_RBUTTONDOWN,MSG_EVENT_CAST(RTemperatureImage::OnMouseRDown),this);
	AddMessage(WM_COMMAND,MSG_EVENT_CAST(RTemperatureImage::OnCommand),this);
}

RTemperatureImage::~RTemperatureImage()
{
	//
}

LRESULT RTemperatureImage::OnCreate(WPARAM wParam, LPARAM lParam)
{	
	HMODULE hImageModule = LoadLibrary(TEXT("Resources.dll"));
	if (hImageModule)
	{
		HDC hdc = GetDC(m_hHandle);
		HBITMAP tmphBitMap = LoadBitmap(hImageModule,MAKEINTRESOURCE(101));
		if (tmphBitMap)
		{
			m_cBeginView.CreateCompatible(hdc,tmphBitMap);
		}
		FreeLibrary(hImageModule);
	}

	return 0;
}

LRESULT RTemperatureImage::OnSize(WPARAM wParam, LPARAM lParam)
{	
	int ClientWidth = LOWORD(lParam);
	int ClientHeight = HIWORD(lParam);
	return 0;
}

LRESULT RTemperatureImage::OnPaint(WPARAM wParam, LPARAM lParam)
{	
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_hHandle,&ps);

	RECT ClientRect;
	GetClientRect(ClientRect);

	if (Image.GetDC())
	{
		if (m_bDrawSight)
		{
			if (m_nSightX)
			{
				// создаём промежуточный буффер
				ImageBuffer tmp_buffer;
				if (!tmp_buffer.CreateCompatible(hdc,ClientRect.right,ClientRect.bottom))
				{
					if (GetLastError() == 8)
					{
						MessageBox(m_hHandle,TEXT("Недостаточно видеопамяти!"),TEXT("Оппаньки :("),MB_OK | MB_ICONERROR);
						EndPaint(m_hHandle,&ps);
					}
					return -1;
				}
				// копируем в промежуточный буффер текущее изображение
				BitBlt(tmp_buffer.GetDC(), 0, 0, Image.GetWidth(), Image.GetHeight(),Image.GetDC(),0,0,SRCCOPY);
				// рисуем в нём прицел
				DrawSight(tmp_buffer.GetDC(),&ps.rcPaint);

				// копируем содержимое промежёточного буфера на форму
				BitBlt(hdc, 0, 0, tmp_buffer.GetWidth(), tmp_buffer.GetHeight(),tmp_buffer.GetDC(),0,0,SRCCOPY);

				// сброс буфера
				tmp_buffer.Clear();
			}
			else
			{
				BitBlt(hdc, 0, 0, Image.GetWidth(), Image.GetHeight(),Image.GetDC(),0,0,SRCCOPY);
			}
		}
		else
		{
			BitBlt(hdc, 0, 0, Image.GetWidth(), Image.GetHeight(),Image.GetDC(),0,0,SRCCOPY);
		}
	}
	else
	{	
		if (m_cBeginView.GetDC())
		{
			// создаём промежуточный буфер
			ImageBuffer tmp_buffer;
			tmp_buffer.CreateCompatible(hdc,ps.rcPaint.right,ps.rcPaint.bottom);
			tmp_buffer.Blacken(); // закрашиваем чёрным
			int ImgXPos = (ClientRect.right - m_cBeginView.GetWidth()) / 2;
			int ImgYPos = (ClientRect.bottom - m_cBeginView.GetHeight()) / 2;
			BitBlt(tmp_buffer.GetDC(),ImgXPos,ImgYPos,m_cBeginView.GetWidth(),m_cBeginView.GetHeight(),m_cBeginView.GetDC(),0,0,SRCCOPY);
			BitBlt(hdc,0,0,ClientRect.right,ClientRect.bottom,tmp_buffer.GetDC(),0,0,SRCCOPY);
		}
		else
		{
			PatBlt(hdc,0,0,ClientRect.right,ClientRect.bottom,BLACKNESS);
		}
	}

	EndPaint(m_hHandle,&ps);
	return 0;
}

LRESULT RTemperatureImage::OnMouseMove(WPARAM wParam, LPARAM lParam)
{	
	if (m_bDrawSight)
	{
		int x_pos = LOWORD(lParam);
		TemperatureMap::iterator iter;
		if ((iter = m_cTemp_map.find(x_pos)) != m_cTemp_map.end())
		{
			m_nSightX = x_pos;
			m_nSightY = iter->second.Y;

			TemperatureValue = TEXT("Температура: ") + RConverter::ToString(iter->second.Temperature) + TEXT(" K.");
			TimeValue = TEXT("Время: ") + RConverter::ToString(iter->second.Time) + TEXT(" с.");
		}
		else
		{
			m_nSightX = 0;
		}

		Refresh();
	}
	return 0;
}

bool RTemperatureImage::SetSightDrawing(bool State)
{
	bool Result = m_bDrawSight;
	m_bDrawSight = State;
	return Result;
}

void RTemperatureImage::DrawSight(HDC hdc, LPRECT DrawRect)
{
	int Indent = 3; // отступ текста в облачке
	int BallonIndent = 2; // отступ прицела от облачка
	
	SetBkMode(hdc,TRANSPARENT);
	
	// отрисовка прицела
	SelectObject(hdc,hSigthAxesPen);

	int SightInch = 6;
	
	MoveToEx(hdc,m_nSightX,0,NULL);
	LineTo(hdc,m_nSightX,m_nSightY - SightInch);

	MoveToEx(hdc,m_nSightX,m_nSightY + SightInch,NULL);
	LineTo(hdc,m_nSightX,Image.GetHeight());


	MoveToEx(hdc,0,m_nSightY,NULL);
	LineTo(hdc,m_nSightX - SightInch,m_nSightY);
	
	MoveToEx(hdc,m_nSightX + SightInch,m_nSightY,NULL);
	LineTo(hdc,Image.GetWidth(),m_nSightY);


	SelectObject(hdc,hBalloonFont);
	SIZE TemperatureValueTextSize;
	if (!GetTextExtentPoint32(hdc,TemperatureValue.c_str(),TemperatureValue.size(),&TemperatureValueTextSize))
	{
		// не рисуем в таком случае
		return;
	}
	// надпись доли
	SIZE TimeValueTextSize;
	if (!GetTextExtentPoint32(hdc,TimeValue.c_str(),TimeValue.size(),&TimeValueTextSize))
	{
		// не рисуем в таком случае
		return;
	}

	SIZE TextSize = {
		TemperatureValueTextSize.cx > TimeValueTextSize.cx ? TemperatureValueTextSize.cx : TimeValueTextSize.cx,
		TemperatureValueTextSize.cy + TimeValueTextSize.cy};

	// включаем отступ в расчёт
	int BalloonSizeCx = TextSize.cx + (Indent << 1);
	int BalloonSizeCy = TextSize.cy + (Indent << 1);

	int BalloonPosX(0), BalloonPosY(0);
	// определяем позицию надписи относительно вертикальной черты
	if (m_nSightX + BalloonSizeCx + BallonIndent < DrawRect->right )
		BalloonPosX = m_nSightX + BallonIndent;
	else
		if (m_nSightX - BalloonSizeCx - BallonIndent > DrawRect->left )
			BalloonPosX = m_nSightX - BalloonSizeCx - BallonIndent;
		else
			return;
	// определяем позицию надписи относительно горизонтальной черты
	if (m_nSightY - BalloonSizeCy - BallonIndent > DrawRect->top )
		BalloonPosY = m_nSightY - BalloonSizeCy - BallonIndent;
	else
		if (m_nSightY + BalloonSizeCy + BallonIndent < DrawRect->bottom )
			BalloonPosY = m_nSightY + BallonIndent;
		else
			return;

	RECT BalloonRect = {BalloonPosX,BalloonPosY,BalloonPosX + BalloonSizeCx,BalloonPosY + BalloonSizeCy};

	SelectObject(hdc,hBalloonBrush);
	SelectObject(hdc,hBalloonPen);
	RoundRect(hdc,BalloonRect.left,BalloonRect.top,BalloonRect.right,BalloonRect.bottom,5,5);

	SetTextColor(hdc,RGB(255,255,255));
	TextOut(hdc,BalloonRect.left + Indent,BalloonRect.top + Indent,TemperatureValue.c_str(),TemperatureValue.size());
	TextOut(hdc,BalloonRect.left + Indent,BalloonRect.top + Indent + TemperatureValueTextSize.cy,TimeValue.c_str(),TimeValue.size());
}

LRESULT RTemperatureImage::OnMouseRDown(WPARAM wParam, LPARAM lParam)
{	
	HMENU hMenu = LoadMenu(GetModuleHandle(NULL),MAKEINTRESOURCE(IDR_POPUP_TEMP_IMAGE));
	if (!hMenu) return 0;
	hMenu = GetSubMenu(hMenu,0);
	if (!hMenu) return 0;

	if (!Image.GetDC())
	{
		EnableMenuItem(hMenu,ID_POPUP_TEMP_SAVE_IMAGE,MF_DISABLED);
	}

	bool EnableCopyTemperatureLayer = false;
	switch(m_eCurrentView)
	{
		case ViewXY:
		case ViewXZ:
		case ViewYZ:
			EnableCopyTemperatureLayer = true;
	}
	if (!EnableCopyTemperatureLayer || !Image.GetDC())
		EnableMenuItem(hMenu,ID_POPUP_TEMP_COPY_DATA, MF_DISABLED);

	if (m_eCurrentView != ViewXY)
		EnableMenuItem(hMenu,ID_POPUP_TEMP_SAVE_IMAGE_SER_Z, MF_DISABLED);

	// вызываем всплывающее меню
	POINT CurPos;
	if (!GetCursorPos(&CurPos)) return 0;
	SetForegroundWindow(m_hHandle);
	TrackPopupMenuEx(hMenu, 0, CurPos.x, CurPos.y, m_hHandle, NULL);
	return 0;
}

LRESULT RTemperatureImage::OnCommand(WPARAM wParam, LPARAM lParam)
{	
	WORD ControlID = LOWORD(wParam);
	switch(ControlID)
	{
		case ID_POPUP_TEMP_SAVE_IMAGE:
			SaveCurrentImage();
			break;
		case ID_POPUP_TEMP_SAVE_IMAGE_SER_Z:
			SaveImageSeriesZ();
			break;
		case ID_POPUP_TEMP_COPY_DATA:
			CopyDataToClipboard();
			break;
	}
	return 0;
}

void RTemperatureImage::SaveCurrentImage()
{
	RSaveFileDialog sfd;
	sfd.DefExt = TEXT("bmp");
	sfd.Filter.assign(TEXT("Bitmap Files\0*.bmp\0\0"),23);
	if (sfd.Show(this->GetParent()))
	{
		Image.SaveToFile(sfd.FileName.c_str());
	}
}

void RTemperatureImage::SaveImageSeriesZ()
{
	ImageBuffer tmp_image;
	if (!tmp_image.CreateCompatible(Image,Image.GetWidth(),Image.GetHeight()))
		return;

	for (int i = 0; i < MainRender.GetL(); i++)
	{
		MainRender.DrawSceneByXY(&tmp_image,i);
		RString FileName = TEXT("image_") + RConverter::ToString(i+1) + TEXT(".bmp");
		tmp_image.SaveToFile(FileName.c_str());
	}
}

void RTemperatureImage::SetCurrentView(eView View)
{
	m_eCurrentView = View;
}

void RTemperatureImage::SetCurrentLayer(int Layer)
{
	m_nCurrentLayer = Layer;
}

void RTemperatureImage::CopyDataToClipboard()
{
	gstring Buffer;
	switch(m_eCurrentView)
	{
		case ViewXY:
			MainRender.CopyXYLayer(Buffer,m_nCurrentLayer);
			break;
		case ViewXZ:
			MainRender.CopyXZLayer(Buffer,m_nCurrentLayer);
			break;
		case ViewYZ:
			MainRender.CopyYZLayer(Buffer,m_nCurrentLayer);
			break;
	}

	if (Buffer.empty())
		return;

	int BufferSize = Buffer.size();

	if (!OpenClipboard(m_hHandle))
        return;
    EmptyClipboard();

    // Размещаем объект для текста в глобальной памяти.
    HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE,
		(BufferSize + 1) * sizeof(TCHAR));
    
	if (hglbCopy == NULL)
    {
        CloseClipboard();
        return;
    }

    // Блокируем дескриптор и копируем текст в буфер.
    LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglbCopy);
	memcpy(lptstrCopy, (LPVOID)(Buffer.data()), BufferSize * sizeof(TCHAR));
    lptstrCopy[BufferSize] = TEXT('\0');    // нулевой символ
    GlobalUnlock(hglbCopy);

    // Помещаем дескриптор в буфер обмена.
#if defined(_UNICODE)
	SetClipboardData(CF_UNICODETEXT, hglbCopy);
#else
    SetClipboardData(CF_TEXT, hglbCopy);
#endif

	CloseClipboard();
}