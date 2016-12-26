#include "RVarEditorBox.h"

VarEditorVariables VarEditor_Vars;
VarEditorUnit CurrentInsertVarUnit;
tVarEditor_VarsAndIndex VarEditor_VarsAndIndex;

VarEditorUnit::VarEditorUnit() 
{

}

VarEditorUnit::VarEditorUnit(RString _Name, RString _Value) 
{
	Name=_Name;
	Value=_Value;
}

INT_PTR WINAPI VarEditorProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_INITDIALOG:
		{			
			return VarEditorProcInit(hwnd,wParam,lParam);
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_VAR_EDIT_OK:
				{
					return VarEditorProcClickOk(hwnd,wParam,lParam);
				}
				case IDC_VAR_EDIT_CANCEL:
				{
					return VarEditorProcClickCancel(hwnd,wParam,lParam);
				}
				case IDC_VAR_EDIT_ADD: // обработка вставки переменой
				{
					return VarEditorProcClickAdd(hwnd,wParam,lParam);
				}
				case IDC_VAR_EDIT_EDIT:
				{
					return VarEditorProcClickEdit(hwnd,wParam,lParam);
				}
				case IDC_VAR_EDIT_REM:
				{
					return VarEditorProcClickRem(hwnd,wParam,lParam);
				}
			}
			break;
		}
		case WM_NOTIFY:
		{
			return VarEditorProcNotify(hwnd,wParam,lParam);
		}
		case WM_CLOSE:
		{
			return VarEditorProcClose(hwnd,wParam,lParam);
		}
	}
	return FALSE;
}

INT_PTR WINAPI VarEditorProcInit(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hListView = GetDlgItem(hwnd,IDC_VAR_EDIT_LIST);
	ListView_SetExtendedListViewStyle( hListView,LVS_EX_FULLROWSELECT);

	// устанавливаем колонки
	LVCOLUMN NewColumn;
	ZeroMemory(&NewColumn,sizeof(LVCOLUMN));
	NewColumn.mask = LVCF_TEXT | LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;
	NewColumn.fmt  = LVCFMT_LEFT;
	NewColumn.cx = 150;
	NewColumn.pszText = TEXT("Значение");
	ListView_InsertColumn(hListView,0,&NewColumn);
	NewColumn.cx = 80;
	NewColumn.pszText = TEXT("Переменная");
	ListView_InsertColumn(hListView,0,&NewColumn);

	// загрузка уже существующих переменных
	VarEditor_VarsAndIndex.clear();
	for (std::size_t i = 0; i < VarEditor_Vars.size(); i++)
	{
		LVITEM lvi;
		ZeroMemory(&lvi,sizeof(LVITEM));
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
		lvi.pszText = (LPTSTR)(VarEditor_Vars[i].Name.c_str());
		lvi.iItem = i;
		int ItemIndex;
				
		ItemIndex = ListView_InsertItem(GetDlgItem(hwnd,IDC_VAR_EDIT_LIST),&lvi);
		ListView_SetItemText(GetDlgItem(hwnd,IDC_VAR_EDIT_LIST),ItemIndex,1,(LPTSTR)VarEditor_Vars[i].Value.c_str());
				
		// сохраняем полученный результат вместе с индексом
		VarEditor_VarsAndIndex.push_back(std::make_pair(VarEditor_Vars[i],ItemIndex));
	}
	return TRUE;
}

INT_PTR WINAPI VarEditorProcClickOk(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	// отгрузка переменных
	VarEditor_Vars.clear();
	for (std::size_t i = 0; i < VarEditor_VarsAndIndex.size(); i++)
	{
		VarEditor_Vars.push_back(VarEditor_VarsAndIndex[i].first);
	}
	EndDialog(hwnd,1);
	return TRUE;
}

INT_PTR WINAPI VarEditorProcClickCancel(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hwnd,0);
	return TRUE;
}

INT_PTR WINAPI VarEditorProcClickAdd(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	
	CurrentInsertVarUnit.Name.clear();
	CurrentInsertVarUnit.Value.clear();
	if (DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_INSERT_VAR),hwnd,VarInsertProc))
	{
		HWND hListView = GetDlgItem(hwnd,IDC_VAR_EDIT_LIST);
		// просматриваем переменные на дубликат
		for (tVarEditor_VarsAndIndex::iterator i = VarEditor_VarsAndIndex.begin();
			i != VarEditor_VarsAndIndex.end(); i++)
		{
			if (CurrentInsertVarUnit.Name == i->first.Name)
			{
				MessageBox(hwnd,TEXT("Переменная с таким именем уже существует!"),TEXT("Ошибка"),MB_OK | MB_ICONERROR);
				return TRUE;
			}
		}
		// очищаем поле
		ListView_DeleteAllItems(hListView);
		// запоминаем добавленную переменную
		VarEditor_VarsAndIndex.push_back(std::make_pair(VarEditorUnit(CurrentInsertVarUnit.Name,CurrentInsertVarUnit.Value),0));
		// заполняем графический список
		for (std::size_t i = 0; i < VarEditor_VarsAndIndex.size(); i++)
		{
			LVITEM lvi;
			ZeroMemory(&lvi,sizeof(LVITEM));
			lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			lvi.pszText = (LPTSTR)(VarEditor_VarsAndIndex[i].first.Name.c_str());
			lvi.iItem = i;
						
			// отображаем текст
			VarEditor_VarsAndIndex[i].second = ListView_InsertItem(hListView,&lvi);
			ListView_SetItemText(hListView,VarEditor_VarsAndIndex[i].second,1,
								    (LPTSTR)VarEditor_VarsAndIndex[i].first.Value.c_str());
		}
	}
	return TRUE;
}

