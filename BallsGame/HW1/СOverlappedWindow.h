#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow() : handle(0) {}
	~COverlappedWindow() {}

	// ���������������� ����� ����
	static bool RegisterClass( HINSTANCE hInstance );

	// ������� ��������� ����
	bool Create( HINSTANCE hInstance );

	// �������� ����
	void Show( int cmdShow );

protected:
	void OnDestroy();

private:
	HWND handle; // ����� ����

	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};
