#include <a_samp>
#include <things>
#include <imessage>
#include <mapandreas>
#include <crashdetect>

#include"defines.inc"
#include"global.inc"
#include"sensors.inc"
#include"load_objects.inc"
#include "../include/gl_common.inc"

#define FILTERSCRIPT

forward update_live_cells();
forward create_inventory_menu(playerid);
forward create_objects_menu(playerid);
forward create_vehicle_menu(playerid, vehicleid);
forward show_inventory(playerid, car_menu);
forward hide_menu(playerid);
forward update_neighbors_objects_menu(playerid);
forward update_objects_menu(playerid);
forward destroy_menu(playerid);

forward stop_all_rotates(playerid);

forward check_vehicle_menu_show();

forward is_one_inventory_cell_selected(playerid);
forward is_one_objects_cell_selected(playerid);
forward is_one_vehicle_cell_selected(playerid);

forward replace_inventory(playerid, inv1, inv2);
forward take_object(playerid, inv, obj);
forward put_object(playerid, cell); //положить созданный объект в инвентарь
forward put_vehicle_object(playerid, vehicleid, cell); //положить созданный объект в инвентарь транспорта

forward take_from_vehicle(playerid, vehicleid, cell, veh_cell); //переложить из инвентаря транспорта в инвентарь персонажа
forward put_in_vehicle(playerid, vehicleid, cell, veh_cell); //переложить из инвентаря персонажа в инвентарь транспорта
forward replace_vehicle_inventory(playerid, vehicleid, inv1, inv2); //переложить объект

forward remove_object(playerid, cell); //просто удалить объект из инвентаря
forward remove_vehicle_object(playerid, vehicleid, cell); //просто удалить объект из инвентаря транспорта
forward drop_object(playerid, inv, obj); //выложить объект

forward bool:is_player_on_gas_station(playerid);
forward ChooseLanguage(playerid);
forward	player_login_menu(); //окно с запросом пароля
forward imes_simple_single(playerid, color, str[]);
forward IsPlayerInVehicleReal(playerid);
forward show_help_for_player(playerid);
forward update_unoccupied_vehicles();
forward update_weapon_ammo();

forward unapply_one_cell(playerid, cell); //отменить действие содержимого ячейки
forward apply_one_cell(playerid, cell); //использовать содержимое ячейки

public OnFilterScriptInit()
{
	print("\n----------------------------------");
	print("SA:MP DayZ+ menu script by Bombo");
	print("----------------------------------\n");

	MapAndreas_Init(MAP_ANDREAS_MODE_FULL);
	init_ifile("imes.txt");
	gLangsNumber = 2;

	//переключаем глобальный чат в "локальный"
	LimitGlobalChatRadius(50.0);
	//отключаем маркеры игроков на карте
	ShowPlayerMarkers(0);
	//отключаем бонусы
	EnableStuntBonusForAll(false);
	//отключаем входы/выходы в интерьерах
	DisableInteriorEnterExits();
	//отключаем автозапуск двигателя и огни автомобилей
	ManualVehicleEngineAndLights();
	//разрешить "огонь по своим" машинам
	EnableVehicleFriendlyFire();
	
	create_things("things.txt", HOST, USER, PASSWD, DBNAME);
	open_database();

	initialize_players();
	create_smokescreen();

	for(new i; i < MAX_VEHICLES; ++i)
	{
		gUnoccupiedVehData[i] = INVALID_PLAYER_ID;
	}
	gUnoccupiedUpdateTimer = -1;
	
	for(new i; i < MAX_PLAYERS; ++i)
	{
		gWeaponUpdate[i] = 0;
	}
	gUpdateWeaponTimer = -1;

	for(new i = 0; i < MAX_PLAYERS; ++i)
	{
	    gPlayerLang[i][0] = 'e';
	    gPlayerLang[i][1] = 'n';
	    gPlayerLang[i][2] = 0;
		gVehicleDataShow[i] = 0;
        gPlayerPasswordRequest[i] = 0;
        gPlayerPasswordCheckCount[i] = 0;
        for(new j = 0; j < TD_STATISTIC_DATA; ++j)
        {
            gStatisticData[i][j] = PlayerText:INVALID_TEXT_DRAW;
        }
		for(new j = 0; j < TD_COUNT_VEHICLE; ++j)
		{
			gTdDataVehicle[i][j] = PlayerText:INVALID_TEXT_DRAW;
		}
	}

	for(new i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(IsPlayerConnected(i))
	    {
			show_smoke_map(i);
	        SetPlayerTeam(i, 0);
			ResetPlayerWeapons(i);
			gPlayerWeapon[i][0] = -1;
			gPlayerWeapon[i][1] = -1;
			if(IsPlayerSpawned(i))
			{
				create_inventory_menu(i);  //нужна проверка!
				show_smokescreen(i);
				show_smoke_statistic(i);
				create_sensors(i);
				create_statistic_data(i);
				show_statistic_data(i);
			}
		}
	}
	
	load_objects();

	for(new i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(IsPlayerConnected(i))
	    {
			gPlayerLang[i][0] = 'e';
			gPlayerLang[i][1] = 'n';
			gPlayerLang[i][2] = 0;
			ChooseLanguage(i);
		}
	}
	
	gTimerid = SetTimer("update_live_cells", INVENTORY_TIMER_UPDATE, true);
	gTimeridUpdateSensors = SetTimer("update_live_sensors", SENSORS_TIMER_UPDATE, true);
	gTimeridSaveSensors = SetTimer("update_character_state", SENSORS_TIMER_SAVE, true);
	gTimeridUpdateVehicles = SetTimer("update_vehicle_state", SENSORS_TIMER_VEHICLES, true);
	gTimerNonCheaters = SetTimer("cheater_finder", NON_CHEATERS_INTERVAL, true);
}

public OnFilterScriptExit()
{
	new i,j,k;

	KillTimer(gTimerNonCheaters);
	KillTimer(gTimeridUpdateVehicles);
	KillTimer(gTimeridSaveSensors);
	KillTimer(gTimeridUpdateSensors);
	KillTimer(gTimerid);

	destroy_objects();

	//удаляем все текстдравы инвентаря
	for(i = 0; i < MAX_PLAYERS; ++i)
	{
		for(j = 0; j < TD_COUNT_VEHICLE; ++j)
		{
			if(gTdDataVehicle[i][j] != PlayerText:INVALID_TEXT_DRAW)
			{
				PlayerTextDrawDestroy(i, gTdDataVehicle[i][j]);
				gTdDataVehicle[i][j] = PlayerText:INVALID_TEXT_DRAW;
			}
		}

		if(!IsPlayerConnected(i))
		    continue;

		ResetPlayerWeapons(i);

		save_character_ammo(i);
	
		//прекращаем выбор
		CancelSelectTextDraw(i);

		for(j = 0; j < 6; ++j)
		for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
		{
			PlayerTextDrawDestroy(i, gTdInventory[i][j][k]);
			if(gObjectsMenuShow[i] > 0)
				PlayerTextDrawDestroy(i, gTdObject[i][j][k]); //ОСТОРОЖНО! Может удалить другие ТД
			if(IsPlayerInAnyVehicle(i))
			{
			    destroy_vehicle_sensors(i);
			}
		}

		if(gVehicleMenuShow[i] > 0)
		{
			for(j = 0; j < MAX_INVENTORY_ON_VEHICLE; ++j)
			for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
			{
				    if(j < gVeh[gVehicleMenuShow[i]][1])
						PlayerTextDrawDestroy(i, gTdVehicle[i][j][k]); //ОСТОРОЖНО! Может удалить другие ТД
			}
		}

		for(k = 0; k < TD_COUNT; ++k)
			PlayerTextDrawDestroy(i, gTdMenu[i][k]);
			
		set_character_state(i, gHealth[i], gHunger[i], gThirst[i], gWound[i]);
		set_character_c_killer(i);
  	    destroy_sensors(i);
  	}

	destroy_smokescreen();
	
	for(new playerid = 0; playerid < MAX_PLAYERS; ++playerid)
	{
		destroy_statistic_data(playerid);
	}

	free_players();

	close_database();
	close_ifile();
	
	MapAndreas_Exit();

	return 1;
}

