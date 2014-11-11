#if defined _SENSORS_INC_
	#endinput
#endif

#define _SENSORS_INC_

#include<a_samp>
#include"global.inc"
#include"load_objects.inc"

#define SENSORS_NUMBER 4
#define SENSORS_START_POSITION_X 610.0
#define SENSORS_START_POSITION_Y 350.0
#define SENSORS_SIZE 15
#define SENSORS_INTERVAL 5

#define SENSOR_HEALTH_NUM 0
#define SENSOR_HUNGER_NUM 1
#define SENSOR_THIRST_NUM 2
#define SENSOR_WOUND_NUM 3

new PlayerText:sensors[MAX_PLAYERS][SENSORS_NUMBER];
new Text:smokescreen[3];

forward update_character_state(); //функция таймера сохранение состояния персонажа в БД
forward update_live_sensors(); //функция таймера обновления состояния персонажа
forward update_vehicle_state(); //функция обновления состояния авто

forward cheater_finder(); //функция вычисления читеров

forward create_sensors(playerid); //сенсоры
forward destroy_sensors(playerid);
forward create_vehicle_sensors(playerid, vehicleid);
forward destroy_vehicle_sensors(playerid);

forward update_vehicle_sensors(playerid);
forward update_sensor_health(playerid, health);
forward update_sensor_hunger(playerid, hunger);
forward update_sensor_thirst(playerid, thirst);
forward update_sensor_wound(playerid, wound);
forward create_smokescreen(); //завеса на радар и время
forward create_statistic_data(playerid);
forward show_smokescreen(playerid);
forward hide_smokescreen(playerid);
forward show_smoke_map(playerid);
forward hide_smoke_map(playerid);
forward show_smoke_statistic(playerid);
forward hide_smoke_statistic(playerid);
forward show_statistic_data(playerid);
forward hide_statistic_data(playerid);
forward update_statistic_data(playerid);

forward destroy_smokescreen();
forward destroy_statistic_data(playerid);
forward IsPlayerSpawned(playerid);

public update_character_state()
{
	new i;

	for(i = 0; i < MAX_PLAYERS; ++i)
	{
		if(!IsPlayerConnected(i))
		    continue;

		if(!IsPlayerSpawned(i))
	        continue;

		if(gHealth[i] <= 0)
		    continue;

//		set_character_state(i, gHealth[i], gHunger[i], gThirst[i], gWound[i]);
//		save_player_position(i);
		save_state_and_position(i);
	}
}

public update_live_sensors()
{
	new i;

	for(i = 0; i < MAX_PLAYERS; ++i)
	{
		if(!IsPlayerConnected(i))
	    continue;

		if(!IsPlayerSpawned(i))
	        continue;

		if(gHealth[i] <= 0)
		    continue;
/*
		if(gHunger[i] > MAX_HUNGER_VALUE)
		    gHunger[i] = MAX_HUNGER_VALUE;

		if(gThirst[i] > MAX_THIRST_VALUE)
		    gThirst[i] = MAX_THIRST_VALUE;
*/
		gHunger[i] = gHunger[i] - 1;
		gThirst[i] = gThirst[i] - 2;
		if(gWound[i] != 0)
			gHealth[i] = gHealth[i] - gWound[i];

		//обновляем сенсоры
		update_sensor_health(i, gHealth[i]);
		update_sensor_hunger(i, gHunger[i]);
		update_sensor_thirst(i, gThirst[i]);
		
		update_statistic_data(i);
	}
}

