// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "maindlg.h"
#include "Dlg2Side.h"

#include <tchar.h>
#include <cctype>
#include <atlcoll.h>
#include <atlconv.h>

using namespace ATL;

#define IDM_TOP_WIN  102


struct TComboEntry
{
	CString str;
	int     nData; //自定义序号
};

static
TComboEntry g_cmbEntries[] = {
	TComboEntry{_T("两边添加"), 0},
	TComboEntry{_T("\\转义"),   1},
	TComboEntry{_T("反\\转义"), 2},
	TComboEntry{_T("处理UTF-8 URL的%XX"),  3},
	TComboEntry{_T("to linux slash /"),    4},
	TComboEntry{_T("to windows slash \\"), 5},
	TComboEntry{_T("to mingW path"),   6},
	TComboEntry{_T("from mingW path"), 7},
	TComboEntry{_T("html转义( < ---> &lt; )"),   8},
	TComboEntry{_T("html反转义( &lt; ---> < )"), 9},
};

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();
	InitDynamicLayout(false);

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	CMenu menuSys(GetSystemMenu(FALSE));
	//menuSys.AppendMenu(MF_STRING, IDM_TOP_WIN, _T("窗口置顶"));
	menuSys.InsertMenu(0, MF_BYPOSITION | MF_STRING, IDM_TOP_WIN, _T("窗口置顶"));
	menuSys.Detach(); //这里不人工detach的话，菜单会被清除!？
	PostMessage(WM_SYSCOMMAND, IDM_TOP_WIN, 0);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	// 		pLoop->AddMessageFilter(this);
	// 		pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	m_richSrc = GetDlgItem(IDC_RICH_SRC);
	m_richTar = GetDlgItem(IDC_RICH_TAR);

	m_cmbOperType.Attach(GetDlgItem(IDC_CMB_TYPE));
	m_cmbOperType.ModifyStyle(CBS_SORT, CBS_AUTOHSCROLL);
	m_cmbOperType.Clear();
	for (int i = 0; i < _countof(g_cmbEntries); i++)
	{
		const TComboEntry& entry = g_cmbEntries[i];
		int nRet = m_cmbOperType.AddString(entry.str);
		if (nRet != CB_ERR)
		{
			m_cmbOperType.SetItemData(nRet, (DWORD_PTR)entry.nData);
		}
	}

	m_cmbOperType.SetCurSel(0);
	
	return TRUE;
}