public OnGameModeInit()
{
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnQueryError(errorid, error[], callback[], query[], connectionHandle)
{
	switch(errorid)
	{
		case CR_SERVER_GONE_ERROR:
		{
			for(new i; i < MAX_PLAYERS; ++i)
			{
				if(IsPlayerAdmin(i))
					SendClientMessage(i, 0xFF0000FF, error);
			}
			printf("Lost connection to server, trying reconnect...");
			mysql_reconnect(connectionHandle);
		}
		case ER_SYNTAX_ERROR:
		{
			printf("Something is wrong in your syntax, query: %s",query);
		}
	}
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}

public OnPlayerConnect(playerid)
{
    gPlayerLang[playerid][0] = 'e';
    gPlayerLang[playerid][1] = 'n';
    gPlayerLang[playerid][2] = 0;
	show_smoke_map(playerid);
//	imes_simple_single(playerid, 0xFFCC00, "HELPLANG");
//	imes_simple_single(playerid, 0xFFCC00, "HELP_HELP");
    gPlayerPasswordRequest[playerid] = 2;
    ChooseLanguage(playerid);
	player_login_menu();
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	//а вот тут бы надо сделать сохранение позиции и инвентаря
	//...
	destroy_menu(playerid);
	set_character_state(playerid, gHealth[playerid], gHunger[playerid], gThirst[playerid], gWound[playerid]);
	set_character_c_killer(playerid);
	destroy_sensors(playerid);
	destroy_statistic_data(playerid);
	save_character_ammo(playerid);
	save_player_position(playerid);
	gPlayersID[playerid] = 0;
	gNonCheaters[playerid][2] = 0.0;
	gNonCheaters[playerid][1] = 0.0;
	gNonCheaters[playerid][0] = 0.0;
	return 1;
}

public OnPlayerSpawn(playerid)
{
	show_smoke_map(playerid);
	if(gPlayersID[playerid] == 0)
	    Kick(playerid);
	show_smokescreen(playerid);
	show_smoke_statistic(playerid);
	destroy_sensors(playerid);
	create_sensors(playerid);
	create_statistic_data(playerid);
	show_statistic_data(playerid);
	load_player_position(playerid);

	//а вот здесь можно было бы проверить прикреплённые к персонажу
	//объекты на наличие оных в инвентаре
	//...

	create_inventory_menu(playerid);	//нужна проверка!
	gPlayerWeapon[playerid][0] = -1;
	gPlayerWeapon[playerid][1] = -1;
	
	imes_simple_single(playerid, 0xFFCC00, "HELPLANG");
	imes_simple_single(playerid, 0xFFCC00, "HELP_HELP");
	
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	new inv[6];
	new i;
	
	if(IsPlayerInAnyVehicle(playerid))
		destroy_vehicle_sensors(playerid);

	destroy_sensors(playerid);
	destroy_statistic_data(playerid);
	get_spawn_place(playerid);
	destroy_menu(playerid);
	save_character_ammo(playerid);
	
	//раскидываем вещи
	//добавить проверку и обработку для транспорта!
	if(!IsPlayerAdmin(playerid))
	{
		load_player_inventory(playerid, inv);
		for(i = 0; i < 6; ++i)
		{
		    if(inv[i] > 0)
				drop_character_inventory_cell(playerid, i, -1);
		}
	}
	
	set_character_state(playerid, START_HEALTH_VALUE, START_HUNGER_VALUE, START_THIRST_VALUE, START_WOUND_VALUE);
	//увеличиваем очки перед сохрвнением киллера!
	upscore_character(gKiller[playerid]);
	set_character_killer(playerid);
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	for(new i; i < MAX_PLAYERS; ++i)
	{
	    if(IsPlayerInVehicle(i, vehicleid))
	    {
	        RemovePlayerFromVehicle(i);
	        destroy_vehicle_sensors(i);
     	}
	
	    if(IsPlayerAdmin(i))
			SendClientMessage(i, 0xFDCABBFF, "OnVehicleSpawn()");
	}

	//раскидываем вещи
	//...

	free_object_from_owner(gVeh[vehicleid][0]);
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	for(new i; i < MAX_PLAYERS; ++i)
	{
	    if(IsPlayerInVehicle(i, vehicleid))
	    {
	        RemovePlayerFromVehicle(i);
	        destroy_vehicle_sensors(i);
     	}

	    if(IsPlayerAdmin(i))
			SendClientMessage(i, 0xFDCABBFF, "OnVehicleDeath()");
	}

	//раскидываем вещи
	//...

	free_object_from_owner(gVeh[vehicleid][0]);
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
    //если двигатель выключен и изменения касаются открытия/закрытия дверей - урон не происходит
    //...
	drop_vehicle_from_dot(playerid, vehicleid);

	if(gVeh[vehicleid][4] == 0)
	    return 1;

//	new str[64]; //отладка!!!
//    format(str, sizeof(str), "Vehicle ID %d of playerid %d was damaged.", vehicleid, playerid); //отладка!!!
//    SendClientMessage(playerid, 0xFACDBCFF, str); //отладка!!!

    gVeh[vehicleid][2] = gVeh[vehicleid][2] - 159;
    save_vehicle_state(playerid, vehicleid);
    
    return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z)
{
	gUnoccupiedVehData[vehicleid] = playerid;
	
	if(gUnoccupiedUpdateTimer == -1)
		gUnoccupiedUpdateTimer = SetTimer("update_unoccupied_vehicles", 10000, false);

	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if(IsPlayerAdmin(playerid))
		SetPlayerPosFindZ(playerid, fX, fY, fZ);
    return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	//показать координаты поверхности
	if(strcmp(cmdtext, "/3d", true, 3) == 0) //отладка!!!
	{
		static object_id[256];
		new Float:X, Float:Y, Float:Z;
		new i, j;
		new Float:dx, Float:dy, Float:Z_coord;
		//new Float:Alpha, Float:Beta;
		
		if(IsValidObject(object_id[0]))
		{
		    for(i = 0; i < 256; ++i)
		    {
				DestroyObject(object_id[i]);
				object_id[i] = 0;
		    }
		    return 1;
		}
		
		GetPlayerPos(playerid, X, Y, Z);

		for(i = 0; i < 16; ++i)
		{
		    for(j = 0; j < 16; ++j)
		    {
		        dx = floatadd(Float:X, floatmul(i, 0.35));//(Float:i)/10.0;
		        dy = floatadd(Float:Y, floatmul(j, 0.35));//(Float:j)/10.0;
//				MapAndreas_Valid_Z_Coordinate(dx,dy,Z,Z_coord,Alpha,Beta);
				MapAndreas_FindZ_For2DCoord_I(dx, dy, Z_coord);
		        object_id[i*16+j] = CreateObject(1857, dx, dy, Z_coord+1.0, 0.0, 0.0, 0.0, 100.0);
//		        new str[160]; //отладка!!!
//		        format(str, sizeof(str), "i=%d, j=%d, object_id[%d]=%d, dx=%f, dy=%f, Z_coord=%f", i, j, i*2+j, object_id[i*2+j], dx, dy, Z_coord); //отладка!!!
//		        SendClientMessage(playerid, 0xFFFF00, str); //отладка!!!
		    }
		}
	    return 1;
	}

	if(strcmp(cmdtext, "/help", true, 5) == 0)
	{
		show_help_for_player(playerid);
		return 1;
	}
	
	//задать язык для пользователя
	if(strcmp(cmdtext, "/lang", true, 5) == 0)
	{
	    ChooseLanguage(playerid);
	    return 1;
	}

	//вывод данных о стране итгроков
	if(strcmp(cmdtext, "/land", true, 5) == 0)
	{
		new mes[256];
		new land_mes[1512];
		new imes1[64];
		new imes3[64];
		new name[64];
		new i;

		strdel(land_mes, 0, sizeof(land_mes));
        for(i = 0; i < MAX_PLAYERS; ++i)
        {
		  if(IsPlayerConnected(i))
		  {
            GetPlayerName(i, name, sizeof(name));
            imessage(imes1, "LAND_COMMAND_RESULT", gPlayerLang[playerid]);
            imessage(imes3, "LANGUAGE_STRING", gPlayerLang[playerid]);
		    format(mes, sizeof(mes), "%s{FF3300}%s: %s%s\n",
			       imes1,
				   name,
				   imes3,
				   gPlayerLang[i]);
			strcat(land_mes, mes);
		  }
          imessage(mes, "LAND_COMMAND_RESULT_TITLE", gPlayerLang[playerid]);
		}
  	    ShowPlayerDialog(playerid, 4459, DIALOG_STYLE_LIST, mes, land_mes, "OK", "");

		return 1;
	}
	
	if (strcmp("/menu", cmdtext, true, 5) == 0)
	{
		show_inventory(playerid, IsPlayerInVehicleReal(playerid));
		return 1;
	}
	
	if (strcmp("/hide", cmdtext, true, 5) == 0)
	{
		hide_menu(playerid);
	    return 1;
	}

	if (strcmp("/set12000", cmdtext, true, 9) == 0)
	{
	    gHealth[playerid] = 12000;
	    set_character_health(playerid, gHealth[playerid]);
	    return 1;
	}

	if (strcmp("/set10", cmdtext, true, 6) == 0)
	{
	    gHealth[playerid] = 10;
	    set_character_health(playerid, gHealth[playerid]);
	    return 1;
	}

	if (strcmp("/wound", cmdtext, true, 6) == 0)
	{
	    gWound[playerid] = 5000;
	    set_character_wound(playerid, gWound[playerid]);
	    return 1;
	}

	if (strcmp("/hun1000", cmdtext, true, 8) == 0)
	{
	    gHunger[playerid] = 1000;
	    set_character_hunger(playerid, gHunger[playerid]);
	    return 1;
	}

	if (strcmp("/hun10", cmdtext, true, 6) == 0)
	{
	    gHunger[playerid] = 10;
	    set_character_hunger(playerid, gHunger[playerid]);
	    return 1;
	}
	
	if (strcmp("/radius", cmdtext, true, 7) == 0)
	{
	    new params[128];
	    new idx;
	    new Float:rad;

	    rad = 0.0;
	    params = strtok(cmdtext, idx); //команда
	    params = strtok(cmdtext, idx); //параметр
	    rad = floatstr(params);
		//меняем радиус видимости чата
		LimitGlobalChatRadius(rad);
		return 1;
	}

	if (strcmp("/addspawn", cmdtext, true, 9) == 0)
	{
	    add_spawn_place(playerid);
	    return 1;
	}

	if (strcmp("/adddot", cmdtext, true, 7) == 0)
	{
	    new params[512];
	    new idx;
	    new type;
	    
	    type = 0;
	    params = strtok(cmdtext, idx); //команда
	    params = strtok(cmdtext, idx); //параметр
	    type = strval(params);
	    if(type != 0)
		    add_dot_place(playerid, type);
		do {
		    params = strtok(cmdtext, idx); //параметр
		    type = strval(params);
		    if(type != 0)
			    upd_dot_place(type);
		}
		while(type != 0);
	    return 1;
	}
	
	if (strcmp("/upddot", cmdtext, true, 7) == 0)
	{
	    new params[512];
	    new idx;
	    new type;

	    type = 0;
	    params = strtok(cmdtext, idx); //команда
		do {
		    params = strtok(cmdtext, idx); //параметр
		    type = strval(params);
		    if(type != 0)
			    upd_dot_place(type);
		}
		while(type != 0);
	    return 1;
	}

	if (strcmp("/addobj", cmdtext, true, 7) == 0)
	{
	    new params[512];
	    new idx;
	    new type;
	    new dup;

	    type = 0;
	    dup = 0;
	    params = strtok(cmdtext, idx); //команда
	    params = strtok(cmdtext, idx); //type
	    type = strval(params);
	    params = strtok(cmdtext, idx); //dup
	    dup = strval(params);
	    if(type <= 0 || dup <= 0)
			return 1;
			
		add_objects_to_gm(type, dup);

	    return 1;
	}

	if (strcmp("/addcar", cmdtext, true, 6) == 0)
	{
	    add_car_place(playerid);
	    return 1;
	}
	
	if (strcmp("/nextcar", cmdtext, true, 7) == 0)
	{
	    go_to_thing_place(playerid, "SOME_VEHICLE");
	    return 1;
	}

	if (strcmp("/nextthing", cmdtext, true, 10) == 0)
	{
	    new thing_type[64];
	    new idx;
	    
	    thing_type = strtok(cmdtext, idx); //команда
	    thing_type = strtok(cmdtext, idx); //параметр
	    if(strlen(thing_type) > 0)
		    go_to_thing_place(playerid, thing_type);
	    return 1;
	}

	if (strcmp("/setnew", cmdtext, true, 7) == 0)
	{
	    set_new_objects_on_places();
	    return 1;
	}

	if (strcmp("/kill", cmdtext, true, 5) == 0)
	{
	    SetPlayerHealth(playerid, 0.0);
	    return 1;
	}

	if (strcmp("/live", cmdtext, true, 5) == 0)
	{
		if(!IsPlayerAdmin(playerid))
		    return 1;

		gHealth[playerid] = START_HEALTH_VALUE;
        gHunger[playerid] = START_HUNGER_VALUE;
		gThirst[playerid] = START_THIRST_VALUE;
		gWound[playerid] = START_WOUND_VALUE;
		update_sensor_health(playerid, gHealth[playerid]);
		update_sensor_hunger(playerid, gHunger[playerid]);
		update_sensor_thirst(playerid, gThirst[playerid]);
		update_sensor_wound(playerid, gWound[playerid]);
		set_character_state(playerid, gHealth[playerid], gHunger[playerid], gThirst[playerid], gWound[playerid]);
		update_statistic_data(playerid);
	    return 1;
	}

	if (strcmp("/give", cmdtext, true, 5) == 0)
	{
		GivePlayerWeapon(playerid, 30, 30);
	    return 1;
	}

	if (strcmp("/update", cmdtext, true, 7) == 0)
	{
	    new i;
	    
	    create_things("things.txt", HOST, USER, PASSWD, DBNAME);
	    
		for(i = 0; i < 6; ++i)
		{
			remove_object(playerid, i);
			put_object(playerid, i);
		}
	    return 1;
	}
	
	if(strcmp(cmdtext, "/glvl", true) == 0)
	{
		new Float:X, Float:Y, Float:Z;
 		GetPlayerPos(playerid,X,Y,Z);

		new msg[128];
		format(msg,128,"Your position is: X:%f Y:%f Z:%f",X,Y,Z);
		SendClientMessage(playerid,0xFFFFFFFF,msg);

        MapAndreas_FindZ_For2DCoord(X,Y,Z);
		format(msg,128,"Highest ground level:               %f",Z+1);
		SendClientMessage(playerid,0xFFFFFFFF,msg);

        MapAndreas_FindZ_For2DCoord_I(X,Y,Z);
		format(msg,128,"Highest ground level interpolated: %f",Z+1);
		SendClientMessage(playerid,0xFFFFFFFF,msg);

	    return 1;
	}
	
	if (strcmp("/tp", cmdtext, true, 3) == 0)
	{
		if(!IsPlayerAdmin(playerid))
		    return 1;
		
	    SetPlayerPos(playerid, -2990.0, 459.0, 5.0);
	    return 1;
	}

	return 0;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	new cell, i;

	if(IsPlayerAdmin(playerid))
		SendClientMessage(playerid, 0xFFFF44, "OnPlayerWeaponShot()");
	//уменьшаем количество патронов с каждым выстрелом
	gPlayerWeapon[playerid][1]--;
	gWeaponUpdate[playerid] = 1;
	if(gUpdateWeaponTimer < 0)
		gUpdateWeaponTimer = SetTimer("update_weapon_ammo", UPDATE_WEAPON_AMMO_TIMER, false);
	if(gPlayerWeapon[playerid][0] <= 0) //если оружие - муляж
	{
		gPlayerWeapon[playerid][0] = -1;
		gPlayerWeapon[playerid][1] = -1;
		ResetPlayerWeapons(playerid); //немножко античита
		return 0;
	}
	if(gPlayerWeapon[playerid][1] <= 0) //если кончились патроны
	{
	    //собственно, освобождаем пустые обоймы
	    save_character_ammo(playerid);
		//разбираем оружие в инвентаре
		cell = disassemble_inventory_object(gPlayerWeapon[playerid][0]);
		if(cell >= 0)
		{
		    for(i = 0; i < 6; ++i)
		    {
		        remove_object(playerid, i);
				put_object(playerid, i);
			}
		}
		
		if(gPlayerWeapon[playerid][1] < 0)
		{
			gPlayerWeapon[playerid][0] = -1;
			gPlayerWeapon[playerid][1] = -1;
			ResetPlayerWeapons(playerid); //немножко античита
		    return 0;
		}

		//убираем оружие из рук
		gPlayerWeapon[playerid][0] = -1;
		gPlayerWeapon[playerid][1] = -1;
		ResetPlayerWeapons(playerid); //немножко античита
	}
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float: amount, weaponid, bodypart)
{
	new str_amount[16];
	new buff;
	
	buff = gHealth[playerid]*100/START_HEALTH_VALUE;

	SetPlayerHealth(playerid, buff>8?buff:8);

    if(gHealth[playerid] < 0)
    {
		kill_character(playerid);
        return 1;
    }
//	SetPlayerHealth(playerid, 1000000.0);

//	new str[128];
//	format(str, sizeof(str), "playerid=%d, issuerid=%d, amount=%f, weaponid=%d, bodypart=%d", playerid, issuerid, amount, weaponid, bodypart);
//	SendClientMessage(playerid, 0xFFFF55, str);

	switch(weaponid)
	{
	    case 54: //падение
	    {
			if(amount < 3.290000 || amount > 3.310000)
			{
				if(gWound[playerid] > MAX_WOUND_REAL_VALUE)
				    return 1;

		        imes_simple_single(playerid, 0x66FF55, "O_O_O_H_H_H");
				format(str_amount, sizeof(str_amount), "%.0f", amount);
				gWound[playerid] = gWound[playerid] + strval(str_amount)*10;
				update_sensor_wound(playerid, gWound[playerid]);
				return 1;
			}
	    }
	    case 53: //утопающий
		{
				if(gWound[playerid] > MAX_WOUND_REAL_VALUE)
				    return 1;
		        imes_simple_single(playerid, 0x66FF55, "A_A_A_H_H_H");
				format(str_amount, sizeof(str_amount), "%.0f", amount);
				gWound[playerid] = gWound[playerid] + strval(str_amount)*100;
				update_sensor_wound(playerid, gWound[playerid]);
				return 1;
		}
	    case 37: //огонь
	    {
	        return 1;
		}
	    default:
        {
			if(issuerid != INVALID_PLAYER_ID)
			{
				if(gPlayersID[issuerid] > 0)
				    gKiller[playerid] = gPlayersID[issuerid];
			}
			if(bodypart == WEAPON_BODY_PART_HEAD)
			{
				kill_character(playerid);
        		return 1;
			}
			if(gWound[playerid] > MAX_WOUND_REAL_VALUE)
			    return 1;
	        imes_simple_single(playerid, 0x66FF55, "A_A_A_H_H_H");
			format(str_amount, sizeof(str_amount), "%.0f", amount);
			gWound[playerid] = gWound[playerid] + strval(str_amount)*10;
			update_sensor_wound(playerid, gWound[playerid]);
			return 1;
		}
	}

    return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	if(IsPlayerAdmin(playerid))
		SendClientMessage(playerid, 0x33FF88, "OnPlayerGiveDamage()");
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	static CheckTimerID_0, CheckTimerID_1;

	//если расход топлива нулевой - "заводим двигатель"
	if(gVeh[vehicleid][5] == 0)
	    gVeh[vehicleid][4] = 1;
	    
	drop_vehicle_from_dot(playerid, vehicleid);
	save_vehicle_state(playerid, vehicleid);
	hide_menu(playerid);

	if(CheckTimerID_0 > 0)
	{
	    KillTimer(CheckTimerID_0);
	    CheckTimerID_0 = 0;
	}
	if(CheckTimerID_1 > 0)
	{
	    KillTimer(CheckTimerID_1);
	    CheckTimerID_1 = 0;
	}
	create_vehicle_sensors(playerid, vehicleid);
	CheckTimerID_0 = SetTimer("check_vehicle_menu_show", VEHICLE_DATA_SHOW_CHECK, false);
	CheckTimerID_1 = SetTimer("check_vehicle_menu_show", VEHICLE_DATA_SHOW_CHECK*2, false);

	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	static CheckTimerID_0, CheckTimerID_1;


	//если расход топлива не нулевой - "глушим двигатель" уменьшения времени
	//обработки таймера обновления состояния авто
	if(gVeh[vehicleid][5] == 0)
	    gVeh[vehicleid][4] = 0;

	//убираем авто из точки респавна, если она заводится вдали от таковой
	drop_vehicle_from_dot(playerid, vehicleid);
	save_vehicle_state(playerid, vehicleid);
	hide_menu(playerid);
	
	if(CheckTimerID_0 > 0)
	{
	    KillTimer(CheckTimerID_0);
	    CheckTimerID_0 = 0;
	}
	if(CheckTimerID_1 > 0)
	{
	    KillTimer(CheckTimerID_1);
	    CheckTimerID_1 = 0;
	}

	destroy_vehicle_sensors(playerid);
	CheckTimerID_0 = SetTimer("check_vehicle_menu_show", VEHICLE_DATA_SHOW_CHECK, false);
	CheckTimerID_1 = SetTimer("check_vehicle_menu_show", VEHICLE_DATA_SHOW_CHECK*2, false);

	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
//	SendClientMessage(0, 0xFF00FF,"Object moved!");
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	new vehicleid;

	if(newkeys & KEY_SUBMISSION && IsPlayerInVehicleReal(playerid))
	{
		vehicleid = GetPlayerVehicleID(playerid);
		if(gVeh[vehicleid][0] <= 0) //немного античита
		{
			DestroyVehicle(vehicleid);
			return 1;
		}
		if(gVeh[vehicleid][4] > 0)
		{
			//если расход не нулевой
		    if(gVeh[vehicleid][5] != 0)
			{
		        create_vehicle_sensors(playerid, vehicleid);
		        imes_simple_single(playerid, 0xFDBCADFF, "ON_STOP_ENGINE");
				//если мотор был включен - глушим
				gVeh[vehicleid][4] = 0;
			}
			save_vehicle_state(playerid, vehicleid);
			return 1;
		}
		else
		{
	        create_vehicle_sensors(playerid, vehicleid);
		    //gVeh[vehicleid][2] = 500; //gVeh[vehicleid][3] = 50; //отладка!!!
			//если мотор был выключен - пытаемся завести
			//если есть работоспособный движок и бензин (либо расход бензина нулевой) - заводим!
			if( (gVeh[vehicleid][2] > 0) && ((gVeh[vehicleid][3] > 0) || gVeh[vehicleid][5] == 0) )
			{
				//убираем авто из точки респавна, если она заводится вдали от таковой
				drop_vehicle_from_dot(playerid, vehicleid);
				
				gVeh[vehicleid][4] = 1;
				save_vehicle_state(playerid, vehicleid);
				
		        imes_simple_single(playerid, 0xFDBCADFF, "ON_START_ENGINE");
			    return 1;
			}
			else
			{
			    //если расход нулевой - выходим
			    if(gVeh[vehicleid][5] == 0)
			        return 1;
			    
			    //если двигатель поломан
			    if(gVeh[vehicleid][2] <= 0)
			    {
			        imes_simple_single(playerid, 0xFF5555FF, "ON_START_BROKEN_ENGINE");
				    return 1;
				}

				//если нету топлива
			    if(gVeh[vehicleid][3] <= 0)
			    {
			        imes_simple_single(playerid, 0xFF8888FF, "ON_START_NO_FUEL_ENGINE");
				    return 1;
				}
			}
		}
	}

	if(newkeys & KEY_YES)
	{
		if(gInventoryMenuShow[playerid] == 0)
		    show_inventory(playerid, IsPlayerInVehicleReal(playerid));
		else
		    hide_menu(playerid);
		return 1;
	}

	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(clickedid == Text:INVALID_TEXT_DRAW && gInventoryMenuShow[playerid] == 1)
	    hide_menu(playerid);
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	new color1, color2;
	new result[64];
	
    get_vehicle_value(vehicleid, "color1", "veh_data", result);
    color1 = strval(result);
    get_vehicle_value(vehicleid, "color2", "veh_data", result);
    color2 = strval(result);
	ChangeVehicleColor(vehicleid, color1, color2);
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
	  case 756:
	  {
        if(response)
        {
		  new imes[254];
		  new lang_name[32];
		  new mes[256];
		  new len;

		  if(listitem == 0) //если оставить какой есть
		  {
		    if(gPlayerPasswordRequest[playerid] > 0)
				gPlayerPasswordRequest[playerid] = 1;
			return 1;
		  }

		  len = strlen(inputtext);
		  if(len < 2)
		  {
   	        imessage(imes, "ERROR_MESSAGE", gPlayerLang[playerid]);
            SendClientMessage(playerid, 0xFFCC00AA, imes);
		    if(gPlayerPasswordRequest[playerid] > 0)
				gPlayerPasswordRequest[playerid] = 1;
			return 1;
		  }
		  imes[0] = inputtext[0];
  		  imes[1] = inputtext[1];
  		  imes[2] = '\0';
		  for(new i = 0; i < gLangsNumber; ++i)
		  {
			if(strcmp(imes, gAllLangs[i]) == 0)
			{
              gPlayerLang[playerid][0] = gAllLangs[i][0];
              gPlayerLang[playerid][1] = gAllLangs[i][1];
              gPlayerLang[playerid][2] = '\0';
   	          imessage(imes, "NEWLANG", gPlayerLang[playerid]);
   	          imessage(lang_name, "LANGUAGE_NAME", gPlayerLang[playerid]);
		      format(mes, sizeof(mes), "%s%s", imes, lang_name);
              SendClientMessage(playerid, 0xFFCC00AA, mes);
		      if(gPlayerPasswordRequest[playerid] > 0)
				gPlayerPasswordRequest[playerid] = 1;
              return 1;
			}
		  }
	      imessage(imes, "ERROR_MESSAGE", gPlayerLang[playerid]);
          SendClientMessage(playerid, 0xFFCC00AA, imes);
		  if(gPlayerPasswordRequest[playerid] > 0)
			gPlayerPasswordRequest[playerid] = 1;
		  return 1;
		}
	  }
	  case 456:
	  {
        if(response)
        {
		    if(strlen(inputtext) < 6)
		    {
                gPlayerPasswordCheckCount[playerid]++;
                if(gPlayerPasswordCheckCount[playerid] == 3)
			        Kick(playerid);
				imes_simple_single(playerid, 0xFFFF00FF, "TOO_SHORT_PASSWORD");
                gPlayerPasswordRequest[playerid] = 1;
		        SetTimer("player_login_menu", 10000, false);
		        return 1;
		    }

		    if(player_login(playerid, inputtext) < 0)
		    {
                gPlayerPasswordCheckCount[playerid]++;
                if(gPlayerPasswordCheckCount[playerid] == 3)
			        Kick(playerid);
				imes_simple_single(playerid, 0xFF0033FF, "WRONG_PASSWORD");
                gPlayerPasswordRequest[playerid] = 1;
		        SetTimer("player_login_menu", 10000, false);
			}
			else
			{
		        gPlayerPasswordCheckCount[playerid] = 0;
			    SetPlayerTeam(playerid, 0);
				imes_simple_single(playerid, 0xFFFF00FF, "YOU_ARE_WELCOME");
	//			imes_simple_single(playerid, 0xFF3300, "HELLO_MESSAGE");
				SpawnPlayer(playerid);
				show_help_for_player(playerid);
			}
		}
		else
	        Kick(playerid);
	  }
	  default: return 1;
    }
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	new j,k,i,cell,res;
	
	//кнопка отмены
    if(playertextid == gTdMenu[playerid][1])
    {
		hide_menu(playerid);
		return 1;
    }

	//кнопка вопроса
    if(playertextid == gTdMenu[playerid][3])
    {
        new name[128];
        new value;
        
        cell = is_one_inventory_cell_selected(playerid);
		if(cell >= 0)
		{
		    //получаем внутримодовое имя вещи и значение
			get_object_data(playerid, cell, INVENTORY_AREA, name, value);
		}
		else
		{
			cell = is_one_objects_cell_selected(playerid);
			if(cell >= 0)
			{
			    //получаем внутримодовое имя вещи и значение
				get_object_data(playerid, cell, OBJECTS_AREA, name, value);
			}
			else
			{
				cell = is_one_vehicle_cell_selected(playerid);
				if(cell >= 0)
				{
				    //получаем внутримодовое имя вещи и значение
					get_object_data(playerid, cell, VEHICLE_AREA, name, value);
				}
			}
		}
		
		stop_all_rotates(playerid);
		if(cell >= 0)
		{
	        new buff1[128], buff2[64], object_meaning_str[64], object_meaning[128], meaning_name[64], title[64], mes[256], ok_mes[32];

			strdel(mes, 0, sizeof(mes));
			//получаем формат строки
			imessage(buff1, "OBJECT_NAME_FOR_ABOUT", gPlayerLang[playerid]);

			if(strlen(name) > 0)
			{
			    //получаем обычное имя вещи
				imessage(buff2, name, gPlayerLang[playerid]);
				
			    //получаем нименование состояния вещи ("количество", "состояние" и т.д.)
			    format(meaning_name, sizeof(meaning_name), "%s_VALUE", name);
			    //вот сюда бы проверочку полученного результата imessage()!
			    //...
				imessage(object_meaning, meaning_name, gPlayerLang[playerid]);
				//...
				
				//получаем окончательную строчку ссостояния вещи
				format(object_meaning_str, sizeof(object_meaning_str), object_meaning, value);
				
				//получаем описание вещи
			    //получаем нименование состояния вещи ("количество", "состояние" и т.д.)
			    format(meaning_name, sizeof(meaning_name), "%s_ABOUT", name);
			    //вот сюда бы проверочку полученного результата imessage()!
			    //...
				imessage(object_meaning, meaning_name, gPlayerLang[playerid]);
				//...

				//формируем окончательное сообщение
				format(mes, sizeof(mes), buff1, strlen(buff2)>0?buff2:name, "\n", object_meaning_str, "\n\n", object_meaning);
			}
			else
			{
				imessage(object_meaning, "DEFAULT_OBJECT_MEANING_STRING", gPlayerLang[playerid]);
				//получаем окончательную строчку ссостоянием вещи
				format(object_meaning_str, sizeof(object_meaning_str), object_meaning, value);
				//формируем окончательное сообщение
				format(mes, sizeof(mes), buff1, strlen(name)>0?name:"unknown", "\n", object_meaning_str, "\n\n", "");
			}
			imessage(title, "TITLE_FOR_ABOUT_OBJECT", gPlayerLang[playerid]);
			imessage(ok_mes, "OK_MESSAGE", gPlayerLang[playerid]);
			ShowPlayerDialog(playerid, 1111, DIALOG_STYLE_MSGBOX, title, mes, ok_mes, "");
		}
		return 1;
    }

	//кнопка "применить"
    if(playertextid == gTdMenu[playerid][5])
    {
        //подсчитываем количество выделенных объектов и решаем - надо ли пытаться создавать композит
        cell = is_one_inventory_cell_selected(playerid);
        if(cell >= 0) //выбрана одна ячейка инвентаря
        {
            //используем объект в ячейке
            apply_one_cell(playerid, cell);
            
            //обновляем содержимое ячейки
			remove_object(playerid, cell);
			put_object(playerid, cell);
			
            cell = -1; //для сброса выбора
        }
		else //выбрано более одной ячейки
		{
	        if(create_composite_object(playerid, cell))
	        {
	            //если обновилось текущее оружие - обновим патроны
	            apply_one_cell(playerid, cell);
			}
	            
			if(cell < 0) //объект не создан
			{
				//здесь пытаемся добавить элементы к композитному объекту
				//...
	  		}
			else //объект создан
			{
			    for(i = 0; i < 6; ++i)
			    {
			        if(gRotate[playerid][i] > 0) //проверка на (cell == i) здесь не требуется!
			        {
						//освобождаем одну ячейку инвентаря
						//если прежний и новый объекты совпадают по типу, то прежний остаётся в своей ячейке в инвентаре
						//т.к. функция remove_object предварительно проверяет содержимое ячейки инвентаря перед удалением
						//текстдравов, то прежние текстдравы объекта останутся на своём месте!
						remove_object(playerid, i);
					}
				}
				//обновляем одну ячейку инвентаря (gInv[playerid][cell] фактически уже инициализирован)
				put_object(playerid, cell);
			}
		}
		
		//остановим вращение (сбросим выбор) объектов
		if(cell < 0)
		{
			stop_all_rotates(playerid);
		}

		return 1;
    }

	//кнопка "гаячный ключ"
    if(playertextid == gTdMenu[playerid][8])
    {
		cell = is_one_inventory_cell_selected(playerid);
        if(cell >= 0 && (res = disassemble_cell_object(playerid, cell)) >= 0)
        {
//            destroy_menu(playerid);
//            create_inventory_menu(playerid);
//            show_inventory(playerid, IsPlayerInVehicleReal(playerid));
		    for(i = 0; i < 6; ++i)
		    {
				//освобождаем одну ячейку инвентаря
				//если прежний и новый объекты совпадают по типу, то прежний остаётся в своей ячейке в инвентаре
				//т.к. функция remove_object предварительно проверяет содержимое ячейки инвентаря перед удалением
				//текстдравов, то прежние текстдравы объекта останутся на своём месте!
				remove_object(playerid, i);
				//обновляем одну ячейку инвентаря (gInv[playerid][cell] фактически уже инициализирован)
				put_object(playerid, i);
			}
        }

		//остановим вращение (сбросим выбор) объектов
		if(res < 0 || cell < 0)
		{
			stop_all_rotates(playerid);
		}

		return 1;
    }

	//проверка и действие при клике на объекте инвентаря
	for(j = 0; j < 6; ++j)
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
	    if(gInventoryMenuShow[playerid] > 0 && playertextid == gTdInventory[playerid][j][k])
	    {
			//если ячейка пустая - пытаемся в неё что-нибудь положить
	        if(gInv[playerid][j] < 0)
	        {
		        cell = is_one_inventory_cell_selected(playerid);
	            if(cell >= 0)
	            {
	                replace_inventory(playerid, cell, j);
		            return 1;
	            }
	            
		        cell = is_one_objects_cell_selected(playerid);
	            if(cell >= 0)
	            {
	                take_object(playerid, j, cell);
		            return 1;
	            }
	            
	            if(gVehicleMenuShow[playerid] > 0)
	            {
			        cell = is_one_vehicle_cell_selected(playerid);
		            if(cell >= 0)
		            {
		                take_from_vehicle(playerid, gVehicleMenuShow[playerid], j, cell);
			            return 1;
		            }
           		}
	            return 1;
		    }
	    
	        //заставляем объект вращаться, либо останавливаем
	        if(gRotate[playerid][j] == 0)
	        {
				PlayerTextDrawHide(playerid, gTdInventory[playerid][j][0]);
				gIndex[playerid][j] = 0;
				gRotate[playerid][j] = 1;
			}
			else
			{
			    gRotate[playerid][j] = 0;
				PlayerTextDrawHide(playerid, gTdInventory[playerid][j][gIndex[playerid][j]]);
				PlayerTextDrawShow(playerid, gTdInventory[playerid][j][0]);
				gIndex[playerid][j] = 0;
			}
			return 1;
	    }
	    
	    if(gObjectsMenuShow[playerid] > 0 && playertextid == gTdObject[playerid][j][k])
	    {
			//если ячейка пустая - пытаемся в неё что-нибудь положить
	        if(gObj[playerid][j] < 0)
	        {
		        cell = is_one_inventory_cell_selected(playerid);
	            if(cell >= 0)
	            {
	                drop_object(playerid, cell, j);
		            return 1;
	            }
	            return 1;
		    }

	        //заставляем объект вращаться, либо останавливаем
	        if(gRotateObject[playerid][j] == 0)
	        {
				PlayerTextDrawHide(playerid, gTdObject[playerid][j][0]);
				gIndexObject[playerid][j] = 0;
				gRotateObject[playerid][j] = 1;
			}
			else
			{
			    gRotateObject[playerid][j] = 0;
				PlayerTextDrawHide(playerid, gTdObject[playerid][j][gIndexObject[playerid][j]]);
				PlayerTextDrawShow(playerid, gTdObject[playerid][j][0]);
				gIndexObject[playerid][j] = 0;
			}
			return 1;
	    }
    }
	    
	for(j = 0; j < MAX_INVENTORY_ON_VEHICLE; ++j)
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
	    if(gVehicleMenuShow[playerid] > 0 && playertextid == gTdVehicle[playerid][j][k])
	    {
	        new vehicleid;

	        vehicleid = gVehicleMenuShow[playerid];

			//если ячейка пустая - пытаемся в неё что-нибудь положить
	        if(gVehObj[vehicleid][j] < 0)
	        {
		        cell = is_one_inventory_cell_selected(playerid);
	            if(cell >= 0)
	            {
	                put_in_vehicle(playerid, vehicleid, cell, j);
		            return 1;
	            }

		        cell = is_one_vehicle_cell_selected(playerid);
	            if(cell >= 0)
	            {
	                replace_vehicle_inventory(playerid, vehicleid, cell, j);
		            return 1;
	            }

	            return 1;
		    }

	        //заставляем объект вращаться, либо останавливаем
	        if(gRotateVehicle[playerid][j] == 0)
	        {
				PlayerTextDrawHide(playerid, gTdVehicle[playerid][j][0]);
				gIndexVehicle[playerid][j] = 0;
				gRotateVehicle[playerid][j] = 1;
			}
			else
			{
			    gRotateVehicle[playerid][j] = 0;
				PlayerTextDrawHide(playerid, gTdVehicle[playerid][j][gIndexVehicle[playerid][j]]);
				PlayerTextDrawShow(playerid, gTdVehicle[playerid][j][0]);
				gIndexVehicle[playerid][j] = 0;
			}
			return 1;
	    }
	}
    return 1;
}

