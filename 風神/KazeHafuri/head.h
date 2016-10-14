#include <Mikan.h>
#include <time.h>

#define WIDTH	640
#define HEIGHT	480
#define LENGTH	400
#define FRAME	(WIDTH - LENGTH) / 2
#define GROUND	400

enum FontType {
	F_BW, F_MW, F_SW
};
#define FS_S 16
#define FS_M 32
#define FS_B 48

enum TextureType {
	T_BOAT, T_WAVE, T_WEIGHT, T_WING
};
#define WAVE_SIZE 20

enum SoundType {
	S_BGM, SE_SAVE, SE_FALL, SE_END
};



class Item {
public:		enum Status {
				YET, FALL, SUCCESS, FAIL
			};

private:	static const int standWing = 20, maxWing = 3 - 1;
			static const int standWeight = 4, maxWeight = 3;
			static const int speed = 3;
			int x, y;
			int wing, weight;
			Status status;

public:		Item();
			void move();
			Status getStatus();
			bool startFall();	// trueÇ»ÇÁóéÇøénÇﬂÇÈ
			void draw();
};


bool titleUpdate();
void mainInit();
bool mainUpdate();	// trueÇ≈ÉQÅ[ÉÄèIóπ
void mainDraw();
bool resultUpdate();


const int windMax = 10;
const int maxItem = 60;
const int boatW = 100;
const int boatH = 30;
const int boatS = 3;


enum Mode {
	TITLE, MAIN, RESULT, EXIT
} mode;
int timer;
Item item[maxItem];
int wind;
int boatX = LENGTH / 2;
bool boatRight = true;
