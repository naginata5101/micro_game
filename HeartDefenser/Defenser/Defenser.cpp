#include <Mikan.h>

#define SIZE 32

#define TITLE 0
#define PLAY 2
#define RESULT 3

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#define WALK_MAX 20
#define HELP_MAX 10
#define GIVE_MAX 500

#define HP_MAX 20000

const int ShotPower = 10000;
const int reach = 2;
const int ShotRag = 100;

struct walker
{
	int x;
	int y;
	int moved;
	int hp;
};

struct helper
{
	int x;
	int y;
	int shotTime;
};

struct given
{
	int x;
	int y;
	int time;
};

//------------------------------------------
int timer = -300;
int mode = 0;
int x = 1;
int y = 1;
int score = 0;
int count = 0;

struct walker walker[ WALK_MAX ];
struct helper helper[ HELP_MAX ];
struct given given[ GIVE_MAX ];


int map[ 15 ][ 20 + 2 ] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};


//------------------------------------
void CursorMove( int *x, int *y )
{
	if( MikanInput->GetKeyNum( K_UP ) % 10 == 1 )
	{
		*y --;
	}
	if( MikanInput->GetKeyNum( K_RIGHT ) % 10 == 1 )
	{
		*x ++;
	}
	if( MikanInput->GetKeyNum( K_DOWN ) % 10 == 1 )
	{
		*y ++;
	}
	if( MikanInput->GetKeyNum( K_LEFT ) % 10 == 1 )
	{
		*x --;
	}

	if( *x < 1 )
	{
		*x = 1;
	}
	else if( *x > 20 + 1 )
	{
		*x = 20 + 1;
	}
	if( *y < 0 )
	{
		*y = 0;
	}
	else if( *y > 15 )
	{
		*y = 15;
	}
}

bool CheckSet( struct helper helper[ HELP_MAX ], int x, int y )
{
	for( int i = 0; i < HELP_MAX; i ++ )
	{
		if( x == helper[ i ].x &&
			y == helper[ i ].y )
		{
			return false;
		}
	}
	return true;
}
bool HelperSet( struct helper helper[ HELP_MAX ], int map[ 15 ][ 22 ], int x, int y )
{
	if( MikanInput->GetKeyNum( K_Z ) == 1 )
	{
		for( int i = 0; i < HELP_MAX; i ++ )
		{
			if( helper[ i ].x < 0 &&
				map[ y ][ x ] != 1 &&
				CheckSet( helper, x, y ) == true )
			{
				helper[ i ].x = x;
				helper[ i ].y = y;
				helper[ i ].shotTime = -100;
				return true;
			}
		}
	}
	return false;
}


void WalkerMake( int map[ 15 ][ 22 ], struct walker walker[ WALK_MAX ] )
{
	for( int i = 0; i < WALK_MAX; i ++ )
	{
		if( walker[ i ].x < -10 )
		{
			for( int j = 0; j < 15; j ++ )
			{
				if( map[ j ][ 1 ] == 1 )
				{
					walker[ i ].x = 1;
					walker[ i ].y = j;
					walker[ i ].moved = 0;
					walker[ i ].hp = HP_MAX;
					return;
				}
			}
		}
	}
}

void WalkerWalk( int map[ 15 ][ 22 ],struct walker walker[ WALK_MAX ] )
{
	for( int i = 0; i < WALK_MAX; i ++ )
	{
		if( map[ walker[ i ].y - 1 ][ walker[ i ].x ] == 1 &&
			walker[ i ].moved != DOWN )
		{
			walker[ i ].y -= 1;
			walker[ i ].moved = UP;
		}
		else if( map[ walker[ i ].y ][ walker[ i ].x + 1 ] == 1 &&
			walker[ i ].moved != LEFT )
		{
			walker[ i ].x += 1;
			walker[ i ].moved = RIGHT;
		}
		else if( map[ walker[ i ].y + 1 ][ walker[ i ].x ] == 1 &&
			walker[ i ].moved != UP )
		{
			walker[ i ].y += 1;
			walker[ i ].moved = DOWN;
		}
		else if( map[ walker[ i ].y ][ walker[ i ].x - 1 ] == 1 &&
			walker[ i ].moved != RIGHT )
		{
			walker[ i ].x -= 1;
			walker[ i ].moved = LEFT;
		}
	}
}

void WalkerOlding( struct walker walker[ WALK_MAX ] )
{
	for( int i = 0; i < WALK_MAX; i ++ )
	{
		if( walker[ i ].hp > 0 )
		{
			walker[ i ].hp -= 100 + walker[ i ].hp / 500;
		}
	}
}

