#include "Balls.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow ) {

	Balls::RegisterClass( hInstance );
	Balls wnd;

	wnd.Create();
	wnd.Show( nCmdShow );
	MSG Msg;
	
	while ( GetMessage( &Msg, NULL, 0, 0 ) ) {
             TranslateMessage( &Msg );
             DispatchMessage( &Msg );
	}
	
	return Msg.wParam;
}