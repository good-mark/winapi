#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow() : handle(0) {}
	~COverlappedWindow() {}

	// Зарегистрировать класс окна
	static bool RegisterClass( HINSTANCE hInstance );

	// Создать экземпляр окна
	bool Create( HINSTANCE hInstance );

	// Показать окно
	void Show( int cmdShow );

protected:
	void OnDestroy();

private:
	HWND handle; // хэндл окна

	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};
