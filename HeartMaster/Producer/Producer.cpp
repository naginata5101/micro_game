//main一本&コメント無し企画
#include <Mikan.h>
#include <time.h>

#define TITLE 0
#define PROLO 1
#define DAILY 2
#define BATTLE 3
#define EPILO 4


int game = 0;
int mode = 0;
int day = 0;

int MyVO = 1;
int MyDA = 1;
int MyVI = 1;

int YoVO = 0;
int YoDA = 0;
int YoVI = 0;

int win = 0;
int lose = 0;

int result;

int r;

void SystemInit( void )
{
	MikanWindow->SetWindowName( "ハートマスター" );
	MikanWindow->SetWindowIcon( "GAME_ICON" );
}

void UserInit( void )
{
	MikanDraw->CreateFont( 0, 40, 0xFF000000 );
	MikanDraw->CreateFont( 1, 80, 0xFFFFFF00 );
	MikanDraw->CreateTexture( 0, "Hearts.png", 0xFFFFFFFF );

	srand( ( unsigned ) time ( NULL ) );
	
	r = rand() % 3;
	if( r == 0 )
	{
		MyVO ++;
	}
	else if( r == 1)
	{
		MyDA ++;
	}
	else if( r == 2 )
	{
		MyVI ++;
	}

	for( int i = 0; i < 10; i ++ )
	{
		r = rand() % 3;

		if( r == 0 )
		{
			YoVO ++;
		}
		else if( r == 1)
		{
			YoDA ++;
		}
		else if( r == 2 )
		{
			YoVI ++;
		}
	}
}

