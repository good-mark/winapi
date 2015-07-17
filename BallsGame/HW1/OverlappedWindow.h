#include <Windows.h>
#include "CEllipseWindow.h"

class COverlappedWindow {
public:
	COverlappedWindow() : handle( 0 ) {}
	//~COverlappedWindow();

	// Зарегистрировать класс окна
	static bool RegisterClass( HINSTANCE hInstance );

	// Создать экземпляр окна
	bool Create( int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = 2 * UNIT_SIZE * WIDTH, int heigth = 2 * UNIT_SIZE * HEIGHT );
	// Показать окно
	void Show( int cmdShow );

protected:
	void OnDestroy();

private:
	static const wchar_t* const className;
	HWND handle; // хэндл окна
	CEllipseWindow wnd;
	HWND handle1;
	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};
