#include <Mikan.h>

#include "head.h"


void SystemInit( void )
{
 MikanWindow->SetWindowName( "MEIRO" );
}

//MainLoop開始前に1度だけ実行
//テクスチャの読み込みなどのゲーム開始前の準備を行う
void UserInit( void )
	{
	MikanDraw->CreateFont( 0, 30, MikanDraw->Color( 255, 0, 0, 0 ) );
	MikanDraw->CreateFont( 1, 15, MikanDraw->Color( 255, 0, 0, 0 ) );

	MikanDraw->CreateTexture( 0, "data/map.png", TRC_ZERO );
	MikanDraw->CreateTexture( 1, "data/map2.png", TRC_ZERO);

	MikanSound->Load( 0, "data/shady one.wav", true );
	


	PlayerHP = -100;
	Score = NULL;
	HighScore = NULL;

	CursorX = 0 * MapSize;
	CursorY = 2 * MapSize;

	return;
	}

//1秒間に60回実行
//0を返している間は何度でも実行される
int MainLoop( void )
{
	//画面の初期化
	MikanDraw->ClearScreen( );





	//ゲームの初期化
	if( PlayerHP <= 0 )
	{
		//スコアの処理
		if( Score > HighScore )
		{
			HighScore = Score;
		}
			//ファイルに対する入出力
		FILE *fp;
		char str[ 8 ] = "0";
		if( fopen_s( &fp, "data/score", HighScore > 0 ? "w" : "r") ){ return 0; }
		if( HighScore > 0 )
		{
			// ファイルへ書き込む。
			fprintf( fp, "%d", HighScore );
		}else
		{
			// ファイルから値を読み込む。
			fgets( str, 256, fp );
		HighScore = atoi( str );
		}
		fclose( fp );



		//敵の初期情報
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			for( i = 0; i < 15; i ++ )
			{
				for( j = 0; j < 20; j ++ )
				{
					for( k = 0; k < 10; k ++ )
					{
						if( map[ i ][ j ][ k ] == 1 )
						{
							EnemyY[ count1 ] = i * MapSize;
							EnemyX[ count1 ] = j * MapSize;
							EnemyZ[ count1 ] = k;
						}
					}
				}
			}
			EnemyCondition[ count1 ] = 1;
			NextMove[ count1 ] = 0;
			LastMove[ count1 ] = 0;
			EnemyNextX[ count1 ] = 0;
			EnemyNextY[ count1 ] = 0;
			EnemyNextZ[ count1 ] = 0;
			EnemyHP[ count1 ] = -1000;
			EnemyLv[ count1 ] = 0;
			Enemy_LastATtime[ count1 ] = -100;
			Enemy_ATtargetX[ count1 ] = -100;
			Enemy_ATtargetY[ count1 ] = -100;

			UnitX[ count1 ] = -100;
			UnitY[ count1 ] = -100;
			UnitZ[ count1 ] = -100;
			UnitHP[ count1 ] = 0;
			Unit_LastATtime[ count1 ] = -100;
			Unit_ATtargetX[ count1 ] = -100;
			Unit_ATtargetY[ count1 ] = -100;
			Unit_ATtarget[ count1 ] = -100;
		}
		EnemyAppear = 0;
		WaveTime = -600;
		Money = 5000;

		//プレイヤーの初期体力は10
		PlayerHP = 10;
		//経過時間を0にする
		FrameTime = 0;
		//ゲームはまだ始まっていない！
		Start = false;
	}







	//Enterが押されたらゲーム開始
	if( MikanInput->GetKeyNum( K_ENTER ) != 0 &&
		Start == false )
	{
		Score = 0;
		Start = true;
	}


	//カーソルの移動
	//マウスによる移動
	if( ( MikanInput->GetMousePosX() >= 0 &&
		  MikanInput->GetMousePosX() <= WIDHT ) &&
		( MikanInput->GetMousePosY() >= 2 * MapSize &&
		  MikanInput->GetMousePosY() <= HEIGHT ) )
	{
		CursorX = ( MikanInput->GetMousePosX() / MapSize ) * MapSize;
		CursorY = ( MikanInput->GetMousePosY() / MapSize ) * MapSize;
	}
	//キーボードによる移動
	else
	{
		if( MikanInput->GetKeyNum( K_LEFT ) == 1 &&
			CursorX >= MapSize )
		{
			CursorX = CursorX - MapSize;
		}
		if( MikanInput->GetKeyNum( K_UP ) == 1 &&
			CursorY >= ( MapSize * 3 ) )
			{
			CursorY = CursorY - MapSize;
		}
		if( MikanInput->GetKeyNum( K_RIGHT ) == 1 &&
			CursorX <= WIDHT - ( MapSize * 2 ) )
		{
			CursorX = CursorX + MapSize;
		}
		if( MikanInput->GetKeyNum( K_DOWN ) == 1 &&
			CursorY <= HEIGHT - ( MapSize * 2 ) )
		{	
			CursorY = CursorY + MapSize;
		}
	}

	//カーソルの場所に味方を配置
	if( ( MikanInput->GetKeyNum( K_Z ) == 1 ||
		  MikanInput->GetMouseNum( 0 ) == 1 ) &&
		Money >= UnitCost )
	{
		Put = true;
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( UnitX[ count1 ] == CursorX &&
				UnitY[ count1 ] == CursorY )
			{
				Put = false;
			}
			if( EnemyX[ count1 ] == CursorX &&
				EnemyY[ count1 ] == CursorY )
			{
				Put = false;
			}
		}
	}
	if( Put == true )
	{
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( UnitHP[ count1 ] <= 0 )
			{
				UnitHP[ count1 ] = UnitHP_MAX;
				UnitX[ count1 ] = CursorX;
				UnitY[ count1 ] = CursorY;
				for( count3 = 0; count3 < MAX2; count3 ++ )
				{
					if( map[ CursorY / MapSize ][ CursorX / MapSize ][ count3 ] >= 1 )
					{
						UnitZ[ count1 ] = count3;
					}
				}
				Money = Money - UnitCost;
				Put = false;
				break;
			}
		}
	}
	//味方の撤退
	if( MikanInput->GetKeyNum( K_X ) == 1 ||
		  MikanInput->GetMouseNum( 1 ) == 1 )
	{
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( UnitX[ count1 ] == CursorX &&
				UnitY[ count1 ] == CursorY )
			{
				Money = Money + ( UnitCost * UnitHP[ count1 ] / UnitHP_MAX / 2 );
				UnitHP[ count1 ] = - 1;
				break;
			}
		}
	}



	//ユニットの選択
	if( MikanInput->GetKeyNum( K_C ) == 1 ||
		MikanInput->GetMouseNum( 2 ) == 1 )
	{
		//カーソル上に何もいなければ選択解除
		Select = -1;
		//カーソルの位置の敵ユニットを選択
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( EnemyX[ count1 ] == CursorX &&
				EnemyY[ count1 ] == CursorY )
			{
				Select = count1 + 100;
				break;
			}
		}
		//同じく味方ユニットを選択
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( UnitX[ count1 ] == CursorX &&
				UnitY[ count1 ] == CursorY )
			{
				Select = count1;
				break;
			}
		}
	}


	//定期的に敵を出現させる
	if( FrameTime % ( MapSize * 3 ) == 0 &&
		FrameTime >= MapSize &&
		FrameTime - WaveTime >= 600 &&
		Start == true )
	{
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( EnemyHP[ count1 ] <= 0 )
			{
				EnemyLv[ count1 ] = 1 + ( EnemyAppear / 10 );
				EnemyHP[ count1 ]
					= ( 10 * EnemyLv[ count1 ] * EnemyLv[ count1 ] ) + ( 100 * EnemyLv[ count1 ] );
				EnemyAppear = EnemyAppear + 1;
				break;
			}
		}
	}

	//敵のレベルが上がる毎に10秒の休憩
	if( FrameTime - WaveTime > 700 &&
		EnemyAppear % 10 == 0 &&
		EnemyAppear > 0 )
	{
		WaveTime = FrameTime;
	}


	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		//敵、味方のコンディションに座標の値を入れる
		EnemyCondition[ count1 ]
			= map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] ];
		UnitCondition[ count1 ]
			= map[ UnitY[ count1 ] / MapSize ][ UnitX[ count1 ] / MapSize ][ UnitZ[ count1 ] ];

		for( count3 = 0; count3 < MAX2; count3 ++ )
		{
			//各々の道にいる敵の数を数える
			if( EnemyCondition[ count1 ] == count3 )
			{
				EnemyNumber[ count3 ] = EnemyNumber[ count3 ] + 1;
			}
			//味方の数も数える
			if( UnitCondition[ count1 ] == count3 )
			{
				UnitNumber[ count3 ] = UnitNumber[ count3 ] + 1;
			}
		}
	}
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		//敵が分かれ道（"3"）に来た時
		if( EnemyCondition[ count1 ] == 3 )
		{
			//味方が少ない道を選ぶ
			if( UnitNumber[ 4 ] > UnitNumber[ 5 ] )
			{
				EnemyCondition[ count1 ] = 5;
			}
			if( UnitNumber[ 4 ] < UnitNumber[ 5 ] )
			{
				EnemyCondition[ count1 ] = 4;
			}
			//味方の数が同じならでも敵が少ない方を選ぶ
			if( UnitNumber[ 4 ] == UnitNumber[ 5 ] )
			{
				if( EnemyNumber[ 4 ] > EnemyNumber[ 5 ] )
				{
					EnemyCondition[ count1 ] = 5;
				}
			}
			//一方の道が塞がっていたらもう一方へ
			for( count2 = 0; count2 < MAX1; count2 ++ )
			{
				if( EnemyX[ count1 ] == EnemyX[ count2 ] &&
					EnemyY[ count1 ] == EnemyY[ count2 ] + MapSize &&
					EnemyZ[ count1 ] == EnemyZ[ count2 ] + 1 )
				{
					EnemyCondition[ count1 ] = 3;
				}
				if( EnemyX[ count1 ] == UnitX[ count2 ] &&
					EnemyY[ count1 ] == UnitY[ count2 ] + MapSize &&
					EnemyZ[ count1 ] == UnitZ[ count2 ] + 1 )
				{
					EnemyCondition[ count1 ] = 3;
				}
			}
			for( count2 = 0; count2 < MAX1; count2 ++ )
			{
				if( EnemyX[ count1 ] == EnemyX[ count2 ] - MapSize &&
					EnemyY[ count1 ] == EnemyY[ count2 ] &&
					EnemyZ[ count1 ] == EnemyZ[ count2 ] + 1 )
				{
					EnemyCondition[ count1 ] = 5;
				}
				if( EnemyX[ count1 ] == UnitX[ count2 ] - MapSize &&
					EnemyY[ count1 ] == UnitY[ count2 ] &&
					EnemyZ[ count1 ] == UnitZ[ count2 ] + 1 )
				{
					EnemyCondition[ count1 ] = 5;
				}
			}
		}
		if( EnemyHP[ count1 ] > 0 )
		{
			//定期的に敵の移動方向を決定する
			if( FrameTime % MapSize == 0 )
			{
				//取り敢えず移動方向をなくす
				NextMove[ count1 ] = 0;
				//１段上に空いているマスがあればそこを移動方向にする
				if( map[ EnemyY[ count1 ] / MapSize - 1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] + 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 12 &&
					LastMove[ count1 ] != 22 &&
					LastMove[ count1 ] != 32 )
				{
					NextMove[ count1 ] = 18;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						//ただし、そこが敵の次の存在マスならば動かない
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] - 1 )
						{
							NextMove[ count1 ] = 0;
						}
						//ただし、そこが肉の壁の存在マスならば動かない
						if( EnemyX[ count1 ] == UnitX[ count2 ] &&
							EnemyY[ count1 ] == UnitY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] - 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize + 1 ][ EnemyZ[ count1 ] + 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 14 &&
					LastMove[ count1 ] != 24 &&
					LastMove[ count1 ] != 34 )
				{
					NextMove[ count1 ] = 16;
						for( count2 = 0; count2 < MAX1; count2 ++ )
						{
							if( EnemyX[ count1 ] == EnemyNextX[ count2 ] - MapSize &&
								EnemyY[ count1 ] == EnemyNextY[ count2 ] &&
								EnemyZ[ count1 ] == EnemyNextZ[ count2 ] - 1 )
							{
									NextMove[ count1 ] = 0;
							}
							if( EnemyX[ count1 ] == UnitX[ count2 ] - MapSize &&
								EnemyY[ count1 ] == UnitY[ count2 ] &&
								EnemyZ[ count1 ] == UnitZ[ count2 ] - 1 &&
								UnitHP[ count2 ] > 0 )
							{
									NextMove[ count1 ] = 0;
							}
						}
				}
				if( map[ EnemyY[ count1 ] / MapSize + 1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] + 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 18 &&
					LastMove[ count1 ] != 28 &&
					LastMove[ count1 ] != 38 )
				{
					NextMove[ count1 ] = 12;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] - MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] - 1 )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] &&
							EnemyY[ count1 ] == UnitY[ count2 ] - MapSize &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] - 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize - 1 ][ EnemyZ[ count1 ] + 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 16 &&
					LastMove[ count1 ] != 26 &&
					LastMove[ count1 ] != 36 )
				{
					NextMove[ count1 ] = 14;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] + MapSize &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] - 1 )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] + MapSize &&
							EnemyY[ count1 ] == UnitY[ count2 ] &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] - 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				//同じ段に空いているマスがあればそこを移動方向にする
				if( map[ EnemyY[ count1 ] / MapSize -1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 12 &&
					LastMove[ count1 ] != 22 &&
					LastMove[ count1 ] != 32 )
				{
					NextMove[ count1 ] = 28;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] &&
							EnemyY[ count1 ] == UnitY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize + 1 ][ EnemyZ[ count1 ] ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 14 &&
					LastMove[ count1 ] != 24 &&
					LastMove[ count1 ] != 34 )
				{
					NextMove[ count1 ] = 26;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{					
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] - MapSize &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] - MapSize &&
							EnemyY[ count1 ] == UnitY[ count2 ] &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize +1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 18 &&
					LastMove[ count1 ] != 28 &&
					LastMove[ count1 ] != 38 )
				{
					NextMove[ count1 ] = 22;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] - MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] &&
							EnemyY[ count1 ] == UnitY[ count2 ] - MapSize &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize - 1 ][ EnemyZ[ count1 ] ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 16 &&
					LastMove[ count1 ] != 26 &&
					LastMove[ count1 ] != 36 )
				{
					NextMove[ count1 ] = 24;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] + MapSize &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] + MapSize &&
							EnemyY[ count1 ] == UnitY[ count2 ] &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
					//１段下に空いているマスがあればそこを移動方向にする
				if( map[ EnemyY[ count1 ] / MapSize -1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] -1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 12 &&
					LastMove[ count1 ] != 22 &&
					LastMove[ count1 ] != 32 )
				{
					NextMove[ count1 ] = 38;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] + 1 )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] &&
							EnemyY[ count1 ] == UnitY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] + 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize + 1 ][ EnemyZ[ count1 ] - 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 14 &&
					LastMove[ count1 ] != 24 &&
					LastMove[ count1 ] != 34 )
				{
					NextMove[ count1 ] = 36;					
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] - MapSize &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] + 1 )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] - MapSize &&
							EnemyY[ count1 ] == UnitY[ count2 ] &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] + 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize + 1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] - 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 18 &&
					LastMove[ count1 ] != 28 &&
					LastMove[ count1 ] != 38 )
				{
					NextMove[ count1 ] = 32;						
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] - MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] + 1 )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] &&
							EnemyY[ count1 ] == UnitY[ count2 ] - MapSize &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] + 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}
				if( map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize - 1 ][ EnemyZ[ count1 ] - 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 16 &&
					LastMove[ count1 ] != 26 &&
					LastMove[ count1 ] != 36 )
				{
					NextMove[ count1 ] = 34;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] + MapSize &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] + 1 )
						{
							NextMove[ count1 ] = 0;
						}
						if( EnemyX[ count1 ] == UnitX[ count2 ] + MapSize &&
							EnemyY[ count1 ] == UnitY[ count2 ] &&
							EnemyZ[ count1 ] == UnitZ[ count2 ] + 1 &&
							UnitHP[ count2 ] > 0 )
						{
							NextMove[ count1 ] = 0;
						}
					}
				}


				//次の移動に合わせて
				//一段上に移動
				if( NextMove[ count1 ] == 18 ||
					NextMove[ count1 ] == 16 ||
					NextMove[ count1 ] == 12 ||
					NextMove[ count1 ] == 14 )
				{
						EnemyZ[ count1 ] = EnemyZ[ count1 ] + 1;
				}
				//一段下に移動
				if( NextMove[ count1 ] == 38 ||
					NextMove[ count1 ] == 36 ||
					NextMove[ count1 ] == 32 ||
					NextMove[ count1 ] == 34 )
				{
						EnemyZ[ count1 ] = EnemyZ[ count1 ] - 1;
				}

				//敵の次のX軸を調整
				if( NextMove[ count1 ] == 14 ||
					NextMove[ count1 ] == 24 ||
					NextMove[ count1 ] == 34 )
				{
					EnemyNextX[ count1 ]
						= EnemyX[ count1 ] - ( EnemyX[ count1 ] % MapSize ) - MapSize;
				}
				if( NextMove[ count1 ] == 16 ||
					NextMove[ count1 ] == 26 ||
					NextMove[ count1 ] == 36 )
				{
					EnemyNextX[ count1 ]
						= EnemyX[ count1 ] - ( EnemyX[ count1 ] % MapSize ) + MapSize;
				}
				if( NextMove[ count1 ] == 12 ||
					NextMove[ count1 ] == 22 ||
					NextMove[ count1 ] == 32 ||
					NextMove[ count1 ] == 18 ||
					NextMove[ count1 ] == 28 ||
					NextMove[ count1 ] == 38 )
				{
					EnemyNextX[ count1 ] = EnemyX[ count1 ];
				}
				//敵の次のY軸を調整
				if( NextMove[ count1 ] == 12 ||
					NextMove[ count1 ] == 22 ||
					NextMove[ count1 ] == 32 )
				{
					EnemyNextY[ count1 ]
						= EnemyY[ count1 ] - ( EnemyY[ count1 ] % MapSize ) + MapSize;
				}
				if( NextMove[ count1 ] == 18 ||
					NextMove[ count1 ] == 28 ||
					NextMove[ count1 ] == 38 )
				{
					EnemyNextY[ count1 ]
						= EnemyY[ count1 ] - ( EnemyY[ count1 ] % MapSize ) - MapSize;
				}
				if( NextMove[ count1 ] == 14 ||
					NextMove[ count1 ] == 24 ||
					NextMove[ count1 ] == 34 ||
					NextMove[ count1 ] == 16 ||
					NextMove[ count1 ] == 26 ||
					NextMove[ count1 ] == 36 )
				{
					EnemyNextY[ count1 ] = EnemyY[ count1 ];
				}
				//敵の次のZ軸を調整
				EnemyNextZ[ count1 ] = EnemyZ[ count1 ];

				//前回の移動の管理
				if( NextMove[ count1 ] != 0 )
				{
					LastMove[ count1 ] = NextMove[ count1 ];
				}
			}
			
			//上に移動
			if( NextMove[ count1 ] == 18 ||
				NextMove[ count1 ] == 28 ||
				NextMove[ count1 ] == 38 )
			{
				EnemyY[ count1 ] = EnemyY[ count1 ] - 1;
			}
			//右に移動
			if( NextMove[ count1 ] == 16 ||
				NextMove[ count1 ] == 26 ||
				NextMove[ count1 ] == 36 )
			{
				EnemyX[ count1 ] = EnemyX[ count1 ] + 1;
			}
			//下に移動
			if( NextMove[ count1 ] == 12 ||
				NextMove[ count1 ] == 22 ||
				NextMove[ count1 ] == 32 )
			{
				EnemyY[ count1 ] = EnemyY[ count1 ] + 1;
			}
			//左に移動
			if( NextMove[ count1 ] == 14 ||
				NextMove[ count1 ] == 24 ||
				NextMove[ count1 ] == 34 )
			{
				EnemyX[ count1 ] = EnemyX[ count1 ] - 1;
			}
		}
		
		for( count2 = 0; count2 < MAX1; count2 ++ )
		{
			//敵の進む道に味方がいたら攻撃！
			if( ( EnemyX[ count1 ] - UnitX[ count2 ] ) *
				( EnemyX[ count1 ] - UnitX[ count2 ] ) + 
				( EnemyY[ count1 ] - UnitY[ count2 ] ) *
				( EnemyY[ count1 ] - UnitY[ count2 ] )
						<= MapSize * MapSize &&
				EnemyZ[ count1 ] - UnitZ[ count2 ] >= -1 &&
				EnemyZ[ count1 ] - UnitZ[ count2 ] <= 1 &&
				FrameTime - Enemy_LastATtime[ count1 ] >= Enemy_ATtime &&
				EnemyHP[ count1 ] > 0 &&
				UnitCondition[ count2 ] != 0 )
			{
				Enemy_LastATtime[ count1 ] = FrameTime;
				Enemy_ATtargetX[ count1 ] = UnitX[ count2 ];
				Enemy_ATtargetY[ count1 ] = UnitY[ count2 ];
				UnitHP[ count2 ]
					= UnitHP[ count2 ] - ( 50 + ( ( EnemyLv[ count1 ] - 1 ) * 10 ) );
			}

			//味方の攻撃範囲内に敵がいたら攻撃発射！
			if( ( UnitX[ count1 ] - EnemyX[ count2 ] ) *
				( UnitX[ count1 ] - EnemyX[ count2 ] ) +
				( UnitY[ count1 ] - EnemyY[ count2 ] ) *
				( UnitY[ count1 ] - EnemyY[ count2 ] ) +
				( UnitZ[ count1 ] - EnemyZ[ count2 ] ) *
				( UnitZ[ count1 ] - EnemyZ[ count2 ] ) * MapSize
					<= Unit_ATreach * Unit_ATreach &&
				EnemyHP[ count2 ] > 0 &&
				FrameTime - Unit_LastATtime[ count1 ] >= Unit_ATtime )
			{
				Unit_LastATtime[ count1 ] = FrameTime;
				Unit_ATtargetX[ count1 ] = EnemyX[ count2 ];
				Unit_ATtargetY[ count1 ] = EnemyY[ count2 ];
				Unit_ATtarget[ count1 ] = count2;
			}
		}
		//味方の攻撃着弾！
		if( FrameTime - Unit_LastATtime[ count1 ] == Unit_ATeffectTime )
		{
			EnemyHP[ Unit_ATtarget[ count1 ] ]
				= EnemyHP[ Unit_ATtarget[ count1 ] ] - Unit_ATpower;
		}

		//プレイヤーの体力･スコアの処理
		if( EnemyHP[ count1 ] > -1000 &&
			EnemyHP[ count1 ] <= 0 )
		{ 
			if( EnemyCondition[ count1 ] == 4 ||
				EnemyCondition[ count1 ] == 5 )
			{
				Score = Score + ( 2 * 10 * EnemyLv[ count1 ] );
			}
			else
			{
				Score = Score + ( 1 * 10 * EnemyLv[ count1 ] );
			}
			Money = Money + 100 + ( ( EnemyLv[ count1 ] - 1 ) * 10 );
		}
		if( EnemyCondition[ count1 ] == 10 )
		{
			PlayerHP = PlayerHP - 1;
		}
		//敵の体力が0以下になるorゴールに着いた時、初期設定に戻す
		if( EnemyHP[ count1 ] <= 0 ||
			EnemyCondition[ count1 ] == 10 )
		{
			for( i = 0; i < 15; i ++ )
			{
				for( j = 0; j < 20; j ++ )
				{
					for( k = 0; k < 10; k ++ )
					{
						if( map[ i ][ j ][ k ] == 1 )
						{
							EnemyY[ count1 ] = i * MapSize;
							EnemyX[ count1 ] = j * MapSize;
							EnemyZ[ count1 ] = k;
						}
					}
				}
			}
			EnemyHP[ count1 ] = -1000;
			EnemyLv[ count1 ] = 0;
			EnemyCondition[ count1 ] = 1;
			NextMove[ count1 ] = 0;
			LastMove[ count1 ] = 0;
			EnemyNextX[ count1 ] = 0;
			EnemyNextY[ count1 ] = 0;
			EnemyNextZ[ count1 ] = 0;
			Enemy_LastATtime[ count1 ] = -100;
			Enemy_ATtargetX[ count1 ] = -100;
			Enemy_ATtargetY[ count1 ] = -100;
			if( Select == count1 + 100 )
			{
				Select = -1;
			}
		}
		//味方の体力が0以下になったら場外退場
		if( UnitHP[ count1 ] <= 0 )
		{
			UnitX[ count1 ] = -100;
			UnitY[ count1 ] = -100;
			UnitZ[ count1 ] = -100;
			if( Select == count1 )
			{
				Select = -1;
			}
		}
	}

	//敵・味方の数の情報を0に戻す
	for( count3 = 0; count3 < MAX2; count3 ++ )
	{
		EnemyNumber[ count3 ] = 0;
		UnitNumber[ count3 ] = 0;
	}








	//背景の描画
	for( i = 0; i < 15; i ++ )
	{
		for( j = 0; j < 20; j ++ )
		{
			for( k = 0; k < 10; k ++ )
			{
				if( map[ i ][ j ][ k ] == 1 ||
					map[ i ][ j ][ k ] == 2 ||
					map[ i ][ j ][ k ] == 3 ||
					map[ i ][ j ][ k ] == 6 ||
					map[ i ][ j ][ k ] == 10 )
				{
					MikanDraw->DrawTexture( 0, j * MapSize, i * MapSize,
						7 * MapSize, 7 * MapSize, MapSize, MapSize );
				}
				if( map[ i ][ j ][ k ] == 4 ||
					map[ i ][ j ][ k ] == 5)
				{
					MikanDraw->DrawTexture( 0, j * MapSize, i * MapSize,
						6 * MapSize, 7 * MapSize, MapSize, MapSize );
				}
				if( map[ i ][ j ][ k ] == 7 )
				{
					MikanDraw->DrawTexture( 0, j * MapSize, i * MapSize,
						5 * MapSize, 7 * MapSize, MapSize, MapSize );
				}
				if( map[ i ][ j ][ k ] == 8 )
				{
					MikanDraw->DrawTexture( 0, j * MapSize, i * MapSize,
						4 * MapSize, 7 * MapSize, MapSize, MapSize );
				}

				if( map[ i ][ j ][ k ] == 9 )
				{
					MikanDraw->DrawTexture( 0, j * MapSize, i * MapSize,
						0 * MapSize, 7 * MapSize, MapSize, MapSize );
				}
			}
		}
	}

	//味方の攻撃範囲の描画
	//選択した味方について
	if( Select >= 0 &&
		Select < 100 )
	{
		//範囲の円を描画
		MikanDraw->DrawTexture( 1, UnitX[ Select ] - ( MapSize / 2 * 3 ), UnitY[ Select ] - ( MapSize / 2 * 3 ),
			1 * MapSize, 0 * MapSize, 4 * MapSize, 4 * MapSize );
		//範囲内のマスを光らせる
		MikanDraw->SetRender( DRAW_ADD );
		for( i = 0; i <= 2; i ++ )
		{
			for( j = 0; j <= 2; j ++ )
			{
				MikanDraw->DrawTexture( 1, UnitX[ Select ] + ( ( i - 1 ) * MapSize ), UnitY[ Select ] + ( ( j - 1 ) * MapSize ),
					0 * MapSize, 2 * MapSize, MapSize, MapSize );
			}
		}
		MikanDraw->SetRender( DRAW_TRC );
	}
	//カーソル上の味方について
	if( ( Select < 0 ||
		  Select >= 100 ) ||
		( CursorX != UnitX[ Select ] ||
		  CursorY != UnitY[ Select ] ) )
	{
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( CursorX == UnitX[ count1 ] &&
				CursorY == UnitY[ count1 ] )
			{
				//範囲の円を描画
				MikanDraw->DrawTexture( 1, UnitX[ count1 ] - ( MapSize / 2 * 3 ), UnitY[ count1 ] - ( MapSize / 2 * 3 ),
					1 * MapSize, 0 * MapSize, 4 * MapSize, 4 * MapSize );
				//範囲内のマスを光らせる
				MikanDraw->SetRender( DRAW_ADD );
				for( i = 0; i <= 2; i ++ )
				{
					for( j = 0; j <= 2; j ++ )
					{
						MikanDraw->DrawTexture( 1, UnitX[ count1 ] + ( ( i - 1 ) * MapSize ), UnitY[ count1 ] + ( ( j - 1 ) * MapSize ),
							0 * MapSize, 2 * MapSize, MapSize, MapSize );
					}
				}
				MikanDraw->SetRender( DRAW_TRC );
				break;
			}	
		}
	}



	//敵の描画
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( EnemyHP[ count1 ] > 0 )
		{
			MikanDraw->DrawTexture( 0, EnemyX[ count1 ], EnemyY[ count1 ],
				( 4 + ( ( FrameTime / 10 ) % 3 ) ) * MapSize, 1 * MapSize, MapSize, MapSize );
		}
	}
	//味方の描画
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( UnitHP[ count1 ] > 0 )
		{
			MikanDraw->DrawTexture( 0, UnitX[ count1 ], UnitY[ count1 ],
				( 4 + ( ( FrameTime / 10 ) % 3 ) ) * MapSize, 2 * MapSize, MapSize, MapSize);
		}
	}
	//カーソルの描画
	MikanDraw->SetRender( DRAW_ADD );
	MikanDraw->DrawTexture( 1, CursorX, CursorY, 0 * MapSize, 1 * MapSize, MapSize, MapSize );
	MikanDraw->SetRender( DRAW_TRC );

	//攻撃エフェクトの描画
	//味方
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( UnitHP[ count1 ] > 0 )
		{
			//攻撃が飛んで行くよ！
			if( FrameTime - Unit_LastATtime[ count1 ] >= Unit_ATeffectTime * 0 &&
				FrameTime - Unit_LastATtime[ count1 ] < Unit_ATeffectTime * 1 )
			{
				MikanDraw->DrawTexture( 1,
					UnitX[ count1 ] - ( ( Unit_ATtargetX[ count1 ] - UnitX[ count1 ] ) *
						( Unit_LastATtime[ count1 ] - FrameTime ) / Unit_ATeffectTime ),
					UnitY[ count1 ] - ( ( Unit_ATtargetY[ count1 ] - UnitY[ count1 ] ) *
						( Unit_LastATtime[ count1 ] - FrameTime ) / Unit_ATeffectTime ),
					0 * MapSize, 0 * MapSize, MapSize, MapSize );
			}
			//エクスプロージョン!!
			if( FrameTime - Unit_LastATtime[ count1 ] >= Unit_ATeffectTime * 1 &&
				FrameTime - Unit_LastATtime[ count1 ] <  Unit_ATeffectTime * 2 )
			{
				MikanDraw->DrawTexture( 0, Unit_ATtargetX[ count1 ], Unit_ATtargetY[ count1 ],
					0 * MapSize, 3 * MapSize, MapSize, MapSize );
			}
			if( FrameTime - Unit_LastATtime[ count1 ] >= Unit_ATeffectTime * 2 &&
				FrameTime - Unit_LastATtime[ count1 ] <  Unit_ATeffectTime * 3 )
			{
				MikanDraw->DrawTexture( 0, Unit_ATtargetX[ count1 ], Unit_ATtargetY[ count1 ],
					1 * MapSize, 3 * MapSize, MapSize, MapSize );
			}
			if( FrameTime - Unit_LastATtime[ count1 ] >= Unit_ATeffectTime * 3 &&
				FrameTime - Unit_LastATtime[ count1 ] <  Unit_ATeffectTime * 4 )
			{
				MikanDraw->DrawTexture( 0, Unit_ATtargetX[ count1 ], Unit_ATtargetY[ count1 ],
					2 * MapSize, 3 * MapSize, MapSize, MapSize );
			}
		}
	}
	//敵
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( EnemyHP[ count1 ] > 0 )
		{
			if( FrameTime - Enemy_LastATtime[ count1 ] >= Enemy_ATeffectTime * 0 &&
				FrameTime - Enemy_LastATtime[ count1 ] <  Enemy_ATeffectTime * 1)
			{
				MikanDraw->DrawTexture( 0, Enemy_ATtargetX[ count1 ], Enemy_ATtargetY[ count1 ],
					0 * MapSize, 4 * MapSize, MapSize, MapSize );
			}
			if( FrameTime - Enemy_LastATtime[ count1 ] >= Enemy_ATeffectTime * 1 &&
				FrameTime - Enemy_LastATtime[ count1 ] <  Enemy_ATeffectTime * 2)
			{
				MikanDraw->DrawTexture( 0, Enemy_ATtargetX[ count1 ], Enemy_ATtargetY[ count1 ],
					1 * MapSize, 4 * MapSize, MapSize, MapSize);
			}
			if( FrameTime - Enemy_LastATtime[ count1 ] >= Enemy_ATeffectTime * 2 &&
				FrameTime - Enemy_LastATtime[ count1 ] <  Enemy_ATeffectTime * 3)
			{
				MikanDraw->DrawTexture( 0, Enemy_ATtargetX[ count1 ], Enemy_ATtargetY[ count1 ],
					2 * MapSize, 4 * MapSize, MapSize, MapSize);
			}
		}
	}

	//コメントの表示
	MikanDraw->Printf( 1,   0,  0, "Your HP = %d", PlayerHP );
	MikanDraw->Printf( 1,   0, 15, "Your money = %d", Money );
	MikanDraw->Printf( 1, 480,  0, "High score = %d", HighScore );
	MikanDraw->Printf( 1, 478, 15, "Your score = %d", Score );
	if( Start == false &&
		( FrameTime / 60 ) % 2 == 0 )
	{
		MikanDraw->Printf( 0, 0, 30, "Push EnterKey to start game!" );
	}
	if( Start == true )
	{
		MikanDraw->Printf( 1, 0, 30, "Next enemy's level = %d", 1 + (EnemyAppear / 10 ) );
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( UnitX[ count1 ] == CursorX &&
				UnitY[ count1 ] == CursorY &&
				UnitHP[ count1 ] > 0 )
			{
				MikanDraw->Printf( 1, 222, 0, "This unit's HP = %d", UnitHP[ count1 ] );
			}
			if( EnemyX[ count1 ] == CursorX &&
				EnemyY[ count1 ] == CursorY &&
				EnemyHP[ count1 ] > 0 )
			{
				MikanDraw->Printf( 1, 222, 0, "This enemy's HP = %d", EnemyHP[ count1 ] );
			}
		}
		if( Select >= 0 &&
			Select < 100 )
		{
			MikanDraw->Printf( 1, 220, 15, "That unit's HP = %d", UnitHP[ Select ] );
		}
		if( Select >= 100 )
		{
			MikanDraw->Printf( 1, 220, 15, "That enemy's HP = %d", EnemyHP[ Select ] );
		}
	}


	//点 線 ！
	MikanDraw->Printf( 0, -2, 48, "-------------------------------------------" );






	//BGMのON/OFF
	if( MikanInput->GetKeyNum( K_S ) == 1 )
	{
		if( sound == false )
		{
			sound = true;
			MikanSound->Play( 0, true );
		}
		else
		{
			sound = false;
			MikanSound->Stop( 0 );
		}
	}



	//経過時間を増やす
	FrameTime = FrameTime+ 1 ;



	//
	if(	MikanInput->GetKeyNum( K_ESCAPE ) > 0 )
	{
		//スコアの処理
		if( Score > HighScore )
		{
			HighScore = Score;
		}
			//ファイルに対する入出力
		FILE *fp;
		char str[ 8 ] = "0";
		if( fopen_s( &fp, "data/score", HighScore > 0 ? "w" : "r") ){ return 0; }
		if( HighScore > 0 )
		{
			// ファイルへ書き込む。
			fprintf( fp, "%d", HighScore );
		}else
		{
			// ファイルから値を読み込む。
			fgets( str, 256, fp );
		HighScore = atoi( str );
		}
		fclose( fp );

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