public update_vehicle_state()
{
	new vehicleid, id;

	for(vehicleid = 0; vehicleid < MAX_VEHICLES; ++vehicleid)
	{
	    if(gVeh[vehicleid][0] <= 0)
	    {
			//античит на случай, если факен читеры додумаются созавать авто с уже заведённым двигателем
			new Float:health;
			GetVehicleHealth(vehicleid, health);
	        if(health > 0.0)
	        {
				new bool:IsAdmin;
				new playerid;
				
				IsAdmin = false;
	            for(playerid = 0; playerid < MAX_PLAYERS; ++playerid)
	            {
	            	if(IsPlayerInVehicle(playerid, vehicleid))
	            	{
						IsAdmin = bool:IsPlayerAdmin(playerid);
						break;
	            	}
	            }
	            if(!IsAdmin)
	            {
			        SendClientMessage(playerid, 0xFFAACCFF, "It was an admin's vehicle!");
		            DestroyVehicle(vehicleid);
				}
	        }
	        continue;
		}

		if(gVeh[vehicleid][4] > 0)
		{
		    //уменьшаем топливо в баке
		    gVeh[vehicleid][3] = gVeh[vehicleid][3] - gVeh[vehicleid][5];
		    //сохраняем позицию
			save_vehicle_state(INVALID_PLAYER_ID, vehicleid);
		}

//		if(gVeh[vehicleid][3] < 0 || gVeh[vehicleid][2] < 0) //не надо сравнивать с нулём, т.к. это чревато бесконечными вызовами!
//			save_vehicle_state(INVALID_PLAYER_ID, vehicleid);
	}

	for(id = 0; id < MAX_PLAYERS; ++id)
	{
//		if(!IsPlayerConnected(id))
//		    continue;
	    if(gVehicleDataShow[id] > 0)
		{
		    update_vehicle_sensors(id);
		}
	}
}

public cheater_finder()
{
	new playerid;
	new Float:x, Float:y, Float:z;
	new Float:max_path;
	new msg[128], name[64];
	
	max_path = floatmul(55.0,float(NON_CHEATERS_INTERVAL/1000)); //max 200 км/ч
	for(playerid = 0; playerid < MAX_PLAYERS; ++playerid)
	{
		if(IsPlayerAdmin(playerid) || gPlayersID[playerid] <= 0 || gNonCheaters[playerid][2] == 0.0 || !IsPlayerSpawned(playerid))
		    continue;

		GetPlayerPos(playerid, x, y, z);
		if(VectorSize(gNonCheaters[playerid][0]-x,gNonCheaters[playerid][1]-y,gNonCheaters[playerid][2]-z) > max_path)
		{
			GetPlayerName(playerid, name, sizeof(name));
		    format(msg, sizeof(msg), "%s(id:%d) seems to be a CHEATER!", name, playerid);
		    SendClientMessageToAll(0xFF0000FF, msg);
		}
		gNonCheaters[playerid][0] = x;
		gNonCheaters[playerid][1] = y;
		gNonCheaters[playerid][2] = z;
	}
}

public create_sensors(playerid)
{
	new i;
	
	for(i = 0; i < SENSORS_NUMBER; ++i)
	{
		switch(i)
		{
			case SENSOR_HEALTH_NUM:
			{
			    //здоровье
				sensors[playerid][i] = CreatePlayerTextDraw(playerid, SENSORS_START_POSITION_X, SENSORS_START_POSITION_Y+i*(SENSORS_SIZE+SENSORS_INTERVAL), "hud:radar_Girlfriend");
				gHealth[playerid] = get_character_health(playerid);
				update_sensor_health(playerid, gHealth[playerid]);
			}
			case SENSOR_HUNGER_NUM:
			{
			    //жратва
				sensors[playerid][i] = CreatePlayerTextDraw(playerid, SENSORS_START_POSITION_X, SENSORS_START_POSITION_Y+i*(SENSORS_SIZE+SENSORS_INTERVAL), "hud:radar_dateFood");
				gHunger[playerid] = get_character_hunger(playerid);
				update_sensor_hunger(playerid, gHunger[playerid]);
			}
			case SENSOR_THIRST_NUM:
			{
				//питьё
				sensors[playerid][i] = CreatePlayerTextDraw(playerid, SENSORS_START_POSITION_X, SENSORS_START_POSITION_Y+i*(SENSORS_SIZE+SENSORS_INTERVAL), "hud:radar_Diner");
				gThirst[playerid] = get_character_thirst(playerid);
				update_sensor_thirst(playerid, gThirst[playerid]);
			}
			case SENSOR_WOUND_NUM:
			{
			    //капля
				sensors[playerid][i] = CreatePlayerTextDraw(playerid, SENSORS_START_POSITION_X, SENSORS_START_POSITION_Y+i*(SENSORS_SIZE+SENSORS_INTERVAL), "hud:radar_centre");
				gWound[playerid] = get_character_wound(playerid);
				update_sensor_wound(playerid, gWound[playerid]);
			}
			case 4:
			{
			    //hud:radar_ammugun - пистолет
			    //hud:radar_spray - спрэй
			    //hud:radar_hostpital - больничный крест
			    //hud:radar_impound - машина
//				sensors[playerid][i] = CreatePlayerTextDraw(playerid, SENSORS_START_POSITION_X, SENSORS_START_POSITION_Y+i*(SENSORS_SIZE+SENSORS_INTERVAL), "hud:radar_impound");
//	  	    	PlayerTextDrawColor(playerid, sensors[playerid][i], 0x00FF00FF);//Цвет
			}
			case 5:
			{
			}
		}
	    PlayerTextDrawFont(playerid, sensors[playerid][i], 4);//Шрифт
	    PlayerTextDrawTextSize(playerid, sensors[playerid][i], SENSORS_SIZE, SENSORS_SIZE);//Размер
		PlayerTextDrawShow(playerid, sensors[playerid][i]);
	}
	gScores[playerid] = get_character_scores(playerid);
	SetPlayerScore(playerid, gScores[playerid]);
}