INT_PTR WINAPI VarEditorProcClickEdit(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hListView = GetDlgItem(hwnd,IDC_VAR_EDIT_LIST);
	int ItemIndex = -1;
	ItemIndex = ListView_GetNextItem(hListView,ItemIndex,LVIS_SELECTED);
	if (ItemIndex != -1)
	{
		// ищем переменную по данному индексу
		for (std::size_t i = 0; i < VarEditor_VarsAndIndex.size(); i++)
		{
			// ежели переменная нашлась
			if (VarEditor_VarsAndIndex[i].second == ItemIndex)
			{
				CurrentInsertVarUnit.Name = VarEditor_VarsAndIndex[i].first.Name;
				CurrentInsertVarUnit.Value = VarEditor_VarsAndIndex[i].first.Value;
				if (DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_INSERT_VAR),hwnd,VarInsertProc))
				{
					// просматриваем на дубликат
					for (tVarEditor_VarsAndIndex::iterator iter = VarEditor_VarsAndIndex.begin();
						iter != VarEditor_VarsAndIndex.end(); iter++)
					{
						if (CurrentInsertVarUnit.Name == iter->first.Name)
						{
							if (CurrentInsertVarUnit.Name != VarEditor_VarsAndIndex[i].first.Name)
							{
								MessageBox(hwnd,TEXT("Переменная с таким именем уже существует!"),TEXT("Ошибка"),MB_OK | MB_ICONERROR);
								return TRUE;
							}
						}
					}
					// обновляем значение в памяти
					VarEditor_VarsAndIndex[i].first.Name = CurrentInsertVarUnit.Name;
					VarEditor_VarsAndIndex[i].first.Value = CurrentInsertVarUnit.Value;
					// обновляем отображение
					ListView_SetItemText(hListView,ItemIndex,0,(LPTSTR)VarEditor_VarsAndIndex[i].first.Name.c_str());
					ListView_SetItemText(hListView,ItemIndex,1,(LPTSTR)VarEditor_VarsAndIndex[i].first.Value.c_str());
				}
				break;
			}
		}
	}
	return TRUE;
}

INT_PTR WINAPI VarEditorProcClickRem(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HWND hListView = GetDlgItem(hwnd,IDC_VAR_EDIT_LIST);
	int ItemIndex = -1;
	// запоминаем помеченные элементы
	std::stack <int> ItemIndexToDelete;
	while ((ItemIndex = ListView_GetNextItem(hListView,ItemIndex,LVIS_SELECTED)) != -1)
	{
		ItemIndexToDelete.push(ItemIndex);
	}
	// удаляем помеченные элементы
	while (!ItemIndexToDelete.empty())
	{
		int index_to_delete = ItemIndexToDelete.top();
		// удаляем из графического списка
		ListView_DeleteItem(hListView,index_to_delete);
		// удаляем из списка в памяти
		for (std::size_t i = 0; i < VarEditor_VarsAndIndex.size(); i++)
		{
			if (VarEditor_VarsAndIndex[i].second == index_to_delete)
			{
				VarEditor_VarsAndIndex.erase(VarEditor_VarsAndIndex.begin() + i);
				break;
			}
		}
		ItemIndexToDelete.pop();
	}
	return TRUE;
}

