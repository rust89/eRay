#ifndef _VIEW3D_H_
#define _VIEW3D_H_

//#include "C:\Program Files (x86)\Microsoft DirectX SDK (February 2010)\Include\d3dx9core.h"

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include "C:\Program Files (x86)\Microsoft DirectX SDK (February 2010)\Include\d3dx9.h"
#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft DirectX SDK (February 2010)\\Lib\\x86\\d3dx9.lib")

#include <vector>

LPDIRECT3D9 pDirect3D = NULL;
LPDIRECT3DDEVICE9 pDirect3DDevice = NULL;
LPD3DXFONT pFont3D = NULL;
#define COMMENT_BUFFER_SIZE 512
TCHAR COMMENT_BUFFER[COMMENT_BUFFER_SIZE];
struct CUSTOMVERTEX {FLOAT X, Y, Z; FLOAT NX, NY, NZ;};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)
typedef std::vector<std::vector<std::vector<CUSTOMVERTEX*>>> NormalCounter;

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer1 = 0;
int g_PrimitiveCount1 = 0;

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer2 = 0;
int g_PrimitiveCount2 = 0;

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer3 = 0;
int g_PrimitiveCount3 = 0;

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer4 = 0;
int g_PrimitiveCount4 = 0;

LPDIRECT3DVERTEXBUFFER9 pVertexBufferBottom = 0;
int g_PrimitiveCountBottom = 0;

// Прототипы функций
bool D3DInitialization(HWND);
void D3DRelease();
bool Create3DFont();
void SetLight();

VOID Rotate(FLOAT, FLOAT, FLOAT);

BOOL DrawScene();
BOOL CreateVertexBuffer(FLOAT** Field, int fieldX, int fieldY, HWND hProgress, HWND hLabel);
BOOL InitVertexBuffer(LPDIRECT3DDEVICE9, LPDIRECT3DVERTEXBUFFER9*, CUSTOMVERTEX*, INT);


bool D3DInitialization(HWND hWindow)
{
	if ((pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return false;
	}
	D3DDISPLAYMODE Display;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&Display)))
	{
		return false;
	}
	D3DPRESENT_PARAMETERS Direct3DParametr;
	ZeroMemory(&Direct3DParametr,sizeof(D3DPRESENT_PARAMETERS));
	Direct3DParametr.Windowed = TRUE;
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Direct3DParametr.BackBufferFormat = Display.Format;
	
	Direct3DParametr.EnableAutoDepthStencil = TRUE;
	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	if (FAILED(pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
									  D3DDEVTYPE_HAL,hWindow,
									  D3DCREATE_HARDWARE_VERTEXPROCESSING,
									  &Direct3DParametr,&pDirect3DDevice)))
	{
		return false;
	}
	pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

	if (FAILED(D3DXCreateFont( pDirect3DDevice, 16, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &pFont3D)))
	{
		return false;
	}
	
	return true;
}

void D3DRelease()
{
	if (pVertexBuffer1)
	{
		pVertexBuffer1->Release();
		pVertexBuffer1 = 0;
	}
	if (pVertexBuffer2)
	{
		pVertexBuffer2->Release();
		pVertexBuffer2 = 0;
	}
	if (pVertexBuffer3)
	{
		pVertexBuffer3->Release();
		pVertexBuffer3 = 0;
	}
	if (pVertexBuffer4)
	{
		pVertexBuffer4->Release();
		pVertexBuffer4 = 0;
	}
	if (pFont3D)
	{
		pFont3D->Release();
		pFont3D = 0;
	}

	if (pDirect3DDevice)
	{
		pDirect3DDevice->Release();
		pDirect3DDevice = 0;
	}
	if (pDirect3D)
	{
		pDirect3D->Release();
		pDirect3D = 0;
	}

}

bool Create3DFont()
{
	HFONT hFont = CreateFont(30,10,0,0,FW_NORMAL,FALSE,FALSE,0,1,
		0,0,0,DEFAULT_PITCH | FF_MODERN,TEXT("Arial"));

	return !FAILED(D3DXCreateFont(pDirect3DDevice, 18, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, TEXT("Arial"), &pFont3D));
}