public destroy_sensors(playerid)
{
	new i;
	
	for(i = 0; i < SENSORS_NUMBER; ++i)
	{
		if(sensors[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, sensors[playerid][i]);
			sensors[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
}

//вывести показатели авто
public create_vehicle_sensors(playerid, vehicleid)
{
	new Float:level;
	new color, buff;

	//если расход топлива нулевой - не показываем состояние бака
	if(gVeh[vehicleid][5] == 0)
	{
        destroy_vehicle_sensors(playerid);
	    gTdDataVehicle[playerid][0] = PlayerText:INVALID_TEXT_DRAW;
	    gTdDataVehicle[playerid][1] = PlayerText:INVALID_TEXT_DRAW;
	}
	else
	{
	    if(gTdDataVehicle[playerid][0] == PlayerText:INVALID_TEXT_DRAW)
	    {
			//уровень топлива автомобиля - фон
			gTdDataVehicle[playerid][0] = CreatePlayerTextDraw(playerid, VEHICLE_START_POSITION_X+2.0, VEHICLE_START_POSITION_Y-2.0, "~n~");
		}
		PlayerTextDrawUseBox(playerid, gTdDataVehicle[playerid][0], 1);
		PlayerTextDrawBoxColor(playerid, gTdDataVehicle[playerid][0], 0x00000055); //цвет
		PlayerTextDrawSetShadow(playerid, gTdDataVehicle[playerid][0], 0);
		PlayerTextDrawColor(playerid, gTdDataVehicle[playerid][0], 0x00000000);
		PlayerTextDrawLetterSize(playerid, gTdDataVehicle[playerid][0], 300.0, 8.5);
		PlayerTextDrawBackgroundColor(playerid, gTdDataVehicle[playerid][0], 0x00000055);
		PlayerTextDrawTextSize(playerid, gTdDataVehicle[playerid][0], 18.0, 70.0); //размер

	    if(gTdDataVehicle[playerid][1] == PlayerText:INVALID_TEXT_DRAW)
	    {
			//уровень топлива автомобиля
			gTdDataVehicle[playerid][1] = CreatePlayerTextDraw(playerid, VEHICLE_START_POSITION_X, VEHICLE_START_POSITION_Y+75.0, "~n~");
		}
		PlayerTextDrawUseBox(playerid, gTdDataVehicle[playerid][1], 1);
		PlayerTextDrawSetShadow(playerid, gTdDataVehicle[playerid][1], 0);
		PlayerTextDrawColor(playerid, gTdDataVehicle[playerid][1], 0x00000000);
		PlayerTextDrawBackgroundColor(playerid, gTdDataVehicle[playerid][1], 0x00FF0055);
		PlayerTextDrawTextSize(playerid, gTdDataVehicle[playerid][1], 20.0, 0.0);

	    if(gVeh[vehicleid][3] < gVeh[vehicleid][6])
	    {
	        buff = (256*gVeh[vehicleid][3])/gVeh[vehicleid][6];
	        color = ((0xFF-buff) << 24) | (buff<<16) | 0x55;
	    }
		else
		    color = 0x00FF0055;

		if(gVeh[vehicleid][3] == 0)
		{
		    level = -0.4;
		}
		else
		{
			level = ((-8.1)*Float:gVeh[vehicleid][3])/Float:gVeh[vehicleid][6] - 0.5;
			if(level < -8.6)
			    level = -8.6;
	    }

		PlayerTextDrawBoxColor(playerid, gTdDataVehicle[playerid][1], color); //цвет
		PlayerTextDrawLetterSize(playerid, gTdDataVehicle[playerid][1], 300.0, level); //от -0.4 до -8.6 //размер

		//фон уровня бензина
		PlayerTextDrawShow(playerid, gTdDataVehicle[playerid][0]);
		//уровень бензина
		PlayerTextDrawShow(playerid, gTdDataVehicle[playerid][1]);
	}
	
	gVehicleDataShow[playerid] = vehicleid;
}

//убрать показатели авто
public destroy_vehicle_sensors(playerid)
{
	new i;

	for(i = 0; i < TD_COUNT_VEHICLE; ++i)
	{
		if(gTdDataVehicle[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
		{
			PlayerTextDrawDestroy(playerid, gTdDataVehicle[playerid][i]);
			gTdDataVehicle[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	gVehicleDataShow[playerid] = 0;
}

public update_vehicle_sensors(playerid)
{
	new vehicleid;
	new Float:level;
	new color, buff;

	if(playerid == INVALID_PLAYER_ID)
	    return;

	//если сенсор не отображается - выходим
	if(gVehicleDataShow[playerid] == 0)// || !IsPlayerInAnyVehicle(playerid))
	    return;

	vehicleid = gVehicleDataShow[playerid];

	//если расход топлива нулевой - не показываем состояние бака
	if(gVeh[vehicleid][5] == 0)
	{
        destroy_vehicle_sensors(playerid);
	    gTdDataVehicle[playerid][0] = PlayerText:INVALID_TEXT_DRAW;
	    gTdDataVehicle[playerid][1] = PlayerText:INVALID_TEXT_DRAW;
	}
	else
	{
	    if(gTdDataVehicle[playerid][1] == PlayerText:INVALID_TEXT_DRAW)
	    {
			//уровень топлива автомобиля
			gTdDataVehicle[playerid][1] = CreatePlayerTextDraw(playerid, VEHICLE_START_POSITION_X, VEHICLE_START_POSITION_Y+75.0, "~n~");
		}
		//уровень бензина
		PlayerTextDrawHide(playerid, gTdDataVehicle[playerid][1]);
		
		PlayerTextDrawUseBox(playerid, gTdDataVehicle[playerid][1], 1);
		PlayerTextDrawSetShadow(playerid, gTdDataVehicle[playerid][1], 0);
		PlayerTextDrawColor(playerid, gTdDataVehicle[playerid][1], 0x00000000);
		PlayerTextDrawTextSize(playerid, gTdDataVehicle[playerid][1], 20.0, 0.0);
		PlayerTextDrawBackgroundColor(playerid, gTdDataVehicle[playerid][1], 0x00FF0055);

	    if(gVeh[vehicleid][3] < gVeh[vehicleid][6])
	    {
	        buff = (256*gVeh[vehicleid][3])/gVeh[vehicleid][6];
	        color = ((0xFF-buff) << 24) | (buff<<16) | 0x55;
	    }
		else
		    color = 0x00FF0055;

		if(gVeh[vehicleid][3] == 0)
		{
		    level = -0.4;
		}
		else
		{
			level = ((-8.1)*Float:gVeh[vehicleid][3])/Float:gVeh[vehicleid][6] - 0.5;
			if(level < -8.6)
			    level = -8.6;
	    }
		PlayerTextDrawBoxColor(playerid, gTdDataVehicle[playerid][1], color); //цвет
		PlayerTextDrawLetterSize(playerid, gTdDataVehicle[playerid][1], 300.0, level); //от -0.4 до -8.6

		//уровень бензина
		PlayerTextDrawShow(playerid, gTdDataVehicle[playerid][1]);
	}
}

public update_sensor_health(playerid, health)
{
	new buff, color;
	
	if(sensors[playerid][SENSOR_HEALTH_NUM] == PlayerText:INVALID_TEXT_DRAW)
	    return;

	PlayerTextDrawHide(playerid, sensors[playerid][SENSOR_HEALTH_NUM]);

	if(health <= 0)
	{
	    kill_character(playerid);
	    return;
	}
	
	if(health > START_HEALTH_VALUE)
	    health = START_HEALTH_VALUE;
	    
    if(health < START_HEALTH_VALUE)
    {
        buff = (256*health)/START_HEALTH_VALUE;
		//0xFF000088 Крайний цвет
        color = ((0xFF-buff) << 24) | (buff<<16) | 0x88;
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_HEALTH_NUM], color);//Цвет
    }
	else
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_HEALTH_NUM], 0x00FF0088);//Цвет
	PlayerTextDrawShow(playerid, sensors[playerid][SENSOR_HEALTH_NUM]);
	
//	SetPlayerHealth(playerid, floatmul(1000.0,(floatdiv(float(health), float(START_HEALTH_VALUE)))));
	buff = health*100/START_HEALTH_VALUE;
	SetPlayerHealth(playerid, buff>8?buff:8);
	
	update_statistic_data(playerid); //обновляем данные игрока
}

public update_sensor_hunger(playerid, hunger)
{
	new buff, color;

	if(sensors[playerid][SENSOR_HUNGER_NUM] == PlayerText:INVALID_TEXT_DRAW)
	    return;

	PlayerTextDrawHide(playerid, sensors[playerid][SENSOR_HUNGER_NUM]);

	if(hunger <= 0)
	{
	    kill_character(playerid);
	    return;
	}

	if(hunger > START_HUNGER_VALUE)
	    hunger = START_HUNGER_VALUE;
	    
    if(hunger < START_HUNGER_VALUE)
    {
        buff = (256*hunger)/START_HUNGER_VALUE;
        color = ((0xFF-buff) << 24) | (buff<<16) | 0x88;
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_HUNGER_NUM], color);//Цвет
    }
	else
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_HUNGER_NUM], 0x00FF0088);//Цвет
	PlayerTextDrawShow(playerid, sensors[playerid][SENSOR_HUNGER_NUM]);
}