void WalkerGoal( int map[ 15 ][ 22 ], struct walker walker[ WALK_MAX ] )
{
	for( int i = 0; i < WALK_MAX; i ++ )
	{
		if( walker[ i ].x >= 20 + 1 &&
			walker[ i ].hp > 0 )
		{
			score ++;
			walker[ i ].hp = 0;
		}
	}
}


void GivenMake( struct given given[ GIVE_MAX ], int X1, int Y1, int X2, int Y2, int timer )
{
	int x1 = X1 * SIZE;
	int y1 = Y1 * SIZE;
	int x2 = X2 * SIZE;
	int y2 = Y2 * SIZE;

	for( int j = 1; j < 5; j ++ )
	{
		for( int i = 0; i < GIVE_MAX; i ++ )
		{
			if( given[ i ].x < 0 )
			{
				given[ i ].x = ( x2 - x1 ) * j / 5 + x1;
				given[ i ].y = ( y2 - y1 ) * j / 5 + y1;
				given[ i ].time = timer;
				break;
			}

		}
	}
}
void HelperShoot( struct helper helper[ HELP_MAX ], struct walker walker[ WALK_MAX ], struct given given[ GIVE_MAX ], int timer )
{
	int disX, disY;

	for( int i = 0; i < HELP_MAX; i ++ )
	{
		if( helper[ i ].x > 0 &&
			timer - helper[ i ].shotTime > ShotRag )
		{
			for( int j = 0; j < WALK_MAX; j ++ )
			{
				disX = ( helper[ i ].x - walker[ j ].x ) * ( helper[ i ].x - walker[ j ].x );
				disY = ( helper[ i ].y - walker[ j ].y ) * ( helper[ i ].y - walker[ j ].y );
				if( walker[ j ].hp > 0 &&
					disX + disY < reach * reach )
				{
					walker[ j ].hp += ShotPower;
					helper[ i ].shotTime = timer;
					GivenMake( given, helper[ i ].x, helper[ i ].y, walker[ j ].x, walker[ j ].y, timer );
					break;
				}
			}
		}
	}
}

void GivenRemove( struct given given[ GIVE_MAX ], int timer )
{
	for( int i = 0; i < GIVE_MAX; i ++ )
	{
		if( timer - given[ i ].time > 10 )
		{
			given[ i ].x = -100;
			given[ i ].y = -100;
			given[ i ].time = -100;
		}
	}
}


void DrawMap( int map[ 15 ][ 22 ] )
{
	for( int i = 0; i < 15; i ++ )
	{
		for( int j = 0; j < 22; j ++ )
		{
			if( map[ i ][ j ] == 0 )
			{
				MikanDraw->DrawTexture( 0, SIZE * ( j - 1 ), SIZE * i, SIZE * 0, SIZE * 0, SIZE, SIZE );
			}
			if( map[ i ][ j ] == 1 )
			{
				MikanDraw->DrawTexture( 0, SIZE * ( j - 1 ), SIZE * i, SIZE * 1, SIZE * 0, SIZE, SIZE );
			}
		}
	}
	MikanDraw->DrawBox( 0, 0, 640, SIZE, 0xFF000000 );
}

void DrawCursor( int x, int y )
{
	MikanDraw->DrawTexture( 0, ( x - 1 ) * SIZE, y * SIZE, SIZE * 2, SIZE * 0, SIZE, SIZE );
}

void DrawWalker( struct walker walker[ WALK_MAX ] )
{
	for( int i = 0; i < WALK_MAX; i ++ )
	{
		if( walker[ i ].hp > 0 )
		{
			MikanDraw->DrawTexture( 0 , ( walker[ i ].x - 1 ) * SIZE, walker[ i ].y * SIZE,
				SIZE * 1, SIZE * 1, SIZE, SIZE );
		}
	}
}

void DrawHelper( struct helper helper[ HELP_MAX ] )
{
	for( int i = 0; i < HELP_MAX; i ++ )
	{
		if( helper[ i ].x > 0 )
		{
			MikanDraw->DrawTexture( 0 , ( helper[ i ].x - 1 ) * SIZE, helper[ i ].y * SIZE,
				SIZE * 0, SIZE * 1, SIZE, SIZE );
		}
	}
}

void DrawGiven( struct given given[ GIVE_MAX ] )
{
	for( int i = 0; i < GIVE_MAX; i ++ )
	{
		if( given[ i ].time > 0 )
		{
			MikanDraw->DrawTexture( 0, given[ i ].x - SIZE + SIZE / 2, given[ i ].y + SIZE / 2, SIZE * 2, SIZE * 1, 5, 5 );
		}
	}
}