public update_live_cells()
{
	new i,j;
	
	for(i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(gInventoryMenuShow[i] == 0 && gObjectsMenuShow[i] == 0 && gVehicleMenuShow[i] == 0)
	        continue;
	
	    for(j = 0; j < 6; ++j)
	    {
			//вращаем выбранные объекты в инвентаре
	        if(gInventoryMenuShow[i] > 0 && gRotate[i][j] > 0)
	        {
				if(gIndex[i][j] < (MAX_TURNS_OF_PREVIEW-1))
					PlayerTextDrawShow(i, gTdInventory[i][j][gIndex[i][j]+1]);
				else
		  		    PlayerTextDrawShow(i, gTdInventory[i][j][0]);
				PlayerTextDrawHide(i, gTdInventory[i][j][gIndex[i][j]]);

				gIndex[i][j]++;
				if(gIndex[i][j] >= MAX_TURNS_OF_PREVIEW)
					gIndex[i][j] = 0;
			}
			
			//вращаем выбранные объекты вне инвентаря
			if(gObjectsMenuShow[i] > 0 && gRotateObject[i][j] > 0)
	        {
				if(gIndexObject[i][j] < (MAX_TURNS_OF_PREVIEW-1))
		  		  PlayerTextDrawShow(i, gTdObject[i][j][gIndexObject[i][j]+1]);
				else
		  		  PlayerTextDrawShow(i, gTdObject[i][j][0]);
				PlayerTextDrawHide(i, gTdObject[i][j][gIndexObject[i][j]]);

				gIndexObject[i][j]++;
				if(gIndexObject[i][j] >= MAX_TURNS_OF_PREVIEW)
					gIndexObject[i][j] = 0;
			}
		}
			
	    for(j = 0; j < MAX_INVENTORY_ON_VEHICLE; ++j)
	    {
			//вращаем выбранные объекты в инвентаре транспорта
			if(gVehicleMenuShow[i] > 0 && j < gVeh[gVehicleMenuShow[i]][1] && gRotateVehicle[i][j] > 0)
	        {
				if(gIndexVehicle[i][j] < (MAX_TURNS_OF_PREVIEW-1))
		  		  PlayerTextDrawShow(i, gTdVehicle[i][j][gIndexVehicle[i][j]+1]);
				else
		  		  PlayerTextDrawShow(i, gTdVehicle[i][j][0]);
				PlayerTextDrawHide(i, gTdVehicle[i][j][gIndexVehicle[i][j]]);

				gIndexVehicle[i][j]++;
				if(gIndexVehicle[i][j] >= MAX_TURNS_OF_PREVIEW)
					gIndexVehicle[i][j] = 0;
			}
		}
	}
}

