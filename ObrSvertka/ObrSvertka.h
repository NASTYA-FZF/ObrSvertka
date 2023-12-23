
// ObrSvertka.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CObrSvertkaApp:
// Сведения о реализации этого класса: ObrSvertka.cpp
//

class CObrSvertkaApp : public CWinApp
{
public:
	CObrSvertkaApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CObrSvertkaApp theApp;
