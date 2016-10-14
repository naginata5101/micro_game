#include <Mikan.h>

#include "head.h"


void SystemInit( void )
{
 MikanWindow->SetWindowName( "MEIRO" );
}

//MainLoop�J�n�O��1�x�������s
//�e�N�X�`���̓ǂݍ��݂Ȃǂ̃Q�[���J�n�O�̏������s��
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

//1�b�Ԃ�60����s
//0��Ԃ��Ă���Ԃ͉��x�ł����s�����
int MainLoop( void )
{
	//��ʂ̏�����
	MikanDraw->ClearScreen( );





	//�Q�[���̏�����
	if( PlayerHP <= 0 )
	{
		//�X�R�A�̏���
		if( Score > HighScore )
		{
			HighScore = Score;
		}
			//�t�@�C���ɑ΂�����o��
		FILE *fp;
		char str[ 8 ] = "0";
		if( fopen_s( &fp, "data/score", HighScore > 0 ? "w" : "r") ){ return 0; }
		if( HighScore > 0 )
		{
			// �t�@�C���֏������ށB
			fprintf( fp, "%d", HighScore );
		}else
		{
			// �t�@�C������l��ǂݍ��ށB
			fgets( str, 256, fp );
		HighScore = atoi( str );
		}
		fclose( fp );



		//�G�̏������
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

		//�v���C���[�̏����̗͂�10
		PlayerHP = 10;
		//�o�ߎ��Ԃ�0�ɂ���
		FrameTime = 0;
		//�Q�[���͂܂��n�܂��Ă��Ȃ��I
		Start = false;
	}







	//Enter�������ꂽ��Q�[���J�n
	if( MikanInput->GetKeyNum( K_ENTER ) != 0 &&
		Start == false )
	{
		Score = 0;
		Start = true;
	}


	//�J�[�\���̈ړ�
	//�}�E�X�ɂ��ړ�
	if( ( MikanInput->GetMousePosX() >= 0 &&
		  MikanInput->GetMousePosX() <= WIDHT ) &&
		( MikanInput->GetMousePosY() >= 2 * MapSize &&
		  MikanInput->GetMousePosY() <= HEIGHT ) )
	{
		CursorX = ( MikanInput->GetMousePosX() / MapSize ) * MapSize;
		CursorY = ( MikanInput->GetMousePosY() / MapSize ) * MapSize;
	}
	//�L�[�{�[�h�ɂ��ړ�
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

	//�J�[�\���̏ꏊ�ɖ�����z�u
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
	//�����̓P��
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



	//���j�b�g�̑I��
	if( MikanInput->GetKeyNum( K_C ) == 1 ||
		MikanInput->GetMouseNum( 2 ) == 1 )
	{
		//�J�[�\����ɉ������Ȃ���ΑI������
		Select = -1;
		//�J�[�\���̈ʒu�̓G���j�b�g��I��
		for( count1 = 0; count1 < MAX1; count1 ++ )
		{
			if( EnemyX[ count1 ] == CursorX &&
				EnemyY[ count1 ] == CursorY )
			{
				Select = count1 + 100;
				break;
			}
		}
		//�������������j�b�g��I��
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


	//����I�ɓG���o��������
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

	//�G�̃��x�����オ�閈��10�b�̋x�e
	if( FrameTime - WaveTime > 700 &&
		EnemyAppear % 10 == 0 &&
		EnemyAppear > 0 )
	{
		WaveTime = FrameTime;
	}


	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		//�G�A�����̃R���f�B�V�����ɍ��W�̒l������
		EnemyCondition[ count1 ]
			= map[ EnemyY[ count1 ] / MapSize ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] ];
		UnitCondition[ count1 ]
			= map[ UnitY[ count1 ] / MapSize ][ UnitX[ count1 ] / MapSize ][ UnitZ[ count1 ] ];

		for( count3 = 0; count3 < MAX2; count3 ++ )
		{
			//�e�X�̓��ɂ���G�̐��𐔂���
			if( EnemyCondition[ count1 ] == count3 )
			{
				EnemyNumber[ count3 ] = EnemyNumber[ count3 ] + 1;
			}
			//�����̐���������
			if( UnitCondition[ count1 ] == count3 )
			{
				UnitNumber[ count3 ] = UnitNumber[ count3 ] + 1;
			}
		}
	}
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		//�G�������ꓹ�i"3"�j�ɗ�����
		if( EnemyCondition[ count1 ] == 3 )
		{
			//���������Ȃ�����I��
			if( UnitNumber[ 4 ] > UnitNumber[ 5 ] )
			{
				EnemyCondition[ count1 ] = 5;
			}
			if( UnitNumber[ 4 ] < UnitNumber[ 5 ] )
			{
				EnemyCondition[ count1 ] = 4;
			}
			//�����̐��������Ȃ�ł��G�����Ȃ�����I��
			if( UnitNumber[ 4 ] == UnitNumber[ 5 ] )
			{
				if( EnemyNumber[ 4 ] > EnemyNumber[ 5 ] )
				{
					EnemyCondition[ count1 ] = 5;
				}
			}
			//����̓����ǂ����Ă�������������
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
			//����I�ɓG�̈ړ����������肷��
			if( FrameTime % MapSize == 0 )
			{
				//��芸�����ړ��������Ȃ���
				NextMove[ count1 ] = 0;
				//�P�i��ɋ󂢂Ă���}�X������΂������ړ������ɂ���
				if( map[ EnemyY[ count1 ] / MapSize - 1 ][ EnemyX[ count1 ] / MapSize ][ EnemyZ[ count1 ] + 1 ]
						>= EnemyCondition[ count1 ] &&
					LastMove[ count1 ] != 12 &&
					LastMove[ count1 ] != 22 &&
					LastMove[ count1 ] != 32 )
				{
					NextMove[ count1 ] = 18;
					for( count2 = 0; count2 < MAX1; count2 ++ )
					{
						//�������A�������G�̎��̑��݃}�X�Ȃ�Γ����Ȃ�
						if( EnemyX[ count1 ] == EnemyNextX[ count2 ] &&
							EnemyY[ count1 ] == EnemyNextY[ count2 ] + MapSize &&
							EnemyZ[ count1 ] == EnemyNextZ[ count2 ] - 1 )
						{
							NextMove[ count1 ] = 0;
						}
						//�������A���������̕ǂ̑��݃}�X�Ȃ�Γ����Ȃ�
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
				//�����i�ɋ󂢂Ă���}�X������΂������ړ������ɂ���
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
					//�P�i���ɋ󂢂Ă���}�X������΂������ړ������ɂ���
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


				//���̈ړ��ɍ��킹��
				//��i��Ɉړ�
				if( NextMove[ count1 ] == 18 ||
					NextMove[ count1 ] == 16 ||
					NextMove[ count1 ] == 12 ||
					NextMove[ count1 ] == 14 )
				{
						EnemyZ[ count1 ] = EnemyZ[ count1 ] + 1;
				}
				//��i���Ɉړ�
				if( NextMove[ count1 ] == 38 ||
					NextMove[ count1 ] == 36 ||
					NextMove[ count1 ] == 32 ||
					NextMove[ count1 ] == 34 )
				{
						EnemyZ[ count1 ] = EnemyZ[ count1 ] - 1;
				}

				//�G�̎���X���𒲐�
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
				//�G�̎���Y���𒲐�
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
				//�G�̎���Z���𒲐�
				EnemyNextZ[ count1 ] = EnemyZ[ count1 ];

				//�O��̈ړ��̊Ǘ�
				if( NextMove[ count1 ] != 0 )
				{
					LastMove[ count1 ] = NextMove[ count1 ];
				}
			}
			
			//��Ɉړ�
			if( NextMove[ count1 ] == 18 ||
				NextMove[ count1 ] == 28 ||
				NextMove[ count1 ] == 38 )
			{
				EnemyY[ count1 ] = EnemyY[ count1 ] - 1;
			}
			//�E�Ɉړ�
			if( NextMove[ count1 ] == 16 ||
				NextMove[ count1 ] == 26 ||
				NextMove[ count1 ] == 36 )
			{
				EnemyX[ count1 ] = EnemyX[ count1 ] + 1;
			}
			//���Ɉړ�
			if( NextMove[ count1 ] == 12 ||
				NextMove[ count1 ] == 22 ||
				NextMove[ count1 ] == 32 )
			{
				EnemyY[ count1 ] = EnemyY[ count1 ] + 1;
			}
			//���Ɉړ�
			if( NextMove[ count1 ] == 14 ||
				NextMove[ count1 ] == 24 ||
				NextMove[ count1 ] == 34 )
			{
				EnemyX[ count1 ] = EnemyX[ count1 ] - 1;
			}
		}
		
		for( count2 = 0; count2 < MAX1; count2 ++ )
		{
			//�G�̐i�ޓ��ɖ�����������U���I
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

			//�����̍U���͈͓��ɓG��������U�����ˁI
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
		//�����̍U�����e�I
		if( FrameTime - Unit_LastATtime[ count1 ] == Unit_ATeffectTime )
		{
			EnemyHP[ Unit_ATtarget[ count1 ] ]
				= EnemyHP[ Unit_ATtarget[ count1 ] ] - Unit_ATpower;
		}

		//�v���C���[�̗̑ͥ�X�R�A�̏���
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
		//�G�̗̑͂�0�ȉ��ɂȂ�or�S�[���ɒ��������A�����ݒ�ɖ߂�
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
		//�����̗̑͂�0�ȉ��ɂȂ������O�ޏ�
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

	//�G�E�����̐��̏���0�ɖ߂�
	for( count3 = 0; count3 < MAX2; count3 ++ )
	{
		EnemyNumber[ count3 ] = 0;
		UnitNumber[ count3 ] = 0;
	}








	//�w�i�̕`��
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

	//�����̍U���͈͂̕`��
	//�I�����������ɂ���
	if( Select >= 0 &&
		Select < 100 )
	{
		//�͈͂̉~��`��
		MikanDraw->DrawTexture( 1, UnitX[ Select ] - ( MapSize / 2 * 3 ), UnitY[ Select ] - ( MapSize / 2 * 3 ),
			1 * MapSize, 0 * MapSize, 4 * MapSize, 4 * MapSize );
		//�͈͓��̃}�X�����点��
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
	//�J�[�\����̖����ɂ���
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
				//�͈͂̉~��`��
				MikanDraw->DrawTexture( 1, UnitX[ count1 ] - ( MapSize / 2 * 3 ), UnitY[ count1 ] - ( MapSize / 2 * 3 ),
					1 * MapSize, 0 * MapSize, 4 * MapSize, 4 * MapSize );
				//�͈͓��̃}�X�����点��
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



	//�G�̕`��
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( EnemyHP[ count1 ] > 0 )
		{
			MikanDraw->DrawTexture( 0, EnemyX[ count1 ], EnemyY[ count1 ],
				( 4 + ( ( FrameTime / 10 ) % 3 ) ) * MapSize, 1 * MapSize, MapSize, MapSize );
		}
	}
	//�����̕`��
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( UnitHP[ count1 ] > 0 )
		{
			MikanDraw->DrawTexture( 0, UnitX[ count1 ], UnitY[ count1 ],
				( 4 + ( ( FrameTime / 10 ) % 3 ) ) * MapSize, 2 * MapSize, MapSize, MapSize);
		}
	}
	//�J�[�\���̕`��
	MikanDraw->SetRender( DRAW_ADD );
	MikanDraw->DrawTexture( 1, CursorX, CursorY, 0 * MapSize, 1 * MapSize, MapSize, MapSize );
	MikanDraw->SetRender( DRAW_TRC );

	//�U���G�t�F�N�g�̕`��
	//����
	for( count1 = 0; count1 < MAX1; count1 ++ )
	{
		if( UnitHP[ count1 ] > 0 )
		{
			//�U�������ōs����I
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
			//�G�N�X�v���[�W����!!
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
	//�G
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

	//�R�����g�̕\��
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


	//�_ �� �I
	MikanDraw->Printf( 0, -2, 48, "-------------------------------------------" );






	//BGM��ON/OFF
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



	//�o�ߎ��Ԃ𑝂₷
	FrameTime = FrameTime+ 1 ;



	//
	if(	MikanInput->GetKeyNum( K_ESCAPE ) > 0 )
	{
		//�X�R�A�̏���
		if( Score > HighScore )
		{
			HighScore = Score;
		}
			//�t�@�C���ɑ΂�����o��
		FILE *fp;
		char str[ 8 ] = "0";
		if( fopen_s( &fp, "data/score", HighScore > 0 ? "w" : "r") ){ return 0; }
		if( HighScore > 0 )
		{
			// �t�@�C���֏������ށB
			fprintf( fp, "%d", HighScore );
		}else
		{
			// �t�@�C������l��ǂݍ��ށB
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

//MainLoop�I�����1�x�������s
//��Еt���Ȃǂ��s��
void CleanUp( void )
{
}