public create_inventory_menu(playerid)
{
	//hud:radar_modGarage - гаечный ключ
	//hud:fist - кулак
	//hud:radar_qmark - знак вопроса
	//hud:radar_hostpital, hud:radar_airYard - крестики

	//hud:radar_dateDisco - аккуратный кружок

	new i,j,k,m;
	new is_auto[6];
	new inv_isrot[6][3];
	new Float:inv_deg[6][3], Float:inv_zoom[6];

	//фон инвентаря
	gTdMenu[playerid][0] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y, "fon");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][0], TEXT_DRAW_FONT_MODEL_PREVIEW);
	PlayerTextDrawUseBox(playerid, gTdMenu[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][0], 0xFFFFFF88); //0x55112233
	PlayerTextDrawSetPreviewRot(playerid, gTdMenu[playerid][0], 90.0, 0.0, 0.0, -100.0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][0], CELL_SIZE*3, CELL_SIZE*2);

	gTdMenu[playerid][11] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y-10.0, "plfon");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][11], TEXT_DRAW_FONT_MODEL_PREVIEW);
	PlayerTextDrawUseBox(playerid, gTdMenu[playerid][11], 0);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][11], 0x000000FF); //0xFFFFFF88
	PlayerTextDrawSetPreviewRot(playerid, gTdMenu[playerid][11], 90.0, 0.0, 0.0, -100.0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][11], CELL_SIZE*3, 10.0);

	gTdMenu[playerid][12] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y-10.0, "Player");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][12], 1);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][12], 0x00FFFF88); //0xFFFFFF88
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][12], 0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][12], CELL_SIZE*3, 6.0);

	//фон для объектов не из инвентаря
	gTdMenu[playerid][7] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+CELL_SIZE*3+10.0, INVENTORY_START_POSITION_Y, "noninventfon");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][7], TEXT_DRAW_FONT_MODEL_PREVIEW);
	PlayerTextDrawUseBox(playerid, gTdMenu[playerid][7], 0);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][7], 0xFFFFFF88); //0x55BB1133
	PlayerTextDrawSetPreviewRot(playerid, gTdMenu[playerid][7], 90.0, 0.0, 0.0, -100.0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][7], CELL_SIZE*2, CELL_SIZE*3);
	
	gTdMenu[playerid][13] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+CELL_SIZE*3+10.0, INVENTORY_START_POSITION_Y-10.0, "grfon");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][13], TEXT_DRAW_FONT_MODEL_PREVIEW);
	PlayerTextDrawUseBox(playerid, gTdMenu[playerid][13], 0);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][13], 0x000000FF); //0xFFFFFF88
	PlayerTextDrawSetPreviewRot(playerid, gTdMenu[playerid][13], 90.0, 0.0, 0.0, -100.0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][13], CELL_SIZE*2, 10.0);

	gTdMenu[playerid][14] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+CELL_SIZE*3+10.0, INVENTORY_START_POSITION_Y-10.0, "Ground");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][14], 1);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][14], 0x00FFFF88); //0xFFFFFF88
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][14], 0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][14], CELL_SIZE*2, 6.0);
	
	//фон для объектов из инвентаря авто
	gTdMenu[playerid][10] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y-CELL_SIZE*2-20.0, "vehfon");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][10], TEXT_DRAW_FONT_MODEL_PREVIEW);
	PlayerTextDrawUseBox(playerid, gTdMenu[playerid][10], 0);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][10], 0xFFFFFF88); //0x88118833
	PlayerTextDrawSetPreviewRot(playerid, gTdMenu[playerid][10], 90.0, 0.0, 0.0, -100.0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][10], CELL_SIZE*3, CELL_SIZE*2);
	
	gTdMenu[playerid][15] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y-CELL_SIZE*2-30.0, "vhfon");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][15], TEXT_DRAW_FONT_MODEL_PREVIEW);
	PlayerTextDrawUseBox(playerid, gTdMenu[playerid][15], 0);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][15], 0x000000FF); //0xFFFFFF88
	PlayerTextDrawSetPreviewRot(playerid, gTdMenu[playerid][15], 90.0, 0.0, 0.0, -100.0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][15], CELL_SIZE*3, 10.0);

	gTdMenu[playerid][16] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y-CELL_SIZE*2-30.0, "Vehicle");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][16], 1);
	PlayerTextDrawBackgroundColor(playerid, gTdMenu[playerid][16], 0x00FFFF88); //0xFFFFFF88
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][16], 0);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][16], CELL_SIZE*3, 6.0);

	//кнопка отмены "X"
	gTdMenu[playerid][2] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X, INVENTORY_START_POSITION_Y+165.0, "hud:radar_light");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][2], 4);
	PlayerTextDrawColor(playerid, gTdMenu[playerid][2], 0x55005544);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][2], 21.0, 21.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][2], 0);

	gTdMenu[playerid][1] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+6, INVENTORY_START_POSITION_Y+170.0, "X");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][1], 3);
	PlayerTextDrawColor(playerid, gTdMenu[playerid][1], 0xFF0000DD);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][1], INVENTORY_START_POSITION_X+16, 200.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, gTdMenu[playerid][1], 1);

	//кнопка "?"
	gTdMenu[playerid][4] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+40.0, INVENTORY_START_POSITION_Y+165.0, "hud:radar_light");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][4], 4);
	PlayerTextDrawColor(playerid, gTdMenu[playerid][4], 0xFF00FF44);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][4], 21.0, 21.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][4], 0);

	gTdMenu[playerid][3] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+47.0, INVENTORY_START_POSITION_Y+170.0, "?");
	PlayerTextDrawColor(playerid, gTdMenu[playerid][3], 0xFFFF00DD);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][3], INVENTORY_START_POSITION_X+57.0, 400.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, gTdMenu[playerid][3], 1);

	//кнопка "V"
	gTdMenu[playerid][6] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+80.0, INVENTORY_START_POSITION_Y+165.0, "hud:radar_light");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][6], 4);
	PlayerTextDrawColor(playerid, gTdMenu[playerid][6], 0x1100FF44);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][6], 21.0, 21.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][6], 0);

	gTdMenu[playerid][5] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+85.5, INVENTORY_START_POSITION_Y+170.0, "V");
	PlayerTextDrawColor(playerid, gTdMenu[playerid][5], 0x0066FFDD);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][5], INVENTORY_START_POSITION_X+95.5, 400.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, gTdMenu[playerid][5], 1);

	//кнопка "гаячный ключ"
	gTdMenu[playerid][9] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+150.0, INVENTORY_START_POSITION_Y+165.0, "hud:radar_light");
	PlayerTextDrawFont(playerid, gTdMenu[playerid][9], 4);
	PlayerTextDrawColor(playerid, gTdMenu[playerid][9], 0x1100FF44);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][9], 21.0, 21.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][9], 0);

	gTdMenu[playerid][8] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+155.0, INVENTORY_START_POSITION_Y+170.0, "M");
	PlayerTextDrawColor(playerid, gTdMenu[playerid][8], 0x552288DD);
	PlayerTextDrawTextSize(playerid, gTdMenu[playerid][8], INVENTORY_START_POSITION_X+165.0, 500.0);
	PlayerTextDrawSetShadow(playerid, gTdMenu[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, gTdMenu[playerid][8], 1);
	
	//создаём анимации для всех предметов в инвентаре
	//17050 (?)
	//17051 патроны дробовик (?)
	//12911 патроны пистолет
	//10811 патроны дробовик
	//2036 снайперка
	//2061 - патроны винтовка и автомат
	m = 0;
	
	//получаем перечень инвентаря персонажа
	get_inventory_properties(playerid, inv_isrot, inv_deg, inv_zoom, is_auto);

	for(i = 0; i < 2; ++i)
	for(j = 0; j < 3; ++j, ++m)
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdInventory[playerid][m][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+j*CELL_SIZE, INVENTORY_START_POSITION_Y+i*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdInventory[playerid][m][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][m][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdInventory[playerid][m][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][m][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][m][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][m][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdInventory[playerid][m][k], CELL_SIZE, CELL_SIZE);
		
		//размещаем объекты в инвентаре
		if(gInv[playerid][m] >= 0)
		{
			PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][m][k], gInv[playerid][m]);
			PlayerTextDrawSetPreviewRot(playerid,
										gTdInventory[playerid][m][k],
										inv_isrot[m][0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][0],
										inv_isrot[m][1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][1],
										inv_isrot[m][2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][2],
										inv_zoom[m]);
		}
		else
		{
		    //если в ячейке пусто, ставим нулевой объект
			PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][m][k], 0);
			PlayerTextDrawSetPreviewRot(playerid, gTdInventory[playerid][m][k], 90.0, 0.0, 0.0, -100.0);
		}
		PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][m][k], 1);
	}
	
	for(i = 0; i < 6; ++i)
	{
	    if(is_auto[i])
	    {
			apply_one_cell(playerid, i);
	    }
	}
}

