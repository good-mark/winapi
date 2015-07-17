#include <Windows.h>
#include "CEllipseWindow.h"

class COverlappedWindow {
public:
	COverlappedWindow() : handle( 0 ) {}
	//~COverlappedWindow();

	// ���������������� ����� ����
	static bool RegisterClass( HINSTANCE hInstance );

	// ������� ��������� ����
	bool Create( int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = 2 * UNIT_SIZE * WIDTH, int heigth = 2 * UNIT_SIZE * HEIGHT );
	// �������� ����
	void Show( int cmdShow );

protected:
	void OnDestroy();

private:
	static const wchar_t* const className;
	HWND handle; // ����� ����
	CEllipseWindow wnd;
	HWND handle1;
	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};