LRESULT CMainDlg::OnSysCommand(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
{
	USHORT nSysCmd = HIWORD(wParam);
	USHORT nID = LOWORD(wParam);
	if (IDM_TOP_WIN == nID)
	{
		static bool bTopMost = false;
		bTopMost = !bTopMost;
		CMenu menuSys(GetSystemMenu(FALSE));
		int nCnt = menuSys.GetMenuItemCount();
		UINT nCmd = MF_BYCOMMAND;
		nCmd |= (bTopMost ? MF_CHECKED : MF_UNCHECKED);
		menuSys.CheckMenuItem(IDM_TOP_WIN, nCmd);
		menuSys.Detach(); //!
		SetWindowPos(bTopMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOREDRAW | SWP_NOSIZE | SWP_NOMOVE);
	}
	bHandled = false;
	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnConverty(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoConvert();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//EndDialog(wID);  //for DoModel
	DestroyWindow(); //for ShowWindow
	::PostQuitMessage(wID);
	return 0;
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_COMMAND)
	{
		ATLTRACE("WM_COMMAND*");
	}
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

void CMainDlg::DoConvert()
{
	int nSel = m_cmbOperType.GetCurSel();
	if (nSel < 0)
	{
		return;
	}
	int nData = (int)m_cmbOperType.GetItemData(nSel);
	switch (nData)
	{
	case 0:
	{
		//两边添加
		Do2SideAdd();
	}
	break;
	case 1:
	{
		//\\转义
		DoSlashEscape();
	}
	break;
	case 2:
	{
		//反\\转义
		DoSlashUnEscape();
	}
	break;
	case 3:
	{
		//处理UTF-8 URL的%XX
		DoURLHexUnEscape();
	}
	break;
	case 4:
	{
		//to linux slash /
		DoToLinuxSlash();
	}
	break;
	case 5:
	{
		//to windows slash \\ 
		DoToWindowSlash();
	}
	break;
	case 6:
	{
		//to mingW path
		DoToMingWPath();
	}
	break;
	case 7:
	{
		//from mingW path
		DoFromMingWPath();
	}
	break;
	case 8:
	{
		DoEscapeHtmlChar();
	}
	break;
	case 9:
	{
		DoUnEscapeHtmlChar();
	}
	break;
	default:
	{
		TCHAR szBuf[128];
		_stprintf_s(szBuf, _T("* unhandle type: %d"), nData);
		OutputDebugString(szBuf);
	}
	break;
	}
}

std::unique_ptr<CSimpleArray<CString>> CMainDlg::GetLines()
{
	TCHAR* pBuf;
	int nLen = m_richSrc.GetWindowTextLength();
	pBuf = new TCHAR[nLen+2];
	//m_richSrc.GetWindowText(pBuf, nLen);
	m_richSrc.GetTextEx(pBuf, nLen+1, GT_USECRLF, CP_WINUNICODE);
	pBuf[nLen]=NULL;
	CString strText(pBuf);
	delete[] pBuf;

	std::unique_ptr<CSimpleArray<CString>> pstrArr = std::make_unique<CSimpleArray<CString>>();
	//CSimpleArray<CString>* pstrArr = new CSimpleArray<CString>();
	const TCHAR* pstrBuf = (LPCTSTR)strText;
	const TCHAR* pstrEnd = pstrBuf+strText.GetLength();
	while(pstrBuf<pstrEnd) {
		const TCHAR* pNext = NULL;
		int  line_len = 0;
		int nDivLen = 0;
		do {
			pNext = _tcsstr(pstrBuf, _T("\r\n"));
			if (pNext) { nDivLen = 2; break; }
			pNext = _tcsstr(pstrBuf, _T("\n"));
			if (pNext) { nDivLen = 1; break; }
		} while (0);
		
		if (pNext) {
			line_len = pNext - pstrBuf;
		}
		else {
			line_len = _tcslen(pstrBuf);
		}
		CString strTemp(pstrBuf, line_len);
		pstrArr->Add(strTemp);

		pstrBuf += line_len + nDivLen;
	}

	return pstrArr;
}

//进行逐行处理
void CMainDlg::Do2SideAdd()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}

	CDlg2Side dlgSetting;
	if (IDOK != dlgSetting.DoModal()) {
		return;
	}

	stSetting2Side setting = dlgSetting.GetSetting();
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line( (*pLines)[i]);
		
		if (setting.bTrimLeftRight) {
			line.TrimLeft();
			line.TrimRight();
		}
		if (setting.bDelEmpty) {
			if (!setting.bTrimLeftRight) 
			{ 
				line.TrimLeft(); 
				line.TrimRight();
			}
			if (line.IsEmpty())
			{
				continue; //!
			}
		}
		if (!setting.strLeft.IsEmpty()) {
			line = setting.strLeft + line;
		}
		if (!setting.strRight.IsEmpty()) {
			line += setting.strRight;
		}
		if (!setting.strReplacedCrlf.IsEmpty()) {
			line += setting.strReplacedCrlf;
		}
		if (setting.bReplacedCrlf) {
			line += _T("\r\n");
		}

		m_richTar.AppendText(line, FALSE);
		//m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}


static
CString StrEscapeBackslashQuotable(CString sin)
{
	CString sOut;
	const int nLength = sin.GetLength();
	for (int iIn = 0; iIn < nLength; iIn++)
	{
		if (sin[iIn] == '\0')
		{
			sOut.AppendChar(_T('\\'));
			sOut.AppendChar(_T('0'));
		}
		else if (sin[iIn] == '\n')
		{
			sOut.AppendChar(_T('\\'));
			sOut.AppendChar(_T('n'));
		}
		else if (sin[iIn] == '"')
		{
			sOut.AppendChar(_T('\\'));
			sOut.AppendChar(_T('\"'));
		}
		else if (sin[iIn] == '\\')
		{
			sOut.AppendChar(_T('\\'));
			sOut.AppendChar(_T('\\'));
		}
		else
		{
			sOut.AppendChar(sin[iIn]);
		}
	}
	return sOut;
}

void CMainDlg::DoSlashEscape()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		line=StrEscapeBackslashQuotable(line);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

static
CString StrUnEscapeBackslashQuotable(CString sin)
{
	CString sOut;
	const int nLength = sin.GetLength();
	for (int iIn = 0; iIn < nLength; iIn++)
	{
		if (sin[iIn] == _T('\\'))
		{
			++iIn;
			if (sin[iIn] == _T('\0'))
			{
				break;
			}
			if (sin[iIn] == _T('n'))
			{
				sOut.AppendChar(_T('\n'));
			}
			else if (sin[iIn] == '0')
			{
				sOut.AppendChar(_T('\0'));
			}
			else
			{
				sOut.AppendChar(sin[iIn]);
			}
		}
		else
		{
			sOut.AppendChar(sin[iIn]);
		}
	}

	return sOut;
}

void CMainDlg::DoSlashUnEscape()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		line = StrUnEscapeBackslashQuotable(line);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

static
uint8_t HexChar(TCHAR c)
{
	if (_T('0') <= c && c <= _T('9')) return (uint8_t)(c - _T('0'));
	if (_T('A') <= c && c <= _T('F')) return (uint8_t)(c - _T('A') + 10);
	if (_T('a') <= c && c <= _T('f')) return (uint8_t)(c - _T('a') + 10);
	return (uint8_t)(-1);
}


static
void HexstrToBytes(const TCHAR* pHexStr, size_t nLen, CAtlArray<uint8_t>& arrOut)
{
	const TCHAR* pstrEnd = pHexStr + nLen;
	bool bExitOut = false;
	for (const TCHAR* ptr = pHexStr; ptr < pstrEnd; )
	{
		while (_istspace(*ptr))
		{
			ptr++;
			if (ptr >= pstrEnd)
			{
				bExitOut = true;
				break;
			}
		}
		if (bExitOut) { break; }

		uint8_t by = HexChar(ptr[0]) << 4 | HexChar(ptr[1]);
		arrOut.Add(by);
		ptr += 2;
	}
}


//处理连续的%xx拼成UTF-8字符串
static
CString UnescapeUrlStr(const TCHAR* pstrIn)
{
	CString str;
	const TCHAR DIV_CHAR = _T('%');

	const TCHAR* pstr = pstrIn;
	const TCHAR* pstrEnd = pstr + _tcslen(pstr);

	while (pstr < pstrEnd)
	{
		if (*pstr != DIV_CHAR)
		{
			str.AppendChar(*pstr);
			pstr++;
		}
		else {
			CAtlArray<uint8_t> qba;
			//连续处理%xx
			do {
				const int MAX_HEX = 2;
				TCHAR szHex[MAX_HEX + 1] = { 0 };
				pstr++;
				if (pstr >= pstrEnd)
				{
					break;
				}
				int i = 0;
				while (_istxdigit(*pstr))
				{
					szHex[i++] = *pstr++;
					if (i > MAX_HEX)
					{
						break;
					}
				}
				if (i % 2)
				{
					//如果是奇数个的话，回退一个
					i--;
					szHex[i] = 0;
					pstr--;
				}
				HexstrToBytes(szHex, i, qba);
				
			} while (*pstr == DIV_CHAR);

			qba.Add(0);
			//USES_CONVERSION_EX;
			//const wchar_t* pwstr = A2CW_CP_EX((char*)qba.GetData(), qba.GetCount(), CP_UTF8);
			
			str.Append(CA2W((char*)qba.GetData(), CP_UTF8));
		}
	}//while

	return str;
}


void CMainDlg::DoURLHexUnEscape()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		line = UnescapeUrlStr(line);		
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}