public create_objects_menu(playerid)
{
	new i,j,k,m;
	new inv_isrot[6][3];
	new Float:inv_deg[6][3], Float:inv_zoom[6];

	//получаем свойства объектов для меню объектов
	get_objects_properties(playerid, inv_isrot, inv_deg, inv_zoom, STANDART_RANGE_VALUE);

	m = 0;

	for(i = 0; i < 3; ++i)
	for(j = 0; j < 2; ++j, ++m)
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdObject[playerid][m][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+CELL_SIZE*3+10.0+j*CELL_SIZE, INVENTORY_START_POSITION_Y+i*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdObject[playerid][m][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdObject[playerid][m][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdObject[playerid][m][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdObject[playerid][m][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdObject[playerid][m][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdObject[playerid][m][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdObject[playerid][m][k], CELL_SIZE, CELL_SIZE);

		if(gObj[playerid][m] > 0)
		{
			PlayerTextDrawSetPreviewModel(playerid, gTdObject[playerid][m][k], gObj[playerid][m]);
//			PlayerTextDrawSetPreviewRot(playerid, gTdObject[playerid][m][k], 90.0, 0.0, 0.0, -100.0);
			PlayerTextDrawSetPreviewRot(playerid,
										gTdObject[playerid][m][k],
										inv_isrot[m][0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][0],
										inv_isrot[m][1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][1],
										inv_isrot[m][2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][2],
										inv_zoom[m]);
		}
		else
		{
		    //ставим нулевой объект
			PlayerTextDrawSetPreviewModel(playerid, gTdObject[playerid][m][k], 0);
			PlayerTextDrawSetPreviewRot(playerid, gTdObject[playerid][m][k], 90.0, 0.0, 0.0, -100.0);
		}
		
		PlayerTextDrawSetSelectable(playerid, gTdObject[playerid][m][k], 1);
	}
}

public create_vehicle_menu(playerid, vehicleid)
{
	new k,m;
	new inv_isrot[MAX_INVENTORY_ON_VEHICLE][3];
	new Float:inv_deg[MAX_INVENTORY_ON_VEHICLE][3], Float:inv_zoom[MAX_INVENTORY_ON_VEHICLE];

	//получаем свойства объектов для меню объектов
	if(get_vehicle_objects_properties(vehicleid, inv_isrot, inv_deg, inv_zoom) < 0)
	    return;

	for(m = 0; m < MAX_INVENTORY_ON_VEHICLE; ++m)
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
	    if(m < gVeh[vehicleid][1])
	    {
			gTdVehicle[playerid][m][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(m/2)*CELL_SIZE, INVENTORY_START_POSITION_Y-CELL_SIZE*2-20.0+(m%2)*CELL_SIZE, "MyText");
			PlayerTextDrawFont(playerid, gTdVehicle[playerid][m][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
			PlayerTextDrawSetPreviewVehCol(playerid, gTdVehicle[playerid][m][k], 0, 1);
			PlayerTextDrawUseBox(playerid, gTdVehicle[playerid][m][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdVehicle[playerid][m][k], 0x00000088);
			PlayerTextDrawSetShadow(playerid, gTdVehicle[playerid][m][k], 0);
			PlayerTextDrawBackgroundColor(playerid, gTdVehicle[playerid][m][k], 0xFFFFFF00);
			PlayerTextDrawTextSize(playerid, gTdVehicle[playerid][m][k], CELL_SIZE, CELL_SIZE);

			if(gVehMod[vehicleid][m] > 0)
			{
				PlayerTextDrawSetPreviewModel(playerid, gTdVehicle[playerid][m][k], gVehMod[vehicleid][m]);
	//			PlayerTextDrawSetPreviewRot(playerid, gTdVehicle[playerid][m][k], 90.0, 0.0, 0.0, -100.0);
				PlayerTextDrawSetPreviewRot(playerid,
											gTdVehicle[playerid][m][k],
											inv_isrot[m][0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][0],
											inv_isrot[m][1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][1],
											inv_isrot[m][2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[m][2],
											inv_zoom[m]);
			}
			else
			{
			    //ставим нулевой объект
				PlayerTextDrawSetPreviewModel(playerid, gTdVehicle[playerid][m][k], 0);
				PlayerTextDrawSetPreviewRot(playerid, gTdVehicle[playerid][m][k], 90.0, 0.0, 0.0, -100.0);
			}

			PlayerTextDrawSetSelectable(playerid, gTdVehicle[playerid][m][k], 1);
		}
		else
		    return;
	}
}

public show_inventory(playerid, car_menu)
{
	new i,j,k,m;
	new vehicleid, cells;
	
	//фон инвентаря
	PlayerTextDrawShow(playerid, gTdMenu[playerid][0]);
	PlayerTextDrawShow(playerid, gTdMenu[playerid][11]);
	PlayerTextDrawShow(playerid, gTdMenu[playerid][12]);
	//кнопка закрытия
	PlayerTextDrawShow(playerid, gTdMenu[playerid][2]);
	PlayerTextDrawShow(playerid, gTdMenu[playerid][1]);
	//кнопка 'вопросик'
	PlayerTextDrawShow(playerid, gTdMenu[playerid][4]);
	PlayerTextDrawShow(playerid, gTdMenu[playerid][3]);
	//кнопка 'действие'
	PlayerTextDrawShow(playerid, gTdMenu[playerid][6]);
	PlayerTextDrawShow(playerid, gTdMenu[playerid][5]);
	//кнопка 'гаячный ключ'
	PlayerTextDrawShow(playerid, gTdMenu[playerid][9]);
	PlayerTextDrawShow(playerid, gTdMenu[playerid][8]);
	
	if(car_menu && (vehicleid = GetPlayerVehicleID(playerid)) > 0)
	{
	    //получим id транспорта
//	    vehicleid = GetPlayerVehicleID(playerid);
	    cells = gVeh[vehicleid][1];
	    if(cells > 8)
	    {
			//скрываем данные игрока, ибо мешаются
			hide_statistic_data(playerid);
	    }
		//фон инвентаря авто
		PlayerTextDrawTextSize(playerid, gTdMenu[playerid][10], (cells > 2)?CELL_SIZE*(cells/2):CELL_SIZE, (cells > 1)?CELL_SIZE*2:CELL_SIZE);
		PlayerTextDrawTextSize(playerid, gTdMenu[playerid][15], (cells > 2)?CELL_SIZE*(cells/2):CELL_SIZE, 10.0);
		PlayerTextDrawShow(playerid, gTdMenu[playerid][10]);
		PlayerTextDrawShow(playerid, gTdMenu[playerid][15]);
		PlayerTextDrawShow(playerid, gTdMenu[playerid][16]);

	}
	else
	{
	    car_menu = 0;
		//фон объектов
		PlayerTextDrawShow(playerid, gTdMenu[playerid][7]);
		PlayerTextDrawShow(playerid, gTdMenu[playerid][13]);
		PlayerTextDrawShow(playerid, gTdMenu[playerid][14]);
	}

	//делаем все элементы инвентаря статическими
	for(k = 0; k < 6; ++k)
	{
		gRotate[playerid][k] = 0;
	}

	for(k = 0; k < 6; ++k)
	{
		gRotateObject[playerid][k] = 0;
	}

	for(k = 0; k < MAX_INVENTORY_ON_VEHICLE; ++k)
	{
		gRotateVehicle[playerid][k] = 0;
	}

	//назначаем начальную позицию вращения
	for(k = 0; k < 6; ++k)
	{
		gIndex[playerid][k] = 0;
	}
	
	for(k = 0; k < 6; ++k)
	{
		gIndexObject[playerid][k] = 0;
	}

	for(k = 0; k < MAX_INVENTORY_ON_VEHICLE; ++k)
	{
		gIndexVehicle[playerid][k] = 0;
	}

	//отображаем элементы инвентаря
	m = 0;
	for(i = 0; i < 2; ++i)
	for(j = 0; j < 3; ++j, ++m)
	{
		PlayerTextDrawShow(playerid, gTdInventory[playerid][m][0]);
	}

	if(car_menu)
	{
		create_vehicle_menu(playerid, vehicleid);
		for(m = 0; m < MAX_INVENTORY_ON_VEHICLE; ++m)
		{
		    if(m < gVeh[vehicleid][1])
				PlayerTextDrawShow(playerid, gTdVehicle[playerid][m][0]);
		}
	}
	else
	{
		create_objects_menu(playerid);
		m = 0;
		for(i = 0; i < 2; ++i)
		for(j = 0; j < 3; ++j, ++m)
		{
			PlayerTextDrawShow(playerid, gTdObject[playerid][m][0]);
		}
	}

	//запускаем выбор элементов инвентаря
	SelectTextDraw(playerid, 0x00FF00FF);
	
	//помечаем меню игрока как открытое
	gInventoryMenuShow[playerid] = 1;
	
	if(car_menu)
	{
		gVehicleMenuShow[playerid] = vehicleid;
	}
	else
	{
		//помечаем меню объектов как открытое
		gObjectsMenuShow[playerid] = 1;
	}
}

public hide_menu(playerid)
{
	new j,k;

	//отображаем данные игрока
	show_statistic_data(playerid);

	//делаем все элементы инвентаря статическими
	//для корректного скрытия
	for(k = 0; k < 6; ++k)
	{
		gRotate[playerid][k] = 0;
	}
	
	for(k = 0; k < 6; ++k)
	{
		gRotateObject[playerid][k] = 0;
	}

	for(k = 0; k < MAX_INVENTORY_ON_VEHICLE; ++k)
	{
		gRotateVehicle[playerid][k] = 0;
	}

	//прячем анимации инвентаря
	for(j = 0; j < 6; ++j)
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawHide(playerid, gTdInventory[playerid][j][k]);
		if(gObjectsMenuShow[playerid])
			PlayerTextDrawDestroy(playerid, gTdObject[playerid][j][k]);
	}
	
	for(j = 0; j < MAX_INVENTORY_ON_VEHICLE; ++j)
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		if(gVehicleMenuShow[playerid] > 0 && j < gVeh[gVehicleMenuShow[playerid]][1])
			PlayerTextDrawDestroy(playerid, gTdVehicle[playerid][j][k]);
	}

	//прячем дополнительные поля
	for(k = 0; k < TD_COUNT; ++k)
	{
		PlayerTextDrawHide(playerid, gTdMenu[playerid][k]);
	}

	//прекращаем выбор
	CancelSelectTextDraw(playerid);
	
	//помечаем меню игрока как закрытое
	gInventoryMenuShow[playerid] = 0;
	
	//помечаем меню объектов как закрытое
	gObjectsMenuShow[playerid] = 0;

	//помечаем меню объектов как закрытое
	gVehicleMenuShow[playerid] = 0;
}

public update_neighbors_objects_menu(playerid)
{
	new i;
	new Float:x, Float:y, Float:z;

	GetPlayerPos(playerid, x, y, z);

	for(i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(IsPlayerStreamedIn(playerid, i) && IsPlayerInRangeOfPoint(i, 50.0, x, y, z))
	    {
	        new Float:x1, Float:y1, Float:z1;
	        new AnimIndex;

			AnimIndex = GetPlayerAnimationIndex(i);
	        if(AnimIndex == 1189 || //стоит
			   AnimIndex == 1274 || //сидит
			   AnimIndex == 1159 || //закончил переход гусиным шагом
			   AnimIndex == 1177 || //очухался после удара (стоит)
			   AnimIndex == 1133) //закончил прыжок
	        {
		        GetPlayerPos(i, x1, y1, z1);
				//обновляем стример игрока
		        SetPlayerPos(i, x1+0.001, y1, z1);
		        //можно было бы ещё обновлять меню объектов, если оно отображается и персонаж находится достаточно близко
		        //...
		        if(gObjectsMenuShow[i] > 0 && IsPlayerInRangeOfPoint(i, 5.0, x, y, z))
				{
				    update_objects_menu(i);
				}
			}
	    }
	}
}

public update_objects_menu(playerid)
{
	new i, j, k, m;
	
	if(gObjectsMenuShow[playerid] > 0)
	{
		//удаляем анимации дополнительного меню объектов
		for(j = 0; j < 6; ++j)
	    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	    {
			PlayerTextDrawDestroy(playerid, gTdObject[playerid][j][k]);
		}
		
		//делаем все элементы инвентаря статическими
		for(k = 0; k < 6; ++k)
		{
			gRotate[playerid][k] = 0;
		}

		for(k = 0; k < 6; ++k)
		{
			gRotateObject[playerid][k] = 0;
		}
		
		//создаём дополнительное меню заново
		create_objects_menu(playerid);
		m = 0;
		for(i = 0; i < 2; ++i)
		for(j = 0; j < 3; ++j, ++m)
		{
			PlayerTextDrawShow(playerid, gTdObject[playerid][m][0]);
		}
	}
}

public destroy_menu(playerid)
{
	new j,k;

	//прекращаем выбор
	CancelSelectTextDraw(playerid);

	//спрячем весь инвентарь
	hide_menu(playerid);

	//удаляем анимации инвентаря
	for(j = 0; j < 6; ++j)
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdInventory[playerid][j][k]);
		if(gObjectsMenuShow[playerid])
			PlayerTextDrawDestroy(playerid, gTdObject[playerid][j][k]);
	}
	
	for(j = 0; j < MAX_INVENTORY_ON_VEHICLE; ++j)
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		if(gVehicleMenuShow[playerid] > 0 && j < gVeh[gVehicleMenuShow[playerid]][1])
			PlayerTextDrawDestroy(playerid, gTdVehicle[playerid][j][k]);
	}

	//удаляем дополнительные поля
	for(k = 0; k < TD_COUNT; ++k)
	{
		PlayerTextDrawDestroy(playerid, gTdMenu[playerid][k]);
	}
	
	//помечаем меню игрока как закрытое
	gInventoryMenuShow[playerid] = 0;
	
	//помечаем меню объектов как закрытое
	gObjectsMenuShow[playerid] = 0;

	//помечаем меню объектов как закрытое
	gVehicleMenuShow[playerid] = 0;
}

//сбросить весь выбор
public stop_all_rotates(playerid)
{
	new i;
	
	for(i = 0; i < 6; ++i)
	{
	    if(gRotate[playerid][i] == 1)
	    {
		    gRotate[playerid][i] = 0;
			PlayerTextDrawHide(playerid, gTdInventory[playerid][i][gIndex[playerid][i]]);
			PlayerTextDrawShow(playerid, gTdInventory[playerid][i][0]);
			gIndex[playerid][i] = 0;
		}

	    if(gObjectsMenuShow[playerid] > 0 && gRotateObject[playerid][i] == 1)
	    {
		    gRotateObject[playerid][i] = 0;
			PlayerTextDrawHide(playerid, gTdObject[playerid][i][gIndexObject[playerid][i]]);
			PlayerTextDrawShow(playerid, gTdObject[playerid][i][0]);
			gIndexObject[playerid][i] = 0;
		}
	}

    for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
	    if(gVehicleMenuShow[playerid] > 0 && i < gVeh[gVehicleMenuShow[playerid]][1] && gRotateVehicle[playerid][i] == 1)
	    {
		    gRotateVehicle[playerid][i] = 0;
			PlayerTextDrawHide(playerid, gTdVehicle[playerid][i][gIndexVehicle[playerid][i]]);
			PlayerTextDrawShow(playerid, gTdVehicle[playerid][i][0]);
			gIndexVehicle[playerid][i] = 0;
		}
	}
}

//проверяет, не нужно ли удалить отображение статуса авто
public check_vehicle_menu_show()
{
	for(new i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(gVehicleDataShow[i] > 0 && !IsPlayerInVehicleReal(i))
	    {
	        destroy_vehicle_sensors(i);
		}
/*
	    if(gVehicleDataShow[i] == 0 && IsPlayerInAnyVehicle(i))
	    {
	        new anim;
	        
	        anim = GetPlayerAnimationIndex(i);
	        if(anim == 0)
		        create_vehicle_sensors(i, GetPlayerVehicleID(i));
		}
*/
	}
}

public is_one_inventory_cell_selected(playerid)
{
	new i, count, mem;
	
	count = 0;
	mem = -1;
	for(i = 0; i < 6; ++i)
	{
	    if(gRotate[playerid][i] > 0)
	    {
	        mem = i;
	        count++;
	    }
	    
	    if(gRotateObject[playerid][i] > 0)
	        count++;
	        
	 }
	 
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
		if(gVehicleMenuShow[playerid])
		{
		    if(gRotateVehicle[playerid][i] > 0)
		    {
		        count++;
		    }
		}
	}
	
	if(count == 1 && mem != -1)
	    return mem;
	else
	    return -1;
}

public is_one_objects_cell_selected(playerid)
{
	new i, count, mem;

	count = 0;
	mem = -1;
	for(i = 0; i < 6; ++i)
	{
	    if(gRotate[playerid][i] > 0)
	    {
	        count++;
	    }

	    if(gRotateObject[playerid][i] > 0)
	    {
	        mem = i;
	        count++;
		}
	}
	
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
		if(gVehicleMenuShow[playerid])
		{
		    if(gRotateVehicle[playerid][i] > 0)
		    {
		        count++;
		    }
		}
	}

	if(count == 1 && mem != -1)
	    return mem;
	else
	    return -1;
}