INT_PTR WINAPI VarEditorProcNotify(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR notifyinfo = (LPNMHDR)lParam;
	if (notifyinfo->idFrom == IDC_VAR_EDIT_LIST)
	{
		if (notifyinfo->code == NM_DBLCLK)
		{
			HWND hListView = GetDlgItem(hwnd,IDC_VAR_EDIT_LIST);
			int ItemIndex = -1;
			ItemIndex = ListView_GetNextItem(hListView,ItemIndex,LVIS_SELECTED);
			if (ItemIndex != -1)
			{
				// ищем переменную по данному индексу
				for (std::size_t i = 0; i < VarEditor_VarsAndIndex.size(); i++)
				{
					// ежели переменная нашлась
					if (VarEditor_VarsAndIndex[i].second == ItemIndex)
					{
						CurrentInsertVarUnit.Name = VarEditor_VarsAndIndex[i].first.Name;
						CurrentInsertVarUnit.Value = VarEditor_VarsAndIndex[i].first.Value;
						if (DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_INSERT_VAR),hwnd,VarInsertProc))
						{
							// просматриваем на дубликат
							for (tVarEditor_VarsAndIndex::iterator iter = VarEditor_VarsAndIndex.begin();
								iter != VarEditor_VarsAndIndex.end(); iter++)
							{
								if (CurrentInsertVarUnit.Name == iter->first.Name)
								{
									if (CurrentInsertVarUnit.Name != VarEditor_VarsAndIndex[i].first.Name)
									{
										MessageBox(hwnd,TEXT("Переменная с таким именем уже существует!"),TEXT("Ошибка"),MB_OK | MB_ICONERROR);
										return TRUE;
									}
								}
							}
							// обновляем значение в памяти
							VarEditor_VarsAndIndex[i].first.Name = CurrentInsertVarUnit.Name;
							VarEditor_VarsAndIndex[i].first.Value = CurrentInsertVarUnit.Value;
							// обновляем отображение
							ListView_SetItemText(hListView,ItemIndex,0,(LPTSTR)VarEditor_VarsAndIndex[i].first.Name.c_str());
							ListView_SetItemText(hListView,ItemIndex,1,(LPTSTR)VarEditor_VarsAndIndex[i].first.Value.c_str());
						}
						break;
					}
				}
			}
		}
	}
	return TRUE;
}

INT_PTR WINAPI VarEditorProcClose(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hwnd, 0);
	return TRUE;
}

INT_PTR WINAPI VarInsertProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_INITDIALOG:
		{			
			return VarInsertProcInit(hwnd,wParam,lParam);
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_INSERT_VAR_OK:
				{
					return VarInsertProcClickOk(hwnd,wParam,lParam);
				}
				case IDC_INSERT_VAR_CANCEL:
				{
					return VarInsertProcClickCancel(hwnd,wParam,lParam);
				}
			}
			break;
		}
		case WM_CLOSE:
		{
			return VarInsertProcClose(hwnd,wParam,lParam);
		}
	}

	return FALSE;
}

INT_PTR WINAPI VarInsertProcInit(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	if (!CurrentInsertVarUnit.Name.empty())
		SetWindowText(GetDlgItem(hwnd,IDC_INSERT_VAR_NAME),CurrentInsertVarUnit.Name.c_str());
	if (!CurrentInsertVarUnit.Value.empty())
		SetWindowText(GetDlgItem(hwnd,IDC_INSERT_VAR_VALUE),CurrentInsertVarUnit.Value.c_str());
	return TRUE;
}

INT_PTR WINAPI VarInsertProcClickOk(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	int NameTextLen, ValueTextLen;
	TCHAR *NameBuffer, *ValueBuffer;
	HWND 
		hNameTextBox = GetDlgItem(hwnd,IDC_INSERT_VAR_NAME),
		hValueTextBox = GetDlgItem(hwnd,IDC_INSERT_VAR_VALUE);

	NameTextLen = GetWindowTextLength(hNameTextBox);
	ValueTextLen = GetWindowTextLength(hValueTextBox);
	if (!NameTextLen || !ValueTextLen)
	{
		MessageBox(hwnd,TEXT("Заполните оба поля"),TEXT("Ошибка"),MB_OK | MB_ICONEXCLAMATION);
		return TRUE;
	}
	// включаем в длину ноль в конце
	NameTextLen++;
	ValueTextLen++;
	NameBuffer = new TCHAR[NameTextLen];
	GetWindowText(hNameTextBox,NameBuffer,NameTextLen);
	for (std::size_t i = 0; i < (unsigned)NameTextLen; i++)
	{
		if (NameBuffer[i] == TEXT(' ') ||
			NameBuffer[i] == TEXT('\t'))
		{
			MessageBox(hwnd,TEXT("Переменная не должна содержать разделители!"),TEXT("Ошибка"),MB_OK | MB_ICONEXCLAMATION);
			delete [] NameBuffer;
			return TRUE;
		}
	}
	CurrentInsertVarUnit.Name.assign(NameBuffer);
	delete [] NameBuffer;
				
	ValueBuffer = new TCHAR[ValueTextLen];
	GetWindowText(hValueTextBox,ValueBuffer,ValueTextLen);
	CurrentInsertVarUnit.Value.assign(ValueBuffer);
	delete [] ValueBuffer;		
	EndDialog(hwnd,1);
	return TRUE;
}

INT_PTR WINAPI VarInsertProcClickCancel(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hwnd,0);
	return TRUE;
}

INT_PTR WINAPI VarInsertProcClose(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	EndDialog(hwnd, 0);
	return TRUE;
}