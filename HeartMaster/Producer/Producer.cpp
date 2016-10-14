//main��{&�R�����g�������
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
	MikanWindow->SetWindowName( "�n�[�g�}�X�^�[" );
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

		MikanDraw->Printf( 0, 480, 0, "��%d����", day );
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
		MikanDraw->Printf( 1, 70, 80, "�n�[�g�}�X�^�[" );
		MikanDraw->Printf( 0, 40, 300, "���̃Q�[���ł̓}�E�X���g����I" );
		MikanDraw->Printf( 0, 70, 340, "�I���̓}�E�X�z�C�[������I" );

		break;

	case PROLO:
		switch( mode )
		{
		case 0:
			MikanDraw->Printf( 0, 0, 320,
				"���鏊�� �n�[�g�N �Ƃ���\n"
				"����ȏ��N�����܂����B\n"
				"�ނ� �́[�Ƃ���� ��\n"
				"��D���ł����B" );
			MikanDraw->DrawTexture( 0,  80, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 1, 120, 120 );
			break;

		case 1:
			MikanDraw->Printf( 0, 0, 320,
				"������ނ� �́[�Ƃ���� ��\n"
				"�������邱�Ƃɂ��܂����B\n"
				"�������c����� �e���x�a ��\n"
				"�́[�Ƃ���� ���D���ł����B" );
			MikanDraw->DrawTexture( 0,  30, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 1, 120, 120 );
			MikanDraw->DrawTexture( 0, 330, 100, 0, 120 * 2, 120, 120 );
			break;

		case 2:
			MikanDraw->Printf( 0, 0, 320,
				"������15���ɂ��y��\n"
				"�b�������̌��ʁA\n"
				"�����ɏ����������ޏ���\n"
				"�������邱�Ƃɂ��܂����B" );
			MikanDraw->DrawTexture( 0,  80, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 2, 120, 120 );
			break;

		case 3:
			MikanDraw->Printf( 0, 0, 320,
				"���̏�ɋ��R�ʂ�߂���\n"
				"���Ȃ��́A�ʔ������Ȃ̂�\n"
				"�n�[�g�N ���v���f���[�X\n"
				"���邱�Ƃɂ��܂����B" );
			MikanDraw->DrawTexture( 0,  30, 40, 120, 120 * 0, 120, 240 );
			MikanDraw->DrawTexture( 0, 180, 100, 0, 120 * 0, 120, 120 );
			MikanDraw->DrawTexture( 0, 330, 100, 0, 120 * 2, 120, 120 );
			break;

		case 4:
			MikanDraw->Printf( 0, 0, 320,
				"�ނ��v���f���[�X����\n"
				"3����̏�����\n"
				"�������Ă����܂��傤�I" );
			MikanDraw->DrawTexture( 0,  80, 40, 120, 120 * 0, 120, 240 );
			MikanDraw->DrawTexture( 0, 280, 100, 0, 120 * 0, 120, 120 );
			break;

		case 5:
			MikanDraw->Printf( 0, 0, 320,
				"�u�v���f���[�T�[����A\n"
				"�X�������肢���܂��I�v" );
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
				"���̃��b�X�������܂����H\n"
				"�@�@�{�[�J��\n"
				"�@�@�_���X\n"
				"�@�@�r�W���A��" );

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
					"�o�b�h���b�X��" );
			}
			else if( r == 1 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"�m�[�}�����b�X��" );
			}
			else if( r == 2 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"�O�b�g���b�X��" );
			}
			else if( r == 3 )
			{
				MikanDraw->Printf( 0, 0, 320,
					"�p�[�t�F�N�g���b�X��" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n1�����I������B" );

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
				"����Ȃ���ȂŁA�Ƃ��Ƃ�\n"
				"�����̓�������ė��܂����B\n"
				"�n�[�g�N �� �e���x�a ��\n"
				"���Ă�̂ł��傤���I�H" );
			break;

		case 1:
			MikanDraw->Printf( 0, 0, 320, "�܂��̓{�[�J�������ł��I" );

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
				MikanDraw->Printf( 0, 0, 320, "�����܂����B" );
			}
			else if( MyVO == YoVO )
			{
				MikanDraw->Printf( 0, 0, 320, "���������܂����B" );
			}
			else if( MyVO > YoVO )
			{
				MikanDraw->Printf( 0, 0, 320, "�����܂����B" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n���̓_���X�����ł��I" );

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
				MikanDraw->Printf( 0, 0, 320, "�����܂����B" );
			}
			else if( MyDA == YoDA )
			{
				MikanDraw->Printf( 0, 0, 320, "���������܂����B" );
			}
			else if( MyDA > YoDA )
			{
				MikanDraw->Printf( 0, 0, 320, "�����܂����B" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n���̓_���X�����ł��I" );

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
				MikanDraw->Printf( 0, 0, 320, "�����܂����B" );
			}
			else if( MyVI == YoVI )
			{
				MikanDraw->Printf( 0, 0, 320, "���������܂����B" );
			}
			else if( MyVI > YoVI )
			{
				MikanDraw->Printf( 0, 0, 320, "�����܂����B" );
			}
			MikanDraw->Printf( 0, 0, 320, "\n\n"
				"�ȏ�ŏ����͏I���ł��B\n"
				"���āA�����̌��ʂ́H�H" );

			break;

		case 5:
			MikanDraw->Printf( 0, 0, 320, "�n�[�g�N �� �e���x�a ��" );
			if( win < lose )
			{
				MikanDraw->Printf( 0, 0, 320, "\n\n�� �� �� �� ��" );
				result = -1;
			}
			else if( win == lose )
			{
				MikanDraw->Printf( 0, 0, 320, "\n\n�� �� �� �� �� �� ��" );
				result = 0;
			}
			else if( win > lose )
			{
				MikanDraw->Printf( 0, 0, 320, "\n\n�� �� �� �� ��" );
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
					"�n�[�g�N �͖����ŋA��A\n"
					"�񑩒ʂ� �e���x�a ��\n"
					"�́[�Ƃ���� ��\n"
					"�������܂����B" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 2, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 1:
				MikanDraw->Printf( 0, 0, 320,
					"�������Ɋ��ƁA\n"
					"�ނ�͎b�����ۂ�����A\n"
					"�ʂ�Ă��܂��������ł��B" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 2, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 2:
				MikanDraw->Printf( 0, 0, 320,
					"������ �n�[�g�N ��\n"
					"����̏����ł�������\n"
					"���M�𖳂������̂�\n"
					"�������܂���ł����B" );
				MikanDraw->DrawTexture( 0, 180, 120, 0, 120 * 0, 120, 120 );
				break;

			case 3:
				MikanDraw->Printf( 0, 0, 320,
					"���ʂ݂�ȕs�K�ɂȂ�A\n"
					"���Ȃ��͂��̗l��\n"
					"�}�΂��Ȃ���N�[����\n"
					"�����čs���܂����B" );
				MikanDraw->DrawTexture( 0, 180, 40, 120, 120 * 0, 120, 240 );
				break;

			case 4:
				MikanDraw->Printf( 0, 0, 320, "BAD END\n�I���" );
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
					"���������̏ꍇ�����߂�\n"
					"���Ȃ�������l�́A\n"
					"����A�ēx����������\n"
					"���Ƃɂ��ċA��܂����B" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 0, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 2, 120, 120 );
				break;

			case 1:
				MikanDraw->Printf( 0, 0, 320,
					"���̎��A��l�̏�����\n"
					"���Ă��� �́[�Ƃ���� ��\n"
					"���Ȃ��̌���\n"
					"�삯����ė��܂����I");
				MikanDraw->DrawTexture( 0, 180, 120, 0, 120 * 1, 120, 120 );
				break;

			case 2:
				MikanDraw->Printf( 0, 0, 320,
					"�u �n�[�g�N ��^����\n"
					"�v���f���[�X���邠�Ȃ���\n"
					"���ꂿ�Ⴂ�܂����B\n"
					"�t�������ĉ������I�v" );
				MikanDraw->DrawTexture( 0, 180, 120, 0, 120 * 1, 120, 120 );
				break;

			case 3:
				MikanDraw->Printf( 0, 0, 320,
					"�u���ꂪ�A�l�Ɖł�\n"
					"��ꏉ�߂ł��B\n"
					"��͐l�ׂ̈Ȃ炸�A\n"
					"�Ƃ������ł��ˁB�v" );
				MikanDraw->DrawTexture( 0, 180, 40, 120, 120 * 0, 120, 240 );
				break;

			case 4:
				MikanDraw->Printf( 0, 0, 320, "TRUE END\n�I���" );
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
					"�n�[�g�N �񑩒ʂ�\n"
					"�́[�Ƃ���� ��\n"
					"�������܂����B" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 0, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 1:
				MikanDraw->Printf( 0, 0, 320,
					"���̕Ԏ���\"OK\"�I\n"
					"�ނ�͕t�������n�߁A\n"
					"5�N��̃N���X�}�X��\n"
					"�������������ł��B" );
				MikanDraw->DrawTexture( 0,  80, 120, 0, 120 * 0, 120, 120 );
				MikanDraw->DrawTexture( 0, 280, 120, 0, 120 * 1, 120, 120 );
				break;

			case 2:
				MikanDraw->Printf( 0, 0, 320,
					"����𕷂��āA���Ȃ���\n"
					"�u�v���f���[�X�Ȃ��\n"
					"���Ȃ���Ηǂ������B�v\n"
					"�ƁA�S��v���܂����B" );
				MikanDraw->DrawTexture( 0, 180, 40, 120, 120 * 0, 120, 240 );
				break;

			case 3:
				MikanDraw->Printf( 0, 0, 320, "HAPPY END\n�I���" );
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