void SetLight()
{
	D3DMATERIAL9 Material;
	D3DLIGHT9 Light;
	ZeroMemory(&Material,sizeof(D3DMATERIAL9));
	Material.Diffuse.r = Material.Ambient.r = 1.0f;
	Material.Diffuse.g = Material.Ambient.g = 0.0f;
	Material.Diffuse.b = Material.Ambient.b = 0.0f;
	Material.Diffuse.a = Material.Ambient.a = 1.0f;
	pDirect3DDevice->SetMaterial(&Material);
	D3DXVECTOR3 VectorDir;
	ZeroMemory(&Light,sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	Light.Range = 10000.0f;
	VectorDir = D3DXVECTOR3(0.0f,1.0f,-1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)(&Light.Direction),&VectorDir);
	
	pDirect3DDevice->SetLight(0,&Light);
	pDirect3DDevice->LightEnable(0,TRUE);

	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING,TRUE);
	pDirect3DDevice->SetRenderState(D3DRS_AMBIENT,0);
}

VOID Rotate(FLOAT PosX, FLOAT PosY, FLOAT PosZ)
{
	D3DXMATRIX MatrixWorld, MatrixWorldX, MatrixWorldY,MatrixWorldZ;
	D3DXMatrixRotationX(&MatrixWorldX,PosX);
	D3DXMatrixRotationY(&MatrixWorldY,PosY);
	D3DXMatrixMultiply(&MatrixWorld, &MatrixWorldX, &MatrixWorldY);
	pDirect3DDevice->SetTransform(D3DTS_WORLD,&MatrixWorld);
}

VOID Rotate(D3DXMATRIX *MatrixWorldX, D3DXMATRIX *MatrixWorldY)
{
	D3DXMATRIX MatrixWorld, tmpMatrixWorld;
	D3DXMatrixMultiply(&MatrixWorld, MatrixWorldX, MatrixWorldY);
	pDirect3DDevice->SetTransform(D3DTS_WORLD,&MatrixWorld);
}

VOID SetCam(FLOAT PosX, FLOAT PosY, FLOAT PosZ)
{
	D3DXMATRIX MatrixView;
	D3DXMATRIX MatrixProjection;

	D3DXMatrixLookAtLH(&MatrixView,&D3DXVECTOR3(PosX,PosY,PosZ),&D3DXVECTOR3(0.0f,0.0f,0.0f),&D3DXVECTOR3(0.0f,1.0f,0.0f));
	pDirect3DDevice->SetTransform(D3DTS_VIEW,&MatrixView);

	
	D3DXMatrixPerspectiveFovLH(&MatrixProjection,D3DX_PI*0.3f,1,1.0f,5000.0f);
	pDirect3DDevice->SetTransform(D3DTS_PROJECTION,&MatrixProjection);

}

void GetNorm (D3DXVECTOR3& a,D3DXVECTOR3& b, D3DXVECTOR3& c, D3DXVECTOR3& n)
{
	D3DXVECTOR3 p, q;
	p = b - a;
	q = c - a;
	D3DXVec3Cross(&n,&p,&q);
	D3DXVec3Normalize(&n,&n);
}

BOOL DrawScene()
{
	if (!pDirect3DDevice)
	{
		return FALSE;
	}
	pDirect3DDevice->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(195,195,195),1.0f,0);
	pDirect3DDevice->BeginScene();
	SetLight();
	// первый квадрант
	if (pVertexBuffer1)
	{
		pDirect3DDevice->SetStreamSource(0,pVertexBuffer1,0,sizeof(CUSTOMVERTEX));
		pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,g_PrimitiveCount1);
	}
	// второй квадрант
	if (pVertexBuffer2)
	{
		pDirect3DDevice->SetStreamSource(0,pVertexBuffer2,0,sizeof(CUSTOMVERTEX));
		pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,g_PrimitiveCount2);
	}
	// третиц квадрант
	if (pVertexBuffer3)
	{
		pDirect3DDevice->SetStreamSource(0,pVertexBuffer3,0,sizeof(CUSTOMVERTEX));
		pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,g_PrimitiveCount3);
	}
	// четвёртый квадрант
	if (pVertexBuffer4)
	{
		pDirect3DDevice->SetStreamSource(0,pVertexBuffer4,0,sizeof(CUSTOMVERTEX));
		pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,g_PrimitiveCount4);
	}

	// четвёртый квадрант
	if (pVertexBufferBottom)
	{
		pDirect3DDevice->SetStreamSource(0,pVertexBufferBottom,0,sizeof(CUSTOMVERTEX));
		pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,g_PrimitiveCountBottom);
	}
	// вывод текста
	if (pFont3D)
	{
		RECT Rect = {0,0,640,480};
#if !defined(UNICODE)
		pFont3D->DrawTextA(NULL,COMMENT_BUFFER,-1,&Rect,DT_LEFT | DT_TOP,0xff00ff00);
#else
		pFont3D->DrawTextW(NULL,COMMENT_BUFFER,-1,&Rect,DT_LEFT | DT_TOP,0xff00ff00);
#endif
	}

	pDirect3DDevice->EndScene();
	pDirect3DDevice->Present(0,0,0,0);
	return TRUE;
}

