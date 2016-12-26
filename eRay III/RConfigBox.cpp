#include "RConfigBox.h"

INT_PTR WINAPI ConfigBox(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_INITDIALOG:
		{
			ConfigParams TrunsitParams;
			if (!SendMessage(GetParent(hwnd),WM_GET_TRANS_PARAMS,(WPARAM)&TrunsitParams, sizeof(ConfigParams)))
			{
				EndDialog(hwnd,0);
			}

			SendMessage(GetDlgItem(hwnd,IDC_COMMON_OPT),BM_SETCHECK,TrunsitParams.СommonOptimiz ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_COMMON_ACCRAYRAD),BM_SETCHECK,TrunsitParams.CommonAccRayRad ? BST_CHECKED : BST_UNCHECKED,0);

			SendMessage(GetDlgItem(hwnd,IDC_CLASSIC_DRAW_AXES),BM_SETCHECK,TrunsitParams.ClassicDrawAxes ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_CLASSIC_DRAW_NODE),BM_SETCHECK,TrunsitParams.ClassicDrawNode ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_CLASSIC_DRAW_BOUND),BM_SETCHECK,TrunsitParams.ClassicDrawBound ? BST_CHECKED : BST_UNCHECKED,0);

			SendMessage(GetDlgItem(hwnd,IDC_GCLOUD_DRAW_AXES),BM_SETCHECK,TrunsitParams.GCloudDrawAxes ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_GCLOUD_DRAW_BOUND),BM_SETCHECK,TrunsitParams.GCloudDrawBound ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_GCLOUD_DRAW_NODE_GRID),BM_SETCHECK,TrunsitParams.GCloudDrawNodeGrid ? BST_CHECKED : BST_UNCHECKED,0);
			SetDlgItemInt(hwnd,IDC_GCLOUD_NODE_GRID_STEP,TrunsitParams.GCloudNodeGridStep,FALSE);
			if (!TrunsitParams.GCloudDrawNodeGrid)
			{
				EnableWindow(GetDlgItem(hwnd,IDC_GCLOUD_NODE_GRID_STEP),FALSE);
			}
			SendMessage(GetDlgItem(hwnd,IDC_NGCLOUD_DRAW_AXES),BM_SETCHECK,TrunsitParams.NGCloudDrawAxes ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_NGCLOUD_DRAW_BOUND),BM_SETCHECK,TrunsitParams.NGCloudDrawBound ? BST_CHECKED : BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hwnd,IDC_NGCLOUD_DRAW_SCALE),BM_SETCHECK,TrunsitParams.NGCloudeDrawScale ? BST_CHECKED : BST_UNCHECKED,0);
			
			{
				int Index1, Index2, Index3;
				HWND hCombo3D = GetDlgItem(hwnd,IDC_COMMON_3D);
				
				Index1 = SendMessage(hCombo3D,CB_ADDSTRING,0,(LPARAM)TEXT("Низкий"));
				if (Index1 == CB_ERR || Index1 == CB_ERRSPACE) return FALSE;
				SendMessage(hCombo3D,CB_SETITEMDATA,Index1,(LPARAM)e3doLow);

				Index2 = SendMessage(hCombo3D,CB_ADDSTRING,0,(LPARAM)TEXT("Средний"));
				if (Index2 == CB_ERR || Index2 == CB_ERRSPACE) return FALSE;
				SendMessage(hCombo3D,CB_SETITEMDATA,Index2,(LPARAM)e3doNorm);

				Index3 = SendMessage(hCombo3D,CB_ADDSTRING,0,(LPARAM)TEXT("Высокий"));
				if (Index3 == CB_ERR || Index3 == CB_ERRSPACE) return FALSE;
				SendMessage(hCombo3D,CB_SETITEMDATA,Index3,(LPARAM)e3doHigh);

				switch(TrunsitParams.Option3D)
				{
					case e3doLow:
						SendMessage(hCombo3D,CB_SETCURSEL,Index1,0);
						break;
					case e3doNorm:
						SendMessage(hCombo3D,CB_SETCURSEL,Index2,0);
						break;
					case e3doHigh:
						SendMessage(hCombo3D,CB_SETCURSEL,Index3,0);
						break;
				}
			}
			return TRUE;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_GCLOUD_DRAW_NODE_GRID:
				{
					if (SendMessage((HWND)lParam,BM_GETCHECK,0,0) == BST_UNCHECKED)
					{
						EnableWindow(GetDlgItem(hwnd,IDC_GCLOUD_NODE_GRID_STEP),FALSE);
					}
					else
					{
						EnableWindow(GetDlgItem(hwnd,IDC_GCLOUD_NODE_GRID_STEP),TRUE);
					}
					return TRUE;
				}
				case ID_CONFIG_CANCEL:
					EndDialog(hwnd, 0);
					return TRUE;
				case ID_CONFIG_OK:
				{
					ConfigParams TrunsitParams;
					
					// общие настройки
					TrunsitParams.СommonOptimiz = SendMessage(GetDlgItem(hwnd,IDC_COMMON_OPT),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.CommonAccRayRad = SendMessage(GetDlgItem(hwnd,IDC_COMMON_ACCRAYRAD),BM_GETCHECK,0,0) == BST_CHECKED;
					// классический график
					TrunsitParams.ClassicDrawAxes = SendMessage(GetDlgItem(hwnd,IDC_CLASSIC_DRAW_AXES),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.ClassicDrawNode = SendMessage(GetDlgItem(hwnd,IDC_CLASSIC_DRAW_NODE),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.ClassicDrawBound = SendMessage(GetDlgItem(hwnd,IDC_CLASSIC_DRAW_BOUND),BM_GETCHECK,0,0) == BST_CHECKED;
					
					// градиентны график 
					TrunsitParams.GCloudDrawAxes = SendMessage(GetDlgItem(hwnd,IDC_GCLOUD_DRAW_AXES),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.GCloudDrawBound = SendMessage(GetDlgItem(hwnd,IDC_GCLOUD_DRAW_BOUND),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.GCloudDrawNodeGrid = SendMessage(GetDlgItem(hwnd,IDC_GCLOUD_DRAW_NODE_GRID),BM_GETCHECK,0,0) == BST_CHECKED;
					BOOL toIntRes;
					TrunsitParams.GCloudNodeGridStep = GetDlgItemInt(hwnd,IDC_GCLOUD_NODE_GRID_STEP,&toIntRes,FALSE);

					// радужный график
					TrunsitParams.NGCloudDrawAxes = SendMessage(GetDlgItem(hwnd,IDC_NGCLOUD_DRAW_AXES),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.NGCloudDrawBound = SendMessage(GetDlgItem(hwnd,IDC_NGCLOUD_DRAW_BOUND),BM_GETCHECK,0,0) == BST_CHECKED;
					TrunsitParams.NGCloudeDrawScale = SendMessage(GetDlgItem(hwnd,IDC_NGCLOUD_DRAW_SCALE),BM_GETCHECK,0,0) == BST_CHECKED;

					{
						HWND hCombo3D = GetDlgItem(hwnd,IDC_COMMON_3D);
						int Index = SendMessage(hCombo3D,CB_GETCURSEL,0,0);
						TrunsitParams.Option3D = (e3DOption)SendMessage(hCombo3D,CB_GETITEMDATA,Index,0);
					}

					if (!SendMessage(GetParent(hwnd),WM_SET_TRANS_PARAMS,(WPARAM)&TrunsitParams, sizeof(ConfigParams)))
					{
						EndDialog(hwnd,0);
					}

					EndDialog(hwnd, 1);
					return TRUE;
				}
			}
			break;
		}
		case WM_CLOSE:
			EndDialog(hwnd, 0);
			return TRUE;
	}

	return FALSE;
}