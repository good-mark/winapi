#include "Balls.h"
#include <stdlib.h>
#include <windowsx.h>
#include "resource.h"

const wchar_t* const Balls::className = L"Шарики";

void swap( ball& a, ball& b) {
	ball temp = a;
	a = b;
	b = temp;
}

void copy2DimensionArrays( ball first[WIDTH][HEIGHT], ball second[WIDTH][HEIGHT] ) {
	for ( int m = 0; m < WIDTH; ++m )
		for ( int n = 0; n < HEIGHT; ++n ) {
			first[m][n].x = second[m][n].x;
			first[m][n].y = second[m][n].y;
			first[m][n].c = second[m][n].c;
			first[m][n].isExist = second[m][n].isExist;
			first[m][n].dfs = second[m][n].dfs;
		}
}

void Balls::tryBackUp() {
	copy2DimensionArrays( backBackUp, balls ); 
}

void Balls::makeBackUp() {
	copy2DimensionArrays( backUp, backBackUp );
}

bool Balls::RegisterClass( HINSTANCE hInstance ) {
	HICON hIcon = reinterpret_cast<HICON>( LoadImage( hInstance, 
												MAKEINTRESOURCE( 115 ), 
												IMAGE_ICON, 
												GetSystemMetrics( SM_CXSMICON ), 
												GetSystemMetrics( SM_CYSMICON ), 
												0 ) );
	WNDCLASSEX tag;
	tag.cbSize        = sizeof( WNDCLASSEX );
	tag.style         = CS_HREDRAW | CS_VREDRAW;
	tag.lpfnWndProc   = windowProc;
	tag.cbClsExtra    = 0;
	tag.cbWndExtra    = 0;
	tag.hIcon         = hIcon;
	tag.hCursor       = LoadCursor( NULL, IDC_ARROW );
	tag.hbrBackground = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
	tag.lpszMenuName  = NULL;
	tag.lpszClassName = className;
	tag.hInstance     = hInstance;
	tag.hIconSm       = hIcon;

	bool result = RegisterClassEx( &tag ) != 0;
	return result;
}

bool Balls::Create( int x, int y, int width, int heigth ) {
	HMENU hMenu = LoadMenu( GetModuleHandle( NULL ), MAKEINTRESOURCE( IDR_MENU1 ) );
	int borderHeigth = GetSystemMetrics( SM_CXSIZE ) + 23;
	int borderWidth = 16;
	handle = ::CreateWindowEx( 0, 
						className, 
						L"Шарики", 
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
						x, 
						y, 
						width + borderWidth, 
						heigth + borderHeigth, 
						0, 
						hMenu, 
						GetModuleHandle(NULL), 
						this );
	bool result = handle != 0;
	return result;
}

void Balls::Show( int cmdShow ) {
	ShowWindow( handle, cmdShow );
	generateBalls();
}

void Balls::OnDestroy() {
	for ( int i = 0; i < sizeof brushes; ++i )
		::DeleteObject( brushes[i] );
	PostQuitMessage( WM_QUIT );
}

void Balls::OnClick( int x, int y ) {
	int i = x / radius;
	int j = y / radius;
	tryBackUp();
	balls[i][j].dfs = true;
	toDelete.push_back( &balls[i][j] );
	killNeighbours( i, j );
	InvalidateRect( handle, NULL, 0 );
}

void Balls::OnDelete( int i, int j ) {
	if ( toDelete.size() == 1 ) {
		balls[i][j].dfs = false;
		toDelete.clear();
	}
	else {
		makeBackUp();
		for ( int k = 0; k < toDelete.size(); ++k ) {
			toDelete[k]->isExist = false;
			toDelete[k]->dfs = false;
		}
		score += bonus( toDelete.size() );
		toDelete.clear();
		replaceBalls();
		InvalidateRect( handle, NULL, 0 );
		if ( checkIsFinal() )
			OnFinal();
	}
}

int Balls::bonus( int k ) {
	if ( k < 2 )
		return 0;
	if ( k < 3 )
		return k;
	if ( k < 6 )
		return k * 2;
	if ( k < 11 )
		return k * 3;
	if ( k < 15 )
		return k * 5;
	if ( k < 20 )
		return k * 7;
	else 
		return k * 10;
}

bool Balls::checkIsFinal() {
	for ( int i = WIDTH - 1; i > 0; --i )
		for ( int j = HEIGHT - 1; j > 0; --j ) 
			if ( balls[i][j].isExist ) {
				color c = balls[i][j].c;
				if ( balls[i-1][j].isExist && balls[i-1][j].c == c ||
					balls[i][j-1].isExist && balls[i][j-1].c == c )
					return false;
			}
	return true;
}

void Balls::OnFinal() {
	wchar_t sc[20];
	wsprintf( sc, L"Ваш счет: %d", score );
	::MessageBox( handle, sc, L"Конец игры!", MB_OK);
	generateBalls();
	InvalidateRect( handle, NULL, 0 );
}

void Balls::replaceBalls() {
	int i, j;
	for ( i = WIDTH - 1; i >= 0; --i )
		for ( j = HEIGHT - 1; j >= 0; --j ) {
			if ( balls[i][j].isExist ) {

				if ( i < WIDTH - 1 )
					if ( !balls[i + 1][j].isExist ) {
						std::swap( balls[i][j], balls[i + 1][j]);
						i += 2;
					}

				if ( j < HEIGHT - 1 )
					if ( !balls[i][j + 1].isExist ) {
						swap( balls[i][j], balls[i][j + 1]);
						j += 2;
					}
			}
		}
}

void Balls::deleteBall( int i, int j ) {
	balls[i][j].isExist = false;
	toDelete.push_back( &balls[i][j] );
	killNeighbours( i, j );
}

