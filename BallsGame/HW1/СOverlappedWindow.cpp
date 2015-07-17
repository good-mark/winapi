#include "COverlappedWindow.h"

bool COverlappedWindow::RegisterClass( HINSTANCE hInstance ) {
		WNDCLASSEX tag;
		tag.cbSize        = sizeof( WNDCLASSEX );
		tag.style         = CS_HREDRAW | CS_VREDRAW;
		tag.lpfnWndProc   = windowProc;
		tag.cbClsExtra    = 0;
		tag.cbWndExtra    = 0;
		tag.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
		tag.hCursor       = LoadCursor( NULL, IDC_ARROW );
		tag.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
		tag.lpszMenuName  = NULL;
		tag.lpszClassName = L"ClassName";
		tag.hInstance     = hInstance;
		tag.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
		RegisterClassEx( &tag );
		return 0;
}

// возвращает 0, если не удалось создать
bool COverlappedWindow::Create( HINSTANCE hInstance ) {
	handle = ::CreateWindowEx( 0,
			  L"ClassName",
			  L"Name of window",
			  WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  NULL,
			  NULL,
			  hInstance,
			  this );
	//SetWindowLong( handle, GWL_USERDATA, (LONG)this );
	return handle != 0;
}

void COverlappedWindow::Show(int pCmdLine) {
	ShowWindow(handle, pCmdLine);
}

void COverlappedWindow::OnDestroy() {
	::KillTimer( handle, TIMER_ID );
	PostQuitMessage(WM_QUIT);
}


void COverlappedWindow::OnTimer() {
	RECT clientRect;
	::GetClientRect( handle, &clientRect );
	::InvalidateRect( handle, &clientRect, FALSE );
}

void COverlappedWindow::OnPaint() {
		RECT clientRect;
		HDC memoryDc;
		HBITMAP memBitmap;
		HGDIOBJ oldBmp;
		PAINTSTRUCT ps; 
		HDC hdc;
		HRGN bgRgn;
		HBRUSH hBrushBlue, hBrushYellow;
		++time;
		hdc = ::BeginPaint( handle, &ps ); 
		hBrushBlue = ::CreateSolidBrush( RGB(0,162,255) ); //син€€ кисть
		hBrushYellow = ::CreateSolidBrush( RGB(255,255,0) );//желта€ кисть
		::GetClientRect( handle, &clientRect );
		bgRgn = ::CreateRectRgnIndirect( &clientRect );
		memoryDc = ::CreateCompatibleDC( hdc );
		memBitmap = ::CreateCompatibleBitmap( hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top );
		oldBmp = ::SelectObject( memoryDc, memBitmap );
		::FillRgn( memoryDc, bgRgn, hBrushBlue );//синий фон
		::SelectObject( memoryDc, hBrushYellow ); // жЄлтый круг
		leftCoordinateOfEllipse += dx;
		topCoordinateOfEllipse += dy;
		if (leftCoordinateOfEllipse + 40 >= clientRect.right) {
			dx = -dx;
		}
		if (topCoordinateOfEllipse <= clientRect.top) {
			dy = -dy;
		}
		if (leftCoordinateOfEllipse <= clientRect.left) {
			dx = -dx;
		}
		if (topCoordinateOfEllipse + 40 >= clientRect.bottom) {
			dy = -dy;
		}
		//leftCoordinateOfEllipse = (clientRect.right - clientRect.left)/3 * sin(  0.01 * time ) + (clientRect.right - clientRect.left)/2;
		//topCoordinateOfEllipse = (clientRect.bottom - clientRect.top)/3 * cos ( 0.01 * time ) + (clientRect.bottom - clientRect.top)/2; 
		::Ellipse(memoryDc, leftCoordinateOfEllipse, topCoordinateOfEllipse, leftCoordinateOfEllipse + 40, topCoordinateOfEllipse + 40); 
		::BitBlt( hdc, 
				0,0, 
				clientRect.right-clientRect.left, clientRect.bottom-clientRect.top, 
				memoryDc, 
				0,0,
				SRCCOPY );
		::SelectObject( memoryDc, oldBmp );
		::DeleteObject( hBrushBlue );
		::DeleteObject( hBrushYellow );
		::DeleteObject( memBitmap );
		::DeleteObject( bgRgn );
		::DeleteDC( memoryDc );
		::EndPaint( handle, &ps ); 
}

LRESULT __stdcall COverlappedWindow::windowProc( HWND _handle, UINT message, WPARAM wParam, LPARAM lParam ) {
		RECT clientRect;
		COverlappedWindow* ptr;
		ptr = reinterpret_cast<COverlappedWindow*>( ::GetWindowLong( _handle, GWL_USERDATA ) );
		switch ( message ) {
			case WM_NCCREATE:
				{
					COverlappedWindow* that = reinterpret_cast<COverlappedWindow*>( reinterpret_cast<LPCREATESTRUCT>( lParam )->lpCreateParams );
					::SetWindowLong(_handle, GWL_USERDATA, reinterpret_cast<LONG>( that ) );
					::DefWindowProc( _handle, message, wParam, lParam );
					::SetTimer( _handle, TIMER_ID, 0.01, NULL );
					break;
				}
			case WM_TIMER:
				ptr->OnTimer();
				break;

			case WM_DESTROY:
				ptr->OnDestroy();
				break;

			case WM_PAINT:
				ptr->OnPaint();
				break;

			default:
				return ::DefWindowProc( _handle, message, wParam, lParam );
		}
		return ::DefWindowProc( _handle, message, wParam, lParam );
}