BOOL CreateVertexRegion(FLOAT** Field, int Xbegin, int Ybegin, int Xend, int Yend, CUSTOMVERTEX **Vertexes, NormalCounter &NormC)
{
	int VertexCount = (Xend - Xbegin)*(Yend - Ybegin)*6;
	CUSTOMVERTEX *Vertex = new CUSTOMVERTEX[VertexCount];
	*Vertexes = Vertex;
	int k = 0;
	for (int i = Xbegin; i < Xend; i++)
	{
		for (int j = Ybegin; j < Yend; j++)
		{
			if (max(Field[i][j],Field[i+1][j+1]) < max(Field[i+1][j],Field[i][j+1]))
			{			
				int ai, bi, ci;
				D3DXVECTOR3 a1, b1, c1, n1;
				// левый треугольник
				ai = k;
				bi = k+1;
				ci = k+2;
				Vertex[k].X = FLOAT(i);
				Vertex[k].Y = FLOAT(j);
				Vertex[k].Z = FLOAT(Field[i][j]);
				NormC[i][j].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i+1);
				Vertex[k].Y = FLOAT(j);
				Vertex[k].Z = FLOAT(Field[i+1][j]);
				NormC[i+1][j].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i);
				Vertex[k].Y = FLOAT(j+1);
				Vertex[k].Z = FLOAT(Field[i][j+1]);
				NormC[i][j+1].push_back(&(Vertex[k]));
				k++;
			
				// задаём нормаль
				a1.x = Vertex[ai].X; a1.y = Vertex[ai].Y; a1.z = Vertex[ai].Z;
				b1.x = Vertex[bi].X; b1.y = Vertex[bi].Y; b1.z = Vertex[bi].Z;
				c1.x = Vertex[ci].X; c1.y = Vertex[ci].Y; c1.z = Vertex[ci].Z;
				GetNorm(a1,b1,c1,n1);
				Vertex[ai].NX = Vertex[bi].NX = Vertex[ci].NX = n1.x;
				Vertex[ai].NY = Vertex[bi].NY = Vertex[ci].NY = n1.y;
				Vertex[ai].NZ = Vertex[bi].NZ = Vertex[ci].NZ = n1.z;
				// правый треугольник
				ai = k;
				bi = k+1;
				ci = k+2;
				Vertex[k].X = FLOAT(i);
				Vertex[k].Y = FLOAT(j+1);
				Vertex[k].Z = FLOAT(Field[i][j+1]);
				NormC[i][j+1].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i+1);
				Vertex[k].Y = FLOAT(j);
				Vertex[k].Z = FLOAT(Field[i+1][j]);
				NormC[i+1][j].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i+1);
				Vertex[k].Y = FLOAT(j+1);
				Vertex[k].Z = FLOAT(Field[i+1][j+1]);
				NormC[i+1][j+1].push_back(&(Vertex[k]));
				k++;
				// задаём нормаль
				a1.x = Vertex[ai].X; a1.y = Vertex[ai].Y; a1.z = Vertex[ai].Z;
				b1.x = Vertex[bi].X; b1.y = Vertex[bi].Y; b1.z = Vertex[bi].Z;
				c1.x = Vertex[ci].X; c1.y = Vertex[ci].Y; c1.z = Vertex[ci].Z;
				GetNorm(a1,b1,c1,n1);
				Vertex[ai].NX = Vertex[bi].NX = Vertex[ci].NX = n1.x;
				Vertex[ai].NY = Vertex[bi].NY = Vertex[ci].NY = n1.y;
				Vertex[ai].NZ = Vertex[bi].NZ = Vertex[ci].NZ = n1.z;
				
			}
			else
			{
				int ai, bi, ci;
				D3DXVECTOR3 a1, b1, c1, n1;
				// левый треугольник
				ai = k;
				bi = k+1;
				ci = k+2;
				Vertex[k].X = FLOAT(i);
				Vertex[k].Y = FLOAT(j);
				Vertex[k].Z = FLOAT(Field[i][j]);
				NormC[i][j].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i+1);
				Vertex[k].Y = FLOAT(j);
				Vertex[k].Z = FLOAT(Field[i+1][j]);
				NormC[i+1][j].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i+1);
				Vertex[k].Y = FLOAT(j+1);
				Vertex[k].Z = FLOAT(Field[i+1][j+1]);
				NormC[i+1][j+1].push_back(&(Vertex[k]));
				k++;
			
				// задаём нормаль
				a1.x = Vertex[ai].X; a1.y = Vertex[ai].Y; a1.z = Vertex[ai].Z;
				b1.x = Vertex[bi].X; b1.y = Vertex[bi].Y; b1.z = Vertex[bi].Z;
				c1.x = Vertex[ci].X; c1.y = Vertex[ci].Y; c1.z = Vertex[ci].Z;
				GetNorm(a1,b1,c1,n1);
				Vertex[ai].NX = Vertex[bi].NX = Vertex[ci].NX = n1.x;
				Vertex[ai].NY = Vertex[bi].NY = Vertex[ci].NY = n1.y;
				Vertex[ai].NZ = Vertex[bi].NZ = Vertex[ci].NZ = n1.z;

				// правый треугольник
				ai = k;
				bi = k+1;
				ci = k+2;
				Vertex[k].X = FLOAT(i+1);
				Vertex[k].Y = FLOAT(j+1);
				Vertex[k].Z = FLOAT(Field[i+1][j+1]);
				NormC[i+1][j+1].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i);
				Vertex[k].Y = FLOAT(j+1);
				Vertex[k].Z = FLOAT(Field[i][j+1]);
				NormC[i][j+1].push_back(&(Vertex[k]));
				k++;
				Vertex[k].X = FLOAT(i);
				Vertex[k].Y = FLOAT(j);
				Vertex[k].Z = FLOAT(Field[i][j]);
				NormC[i][j].push_back(&(Vertex[k]));
				k++;
				// задаём нормаль
				a1.x = Vertex[ai].X; a1.y = Vertex[ai].Y; a1.z = Vertex[ai].Z;
				b1.x = Vertex[bi].X; b1.y = Vertex[bi].Y; b1.z = Vertex[bi].Z;
				c1.x = Vertex[ci].X; c1.y = Vertex[ci].Y; c1.z = Vertex[ci].Z;
				GetNorm(a1,b1,c1,n1);
				Vertex[ai].NX = Vertex[bi].NX = Vertex[ci].NX = n1.x;
				Vertex[ai].NY = Vertex[bi].NY = Vertex[ci].NY = n1.y;
				Vertex[ai].NZ = Vertex[bi].NZ = Vertex[ci].NZ = n1.z;
			}	
		}
	}
	return TRUE;
}

