#ifndef _ADDITIONALWNDMSGS_H_
#define _ADDITIONALWNDMSGS_H_

#define WM_CHANGE_RANGING WM_USER + 1 // wParam - не используется, lParam  - не используется
#define WM_GET_TRANS_PARAMS WM_USER + 2 // wParam - *ConfigParams, lParam  - sizeof(ConfigParams)
#define WM_SET_TRANS_PARAMS WM_USER + 3 // wParam - *ConfigParams, lParam  - sizeof(ConfigParams)
#define WM_VIEW_ADD_PARAMS WM_USER + 4 // wParam - не используется, lParam  - не используется
#define WM_VIEW_TEMP_MODEL WM_USER + 5 // wParam - не используется, lParam  - не используется

#endif