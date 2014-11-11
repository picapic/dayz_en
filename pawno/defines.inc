#if defined _DEFINES_INC_
	#endinput
#endif

#define _DEFINES_INC_

//database connection data
#define HOST "127.0.0.1" //host name/addr
#define USER "uzer" 	 //user name
#define PASSWD "pazzwd"  //user password
#define DBNAME "dayz"	 //database name

#define TD_COUNT 17		//количество дополнительных текстдравов меню
#define TD_COUNT_VEHICLE 2	//количество текстдравов показателей транспорта

#define TD_STATISTIC_DATA 3

#define VEHICLE_BUFFER_DATA_NUMBER 7

#define MAX_TURNS_OF_PREVIEW 12
#define MAX_INVENTORY_ON_VEHICLE 12

#define INVENTORY_TIMER_UPDATE 400
#define SENSORS_TIMER_UPDATE 5000
#define SENSORS_TIMER_SAVE 30000
#define SENSORS_TIMER_VEHICLES 15000
#define VEHICLE_DATA_SHOW_CHECK 5000
#define UPDATE_WEAPON_AMMO_TIMER 5000 //время обновления количества патронов после первого выстрела

#define NON_CHEATERS_INTERVAL 5000 //таймер проверки на читерство

#define START_HEALTH_VALUE 12000
#define MAX_HEALTH_VALUE 14000
#define START_HUNGER_VALUE 1000
#define MAX_HUNGER_VALUE 1200
#define START_THIRST_VALUE 1000
#define MAX_THIRST_VALUE 1200
#define START_WOUND_VALUE 0
#define MAX_WOUND_VALUE 1000 	   //max значение ранения для цвета сенсора
#define MAX_WOUND_REAL_VALUE 14000 //max значение самого ранения

#define CELL_SIZE 65.0

#define INVENTORY_START_POSITION_X 220.0
#define INVENTORY_START_POSITION_Y 190.0

#define VEHICLE_START_POSITION_X  32.0
#define VEHICLE_START_POSITION_Y 150.0

#define INVENTORY_AREA 1
#define OBJECTS_AREA 2
#define VEHICLE_AREA 3

//0.3z defines:
#define WEAPON_BODY_PART_CHEST 3
#define WEAPON_BODY_PART_CROTCH 4
#define WEAPON_BODY_PART_LEFT_ARM 5
#define WEAPON_BODY_PART_RIGHT_ARM 6
#define WEAPON_BODY_PART_LEFT_LEG 7
#define WEAPON_BODY_PART_RIGHT_LEG 8
#define WEAPON_BODY_PART_HEAD 9

#define BULLET_HIT_TYPE_NONE            0
#define BULLET_HIT_TYPE_PLAYER          1
#define BULLET_HIT_TYPE_VEHICLE         2
#define BULLET_HIT_TYPE_OBJECT          3
#define BULLET_HIT_TYPE_PLAYER_OBJECT   4

#define OBJECT_DISTANCE 400.0
#define STANDART_RANGE_VALUE 2.5 //максимальная дистанция до подбираемых объектов

#define DROP_DISTANCE_FROM_PLAYER 1.5 //дистанция до выкладываемого объекта

#define CreateObject(%1,%2,%3,%4,%5,%6,%7,%8) CreateDynamicObject(%1, %2, %3, %4, %5, %6, %7, -1, -1, -1, %8, 0.0)
#define IsValidObject IsValidDynamicObject
#define DestroyObject DestroyDynamicObject
