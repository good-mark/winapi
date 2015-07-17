#include <Windows.h>
#include <vector>

#define HEIGHT 10
#define WIDTH 15
#define UNIT_SIZE 28
#define radius  (2*UNIT_SIZE)

enum color { yellow, red, blue, green };

typedef struct ball {
	int x;
	int y;
	color c;
	bool isExist;
	bool dfs;
	void operator=( const ball b ) {
		c = b.c;
		isExist = b.isExist;
		dfs = b.dfs;
	}
} ball;

class Balls {
public:
	Balls() : handle(0), score(0) {
		brushes[0] = ::CreateSolidBrush( RGB( 250, 250, 250 ) ); // white
		brushes[1] = ::CreateSolidBrush( RGB( 250, 244, 79 ) ); // yellow
		brushes[2] = ::CreateSolidBrush( RGB( 255, 36, 0 ) ); // red
		brushes[3] = ::CreateSolidBrush( RGB( 31, 117, 254 ) ); // blue
		brushes[4] = ::CreateSolidBrush( RGB( 50, 205, 50 ) ); // green
		brushes[5] = ::CreateSolidBrush( RGB( 0, 0, 0 ) ); // green
	}
	~Balls() {}

	// Зарегистрировать класс окна
	static bool RegisterClass( HINSTANCE hInstance );

	// Создать экземпляр окна
	bool Create( int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, 
							int width = 2 * UNIT_SIZE * WIDTH, int heigth = 2 * UNIT_SIZE * HEIGHT );

	// Показать окно
	void Show( int cmdShow );

protected:
	void OnClick( int x, int y );
	void OnDestroy();
	void OnPaint();
	void OnDelete( int i, int j );
	void returnBack();
	void OnFinal();

private:
	static const wchar_t* const className;
	HWND handle; // хэндл окна
	HBRUSH brushes[6];
	ball balls[WIDTH][HEIGHT];
	ball backUp[WIDTH][HEIGHT];
	ball backBackUp[WIDTH][HEIGHT];
	std::vector<ball*> toDelete;
	int score;

	void generateBalls();
	void replaceBalls();
	void killNeighbours( int i, int j );
	void deleteBall( int i, int j );
	int bonus( int k );
	void tryBackUp();
	void makeBackUp();
	bool checkIsFinal();
	static LRESULT __stdcall windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
};

void swap( ball& a, ball& b);