static
void StrReplacePathSep(CString& str, bool bToWin)
{
	const char chSepSrc = bToWin ? _T('/') : _T('\\');
	const char chSepTar = bToWin ? _T('\\') : _T('/');
	for (size_t i = 0; i < str.GetLength(); i++)
	{
		auto ch = str[i];
		if (chSepSrc == ch)
		{
			str.SetAt(i, chSepTar);
		}
	}
}

void CMainDlg::DoToLinuxSlash()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		StrReplacePathSep(line, false);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

void CMainDlg::DoToWindowSlash()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		StrReplacePathSep(line, true);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}


static
CString StrToMingwPath(CString str)
{
	const TCHAR chSepSrc = _T('\\');
	const TCHAR chSepTar = _T('/');
	CString sRet;

	sRet.AppendChar(chSepTar);
	for (size_t i = 0; i < str.GetLength(); i++)
	{
		TCHAR ch = str[i];
		if (chSepSrc == ch)
		{
			ch = chSepTar;
		}
		else if (_T(':') == ch)
		{
			continue;
		}
		sRet.AppendChar(ch);
	}
	return sRet;
}

static
CString StrFromMingwPath(CString str)
{
	const TCHAR chSepSrc = _T('/');
	const TCHAR chSepTar = _T('\\');
	CString sRet;

	bool bFirstSep = false;
	for (size_t i = 1; i < str.GetLength(); i++)
	{
		TCHAR ch = str[i];
		if (chSepSrc == ch)
		{
			ch = chSepTar;
			if (!bFirstSep)
			{
				bFirstSep = true;
				sRet.AppendChar(_T(':'));
			}
		}
		sRet.AppendChar(ch);
	}
	return sRet;
}