public update_sensor_thirst(playerid, thirst)
{
	new buff, color;

	if(sensors[playerid][SENSOR_THIRST_NUM] == PlayerText:INVALID_TEXT_DRAW)
	    return;

	PlayerTextDrawHide(playerid, sensors[playerid][SENSOR_THIRST_NUM]);

	if(thirst <= 0)
	{
	    kill_character(playerid);
	    return;
	}

	if(thirst > START_THIRST_VALUE)
	    thirst = START_THIRST_VALUE;
    if(thirst < START_THIRST_VALUE)
    {
        buff = (256*thirst)/START_THIRST_VALUE;
        color = ((0xFF-buff) << 24) | (buff<<16) | 0x88;
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_THIRST_NUM], color);//Цвет
    }
	else
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_THIRST_NUM], 0x00FF0088);//Цвет
	PlayerTextDrawShow(playerid, sensors[playerid][SENSOR_THIRST_NUM]);
}

public update_sensor_wound(playerid, wound)
{
	new buff, color;

	if(sensors[playerid][SENSOR_WOUND_NUM] == PlayerText:INVALID_TEXT_DRAW)
	    return;

	PlayerTextDrawHide(playerid, sensors[playerid][SENSOR_WOUND_NUM]);

	if(wound > MAX_WOUND_VALUE)
	    wound = MAX_WOUND_VALUE;
	    
    if(wound > 0)
    {
        buff = (256*wound)/MAX_WOUND_VALUE + 0xAA;
        if(buff >= 0xFF)
            buff = 0xFF;
        color = (buff<<24) | ((0xFF-buff) << 16) | 0x88;
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_WOUND_NUM], color);//Цвет
	}
	else
	    PlayerTextDrawColor(playerid, sensors[playerid][SENSOR_WOUND_NUM], 0x00FF0088);//Цвет
	PlayerTextDrawShow(playerid, sensors[playerid][SENSOR_WOUND_NUM]);
}

