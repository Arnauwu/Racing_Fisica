#pragma once

#include "Globals.h"
#include "Module.h"
#include "Entity.h"
#include "Player.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include <set>

class ModuleGame : public Module
{
public:
	enum class Maps
	{
		MOSS_GROTTO_1,
		MOSS_GROTTO_2,
		CRYSTAL_PEAK_1,
		CRYSTAL_PEAK_2,
		BELLHART_1,
		BELLHART_2
	};
	Maps map;
	Screens currentScreen;

	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void LoadMap(Maps _map);
	void carSetup(Car* _car, Characters* _char);

	void LoadScreen(Screens _screen);
	void UnloadScreen();

public:

	std::vector<Entity*> entities;

	PhysBody* sensor;
	bool sensed;

	Texture2D circle;
	Texture2D box;

	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;

	std::set<std::set<Entity*>> collidingEntities;

	ModulePlayer* player;

	PhysBody* CheckPoint1;
	PhysBody* CheckPoint2;
	PhysBody* CheckPoint3;
	PhysBody* CheckPoint4;

	



#pragma region maps
	const int MossGrottoEXT[196] = {
72, 237,
71, 917,
102, 920,
106, 952,
134, 954,
140, 985,
170, 986,
173, 1044,
195, 1051,
204, 1078,
259, 1083,
265, 1108,
290, 1109,
297, 1141,
325, 1144,
329, 1175,
356, 1175,
363, 1207,
516, 1208,
523, 1237,
1043, 1239,
1044, 1210,
1141, 1208,
1142, 1180,
1172, 1174,
1174, 1144,
1205, 1138,
1205, 1084,
1236, 1077,
1238, 1017,
1271, 1012,
1266, 719,
1235, 713,
1235, 686,
1201, 682,
1197, 621,
1174, 613,
1172, 573,
1205, 561,
1205, 507,
1234, 504,
1236, 374,
1267, 374,
1268, 240,
1240, 230,
1235, 107,
1205, 101,
1205, 74,
1174, 72,
1172, 42,
1109, 36,
1108, 9,
810, 12,
811, 37,
749, 42,
743, 72,
717, 73,
709, 169,
686, 170,
680, 197,
654, 202,
652, 598,
680, 599,
681, 630,
712, 632,
711, 645,
686, 652,
680, 677,
498, 680,
502, 653,
469, 650,
470, 630,
498, 630,
505, 601,
533, 598,
537, 570,
565, 567,
565, 537,
598, 535,
601, 506,
627, 500,
629, 209,
599, 202,
595, 141,
566, 137,
564, 106,
531, 102,
534, 73,
501, 68,
500, 40,
202, 42,
202, 72,
174, 74,
167, 102,
143, 104,
133, 163,
106, 171,
104, 230
	};
	const int MossGrottoINT[156] = {
	311, 278,
	309, 249,
	354, 248,
	362, 277,
	388, 280,
	392, 309,
	419, 313,
	421, 418,
	395, 426,
	389, 454,
	364, 457,
	361, 487,
	329, 492,
	326, 513,
	294, 525,
	299, 726,
	329, 730,
	330, 759,
	357, 761,
	360, 820,
	391, 824,
	395, 858,
	418, 857,
	431, 887,
	789, 883,
	788, 861,
	821, 857,
	820, 827,
	853, 820,
	853, 793,
	883, 786,
	883, 557,
	851, 551,
	852, 526,
	821, 520,
	821, 493,
	786, 487,
	788, 346,
	821, 338,
	822, 313,
	852, 307,
	852, 218,
	879, 219,
	884, 185,
	966, 185,
	972, 214,
	999, 217,
	1000, 246,
	1028, 246,
	1032, 305,
	1061, 312,
	1061, 323,
	1040, 328,
	1030, 391,
	999, 394,
	998, 447,
	972, 465,
	968, 518,
	938, 523,
	941, 721,
	964, 732,
	970, 791,
	998, 795,
	1001, 899,
	972, 908,
	966, 933,
	851, 932,
	853, 909,
	341, 902,
	339, 880,
	305, 871,
	307, 843,
	279, 839,
	269, 813,
	248, 809,
	245, 311,
	272, 312,
	279, 281
	};
#pragma endregion
};