public is_one_vehicle_cell_selected(playerid)
{
	new i, count, mem;

	count = 0;
	mem = -1;
	for(i = 0; i < 6; ++i)
	{
	    if(gRotate[playerid][i] > 0)
	    {
	        count++;
	    }

	    if(gRotateObject[playerid][i] > 0)
	    {
	        count++;
		}
	}
	
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
		if(gVehicleMenuShow[playerid])
		{
		    if(gRotateVehicle[playerid][i] > 0)
		    {
		        mem = i;
		        count++;
		    }
		}
	}

	if(count == 1 && mem != -1)
	    return mem;
	else
	    return -1;
}

//inv1 - откуда
//inv2 - куда
//нумерация с 0
public replace_inventory(playerid, inv1, inv2)
{
	new k;
	new is_auto;
	new inv_isrot[3];
	new Float:inv_deg[3], Float:inv_zoom;

	//защита от некорректного вызова функции
	if(gInv[playerid][inv2] != -1)
	    return;

	//остановим вращение выбранного объекта
	gRotate[playerid][inv1] = 0;
	gIndex[playerid][inv1] = 0;
	gRotate[playerid][inv2] = 0;
	gIndex[playerid][inv2] = 0;

	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdInventory[playerid][inv1][k]);
		PlayerTextDrawDestroy(playerid, gTdInventory[playerid][inv2][k]);
	}
	
	//производим перенос в БД
	move_character_inventory_cell(playerid, inv1, inv2);

	//получаем данные инвентаря персонажа
	get_inventory_properties_cell(playerid, inv2, inv_isrot, inv_deg, inv_zoom, is_auto);

	//создаём перенесённую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdInventory[playerid][inv2][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(inv2%3)*CELL_SIZE, INVENTORY_START_POSITION_Y+(inv2/3)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdInventory[playerid][inv2][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][inv2][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdInventory[playerid][inv2][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][inv2][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][inv2][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][inv2][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdInventory[playerid][inv2][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][inv2][k], gInv[playerid][inv2]);
		PlayerTextDrawSetPreviewRot(playerid,
									gTdInventory[playerid][inv2][k],
									inv_isrot[0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[0],
									inv_isrot[1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[1],
									inv_isrot[2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[2],
									inv_zoom);
		PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][inv2][k], 1);
	}

	//создаём новую пустую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdInventory[playerid][inv1][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(inv1%3)*CELL_SIZE, INVENTORY_START_POSITION_Y+(inv1/3)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdInventory[playerid][inv1][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][inv1][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdInventory[playerid][inv1][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][inv1][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][inv1][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][inv1][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdInventory[playerid][inv1][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][inv1][k], 0);
		PlayerTextDrawSetPreviewRot(playerid, gTdInventory[playerid][inv1][k], 90.0, 0.0, 0.0, -100.0);

		PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][inv1][k], 1);
	}
	
	//отображаем пустую ячейку
	PlayerTextDrawShow(playerid, gTdInventory[playerid][inv1][0]);
	
	//отображаем перенесённый объект
	PlayerTextDrawShow(playerid, gTdInventory[playerid][inv2][0]);
}

//inv - ячейка инвентаря
//obj - ячейка объектов
//нумерация с 0
public take_object(playerid, inv, obj)
{
	new k;
	new is_auto;
	new inv_isrot[3];
	new Float:inv_deg[3], Float:inv_zoom;

	//защита от некорректного вызова функции
	if(gInv[playerid][inv] != -1 || gObj[playerid][obj] == -1)
	    return;

	//остановим вращение выбранного объекта
	gRotate[playerid][inv] = 0;
	gRotateObject[playerid][obj] = 0;

	//проверяем, не автомобиль ли перемещаемый объект
	for(new i = 0; i < MAX_VEHICLES; ++i)
	{
	    if(gVeh[i][0] == gObjThing[playerid][obj])
	    {
			PlayerTextDrawHide(playerid, gTdObject[playerid][obj][gIndexObject[playerid][obj]]);
			PlayerTextDrawShow(playerid, gTdObject[playerid][obj][0]);
			gIndex[playerid][inv] = 0;
			gIndexObject[playerid][obj] = 0;
			return;
		}
	}

	//установим индексы активных текстдравов
	gIndex[playerid][inv] = 0;
	gIndexObject[playerid][obj] = 0;

	//производим перенос в БД, если объект ещё никто не подобрал
	if(set_character_inventory_cell(playerid, inv, gObjThing[playerid][obj]) > 0)
	{
		//удаляем анимации выбранных объектов
	    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	    {
			PlayerTextDrawDestroy(playerid, gTdInventory[playerid][inv][k]);
		}

		//получаем данные инвентаря персонажа
		get_inventory_properties_cell(playerid, inv, inv_isrot, inv_deg, inv_zoom, is_auto);

		//создаём перенесённую ячейку
		for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
		{
			gTdInventory[playerid][inv][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(inv%3)*CELL_SIZE, INVENTORY_START_POSITION_Y+(inv/3)*CELL_SIZE, "MyText");
			PlayerTextDrawFont(playerid, gTdInventory[playerid][inv][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
			PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][inv][k], 0, 1);
			PlayerTextDrawUseBox(playerid, gTdInventory[playerid][inv][k], 0);
	//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][inv][k], 0xFFFFFF88);
			PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][inv][k], 0);
			PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][inv][k], 0xFFFFFF00);
			PlayerTextDrawTextSize(playerid, gTdInventory[playerid][inv][k], CELL_SIZE, CELL_SIZE);

			PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][inv][k], gInv[playerid][inv]);
			//оптимизация!
			//заменить *,/ и + на функции для float
			//...
			PlayerTextDrawSetPreviewRot(playerid,
										gTdInventory[playerid][inv][k],
										inv_isrot[0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[0],
										inv_isrot[1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[1],
										inv_isrot[2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[2],
										inv_zoom);
			PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][inv][k], 1);
		}
		
		//отображаем перенесённый объект
		if(gInventoryMenuShow[playerid] > 0)
			PlayerTextDrawShow(playerid, gTdInventory[playerid][inv][0]);
			
		if(is_auto)
			apply_one_cell(playerid, inv);
	}

	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdObject[playerid][obj][k]);
	}

	//создаём новую пустую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdObject[playerid][obj][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+CELL_SIZE*3+10.0+(obj%2)*CELL_SIZE, INVENTORY_START_POSITION_Y+(obj/2)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdObject[playerid][obj][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdObject[playerid][obj][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdObject[playerid][obj][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdObject[playerid][obj][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdObject[playerid][obj][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdObject[playerid][obj][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdObject[playerid][obj][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdObject[playerid][obj][k], 0);
		PlayerTextDrawSetPreviewRot(playerid, gTdObject[playerid][obj][k], 90.0, 0.0, 0.0, -100.0);

		PlayerTextDrawSetSelectable(playerid, gTdObject[playerid][obj][k], 1);
	}
	gObj[playerid][obj] = -1;
	gObjThing[playerid][obj] = -1;

	//отображаем пустую ячейку
	if(gObjectsMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdObject[playerid][obj][0]);
		
	update_neighbors_objects_menu(playerid);
}

//положить созданный объект в инвентарь
public put_object(playerid, cell)
{
	new k;
	new is_auto;
	new inv_num[8];
	new result[64];
	new inv_isrot[3];
	new Float:inv_deg[3], Float:inv_zoom;

	//проверяем, не пустая ли эта ячейка инвентаря на самом деле?
	strdel(result, 0, sizeof(result));
	strdel(inv_num, 0, sizeof(inv_num));
	format(inv_num, sizeof(inv_num), "inv%d", cell+1);
	get_character_value(playerid, inv_num, "inventory", result);
	if(strval(result) < 0) //если ячейка пуста - выходим
	{
	    gInv[playerid][cell] = -1;
	    gInvThing[playerid][cell] = -1;
	    return;
	}

	//получаем данные выкладываемого объекта
	get_inventory_properties_cell(playerid, cell, inv_isrot, inv_deg, inv_zoom, is_auto);

	//остановим вращение выбранного объекта
	gRotate[playerid][cell] = 0;
	gIndex[playerid][cell] = 0;

	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdInventory[playerid][cell][k]);
	}
	
	//создаём ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdInventory[playerid][cell][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(cell%3)*CELL_SIZE, INVENTORY_START_POSITION_Y+(cell/3)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdInventory[playerid][cell][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][cell][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdInventory[playerid][cell][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][cell][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][cell][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][cell][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdInventory[playerid][cell][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][cell][k], gInv[playerid][cell]);
		PlayerTextDrawSetPreviewRot(playerid,
									gTdInventory[playerid][cell][k],
									inv_isrot[0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[0],
									inv_isrot[1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[1],
									inv_isrot[2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[2],
									inv_zoom);
		PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][cell][k], 1);
	}
	
	//отображаем созданный объект
	if(gInventoryMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdInventory[playerid][cell][0]);
		
	if(is_auto)
	    apply_one_cell(playerid, cell);
}