BOOL CreateVertexBuffer(FLOAT** Field, int fieldX, int fieldY, HWND hProgress, HWND hLabel)
{
	if (hLabel)
	{
		SetWindowText(hLabel,TEXT("Подготовка поверхности. . ."));
	}
	if (hProgress)
	{
		SendMessage(hProgress,PBM_SETRANGE,0,MAKELPARAM(0,5));
		SendMessage(hProgress,PBM_SETPOS,0,0);
	}
	int ShiftX = fieldX >> 1;
	int ShiftY = fieldY >> 1;
	int CenterX = fieldX >> 1;
	int CenterY = fieldY >> 1;
	NormalCounter varNormalCounter;
	varNormalCounter.resize(fieldX);
	for (std::size_t i = 0; i < (unsigned)fieldX; i++)
		varNormalCounter[i].resize(fieldY);

	// задаём дно
	CUSTOMVERTEX VertexBottom[6];
	g_PrimitiveCountBottom = 2;
	// треугольник раз
	VertexBottom[0].X = FLOAT(0 - ShiftX);  VertexBottom[1].X = FLOAT(0 - ShiftX);       VertexBottom[2].X = FLOAT(fieldX - ShiftX);
	VertexBottom[0].Y = FLOAT(0 - ShiftY);  VertexBottom[1].Y = FLOAT(fieldY - ShiftY);  VertexBottom[2].Y = FLOAT(fieldY - ShiftY);
	VertexBottom[0].Z = FLOAT(0);           VertexBottom[1].Z = FLOAT(0);                VertexBottom[2].Z = FLOAT(0);
	VertexBottom[0].NX = VertexBottom[1].NX = VertexBottom[2].NX = 0;
	VertexBottom[0].NY = VertexBottom[1].NY = VertexBottom[2].NY = 0;
	VertexBottom[0].NZ = VertexBottom[1].NZ = VertexBottom[2].NZ = 1;
	// треугольник два
	VertexBottom[3].X = FLOAT(fieldX - ShiftX);  VertexBottom[4].X = FLOAT(fieldX - ShiftX);  VertexBottom[5].X = FLOAT(0 - ShiftX);
	VertexBottom[3].Y = FLOAT(fieldY - ShiftY);  VertexBottom[4].Y = FLOAT(0 - ShiftY);       VertexBottom[5].Y = FLOAT(0 - ShiftY);
	VertexBottom[3].Z = FLOAT(0);                VertexBottom[4].Z = FLOAT(0);                VertexBottom[5].Z = FLOAT(0);
	VertexBottom[3].NX = VertexBottom[4].NX = VertexBottom[5].NX = 0;
	VertexBottom[3].NY = VertexBottom[4].NY = VertexBottom[5].NY = 0;
	VertexBottom[3].NZ = VertexBottom[4].NZ = VertexBottom[5].NZ = 1;
	if (!InitVertexBuffer(pDirect3DDevice,&pVertexBufferBottom,VertexBottom,6))
	{
		return FALSE;
	}
	if (hProgress) SendMessage(hProgress,PBM_SETPOS,1,0);
	// задаём первый квадрат
	CUSTOMVERTEX *Vertex1;
	int VertexCount1 = (CenterX)*(CenterY)*6;
	g_PrimitiveCount1 = VertexCount1/3;
	CreateVertexRegion(Field,0,0,CenterX,CenterY,&Vertex1, varNormalCounter);
	if (hProgress) SendMessage(hProgress,PBM_SETPOS,2,0);
	// задаём второй квадрат
	CUSTOMVERTEX *Vertex2;
	int VertexCount2 = ((fieldX - CenterX) - 1)*(CenterY)*6;
	g_PrimitiveCount2 = VertexCount2/3;
	CreateVertexRegion(Field,CenterX,0,fieldX - 1,CenterY,&Vertex2, varNormalCounter);
	if (hProgress) SendMessage(hProgress,PBM_SETPOS,3,0);
	// задаём третий квадрат
	CUSTOMVERTEX *Vertex3;
	int VertexCount3 = (CenterX)*((fieldY - CenterY) - 1)*6;
	g_PrimitiveCount3 = VertexCount3/3;
	CreateVertexRegion(Field,0,CenterY,CenterX,fieldY - 1,&Vertex3, varNormalCounter);
	if (hProgress) SendMessage(hProgress,PBM_SETPOS,4,0);
	// задаём четвёртый квадрат
	CUSTOMVERTEX *Vertex4;
	int VertexCount4 = (fieldX - CenterX - 1)*(fieldY - CenterY - 1)*6;
	g_PrimitiveCount4 = VertexCount4/3;
	CreateVertexRegion(Field,CenterX,CenterY,fieldX - 1,fieldY - 1,&Vertex4, varNormalCounter);
	if (hProgress) 
	{
		SendMessage(hProgress,PBM_SETPOS,5,0);
		Sleep(500);
	}

	if (hLabel)
	{
		SetWindowText(hLabel,TEXT("Оптимизация. . ."));
	}
	if (hProgress)
	{
		SendMessage(hProgress,PBM_SETRANGE,0,MAKELPARAM(0,varNormalCounter.size()));
		SendMessage(hProgress,PBM_SETPOS,0,0);
	}
	for (std::size_t i = 0; i < varNormalCounter.size(); i++)
	{
		for (std::size_t j = 0; j < varNormalCounter[i].size(); j++)
		{
			if (varNormalCounter[i][j].size() > 1)
			{
				D3DXVECTOR3 Result;
				Result.x = varNormalCounter[i][j][0]->NX;
				Result.y = varNormalCounter[i][j][0]->NY;
				Result.z = varNormalCounter[i][j][0]->NZ;
				for (std::size_t k = 1; k < varNormalCounter[i][j].size(); k++)
				{
					D3DXVECTOR3 Temp;
					Temp.x = varNormalCounter[i][j][k]->NX;
					Temp.y = varNormalCounter[i][j][k]->NY;
					Temp.z = varNormalCounter[i][j][k]->NZ;

					Result+= Temp;
					D3DXVec3Normalize(&Result,&Result);
				}

				for (std::size_t k = 0; k < varNormalCounter[i][j].size(); k++)
				{
					varNormalCounter[i][j][k]->NX = Result.x;
					varNormalCounter[i][j][k]->NY = Result.y;
					varNormalCounter[i][j][k]->NZ = Result.z;
				}

			}
		}
		if (hProgress) SendMessage(hProgress,PBM_SETPOS,i,0);
	}
	if (hProgress) Sleep(500);
	// инициализируем Direct3d вершинные буферы
	for (int i = 0; i < VertexCount1; i++) {Vertex1[i].X -= ShiftX; Vertex1[i].Y -= ShiftY;} 
	if (!InitVertexBuffer(pDirect3DDevice,&pVertexBuffer1,Vertex1,VertexCount1))
	{
		delete [] Vertex1;
		return FALSE;
	}
	delete [] Vertex1;
	for (int i = 0; i < VertexCount2; i++) {Vertex2[i].X -= ShiftX; Vertex2[i].Y -= ShiftY;} 
	if (!InitVertexBuffer(pDirect3DDevice,&pVertexBuffer2,Vertex2,VertexCount2))
	{
		delete [] Vertex2;
		return FALSE;
	}
	delete [] Vertex2;
	for (int i = 0; i < VertexCount3; i++) {Vertex3[i].X -= ShiftX; Vertex3[i].Y -= ShiftY;} 
	if (!InitVertexBuffer(pDirect3DDevice,&pVertexBuffer3,Vertex3,VertexCount3))
	{
		delete [] Vertex3;
		return FALSE;
	}
	delete [] Vertex3;
	for (int i = 0; i < VertexCount4; i++) {Vertex4[i].X -= ShiftX; Vertex4[i].Y -= ShiftY;} 
	if (!InitVertexBuffer(pDirect3DDevice,&pVertexBuffer4,Vertex4,VertexCount4))
	{
		delete [] Vertex4;
		return FALSE;
	}
	delete [] Vertex4;

	return TRUE;
}

BOOL InitVertexBuffer(LPDIRECT3DDEVICE9 lpd3dd, LPDIRECT3DVERTEXBUFFER9 *lpd3dvb, CUSTOMVERTEX *Vertex, INT VertexCount)
{
	// работаем с буфером вершин
	if (FAILED(lpd3dd->CreateVertexBuffer(VertexCount*sizeof(CUSTOMVERTEX),0,D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT,lpd3dvb,NULL)))
	{
		return FALSE;
	}
	VOID *pBV;
	if (FAILED((*lpd3dvb)->Lock(0,VertexCount*sizeof(CUSTOMVERTEX),&pBV,0)))
	{
		return FALSE;
	}
	memcpy(pBV,Vertex,VertexCount*sizeof(CUSTOMVERTEX));
	if (FAILED((*lpd3dvb)->Unlock()))
	{
		return FALSE;
	}
	return TRUE;
}

#endif