bool CloseGame( struct walker walker[ WALK_MAX ] )
{
	for( int i = 0; i < WALK_MAX; i ++ )
	{
		if( walker[ i ].hp > 0 )
		{
			return false;
		}
	}
	return true;
}



//-------------------------------------

//プログラム起動時に1度だけ実行
//システム設定などを行う
void SystemInit( void )
{
	MikanWindow->SetWindowName( "Heart Defense" );
	MikanWindow->SetWindowIcon( "GAME_ICON" );
}

//MainLoop開始前に1度だけ実行
//テクスチャの読み込みなどのゲーム開始前の準備を行う
void UserInit( void )
{
	MikanDraw->CreateTexture( 0, "chips.png", 0xFFFFFFFF );
	MikanDraw->CreateFont( 0, 32, 0xFFFFFFFF );
	MikanDraw->CreateFont( 1, 64, 0xFFFFFF00 );

	for( int i = 0; i < WALK_MAX; i ++ )
	{
		walker[ i ].x = -100;
		walker[ i ].y = -100;
		walker[ i ].moved = 0;
		walker[ i ].hp = 0;
	}

	for( int i = 0; i < HELP_MAX; i ++ )
	{
		helper[ i ].x = -100;
		helper[ i ].y = -100;
		helper[ i ].shotTime = -100;
	}

	for( int i = 0; i < GIVE_MAX; i ++ )
	{
		given[ i ].x = -100;
		given[ i ].y = -100;
	}
}

//1秒間に60回実行
//0を返している間は何度でも実行される
int MainLoop( void )
{
	//画面の初期化
	MikanDraw->ClearScreen();
	switch( mode )
	{
	case TITLE:
		MikanDraw->Printf( 1, 640 - 64 * 9, 40, "ハートディフェンス" );
		MikanDraw->Printf( 0, 32, 300,
			"お助けハートの「はーとちゃん」を配置し、\n"
			"心が折れそうな「ハート君」を助けてよう！");
		if( MikanInput->GetKeyNum( K_Z ) )
		{
			mode = PLAY;
		}
		break;


	case PLAY:
		//CursorMove( &x, &y );
		if( MikanInput->GetKeyNum( K_UP ) % 10 == 1 )
		{
			y --;
		}
		if( MikanInput->GetKeyNum( K_RIGHT ) % 10 == 1 )
		{
			x ++;
		}
		if( MikanInput->GetKeyNum( K_DOWN ) % 10 == 1 )
		{
			y ++;
		}
		if( MikanInput->GetKeyNum( K_LEFT ) % 10 == 1 )
		{
			x --;
		}

		if( x < 1 )
		{
			x = 1;
		}
		else if( x > 20 + 1 )
		{
			x = 20 + 1;
		}
		if( y < 1 )
		{
			y = 1;
		}
		else if( y > 15 )
		{
			y = 15;
		}




		if( HelperSet( helper, map, x, y ) )
		{
			count ++;
		}


		WalkerOlding( walker );
		if( timer % 20 == 0 &&
			timer > 0 )
		{
			WalkerWalk( map, walker );
			WalkerGoal( map, walker );
			if( timer % 3 == 0 )
			{
				WalkerMake( map, walker );
			}
		}
		HelperShoot( helper, walker, given, timer );
		GivenRemove( given, timer );



		DrawMap( map );
		DrawWalker( walker );
		DrawHelper( helper );
		DrawGiven( given );

		MikanDraw->SetAlpha( 0, 170 );
		DrawCursor( x, y );
		MikanDraw->SetAlpha( 0, 255 );

		MikanDraw->Printf( 0, 0, 0, "はーとちゃん残り：%2d　　　　お助け人数：%d", 10 - count, score );


		if( timer > 100 &&
			CloseGame( walker ) )
		{
			mode = RESULT;
		}

		timer ++;
		break;


	case RESULT:
		if( MikanInput->GetKeyNum( K_Z ) )
		{
			return 1;
		}

		MikanDraw->Printf( 0, ( 640 - 9 * 32 ) / 2     , 200, "あなたのお助け度…" );
		MikanDraw->Printf( 1, ( 640 - 9 * 32 ) / 2 + 64, 232, "%d％", score * 100 / WALK_MAX );
		break;
	}
	if( MikanInput->GetKeyNum( K_ESC ) > 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//MainLoop終了後に1度だけ実行
//後片付けなどを行う
void CleanUp( void )
{
}