//отобразить созданный объект в инвентаре транспорта
public put_vehicle_object(playerid, vehicleid, cell)
{
	new k;
	new inv_num[8];
	new result[64];
	new inv_isrot[3];
	new Float:inv_deg[3], Float:inv_zoom;

	//проверяем, не пустая ли эта ячейка инвентаря на самом деле?
	strdel(result, 0, sizeof(result));
	strdel(inv_num, 0, sizeof(inv_num));
	format(inv_num, sizeof(inv_num), "inv%d", cell+1);
	get_vehicle_value(vehicleid, inv_num, "veh_invent", result);
	if(strval(result) < 0) //если ячейка пуста - выходим
	{
	    gVehMod[vehicleid][cell] = -1;
	    gVehObj[vehicleid][cell] = -1;
	    return;
	}

	//получаем данные выкладываемого объекта
	get_vehicle_properties_cell(vehicleid, cell, inv_isrot, inv_deg, inv_zoom);

	//остановим вращение выбранного объекта
	gRotateVehicle[playerid][cell] = 0;
	gIndexVehicle[playerid][cell] = 0;

	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdVehicle[playerid][cell][k]);
	}

	//создаём ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdVehicle[playerid][cell][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(cell/2)*CELL_SIZE, INVENTORY_START_POSITION_Y-CELL_SIZE*2-20.0+(cell%2)*CELL_SIZE, "VehObj");
		PlayerTextDrawFont(playerid, gTdVehicle[playerid][cell][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdVehicle[playerid][cell][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdVehicle[playerid][cell][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdVehicle[playerid][cell][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdVehicle[playerid][cell][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdVehicle[playerid][cell][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdVehicle[playerid][cell][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdVehicle[playerid][cell][k], gVehMod[vehicleid][cell]);
		PlayerTextDrawSetPreviewRot(playerid,
									gTdVehicle[playerid][cell][k],
									inv_isrot[0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[0],
									inv_isrot[1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[1],
									inv_isrot[2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[2],
									inv_zoom);
		PlayerTextDrawSetSelectable(playerid, gTdVehicle[playerid][cell][k], 1);
	}

	//отображаем созданный объект
	if(gVehicleMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdVehicle[playerid][cell][0]);
}

//просто удалить объект из инвентаря
public remove_object(playerid, cell)
{
	new k;
	new inv_num[8];
	new result[64];

	//проверяем, пустая ли эта ячейка инвентаря на самом деле?
	strdel(result, 0, sizeof(result));
	strdel(inv_num, 0, sizeof(inv_num));
	format(inv_num, sizeof(inv_num), "inv%d", cell+1);
	get_character_value(playerid, inv_num, "inventory", result);
	if(strval(result) >= 0) //если ячейка не пуста - выходим
	{
	    return;
	}

	//остановим вращение выбранного объекта
	gRotate[playerid][cell] = 0;
	gIndex[playerid][cell] = 0;
	
	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdInventory[playerid][cell][k]);
	}
	
	//обнуляем переменные, т.к. в ячейке пусто
	gInv[playerid][cell] = -1;
	gInvThing[playerid][cell] = -1;
	
	//создаём новую пустую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdInventory[playerid][cell][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(cell%3)*CELL_SIZE, INVENTORY_START_POSITION_Y+(cell/3)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdInventory[playerid][cell][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][cell][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdInventory[playerid][cell][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][cell][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][cell][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][cell][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdInventory[playerid][cell][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][cell][k], 0);
		PlayerTextDrawSetPreviewRot(playerid, gTdInventory[playerid][cell][k], 90.0, 0.0, 0.0, -100.0);

		PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][cell][k], 1);
	}
	
	//отображаем пустую ячейку
	if(gInventoryMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdInventory[playerid][cell][0]);
}

//просто удалить объект из инвентаря
public remove_vehicle_object(playerid, vehicleid, cell)
{
	new k;
	new inv_num[8];
	new result[64];

	//проверяем, пустая ли эта ячейка инвентаря на самом деле?
	strdel(result, 0, sizeof(result));
	strdel(inv_num, 0, sizeof(inv_num));
	format(inv_num, sizeof(inv_num), "inv%d", cell+1);
	get_vehicle_value(vehicleid, inv_num, "veh_invent", result);
	if(strval(result) >= 0) //если ячейка не пуста - выходим
	{
	    return;
	}

	//остановим вращение выбранного объекта
	gRotateVehicle[playerid][cell] = 0;
	gIndexVehicle[playerid][cell] = 0;

	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdVehicle[playerid][cell][k]);
	}

	//создаём новую пустую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdVehicle[playerid][cell][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(cell/2)*CELL_SIZE, INVENTORY_START_POSITION_Y-CELL_SIZE*2-20.0+(cell%2)*CELL_SIZE, "VehObj");
		PlayerTextDrawFont(playerid, gTdVehicle[playerid][cell][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdVehicle[playerid][cell][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdVehicle[playerid][cell][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdVehicle[playerid][cell][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdVehicle[playerid][cell][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdVehicle[playerid][cell][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdVehicle[playerid][cell][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdVehicle[playerid][cell][k], 0);
		PlayerTextDrawSetPreviewRot(playerid, gTdVehicle[playerid][cell][k], 90.0, 0.0, 0.0, -100.0);

		PlayerTextDrawSetSelectable(playerid, gTdVehicle[playerid][cell][k], 1);
	}

	//отображаем пустую ячейку
	if(gVehicleMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdVehicle[playerid][cell][0]);
}

public take_from_vehicle(playerid, vehicleid, cell, veh_cell)
{
    take_vehicle_inventory_cell(playerid, vehicleid, cell, veh_cell);
    put_object(playerid, cell);
	remove_vehicle_object(playerid, vehicleid, veh_cell);
}

public put_in_vehicle(playerid, vehicleid, cell, veh_cell)
{
	//отменить действие вещи
	unapply_one_cell(playerid, cell);
    give_vehicle_inventory_cell(playerid, vehicleid, cell, veh_cell);
    remove_object(playerid, cell);
	put_vehicle_object(playerid, vehicleid, veh_cell);
}

public replace_vehicle_inventory(playerid, vehicleid, inv1, inv2)
{
	move_vehicle_inventory_cell(vehicleid, inv1, inv2);
	remove_vehicle_object(playerid, vehicleid, inv1);
	put_vehicle_object(playerid, vehicleid, inv2);
}

//inv - ячейка инвентаря
//obj - ячейка объектов
//нумерация с 0
public drop_object(playerid, inv, obj)
{
	new k;
	new is_auto;
	new inv_isrot[3];
	new Float:inv_deg[3], Float:inv_zoom;

	//защита от некорректного вызова функции
	if(gInv[playerid][inv] == -1 || gObj[playerid][obj] != -1)
	    return;

	//остановим вращение выбранного объекта
	gRotate[playerid][inv] = 0;
	gIndex[playerid][inv] = 0;
	gRotateObject[playerid][obj] = 0;
	gIndexObject[playerid][obj] = 0;

	//удаляем анимации выбранных объектов
    for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
    {
		PlayerTextDrawDestroy(playerid, gTdInventory[playerid][inv][k]);
		PlayerTextDrawDestroy(playerid, gTdObject[playerid][obj][k]);
	}

	//получаем данные выкладываемого объекта
	get_inventory_properties_cell(playerid, inv, inv_isrot, inv_deg, inv_zoom, is_auto);
	
	if(is_auto)
	{
		//отменить действие вещи
		unapply_one_cell(playerid, inv);
	}

	//производим перенос в БД
	drop_character_inventory_cell(playerid, inv, obj);

	//создаём новую пустую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdInventory[playerid][inv][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+(inv%3)*CELL_SIZE, INVENTORY_START_POSITION_Y+(inv/3)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdInventory[playerid][inv][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdInventory[playerid][inv][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdInventory[playerid][inv][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdInventory[playerid][inv][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdInventory[playerid][inv][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdInventory[playerid][inv][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdInventory[playerid][inv][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdInventory[playerid][inv][k], 0);
		PlayerTextDrawSetPreviewRot(playerid, gTdInventory[playerid][inv][k], 90.0, 0.0, 0.0, -100.0);

		PlayerTextDrawSetSelectable(playerid, gTdInventory[playerid][inv][k], 1);
	}

	//создаём перенесённую ячейку
	for(k = 0; k < MAX_TURNS_OF_PREVIEW; ++k)
	{
		gTdObject[playerid][obj][k] = CreatePlayerTextDraw(playerid, INVENTORY_START_POSITION_X+CELL_SIZE*3+10.0+(obj%2)*CELL_SIZE, INVENTORY_START_POSITION_Y+(obj/2)*CELL_SIZE, "MyText");
		PlayerTextDrawFont(playerid, gTdObject[playerid][obj][k], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetPreviewVehCol(playerid, gTdObject[playerid][obj][k], 0, 1);
		PlayerTextDrawUseBox(playerid, gTdObject[playerid][obj][k], 0);
//			PlayerTextDrawBoxColor(playerid, gTdObject[playerid][obj][k], 0xFFFFFF88);
		PlayerTextDrawSetShadow(playerid, gTdObject[playerid][obj][k], 0);
		PlayerTextDrawBackgroundColor(playerid, gTdObject[playerid][obj][k], 0xFFFFFF00);
		PlayerTextDrawTextSize(playerid, gTdObject[playerid][obj][k], CELL_SIZE, CELL_SIZE);

		PlayerTextDrawSetPreviewModel(playerid, gTdObject[playerid][obj][k], gObj[playerid][obj]);
		PlayerTextDrawSetPreviewRot(playerid,
									gTdObject[playerid][obj][k],
									inv_isrot[0]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[0],
									inv_isrot[1]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[1],
									inv_isrot[2]*(k)*(360.0/MAX_TURNS_OF_PREVIEW)+inv_deg[2],
									inv_zoom);

		PlayerTextDrawSetSelectable(playerid, gTdObject[playerid][obj][k], 1);
	}

	//отображаем пустую ячейку
	if(gObjectsMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdObject[playerid][obj][0]);

	//отображаем перенесённый объект
	if(gInventoryMenuShow[playerid] > 0)
		PlayerTextDrawShow(playerid, gTdInventory[playerid][inv][0]);
		
	update_neighbors_objects_menu(playerid);
}

public bool:is_player_on_gas_station(playerid)
{
	new Float:radius;
	
	radius=50.0; //пока не сделаю свои точки
	
	if( IsPlayerInRangeOfPoint(playerid, radius, 1920.6521, -1791.8212, 13.3802) ||	//Idlewood Gas
		IsPlayerInRangeOfPoint(playerid, radius, 995.0821, -920.3821, 42.1803) ||	//Mulholland Gas
		IsPlayerInRangeOfPoint(playerid, radius, 647.5121, -564.8921, 16.21022) ||	//Dillimore Gas
		IsPlayerInRangeOfPoint(playerid, radius, 1369.6621, 464.6521, 20.0112) ||	//Montgomery Gas
		IsPlayerInRangeOfPoint(playerid, radius, -66.4521, -1172.1201, 1.8501) ||	//Flint Gas
		IsPlayerInRangeOfPoint(playerid, radius, -1581.6712, -2714.9921, 48.5401) ||	//Whestone Gas
		IsPlayerInRangeOfPoint(playerid, radius, -2237.3245, -2570.9921, 31.9201) ||	//Angel Pine Gas
		IsPlayerInRangeOfPoint(playerid, radius, 2061.2015, 159.2013, 28.8412) ||	//Doherty Gas
		IsPlayerInRangeOfPoint(playerid, radius, -2428.3821, 993.9012, 45.3012) ||	//Juniper Gas
		IsPlayerInRangeOfPoint(playerid, radius, -1699.9421, 413.5921, 7.1821) ||	//Easter Gas
		IsPlayerInRangeOfPoint(playerid, radius, -1326.5312, 2698.5210, 50.0612) ||	//El Guebrabos Gas
		IsPlayerInRangeOfPoint(playerid, radius, -1462.7121, 1864.5721, 32.6312) ||	//Tierra Robada Gas
		IsPlayerInRangeOfPoint(playerid, radius, 65.1613, 1211.3212, 18.8101) ||	//Fort Carson Gas
		IsPlayerInRangeOfPoint(playerid, radius, 2173.1231, 2478.3012, 10.8201) ||	//Emerald Isle Gas
		IsPlayerInRangeOfPoint(playerid, radius, 2138.4712, 2747.6012, 10.8201) ||	//Prickle Pine Gas
		IsPlayerInRangeOfPoint(playerid, radius, 2639.6113, 1121.9921, 10.8201) ||	//Jullius Gas
		IsPlayerInRangeOfPoint(playerid, radius, 2115.6001, 919.36, 10.8201) ||	//Come A Lot Gas
		IsPlayerInRangeOfPoint(playerid, radius, 606.2412, 1688.6121, 6.9915) ||	//Bone County Gas
		IsPlayerInRangeOfPoint(playerid, radius, 1597.5412, 2213.2810, 10.8201) )	//Redsands West Gas
	{
        return true;
	}

	return false;
}

stock IsPlayerInWater(playerid)
{
	new animid;
	
//	new str[64]; //отладка!!!
//	format(str, sizeof(str), "%d", GetPlayerAnimationIndex(playerid)); //отладка!!!
//	SendClientMessage(playerid, 0x55FFCAFF, str); //отладка!!!

    animid = GetPlayerAnimationIndex(playerid);
	
	if(animid == 1250 ||
	   animid == 1540 ||
	   animid == 1541 ||
	   animid == 1544 ||
	   animid == 1539 ||
	   animid == 1538)
	{
		new Float:pnX,Float:pnY,Float:pnZ;

		GetPlayerPos(playerid,pnX,pnY,pnZ);

		if(pnZ < 1.5)
			return 1;

		if((IsPlayerInRangeOfPoint(playerid, 300.0, -1168.11,2151.00,40.0) ||
	        IsPlayerInRangeOfPoint(playerid, 300.0, -958.11,2401.0,40.0) ||
            IsPlayerInRangeOfPoint(playerid, 300.0, -1084.94,2649.50,40.04))&& pnZ < 45)
			return 1;

	    if((IsPlayerInRangeOfPoint(playerid, 100.0, 1962.16,1599.64,10.0) ||
	        IsPlayerInRangeOfPoint(playerid, 50.0, 2147.28,1131.29,10.0) ||
	        IsPlayerInRangeOfPoint(playerid, 30.0, 1237.01,-2378.30,10.0) ||
			IsPlayerInRangeOfPoint(playerid, 50.0, 2108.07,1907.37,10.0)) && pnZ < 10)
			return 1;
			
	    if(IsPlayerInRangeOfPoint(playerid, 50.0, 1962.66,-1190.69,17.45) && pnZ < 20)
			return 1;
	}
    return 0;
}

public ChooseLanguage(playerid)
{
  new i;
  new mes[1024]; //само сообщение
  new lang_mes[32]; //языки
  new ok_mes[16]; //надпись на кнопке "OK"
  new cancel_mes[16]; //надпись на кнопке "Отмена"
  new title[64]; //заголовок

  strdel(mes, 0, sizeof(mes));
  strcat(mes, "{FFFFFF}");
  imessage(mes, "DONT_CHANGE", gPlayerLang[playerid]);
  strcat(mes, "\n");
  for(i = 0; i  < gLangsNumber; ++i)
  {
    imessage(lang_mes, "LANGUAGE_NAME", gAllLangs[i]);
    strcat(mes, "{FFFF00}");
    strcat(mes, gAllLangs[i]);
    strcat(mes, "\t{0000FF}[{00FF00}");
    strcat(mes, lang_mes);
    strcat(mes, "{0000FF}]\n");
  }
  imessage(title, "LANGUAGE_TITLE", gPlayerLang[playerid]);
  imessage(ok_mes, "OK_MESSAGE", gPlayerLang[playerid]);
  imessage(cancel_mes, "CANCEL_MESSAGE", gPlayerLang[playerid]);
  ShowPlayerDialog(playerid, 756, DIALOG_STYLE_LIST, title, mes, ok_mes, cancel_mes);
}

public player_login_menu()
{
	new mes[1024]; //само сообщение
	new ok_mes[16]; //надпись на кнопке "OK"
	new cancel_mes[16]; //надпись на кнопке "Отмена"
	new title[64]; //заголовок
	new bool:restart;

	restart = false;
	for(new playerid = 0; playerid < MAX_PLAYERS; ++playerid)
	{
	    if(gPlayerPasswordRequest[playerid] == 0)
	        continue;

	    if(gPlayerPasswordRequest[playerid] == 2)
	    {
			restart = true;
		}
		else
		{
	        gPlayerPasswordRequest[playerid] = 0;
			strdel(mes, 0, sizeof(mes));
			imessage(mes, "PASSWORD_WORD", gPlayerLang[playerid]);
			imessage(title, "PASSWORD_INPUT_TITLE", gPlayerLang[playerid]);
			imessage(ok_mes, "OK_MESSAGE", gPlayerLang[playerid]);
			imessage(cancel_mes, "CANCEL_MESSAGE", gPlayerLang[playerid]);
			ShowPlayerDialog(playerid, 456, DIALOG_STYLE_PASSWORD, title, mes, ok_mes, cancel_mes);
		}
	}
	
	if(restart)
	    SetTimer("player_login_menu", 1000, false);
}

public imes_simple_single(playerid, color, str[])
{
	new imes[256];

	imessage(imes, str, gPlayerLang[playerid]);
	//переделать бы плагин и добавить сюда проверку на нахождение искомой строки
	//и если строка не найдена - отсылается её имя (т.е. str)
	//...
	SendClientMessage(playerid, color, imes);
}

public IsPlayerInVehicleReal(playerid)
{
	new anim;
	
	if( IsPlayerInAnyVehicle(playerid))
		return 1;

	anim = GetPlayerAnimationIndex(playerid);
	if( anim == 0 || anim == 1009 || anim == 1043 || anim == 1026 || anim == 1013 ||
	    anim == 1010 || anim == 1044 || anim == 1027 || anim == 1014 || anim == 1054)
		return 1;

	return 0;
}

public show_help_for_player(playerid)
{
	new mes[256];
	new help_mes[1536];

	strdel(help_mes, 0, sizeof(help_mes));
	imessage(mes, "HELP_LANG",gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_LAND", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_HELP", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n\n");

	imessage(mes, "HELP_ALL_COMMANDS_ABOUT", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_ALL_COMMANDS_Y", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_ALL_COMMANDS_2", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n\n");

	imessage(mes, "HELP_INVENTORY_BUTTONS_ABOUT", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_INVENTORY_BUTTONS_X", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_INVENTORY_BUTTONS_Q", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_INVENTORY_BUTTONS_V", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_INVENTORY_BUTTONS_M", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n\n");

	imessage(mes, "HELP_TEST_ABOUT", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_TEST_CMDS_TP", gPlayerLang[playerid]);
	strcat(help_mes, mes);
	strcat(help_mes, "\n");
	imessage(mes, "HELP_TEST_CMDS_LIVE", gPlayerLang[playerid]);
	strcat(help_mes, mes);

	ShowPlayerDialog(playerid,4557,DIALOG_STYLE_MSGBOX,"/help",help_mes,"OK","");
}

//обновляем позицию перемещённого транспорта
public update_unoccupied_vehicles()
{
	gUnoccupiedUpdateTimer = -1;
	
	for(new vehicleid = 1; vehicleid < MAX_VEHICLES; ++vehicleid)
	{
	    if(gUnoccupiedVehData[vehicleid] != INVALID_PLAYER_ID)
		{
			drop_vehicle_from_dot(gUnoccupiedVehData[vehicleid], vehicleid);
		    save_vehicle_position(vehicleid);
			gUnoccupiedVehData[vehicleid] = INVALID_PLAYER_ID;
		}
	}
}

public update_weapon_ammo()
{
	gUpdateWeaponTimer = -1;
	
	for(new playerid = 0; playerid < MAX_PLAYERS; ++playerid)
	{
	    if(gWeaponUpdate[playerid] > 0)
	    {
		    //собственно, освобождаем пустые обоймы
		    save_character_ammo(playerid);
		}
	}
}

public unapply_one_cell(playerid, cell)
{
	new name[128];
	new value;
//	new vehicleid, out_cell;

	//получим имя вещи и присвоенное ей значение
	value = 0;
	get_object_data(playerid, cell, INVENTORY_AREA, name, value);
	
	if(strcmp(name, "GPS_NAVIGATOR") == 0)
	{
	    show_smokescreen(playerid);
		show_smoke_map(playerid);
	    return;
	}
}

//использовать объект в ячейке
public apply_one_cell(playerid, cell)
{
	new name[128];
	new value;
	new vehicleid, out_cell;

	//получим имя вещи и присвоенное ей значение
	value = 0;
	get_object_data(playerid, cell, INVENTORY_AREA, name, value);
	
//	imes_simple_single(playerid, 0xFFFF00, name); //отладка!!!
	
	if(strcmp(name, "EMPTY_AK47") == 0)
	{
	    return;
	}

	if(strcmp(name, "LOADED_AK47") == 0)
	{
		ResetPlayerWeapons(playerid);
		give_character_weapon(playerid, cell); //инициализация gPlayerWeapon[playerid][0] и gPlayerWeapon[playerid][1]
		if(gPlayerWeapon[playerid][0] != -1)
			GivePlayerWeapon(playerid, 30, gPlayerWeapon[playerid][1]); //даём автомат Калашникова
	    return;
	}

	if(strcmp(name, "LOADED_M4") == 0)
	{
		ResetPlayerWeapons(playerid);
		give_character_weapon(playerid, cell);
		if(gPlayerWeapon[playerid][0] != -1)
			GivePlayerWeapon(playerid, 31, gPlayerWeapon[playerid][1]); //даём M4
	    return;
	}

	if(strcmp(name, "LOADED_PISTOL") == 0)
	{
		ResetPlayerWeapons(playerid);
		give_character_weapon(playerid, cell);
		if(gPlayerWeapon[playerid][0] != -1)
			GivePlayerWeapon(playerid, 24, gPlayerWeapon[playerid][1]); //даём Пустынный Орёл
	    return;
	}

	if(strcmp(name, "LOADED_RIFLE") == 0)
	{
		ResetPlayerWeapons(playerid);
		give_character_weapon(playerid, cell);
		if(gPlayerWeapon[playerid][0] != -1)
			GivePlayerWeapon(playerid, 33, gPlayerWeapon[playerid][1]); //даём Винтовку (снайперка 34)
	    return;
	}

	if(strcmp(name, "BOTTLE_OF_LEMONADE") == 0 ||
	   strcmp(name, "BOTTLE_OF_JUICE") == 0)
	{
	    if(gThirst[playerid] >= MAX_THIRST_VALUE-5)
	    {
	        imes_simple_single(playerid, 0x008800FF, "YOU_ARE_NOT_THIRSTY");
	        return;
	    }
		gThirst[playerid] = gThirst[playerid] + value; //утоляем жажду
		if(gThirst[playerid] > MAX_THIRST_VALUE)
		    gThirst[playerid] = MAX_THIRST_VALUE;
		set_character_thirst(playerid, gThirst[playerid]);
		create_composite_object(playerid, out_cell);

//		free_cell_from_owner(playerid, cell);
//		remove_object(playerid, cell);

	    return;
	}
	
	if(strcmp(name, "EMPTY_BOTTLE") == 0)
	{
		if(IsPlayerInWater(playerid))
		{
			create_composite_object(playerid, out_cell); //наполняем водой
		}
	    return;
	}

	if(strcmp(name, "BOTTLE_OF_WATER") == 0)
	{
	    if(gThirst[playerid] >= MAX_THIRST_VALUE-5)
	    {
	        imes_simple_single(playerid, 0x008800FF, "YOU_ARE_NOT_THIRSTY");
	        return;
	    }
		gThirst[playerid] = gThirst[playerid] + value; //утоляем жажду
		if(gThirst[playerid] > MAX_THIRST_VALUE)
		    gThirst[playerid] = MAX_THIRST_VALUE;
		set_character_thirst(playerid, gThirst[playerid]);

		disassemble_cell_object(playerid, cell);
	    return;
	}

	if(strcmp(name, "EMPTY_JERRYCAN") == 0)
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
		    vehicleid = GetPlayerVehicleID(playerid);
		    
			if(vehicleid > 0 && gVeh[vehicleid][3] > 0 && gVehicleDataShow[playerid] == vehicleid) //если персонаж тот, за кого себя выдаёт
			{
		        new def_value;
		        new out_data[64];

				get_thing_field("def_value", "FULL_JERRYCAN", out_data);
				def_value = strval(out_data);

			    if(gVeh[vehicleid][3] >= def_value) //если в баке достаточно на полную канистру
			    {
				    gVeh[vehicleid][3] = gVeh[vehicleid][3] - def_value; //сливаем из него
				    save_vehicle_state(playerid, vehicleid); //обновляем состояние авто
					create_composite_object(playerid, out_cell); //наполняем канистру из бака доверху
				}
				else
				{
					create_composite_object(playerid, out_cell); //наполняем канистру "из воздуха"
					set_character_cell_value(playerid, out_cell, gVeh[vehicleid][3]); //придаём необходимый объём бензину в канистре
				    gVeh[vehicleid][3] = 0; //сливаем из него
				    save_vehicle_state(playerid, vehicleid); //обновляем состояние авто
				}
				update_vehicle_sensors(playerid);

				if(gVeh[vehicleid][3] > 0)
				{
					//сообщаем игроку о том, что он сейчас сделал
					imes_simple_single(playerid, 0xFFFF44FF, "YOU_POURED_OUT_A_GAS");
				}
				else
				{
					//сообщаем игроку о том, что он сейчас сделал
					imes_simple_single(playerid, 0xFF5577FF, "YOU_POURED_OFF_A_GAS");
				}
			    return;
			}
		}
	
	    if(is_player_on_gas_station(playerid))
	    {
			create_composite_object(playerid, out_cell); //наполняем канистру из калонки
		    return;
		}
	    return;
	}
	
	if(strcmp(name, "FULL_JERRYCAN") == 0)
	{
	    if(IsPlayerInAnyVehicle(playerid))
	    {
	        vehicleid = GetPlayerVehicleID(playerid);

            if(gVeh[vehicleid][5] == 0)
            {
		        imes_simple_single(playerid, 0xFF5577FF, "CANT_DO_WITH_VEHICLE");
                return;
            }

            if(gVeh[vehicleid][3] >= gVeh[vehicleid][6])
            {
		        new def_value;
		        new out_data[64];

				get_thing_field("def_value", "FULL_JERRYCAN", out_data);
				def_value = strval(out_data);

				if(def_value > value)
				{
				    gVeh[vehicleid][3] = gVeh[vehicleid][3] - (def_value-value); //сливаем из него
				    save_vehicle_state(playerid, vehicleid); //обновляем состояние авто
					set_character_cell_value(playerid, cell, def_value); //придаём необходимый объём бензину в канистре
					update_vehicle_sensors(playerid);
					imes_simple_single(playerid, 0xFFFF44FF, "YOU_POURED_OUT_A_GAS");
				}
				else
				{
			        imes_simple_single(playerid, 0xFF5577FF, "THE_GAS_TANK_IS_FULL");
					//сообщаем игроку о том, что он сейчас сделал
				}
                return;
            }
	        
	        gVeh[vehicleid][3] = gVeh[vehicleid][3] + value; //заливаем бензин в бак
	        if(gVeh[vehicleid][3] > gVeh[vehicleid][6])
	        {
				set_character_cell_value(playerid, cell, gVeh[vehicleid][3] - gVeh[vehicleid][6]); //придаём необходимый объём бензину в канистре
	            gVeh[vehicleid][3] = gVeh[vehicleid][6];
		        imes_simple_single(playerid, 0xFF8800FF, "THE_GAS_TANK_IS_FULL");
			}
			else
			{
				disassemble_cell_object(playerid, cell);
			}
			update_vehicle_sensors(playerid);
	        save_vehicle_state(playerid, vehicleid);
	        return;
		}
		
	    if(is_player_on_gas_station(playerid))
	    {
	        new def_value;
	        new out_data[64];

			get_thing_field("def_value", "FULL_JERRYCAN", out_data);
			def_value = strval(out_data);

			set_character_cell_value(playerid, cell, def_value); //придаём необходимый объём бензину в канистре
		    return;
		}
	    return;
	}
	
	if(strcmp(name, "CAR_WHEEL") == 0)
	{
	    if(IsPlayerInAnyVehicle(playerid))
	    {
			new panels, doors, lights, tires;
			
	        vehicleid = GetPlayerVehicleID(playerid);

			GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
			
			if(tires == 0)
			{
		        imes_simple_single(playerid, 0x008800FF, "ALL_VEHICLE_TIRES_ARE_GOOD");
				return;
			}
			else
			{
				new tires_state;
				
				tires_state = 0x01;
				while(!(tires_state & 0x100))
				{
					if(tires & tires_state)
					{
						tires = tires ^ tires_state;
						break;
					}
					tires_state = tires_state << 1;
				}
			    
				UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
				save_vehicle_state(playerid, vehicleid);
			    free_cell_from_owner(playerid, cell);
				return;
            }
		}
	    return;
	}

	//временная функция для ремонта двигателя
	if(strcmp(name, "CAR_ENGINE") == 0)
	{
	    if(IsPlayerInAnyVehicle(playerid))
	    {
	        new def_value;
	        new out_data[64];

			get_thing_field("def_value", "CAR_ENGINE", out_data);
			def_value = strval(out_data);

	        vehicleid = GetPlayerVehicleID(playerid);

			if(gVeh[vehicleid][2] >= def_value)
			{
		        imes_simple_single(playerid, 0x008800FF, "VEHICLE_ENGINE_IS_GOOD");
				return;
			}
			else
			{
				gVeh[vehicleid][2] = def_value;
				save_vehicle_state(playerid, vehicleid);
			    free_cell_from_owner(playerid, cell);
				return;
            }
		}
	    return;
	}

	//временная функция для ремонта авто
	if(strcmp(name, "CAR_TOOLBOX") == 0)
	{
	    if(IsPlayerInAnyVehicle(playerid))
	    {
			new panels, doors, lights, tires;

	        vehicleid = GetPlayerVehicleID(playerid);

			GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

			if(panels == 0 && doors == 0)
			{
		        imes_simple_single(playerid, 0x008800FF, "VEHICLE_STATE_IS_GOOD");
				return;
			}
			else
			{
				RepairVehicle(vehicleid);
				UpdateVehicleDamageStatus(vehicleid, 0, 0, lights, tires);
				save_vehicle_state(playerid, vehicleid);
			    free_cell_from_owner(playerid, cell);
				return;
            }
		}
	    return;
	}

	if(strcmp(name, "THE_BANDAGE") == 0)
	{
	    if(gWound[playerid] == 0)
	    {
	        imes_simple_single(playerid, 0x008800FF, "YOU_HAVE_NO_WOUND");
	        return;
	    }
		gWound[playerid] = gWound[playerid] - value; //лечим рану
		if(gWound[playerid] < 0)
		    gWound[playerid] = 0;
		set_character_wound(playerid, gWound[playerid]);
		free_cell_from_owner(playerid, cell);
	    return;
	}

	if(strcmp(name, "PIECE_OF_PIZZA") == 0 ||
	   strcmp(name, "FULL_PIZZA") == 0 ||
	   strcmp(name, "BIG_FOOD") == 0 ||
	   strcmp(name, "HUMBURGER_FOOD") == 0)
	{
	    if(gHunger[playerid] >= MAX_HUNGER_VALUE-10)
	    {
	        imes_simple_single(playerid, 0xFF5577FF, "YOU_ARE_NOT_HUNGRY");
	        return;
	    }
		gHunger[playerid] = gHunger[playerid] + value; //утоляем голод
		if(gHunger[playerid] > MAX_HUNGER_VALUE)
		    gHunger[playerid] = MAX_HUNGER_VALUE;
		set_character_hunger(playerid, gHunger[playerid]);

		if(value < 500)
			gHealth[playerid] = gHealth[playerid] + value*7; //лечим персонажа
		else
			gHealth[playerid] = gHealth[playerid] + value*4; //лечим персонажа

		if(gHealth[playerid] > MAX_HEALTH_VALUE)
		    gHealth[playerid] = MAX_HEALTH_VALUE;
		set_character_health(playerid, gHealth[playerid]);
		free_cell_from_owner(playerid, cell);
	    return;
	}
	
	if(strcmp(name, "GPS_NAVIGATOR") == 0)
	{
	    hide_smokescreen(playerid);
		hide_smoke_map(playerid);
	    return;
	}

}