int MainLoop( void )
{
	MikanDraw->ClearScreen( 0xFFDB7093 );

	if( game != TITLE )
	{
		MikanDraw->DrawBox( 0, 0, 480, 320, 0xFFFFFFFF );
		MikanDraw->DrawBox( 0, 320, 480, 160, 0xFFFFB6C1 );

		MikanDraw->Printf( 0, 480, 0, "第%d日目", day );
		if(	game != PROLO )
		{
			MikanDraw->Printf( 0, 480,  80, "VO." );
			for( int i = 0; i < MyVO; i ++ )
			{
				MikanDraw->DrawBox( 490 + ( i % 7 ) * 20, 120 + ( i / 7 ) * 20, 20, 20, 0xFF000000 );
				MikanDraw->DrawBox( 490 + ( i % 7 ) * 20 + 2, 120 + ( i / 7 ) * 20 + 2, 20 - 4, 20 - 4, 0xFFEE82EE );
			}
			MikanDraw->Printf( 0, 480, 160, "DA." );
			for( int i = 0; i < MyDA; i ++ )
			{
				MikanDraw->DrawBox( 490 + ( i % 7 ) * 20, 200 + ( i / 7 ) * 20, 20, 20, 0xFF000000 );
				MikanDraw->DrawBox( 490 + ( i % 7 ) * 20 + 2, 200 + ( i / 7 ) * 20 + 2, 20 - 4, 20 - 4, 0xFF87CEFA );
			}
			MikanDraw->Printf( 0, 480, 240, "VI." );
			for( int i = 0; i < MyVI; i ++ )
			{
				MikanDraw->DrawBox( 490 + ( i % 7 ) * 20, 280 + ( i / 7 ) * 20, 20, 20, 0xFF000000 );
				MikanDraw->DrawBox( 490 + ( i % 7 ) * 20 + 2, 280 + ( i / 7 ) * 20 + 2, 20 - 4, 20 - 4, 0xFFF0E68C );
			}
		}
	}


	switch( game )
	{
	case TITLE:
		if( MikanInput->GetMouseNum( 0 ) == 1 ||
			MikanInput->GetMouseNum( 1 ) == 1 )
		{
			game = PROLO;
		}
		MikanDraw->Printf( 1, 70, 80, "ハートマスター" );
		MikanDraw->Printf( 0, 40, 300, "このゲームではマウスを使うよ！" );
		MikanDraw->Printf( 0, 70, 340, "終了はマウスホイールだよ！" );

		break;

	case PROLO:
		switch( mode )
		{
		case 0:
			MikanDraw->Printf( 0, 0, 320,
				"ある所に ハート君 という\n"
				"純情な少年がいました。\n"
				"彼は はーとちゃん が\n"
				"大好きでした。" );
			MikanDraw->DrawTexture( 0,  80, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 1, 120, 120 );
			break;

		case 1:
			MikanDraw->Printf( 0, 0, 320,
				"ある日彼は はーとちゃん に\n"
				"告白することにしました。\n"
				"しかし幼馴染の 覇亜富殿 も\n"
				"はーとちゃん が好きでした。" );
			MikanDraw->DrawTexture( 0,  30, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 1, 120, 120 );
			MikanDraw->DrawTexture( 0, 330, 100, 0, 120 * 2, 120, 120 );
			break;

		case 2:
			MikanDraw->Printf( 0, 0, 320,
				"そこで15分にも及ぶ\n"
				"話し合いの結果、\n"
				"勝負に勝った方が彼女に\n"
				"告白することにしました。" );
			MikanDraw->DrawTexture( 0,  80, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 2, 120, 120 );
			break;

		case 3:
			MikanDraw->Printf( 0, 0, 320,
				"その場に偶然通り過ぎた\n"
				"あなたは、面白そうなので\n"
				"ハート君 をプロデュース\n"
				"することにしました。" );
			MikanDraw->DrawTexture( 0,  30, 40, 120, 120 * 0, 120, 240 );
			MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 330, 100, 0, 120 * 2, 120, 120 );
			break;

		case 4:
			MikanDraw->Printf( 0, 0, 320,
				"彼をプロデュースして\n"
				"3日後の勝負に\n"
				"勝たせてあげましょう！" );
			MikanDraw->DrawTexture( 0,  80, 40, 120, 120 * 0, 120, 240 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 0, 120, 120 );
			break;

		case 5:
			MikanDraw->Printf( 0, 0, 320,
				"「プロデューサーさん、\n"
				"宜しくお願いします！」" );
			MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 0, 120, 120 );
			break;

		default:
			game = DAILY;
			day = 1;
			mode = 0;
			break;
		}

		if( MikanInput->GetMouseNum( 0 ) == 1 )
		{
			mode ++;
		}
		if( MikanInput->GetMouseNum( 1 ) == 1 )
		{
			mode = 100;
		}

		break;

	case DAILY:
		switch( mode )
		{
		case 0:
			if( MikanInput->GetMouseNum( 0 ) == 1 ||
				MikanInput->GetMouseNum( 1 ) == 1 )
			{
				if( MikanInput->GetMousePosY() >= 360 && 
					MikanInput->GetMousePosY() < 400 )
				{
					mode = 1;
				}
				if( MikanInput->GetMousePosY() >= 400 && 
					MikanInput->GetMousePosY() < 440 )
				{
					mode = 2;
				}
				if( MikanInput->GetMousePosY() >= 440 && 
					MikanInput->GetMousePosY() < 480 )
				{
					mode = 3;
				}

				r = ( rand() % 10 ) / 3;
			}

			if( MikanInput->GetMousePosY() >= 360 && 
				MikanInput->GetMousePosY() < 400 )
			{
				MikanDraw->DrawBox( 0, 360, 480, 40, 0x60000000 );
			}
			if( MikanInput->GetMousePosY() >= 400 && 
				MikanInput->GetMousePosY() < 440 )
			{
				MikanDraw->DrawBox( 0, 400, 480, 40, 0x60000000 );
			}
			if( MikanInput->GetMousePosY() >= 440 && 
				MikanInput->GetMousePosY() < 480 )
			{
				MikanDraw->DrawBox( 0, 440, 480, 40, 0x60000000 );
			}

			MikanDraw->Printf( 0, 0, 320,
				"何のレッスンをしますか？\n"
				"　　ボーカル\n"
				"　　ダンス\n"
				"　　ビジュアル" );

			break;

		case 1:
			MyVO += r + 1;
			mode = 4;
			break;
		case 2:
			MyDA += r + 1;
			mode = 4;
			break;
		case 3:
			MyVI += r + 1;
			mode = 4;
			break;

		case 4:
			if( r == 0 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"バッドレッスン" );
			}
			else if( r == 1 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"ノーマルレッスン" );
			}
			else if( r == 2 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"グットレッスン" );
			}
			else if( r == 3 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"パーフェクトレッスン" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n1日が終わった。" );

			if( MikanInput->GetMouseNum( 0 ) == 1 ||
				MikanInput->GetMouseNum( 1 ) == 1 )
			{
				day ++;
				if( day >= 4 )
				{
					game = BATTLE;
				}

				mode = 0;
			}
			break;

		default:
			game = BATTLE;
			break;
		}

		MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 0, 120, 120 );

		break;

	case BATTLE:
		switch( mode )
		{
		case 0:
			MikanDraw->Printf( 0, 0, 320,
				"そんなこんなで、とうとう\n"
				"勝負の日がやって来ました。\n"
				"ハート君 は 覇亜富殿 に\n"
				"勝てるのでしょうか！？" );
			break;

		case 1:
			MikanDraw->Printf( 0, 0, 320, "まずはボーカル勝負です！" );

			if( MikanInput->GetMouseNum( 0 ) == 1 )
			{
				if( MyVO < YoVO )
				{
					lose ++;
				}
				else if( MyVO > YoVO )
				{
					win ++;
				}
			}

			break;

		case 2:
			if( MyVO < YoVO )
			{
				MikanDraw->Printf( 0, 0, 320, "負けました。" );
			}
			else if( MyVO == YoVO )
			{
				MikanDraw->Printf( 0, 0, 320, "引き分けました。" );
			}
			else if( MyVO > YoVO )
			{
				MikanDraw->Printf( 0, 0, 320, "勝ちました。" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n次はダンス勝負です！" );

			if( MikanInput->GetMouseNum( 0 ) == 1 )
			{
				if( MyDA < YoDA )
				{
					lose ++;
				}
				else if( MyDA > YoDA )
				{
					win ++;
				}
			}

			break;

		case 3:
			if( MyDA < YoDA )
			{
				MikanDraw->Printf( 0, 0, 320, "負けました。" );
			}
			else if( MyDA == YoDA )
			{
				MikanDraw->Printf( 0, 0, 320, "引き分けました。" );
			}
			else if( MyDA > YoDA )
			{
				MikanDraw->Printf( 0, 0, 320, "勝ちました。" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n次はダンス勝負です！" );

			if( MikanInput->GetMouseNum( 0 ) == 1 )
			{
				if( MyVI < YoVI )
				{
					lose ++;
				}
				else if( MyVI > YoVI )
				{
					win ++;
				}
			}

			break;

		case 4:
			if( MyVI < YoVI )
			{
				MikanDraw->Printf( 0, 0, 320, "負けました。" );
			}
			else if( MyVI == YoVI )
			{
				MikanDraw->Printf( 0, 0, 320, "引き分けました。" );
			}
			else if( MyVI > YoVI )
			{
				MikanDraw->Printf( 0, 0, 320, "勝ちました。" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n"
				"以上で勝負は終了です。\n"
				"さて、勝負の結果は？？" );

			break;

		case 5:
			MikanDraw->Printf( 0, 0, 320, "ハート君 は 覇亜富殿 に" );
			if( win < lose )
			{
				MikanDraw->Printf( 0, 0, 320, "\n\n負 け ま し た" );
				result = -1;
			}
			else if( win == lose )
			{
				MikanDraw->Printf( 0, 0, 320, "\n\n引 き 分 け ま し た" );
				result = 0;
			}
			else if( win > lose )
			{
				MikanDraw->Printf( 0, 0, 320, "\n\n勝 ち ま し た" );
				result = 1;
			}

			break;

		default:
			game = EPILO;
			mode = 0;
			break;
		}
		if( mode == 0 )
		{
			MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 0, 120, 120 );
		}
		else
		{
			MikanDraw->DrawTexture( 0,  80, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 2, 120, 120 );
		}


		if( MikanInput->GetMouseNum( 0 ) == 1 )
		{
			mode ++;
		}
		if( MikanInput->GetMouseNum( 1 ) == 1 )
		{
			if( mode == 5 )
			{
				mode = 100;
			}
			else
			{
				if( mode <= 1 )
				{
					if( MyVO < YoVO )
					{
						lose ++;
					}
					else if( MyVO > YoVO )
					{
						win ++;
					}

					if( MyDA < YoDA )
					{
						lose ++;
					}
					else if( MyDA > YoDA )
					{
						win ++;
					}

					if( MyVI < YoVI )
					{
						lose ++;
					}
					else if( MyVI > YoVI )
					{
						win ++;
					}
				}
				else if( mode == 2 )
				{
					if( MyDA < YoDA )
					{
						lose ++;
					}
					else if( MyDA > YoDA )
					{
						win ++;
					}

					if( MyVI < YoVI )
					{
						lose ++;
					}
					else if( MyVI > YoVI )
					{
						win ++;
					}
				}
				else if( mode = 3 )
				{
					if( MyVI < YoVI )
					{
						lose ++;
					}
					else if( MyVI > YoVI )
					{
						win ++;
					}
				}

				mode = 5;
			}
		}

		break;

	case EPILO:
		if( result == -1 )
		{
			switch( mode )
			{
			case 0:
				MikanDraw->Printf( 0, 0, 320,
					"ハート君 は無言で帰り、\n"
					"約束通り 覇亜富殿 は\n"
					"はーとちゃん に\n"
					"告白しました。" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 2, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 1:
				MikanDraw->Printf( 0, 0, 320,
					"聞く所に寄ると、\n"
					"彼らは暫く交際した後、\n"
					"別れてしまったそうです。" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 2, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 2:
				MikanDraw->Printf( 0, 0, 320,
					"しかし ハート君 は\n"
					"今回の勝負ですっかり\n"
					"自信を無くしたので\n"
					"告白しませんでした。" );
				MikanDraw->DrawTexture( 0, 180, 120, 0, 120 * 0, 120, 120 );
				break;

			case 3:
				MikanDraw->Printf( 0, 0, 320,
					"結果みんな不幸になり、\n"
					"あなたはその様を\n"
					"嘲笑いながらクールに\n"
					"去って行きました。" );
				MikanDraw->DrawTexture( 0, 180, 40, 120, 120 * 0, 120, 240 );
				break;

			case 4:
				MikanDraw->Printf( 0, 0, 320, "BAD END\n終わり" );
				break;

			default:
				return 1;
			}
		}
		else if( result == 0 )
		{
			switch( mode )
			{
			case 0:
				MikanDraw->Printf( 0, 0, 320,
					"引き分けの場合を決めて\n"
					"いなかった二人は、\n"
					"後日、再度勝負をする\n"
					"ことにして帰りました。" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 0, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 2, 120, 120 );
				break;

			case 1:
				MikanDraw->Printf( 0, 0, 320,
					"その時、二人の勝負を\n"
					"見ていた はーとちゃん が\n"
					"あなたの元へ\n"
					"駆け寄って来ました！");
				MikanDraw->DrawTexture( 0, 180, 120, 0, 120 * 1, 120, 120 );
				break;

			case 2:
				MikanDraw->Printf( 0, 0, 320,
					"「 ハート君 を真剣に\n"
					"プロデュースするあなたに\n"
					"惚れちゃいました。\n"
					"付き合って下さい！」" );
				MikanDraw->DrawTexture( 0, 180, 120, 0, 120 * 1, 120, 120 );
				break;

			case 3:
				MikanDraw->Printf( 0, 0, 320,
					"「これが、僕と嫁の\n"
					"馴れ初めです。\n"
					"情けは人の為ならず、\n"
					"という事ですね。」" );
				MikanDraw->DrawTexture( 0, 180, 40, 120, 120 * 0, 120, 240 );
				break;

			case 4:
				MikanDraw->Printf( 0, 0, 320, "TRUE END\n終わり" );
				break;

			default:
				return 1;
			}
		}
		else if( result == 1 )
		{
			switch( mode )
			{
			case 0:
				MikanDraw->Printf( 0, 0, 320,
					"ハート君 約束通り\n"
					"はーとちゃん に\n"
					"告白しました。" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 0, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 1:
				MikanDraw->Printf( 0, 0, 320,
					"その返事は\"OK\"！\n"
					"彼らは付き合い始め、\n"
					"5年後のクリスマスに\n"
					"結婚したそうです。" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 0, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 2:
				MikanDraw->Printf( 0, 0, 320,
					"それを聞いて、あなたは\n"
					"「プロデュースなんて\n"
					"しなければ良かった。」\n"
					"と、心底思いました。" );
				MikanDraw->DrawTexture( 0, 180, 40, 120, 120 * 0, 120, 240 );
				break;

			case 3:
				MikanDraw->Printf( 0, 0, 320, "HAPPY END\n終わり" );
				break;

			default:
				return 1;;
			}
		}

		if( MikanInput->GetMouseNum( 0 ) == 1 )
		{
			mode ++;
		}
		if( MikanInput->GetMouseNum( 1 ) == 1 )
		{
			if( result == -1 ||
				result == 0 )
			{
				if( mode == 4 )
				{
					return 1;
				}
				else
				{
					mode = 4;
				}
			}
			else if( result == 1 )
			{
				if( mode == 3 )
				{
					return 1;
				}
				else
				{
					mode = 3;
				}
			}
		}

		break;
	}

	if( MikanInput->GetKeyNum( K_ESC ) > 0 ||
		MikanInput->GetMouseNum( 2 ) > 0 )
	{
		return 1;
	}

	return 0;
}

void CleanUp( void )
{
}