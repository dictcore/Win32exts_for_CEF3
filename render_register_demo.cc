
void OnContextCreated() {

	CefRefPtr<CefV8Value> windowObject = context->GetGlobal();

	//
	// To register win32exts, just call RegisterActiveXObject_CEF() API.
	//
	typedef bool (* RegisterActiveXObject_CEF_T)(CefV8Value *windowObject, LPVOID lpReserved1, LPVOID lpReserved2);
	typedef void (* SetRemoteBrowser_CEF_T)(HWND hWnd);

	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	PathRemoveFileSpecW(szPath);
	PathAppendW(szPath, L"win32exts_web.dll");
	HMODULE hDll = LoadLibraryW(szPath);
	if (hDll){
		RegisterActiveXObject_CEF_T pfnRegisterActiveXObject_CEF =
			(RegisterActiveXObject_CEF_T)GetProcAddress(hDll, "RegisterActiveXObject_CEF");

		if (pfnRegisterActiveXObject_CEF){
			pfnRegisterActiveXObject_CEF(windowObject, NULL, NULL);
		}

		//
  		// Set the browser window handle for ActiveX Control.
  		//
 	 	SetRemoteBrowser_CEF_T pfnSetRemoteBrowser_CEF =
   			 (SetRemoteBrowser_CEF_T)GetProcAddress(hDll, "SetRemoteBrowser_CEF");
 		 if (pfnSetRemoteBrowser_CEF) {
    			pfnSetRemoteBrowser_CEF(g_hRemoteBrowser);
 	 	}
	}
}


/***************************************************************

除原来的 ActiveXObject 之外， 新增对 ActiveX 控件的支持： 
创建：
var activex_ctrl = new ActiveXControl("prog_id", parent_wnd, x, y, r, b)

（parent_wnd 非常重要，将直接影响控件风格。
如果指定 0，将在一个弹出窗口创建；如果制定一个负数，则将优先选择
引擎传入的 SetRemoteBrowser( hwnd ) 浏览器句柄。）

每个控件都含有下列几个 Ax_*** 开头的内置的成员函数：
ptr = activex_ctrl.Ax_GetRaw()
pUnk = activex_ctrl.Ax_GetHost()
pDisp = activex_ctrl.Ax_GetControl()
hwnd = activex_ctrl.Ax_GetWindow()
activex_ctrl.Ax_ShowWindow( nCmdShow )
activex_ctrl.Ax_MoveWindow( [z_order] or [x, y] or [x, y, r, b] )
strInfo = activex_ctrl.Ax_ListSym()

一个控件有哪些方法、属性可以通过 Ax_ListSym() 函数查看，当然也可以用其他工具。
例如笔者开发的 90坦克大战 游戏控件：
----Function----
QueryInterface=1610612736(1)
AddRef=1610612737(1)
Release=1610612738(1)
GetTypeInfoCount=1610678272(1)
GetTypeInfo=1610678273(1)
GetIDsOfNames=1610678274(1)
Invoke=1610678275(1)
SetOwnSpeed=1610809344(0)
GetOwnSpeed=1610809345(0)
SetTotalCount=1610809346(0)
GetTotalCount=1610809347(0)
SetKeyboard=1610809348(0)
GetKeyboard=1610809349(0)
SetMusic=1610809350(0)
StartGame=1610809351(0)
EndGame=1610809352(0)
GetCurCount=1610809353(0)
GetCurScore=1610809354(0)
GetCurLevel=1610809355(0)
Execute=1610809356(0)
System=1610809357(0)
GetHelp=1610809358(0)
GetAbout=1610809359(0)
GetAppPath=1610809360(0)
InputMsgBox=1610809361(0)


调用控件方法：
ret = activex_ctrl.FunctionName( args ... )
获取属性：
ret = activex_ctrl.get_AttributeName()
修改属性：
activex_ctrl.put_AttributeName( new_val )


*************************************************************/
