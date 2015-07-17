#include "OverlappedWindow.h"
#include <iostream>

#define ID_EDITCHILD 100

const wchar_t* const COverlappedWindow::className = L"Шарики";

bool COverlappedWindow::RegisterClass( HINSTANCE hInstance ) {
		WNDCLASSEX tag;
		tag.cbSize        = sizeof( WNDCLASSEX );
		tag.style         = CS_HREDRAW | CS_VREDRAW;
		tag.lpfnWndProc   = windowProc;
		tag.cbClsExtra    = 0;
		tag.cbWndExtra    = 0;
		tag.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
		tag.hCursor       = LoadCursor( NULL, IDC_ARROW );
		tag.hbrBackground = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
		tag.lpszMenuName  = NULL;
		tag.lpszClassName = className;
		tag.hInstance     = hInstance;
		tag.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );

		CEllipseWindow::RegisterClass( hInstance );

		bool result = RegisterClassEx( &tag ) != 0;
		return result;
}

void COverlappedWindow::OnDestroy() {
	PostQuitMessage( WM_QUIT );
}

bool COverlappedWindow::Create( int x, int y, int width, int heigth ) {
	int borderHeigth = GetSystemMetrics( SM_CXSIZE ) + GetSystemMetrics( SM_CXBORDER ) + 2;
	int borderWidth = 16;
	handle = ::CreateWindowEx( 0, 
							L"Шарики", 
							className, 
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
							x, 
							y, 
							width + borderWidth, 
							heigth + borderHeigth, 
							0, 
							0, 
							GetModuleHandle( NULL ), 
							this );
	
	bool r = wnd.Create( 0, 0, width, heigth );

	bool result = handle != 0 && r;
	
	return result;
}

void COverlappedWindow::Show( int cmdShow ) {
	ShowWindow( handle, cmdShow );
	wnd.Show( cmdShow );
}

LRESULT __stdcall COverlappedWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam ) {
	COverlappedWindow* ptr = reinterpret_cast<COverlappedWindow*>( ::GetWindowLong( handle, GWL_USERDATA ) );
	
	switch ( message ) {
		
		case WM_NCCREATE:
			{
				COverlappedWindow* that = reinterpret_cast<COverlappedWindow*>( reinterpret_cast<LPCREATESTRUCT>( lParam )->lpCreateParams );
				::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>( that ) );
				//that->handle = handle;
				return ::DefWindowProc( handle, message, wParam, lParam );
			}

		case WM_DESTROY:
			ptr->OnDestroy();
			break;

		default:
			return ::DefWindowProc( handle, message, wParam, lParam );
	}
}