void CMainDlg::DoToMingWPath()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}

	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		line = StrToMingwPath(line);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

void CMainDlg::DoFromMingWPath()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}

	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		line = StrFromMingwPath(line);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

static void output_html_string(FILE* out, const char* z) {
	int i;
	while (*z) {
		for (i = 0;   z[i]
			&& z[i] != '<'
			&& z[i] != '&'
			&& z[i] != '>'
			&& z[i] != '\"'
			&& z[i] != '\'';
			i++) {
		}
		if (i > 0) {
			fprintf(out, "%.*s", i, z);
		}
		if (z[i] == '<') {
			fprintf(out, "&lt;");
		}
		else if (z[i] == '&') {
			fprintf(out, "&amp;");
		}
		else if (z[i] == '>') {
			fprintf(out, "&gt;");
		}
		else if (z[i] == '\"') {
			fprintf(out, "&quot;");
		}
		else if (z[i] == '\'') {
			fprintf(out, "&#39;");
		}
		else {
			break;
		}
		z += i + 1;
	}
}


struct THtmlCharMapping
{
	TCHAR    ch;
	CString  sch;
};

static
const THtmlCharMapping g_arrHtmlCharMapping[] = {
	{_T('<'), _T("&lt;")},
	{_T('>'), _T("&gt;")},
	{_T('&'), _T("&amp;")},	
	{_T('\"'), _T("&quot;")},
	{_T('\''), _T("&#39;")},
};

static
CString StrEscapeHtmlChar(CString sin)
{
	CString sOut;
	const int nLength = sin.GetLength();
	for (int iIn = 0; iIn < nLength; iIn++)
	{
		bool bReplaced = false;
		for (int i = 0; i < _countof(g_arrHtmlCharMapping); i++)
		{
			const THtmlCharMapping& rHtml = g_arrHtmlCharMapping[i];
			if (rHtml.ch == sin[iIn])
			{
				sOut.Append(rHtml.sch);
				bReplaced = true;
				break;
			}
		}
		if(!bReplaced)
		{
			sOut.AppendChar(sin[iIn]);
		}
	}
	return sOut;
}

static
void StrUnEscapeHtmlChar(CString& str)
{
	TCHAR szTemp[2] = { 0 };
	for (int i = 0; i < _countof(g_arrHtmlCharMapping); i++)
	{
		const THtmlCharMapping& rHtml = g_arrHtmlCharMapping[i];
		szTemp[0] = rHtml.ch;
		str.Replace(rHtml.sch, szTemp);
	}
}

void CMainDlg::DoEscapeHtmlChar()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}

	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		line = StrEscapeHtmlChar(line);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

void CMainDlg::DoUnEscapeHtmlChar()
{
	std::unique_ptr<CSimpleArray<CString>> pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		return;
	}

	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line((*pLines)[i]);

		line.TrimLeft();
		line.TrimRight();
		if (line.IsEmpty())
		{
			continue; //!
		}

		StrUnEscapeHtmlChar(line);
		m_richTar.AppendText(line, FALSE);
		m_richTar.AppendText(_T("\r\n"), FALSE);
	}
}