void Balls::killNeighbours( int i, int j ) {
	color c = balls[i][j].c;
	if ( i > 0 ) 
		if ( balls[i - 1][j].isExist && balls[i - 1][j].c == c && !balls[i - 1][j].dfs ) {
			deleteBall( i - 1, j );
		}
	if ( j > 0 ) 
		if ( balls[i][j - 1].isExist && balls[i][j - 1].c == c && !balls[i][j - 1].dfs ) {
			deleteBall( i, j - 1 );
		}
	if ( i < WIDTH - 1 )  
		if ( balls[i + 1][j].isExist && balls[i + 1][j].c == c && !balls[i + 1][j].dfs ) {
			deleteBall( i + 1, j );
		}
	if ( j < HEIGHT - 1 )
		if ( balls[i][j + 1].isExist && balls[i][j + 1].c == c && !balls[i][j + 1].dfs ) {
			deleteBall( i, j + 1 );
		}
}

void Balls::OnPaint() {
		RECT rect;
		PAINTSTRUCT ps; 
		
		HDC hdc = ::BeginPaint( handle, &ps ); 
		::GetClientRect( handle, &rect );
		
		HBITMAP memBitmap;
		HDC memoryDc;
		memBitmap = ::CreateCompatibleBitmap( hdc, radius, radius );
		memoryDc = ::CreateCompatibleDC( hdc );
		::SelectObject( memoryDc, memBitmap );

		for ( int i = 0; i < WIDTH; ++i )
			for ( int j = 0; j < HEIGHT; ++j ) {
				// random choise of color
				if ( balls[i][j].isExist ) {
					switch ( balls[i][j].c ) {
						case yellow:
							::SelectObject( memoryDc, brushes[1] );
							break;

						case red:
							::SelectObject( memoryDc, brushes[2] );
							break;

						case blue:
							::SelectObject( memoryDc, brushes[3] );
							break;

						case green:
							::SelectObject( memoryDc, brushes[4] );
							break;
					}
				}	
				else 
					::SelectObject( memoryDc, brushes[5] );
				::Ellipse( memoryDc, 0, 0, radius, radius ); 
				::BitBlt( hdc, balls[i][j].x, balls[i][j].y, radius, radius, memoryDc, 0, 0, SRCCOPY );
			}

		if ( toDelete.size() > 0 ) {
			for ( std::vector<ball*>::iterator b = toDelete.begin(); b != toDelete.end(); ++b ) {
				::SelectObject( memoryDc, brushes[0] );
				::Ellipse( memoryDc, 0, 0, radius, radius ); 
				::BitBlt( hdc, (*b)->x, (*b)->y, radius, radius, memoryDc, 0, 0, SRCCOPY );
				SetTextColor( hdc, RGB( 250, 250, 250 ) );
				SetBkMode( hdc, TRANSPARENT );
				wchar_t str[4];
				int sc = bonus( toDelete.size() );
				wsprintf( str, L"%d", sc );
				int len = 1;
				if ( sc / 10 )
					len++;
				if ( sc / 100 )
					len++;
				TextOut( hdc, 10, 0, str, len );
			}
		}

		::DeleteObject( memBitmap );
		::DeleteDC( memoryDc );
		::EndPaint( handle, &ps ); 
}

LRESULT __stdcall Balls::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam ) {

	Balls* ptr = reinterpret_cast<Balls*>( ::GetWindowLong( handle, GWL_USERDATA ) );
	switch ( message ) {
		case WM_NCCREATE:
			{				
				Balls* that = reinterpret_cast<Balls*>( reinterpret_cast<LPCREATESTRUCT>( lParam )->lpCreateParams );
				::SetWindowLong( handle, GWL_USERDATA, reinterpret_cast<LONG>( that ) );
				break;
			}

		case WM_LBUTTONUP:
			{
				int xPos = GET_X_LPARAM( lParam ) / radius; 
				int yPos = GET_Y_LPARAM( lParam ) / radius; 
				ptr->OnDelete( xPos, yPos );
				break;
			}

		case WM_LBUTTONDOWN:
			{ 
				::SetFocus( handle );
				int xPos = GET_X_LPARAM( lParam ); 
				int yPos = GET_Y_LPARAM( lParam ) ; 
				ptr->OnClick( xPos, yPos );
				break;
			}

		case WM_PAINT:
			ptr->OnPaint();
			break;

		case WM_DESTROY:
			ptr->OnDestroy();
			break;

		case WM_COMMAND:
			if ( HIWORD( wParam ) == 0 ) // menu
			{	
				switch (  LOWORD( wParam ) ) {
					case ID_RETURN:
						ptr->returnBack();
						break;

					case ID_NEW_GAME:
						ptr->generateBalls();
						ptr->score = 0;
						InvalidateRect( ptr->handle, 0, 0 );
						break;

					default:
						break;
				}
			}
			break;

		default:
			return ::DefWindowProc( handle, message, wParam, lParam );
	}

	return ::DefWindowProc( handle, message, wParam, lParam );
}

void Balls::generateBalls() {
	for ( int i = 0; i < WIDTH; ++i )
		for ( int j = 0; j < HEIGHT; ++j ) {
			balls[i][j].isExist = true;
			balls[i][j].dfs = false;
			balls[i][j].x = radius * i;
			balls[i][j].y = radius * j;
			switch ( rand() % 4 ) {
				case 0:
					balls[i][j].c = yellow;
					break;
				case 1:
					balls[i][j].c = red;
					break;
				case 2:
					balls[i][j].c = blue;
					break;
				case 3:
					balls[i][j].c = green;
					break;
			}
		}	
	copy2DimensionArrays( backUp, balls );
}

void Balls::returnBack() {
	copy2DimensionArrays( balls, backUp );
	InvalidateRect( handle, NULL, 0 );
}