public create_smokescreen()
{
	smokescreen[0] = TextDrawCreate(25.0, 330.0, "hud:radar_north");
    TextDrawTextSize(smokescreen[0], 125.0, 104.0);//Размер
    TextDrawFont(smokescreen[0], 4);//Шрифт
   	TextDrawColor(smokescreen[0], 0x000000FF);//Цвет
   	
	smokescreen[1] = TextDrawCreate(495.0, 20.0, "hud:radar_saveGame");
    TextDrawTextSize(smokescreen[1], 115.0, 82.0);//Размер
    TextDrawFont(smokescreen[1], 4);//Шрифт
//   	TextDrawColor(smokescreen[1], 0x000000FF);//Цвет
   	TextDrawColor(smokescreen[1], 0x000000FF);//Цвет

	gHideMapZone = GangZoneCreate(-6000.0, -6000.0, 6000.0, 6000.0);
	GangZoneShowForAll(gHideMapZone, 0xFF0000FF);
}

public create_statistic_data(playerid)
{
	if(gStatisticData[playerid][0] == PlayerText:INVALID_TEXT_DRAW)
	{
		new nick[64];
		
		GetPlayerName(playerid, nick, sizeof(nick));
		gStatisticData[playerid][0] = CreatePlayerTextDraw(playerid, 510.0, 24.0, nick);
	    PlayerTextDrawFont(playerid, gStatisticData[playerid][0], 0);//Шрифт
    	PlayerTextDrawAlignment(playerid, gStatisticData[playerid][0], 1);
	   	PlayerTextDrawColor(playerid, gStatisticData[playerid][0], 0x00FF00FF);//Цвет
	   	
		format(nick, sizeof(nick), "Score: ~b~%d", gScores[playerid]);
		gStatisticData[playerid][1] = CreatePlayerTextDraw(playerid, 506.0, 38.0, nick);
	    PlayerTextDrawFont(playerid, gStatisticData[playerid][1], 1);//Шрифт
    	PlayerTextDrawAlignment(playerid, gStatisticData[playerid][1], 1);
	   	PlayerTextDrawColor(playerid, gStatisticData[playerid][1], 0x00FF00FF);//Цвет
	   	
		format(nick, sizeof(nick), "Blood: ~r~%d", gHealth[playerid]);
		gStatisticData[playerid][2] = CreatePlayerTextDraw(playerid, 506.0, 48.0, nick);
	    PlayerTextDrawFont(playerid, gStatisticData[playerid][2], 1);//Шрифт
    	PlayerTextDrawAlignment(playerid, gStatisticData[playerid][2], 1);
	   	PlayerTextDrawColor(playerid, gStatisticData[playerid][2], 0x00FF00FF);//Цвет
	}
}

public show_smokescreen(playerid)
{
	TextDrawShowForPlayer(playerid, smokescreen[0]);
}

public hide_smokescreen(playerid)
{
	TextDrawHideForPlayer(playerid, smokescreen[0]);
}

public show_smoke_map(playerid)
{
   	GangZoneShowForPlayer(playerid, gHideMapZone, 0xFF0000FF);
}

public hide_smoke_map(playerid)
{
   	GangZoneHideForPlayer(playerid, gHideMapZone);
}

public show_smoke_statistic(playerid)
{
	TextDrawShowForPlayer(playerid, smokescreen[1]);
}

public hide_smoke_statistic(playerid)
{
	TextDrawHideForPlayer(playerid, smokescreen[1]);
}

public show_statistic_data(playerid)
{
	for(new i; i < TD_STATISTIC_DATA; ++i)
		PlayerTextDrawShow(playerid, gStatisticData[playerid][i]);
}

public hide_statistic_data(playerid)
{
	for(new i; i < TD_STATISTIC_DATA; ++i)
		PlayerTextDrawHide(playerid, gStatisticData[playerid][i]);
}

public update_statistic_data(playerid)
{
	new Buff[32];

	PlayerTextDrawHide(playerid, gStatisticData[playerid][1]);
	PlayerTextDrawHide(playerid, gStatisticData[playerid][2]);

	format(Buff, sizeof(Buff), "Score: ~b~%d", gScores[playerid]);
	PlayerTextDrawSetString(playerid, gStatisticData[playerid][1], Buff);
	format(Buff, sizeof(Buff), "Blood: ~r~%d", gHealth[playerid]);
	PlayerTextDrawSetString(playerid, gStatisticData[playerid][2], Buff);

	PlayerTextDrawShow(playerid, gStatisticData[playerid][1]);
	PlayerTextDrawShow(playerid, gStatisticData[playerid][2]);
}

public destroy_smokescreen()
{
	TextDrawDestroy(smokescreen[0]);
	TextDrawDestroy(smokescreen[1]);
//	TextDrawDestroy(smokescreen[2]);
	GangZoneDestroy(gHideMapZone);
}

public destroy_statistic_data(playerid)
{
	for(new i = 0; i < TD_STATISTIC_DATA; ++i)
	{
	    if(gStatisticData[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
	    {
		    PlayerTextDrawDestroy(playerid, gStatisticData[playerid][i]);
		    gStatisticData[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
}

stock IsPlayerSpawned(playerid)
{
    new player_state;

	player_state = GetPlayerState(playerid);

    return (player_state != PLAYER_STATE_NONE && player_state != PLAYER_STATE_WASTED && player_state != PLAYER_STATE_SPECTATING);
}

