#if defined _LOAD_OBJECTS_INC_
	#endinput
#endif

#define _LOAD_OBJECTS_INC_

#define LOAD_DEBUG

#include<a_samp>
#include<things>
#include<a_mysql>
#include<mapandreas>
#include<streamer>

#include"defines.inc"
#include"sensors.inc"
#include"global.inc"

forward open_database();
forward close_database();
forward player_login(playerid, password[]); //вызывается при входе игрока на сервер
forward create_player(playerid, password[]); //создаёт нового игрока
forward set_player_id(playerid); //задаёт gPlayersID игрока
forward update_player(playerid); //обновляет время и ip игрока
forward initialize_players(); //задаёт gPlayersID для всех онлайн-игроков
forward free_players(); //обнуляет все gPlayersID
forward load_objects();
forward set_new_objects_on_places();
forward destroy_objects();
forward set_objects_cells(playerid, Float:range); //заполнить ячейки меню объектов
forward set_vehicle_cells(vehicleid); //заполнить ячейки меню транспорта
forward is_any_object_in_range(Float:x, Float:y, Float:z, Float:range); //определить, лежит ли какой-нибудь объект в указанном радиусе от заданной точки
forward set_character_inventory_cell(playerid, cell, object_id); //положить объект в ячейку инвентаря
forward set_vehicle_inventory_cell(vehicleid, cell, object_id); //положить объект в ячейку инвентаря транспорта
forward drop_character_inventory_cell(playerid, cell, obj_cell); //выложить объект из инвентаря
forward take_vehicle_inventory_cell(playerid, vehicleid, cell, veh_cell); //переложить объект из транспорта в инвентарь персонажа
forward give_vehicle_inventory_cell(playerid, vehicleid, cell, veh_cell); //переложить объект из инвентаря персонажа в транспорт
forward get_objects_properties(playerid, inv_isrot[6][3], Float:inv_deg[6][3], Float:inv_zoom[6], Float:range); //получить свойства объектов для меню объектов
forward get_vehicle_objects_properties(vehicleid, inv_isrot[MAX_INVENTORY_ON_VEHICLE][3], Float:inv_deg[MAX_INVENTORY_ON_VEHICLE][3], Float:inv_zoom[MAX_INVENTORY_ON_VEHICLE]); //получить свойства объектов для меню транспорта
forward move_character_inventory_cell(playerid, cell1, cell2); //переложить объект в инвентаре на пустое место
forward move_vehicle_inventory_cell(vehicleid, cell1, cell2); //переложить объект в инвентаре на пустое место
forward get_inventory_properties_cell(playerid, cell, inv_isrot[3], Float:inv_deg[3], &Float:inv_zoom, &is_auto); //получить содержимое ячейки инвентаря
forward get_inventory_properties(playerid, inv_isrot[6][3], Float:inv_deg[6][3], Float:inv_zoom[6], is_auto[6]); //получить содержимое всего инвентаря персонажа
forward get_vehicle_properties_cell(vehicleid, cell, inv_isrot[3], Float:inv_deg[3], &Float:inv_zoom); //получить содержимое ячейки инвентаря
forward get_object_data(playerid, cell, area, name[128], &value);
forward give_character_weapon(playerid, cell);
forward save_character_ammo(playerid);
forward save_vehicle_state(playerid, vehicleid);
forward save_vehicle_position(vehicleid);
forward create_new_vehicle(object_id, Float:x, Float:y, Float:z, Float:rotz, inworld_id, def_value);
forward create_old_vehicle(object_id, Float:x, Float:y, Float:z, Float:rotz, inworld_id);
forward drop_vehicle_from_dot(playerid, vehicleid); //удалить авто из точки появления, если оно не находится в этой точке

forward get_unused_object(thing_id); //получить из базы свободный (unused=1) объект заданного типа, либо создать новый такой
forward set_object_owner(th_owner_id, object_id); //присвоить объект новому объекту-владельцу
forward create_resident_object(object_id, old_thing_id, new_thing_id); //создать резидентный объект (для объектов, помеченных как '!' в файле описания вещей)
forward create_composite_object(playerid, &cell); //создать новую составную вещь
forward disassemble_resident_object(object_id, prev_id); //разобрать резидентный объект (по-сути лишь вернуть прежний thing_id)
forward disassemble_cell_object(playerid, cell); //разобрать составной объект из ячейки
forward disassemble_inventory_object(object_id); //разобрать объект с автопоиском ячейки в инвентаре
forward pick_object_from_owner(obj_id); //выковырять объект из композитного объекта
forward clear_all_owners(object_id); //освободить объект от всех владельцев (используется при профилактике БД)
forward free_cell_from_owner(playerid, cell); //освободить объект, находящийся в ячейке инвентаря и разместить на свободном месте
forward full_free_object_from_owner(object_id); //полное освобождение объекта с освобождением всех вложенных объектов
forward free_object_from_owner(object_id); //освободить объект от владельца и разместить на свободном месте (используется для конечных вещей, таких как пули, еда и т.д.)
forward put_object_on_free_place(object_id); //помещает объект на найденное подходящее свободное место

forward get_character_value(playerid, row_name[], table_name[], result[64]); //получить значение поля таблицы для персонажа
forward get_vehicle_value(vehicleid, row_name[], table_name[], result[64]); //получить значение поля таблицы для транспорта
forward get_thing_field(field_name[], thing[], out_data[64]); //получить поле таблицы 'things' по имени вещи
forward set_character_cell_value(playerid, cell, value); //установить 'value' для объекта, лежащего в ячейке инвентаря персонажа

forward get_character_state(playerid, &health, &hunger, &thirst, &wound);
forward set_character_state(playerid, health, hunger, thirst, wound);
forward get_character_health(playerid);
forward set_character_health(playerid, health);
forward get_character_hunger(playerid);
forward set_character_hunger(playerid, hunger);
forward get_character_thirst(playerid);
forward set_character_thirst(playerid, thirst);
forward get_character_wound(playerid);
forward set_character_wound(playerid, wound);
forward get_character_scores(playerid);
forward set_character_scores(playerid, scores);

forward get_character_killer_name(playerid, name[64]);
forward set_character_c_killer(playerid);
forward set_character_killer(playerid);
forward upscore_character(db_playerid);

forward save_player_position(playerid);
forward load_player_position(playerid);
forward save_state_and_position(playerid); //посылает один единственный запрос в БД

forward load_player_inventory(playerid, inv[6]);
forward load_vehicle_inventory(vehicleid, inv[MAX_INVENTORY_ON_VEHICLE], obj[MAX_INVENTORY_ON_VEHICLE]);
forward get_spawn_place(playerid);
forward kill_character(playerid); //собственно, обезглавить персонаж

#if defined LOAD_DEBUG
forward update_player_position(playerid);
forward add_spawn_place(playerid);
forward add_dot_place(playerid, type);
forward upd_dot_place(type);
forward add_objects_to_gm(type, dup);
forward add_car_place(playerid);
forward go_to_thing_place(playerid, thing_type[]);
#endif

public open_database()
{
	gMysql = mysql_connect(HOST, USER, DBNAME, PASSWD);
	mysql_query(gMysql, "SET NAMES 'cp1251'");
	mysql_query(gMysql, "SET CHARACTER SET 'cp1251'");
	mysql_query(gMysql, "SET @@character_set_connection='cp1251'");
//	mysql_query(gMysql, "SET @@character_set_result='cp1251'");
	mysql_query(gMysql, "SET @@character_set_client='cp1251'");
}

public close_database()
{
	mysql_close(gMysql);
}

//вызывается при входе игрока на сервер
/*
public player_login(playerid, password[])
{
	new name[48];
	new rows, fields;
	new request[128];
	new db_pass[64];
	new Cache:result;

    GetPlayerName(playerid, name, sizeof(name));
    
    mysql_format(gMysql, request, sizeof(request), "SELECT name,IFNULL(passwd, '') as passwd,reg_ip,last_ip,reg_date,last_date FROM players where name=\"%s\"", name);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
		create_player(playerid, password);
	else
	{
	    strdel(db_pass, 0, sizeof(db_pass));
		cache_get_field_content(0, "passwd", db_pass, gMysql);
		if(strcmp(password, db_pass, false) == 0)
		{
       	    update_player(playerid);
			set_player_id(playerid);
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		}
		else
		{
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		    return -1;
		}
	}
	
	return 1;
}
*/
public player_login(playerid, password[])
{
	new name[48];
	new rows, fields;
	new request[512];
	new db_pass[64], input_pass[64];
	new Cache:result;

    GetPlayerName(playerid, name, sizeof(name));

    mysql_format(gMysql, request, sizeof(request), "SELECT name,IFNULL(passwd, md5(\"password\")) as passwd,reg_ip,last_ip,reg_date,last_date,md5(\"%s\") as input_passwd FROM players where name=\"%s\"", password, name);
//    printf("%s\n", request);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
		create_player(playerid, password);
	else
	{
	    strdel(db_pass, 0, sizeof(db_pass));
	    strdel(input_pass, 0, sizeof(input_pass));
		cache_get_field_content(0, "passwd", db_pass, gMysql);
		cache_get_field_content(0, "input_passwd", input_pass, gMysql);
		if(strcmp(input_pass, db_pass, false) == 0)
		{
       	    update_player(playerid);
			set_player_id(playerid);
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		}
		else
		{
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		    return -1;
		}
	}

	return 1;
}


//создаёт нового игрока
public create_player(playerid, password[])
{
	new name[48];
	new ip[24];
	new request[480];

	GetPlayerName(playerid, name, sizeof(name));
	GetPlayerIp(playerid, ip, sizeof(ip));

    mysql_format(gMysql, request, sizeof(request), "INSERT INTO players(name,passwd,reg_ip,last_ip,reg_ip_int,last_ip_int,reg_date,last_date) VALUES(\"%s\", md5(\"%s\"), \"%s\", \"%s\", %d, %d, now(), now())", name, password, ip, ip, 0, 0);
	mysql_query(gMysql, request);
	
	set_player_id(playerid);
	
    mysql_format(gMysql, request, sizeof(request), "INSERT INTO characters(player_id) VALUES(%d)", gPlayersID[playerid]);
	mysql_query(gMysql, request);

    mysql_format(gMysql, request, sizeof(request), "INSERT INTO inventory(player_id) VALUES(%d)", gPlayersID[playerid]);
	mysql_query(gMysql, request);
	
	get_spawn_place(playerid);
}

//задаёт gPlayersID игрока
public set_player_id(playerid)
{
	new name[48];
	new request[128];
	new rows, fields;
	new Cache:result;

	gPlayersID[playerid] = 0;

//	if(!IsPlayerConnected(playerid))
//		return;
		
    GetPlayerName(playerid, name, sizeof(name));

    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM players WHERE name=\"%s\" LIMIT 1", name);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
		gPlayersID[playerid] = cache_get_field_content_int(0, "id", gMysql);

	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//обновляет время и ip игрока
public update_player(playerid)
{
	new ip[24];
	new request[256];
	new name[48];

    GetPlayerName(playerid, name, sizeof(name));
	GetPlayerIp(playerid, ip, sizeof(ip));

    mysql_format(gMysql, request, sizeof(request), "update players set last_ip=\"%s\",last_ip_int=%d,last_date=now() where name=\"%s\"", ip, 0, name);
	mysql_query(gMysql, request);
}

//задаёт gPlayersID для всех онлайн-игроков
public initialize_players()
{
	new i, num;
	new str[8];
	new name[48];
	new request[128];
	new Cache:result;

	for(i = 0; i < MAX_PLAYERS; ++i)
	{
	    gPlayersID[i] = 0;
		if(IsPlayerConnected(i))
		{
			GetPlayerName(i, name, sizeof(name));

		    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM players WHERE name=\"%s\"", name);
			result = mysql_query(gMysql, request);
			mysql_store_result();

			num = mysql_num_rows();
			if(num == 0)
			{
				mysql_free_result();
				continue;
			}
			else
			{
			    if(mysql_retrieve_row())
			    {
					strdel(str, 0, sizeof(str));
			        mysql_fetch_field_row(str,"id",gMysql);
					gPlayersID[i] = strval(str);
			    }
			}
			mysql_free_result();
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		}
	}
}

//обнуляет все gPlayersID
public free_players()
{
	//сюда же надо будет добавить сохранение позиций и инвентаря
	//...
	new i;
	
	for(i = 0; i < MAX_PLAYERS; ++i)
	{
		if(IsPlayerConnected(i))
			save_player_position(i);

		gPlayersID[i] = 0;
	}
}

//начальная загрузка всех объектов из БД в игру
public load_objects()
{
	new request[1024];
	new Float:x, Float:y, Float:z, Float:rotx, Float:roty, Float:rotz, Float:height;
	new Float:add_rotx, Float:add_roty, Float:add_rotz, Float:Z_coord, Float:Alpha, Float:Beta;
	new rows, fields, rows1, fields1, i, inworld_id, id, obj_id, type_id, dot_id, def_value, pl_owner_id, is_vehicle;
	new rotatable, res, main_thing_id, prev_id, ret;
	new Cache:dots, Cache:result;
	
	//желательно сделать проверку состава композитных объектов (по-крайней мере устанавливать unused в 1 для тех, у которых нету ни одного привязанного объекта)
	//композитные - это те, у которых поле composition (в плагине things) не NULL. разумеется, требуется функция, дающая список таких ВЕЩЕЙ, а уже по id этих вещей и можно искать объекты
	//...
	
	//освобождаем пустые обоймы
	mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.pl_owner_id,a.dot_id FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE b.is_consumble=1 AND a.value=0 AND a.unused=0");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
		    pl_owner_id = 0;
		    dot_id = 0;
			id = cache_get_field_content_int(i, "id", gMysql);
			pl_owner_id = cache_get_field_content_int(i, "pl_owner_id", gMysql);
			dot_id = cache_get_field_content_int(i, "dot_id", gMysql);

			//если объект где-то лежит - удаляем его оттуда
			if(dot_id > 0)
			{
				mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL,last_time=now() WHERE object_id=%d", id);
				mysql_query(gMysql, request);
			}

			//если есть владелец, удаляем у него пустую обойму
			if(pl_owner_id > 0)
			{
				clear_all_owners(id);
			}
			//обНУЛЛяем обойму
			mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,obj_id=NULL,dot_id=NULL,pl_owner_id=NULL,th_owner_id=NULL,is_dropped=NULL WHERE id=%d", id);
			mysql_query(gMysql, request);
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//ищем потерянные объекты, объекты-владельцы которых не используются
	mysql_format(gMysql, request, sizeof(request), "SELECT a.id FROM objects a INNER JOIN objects b ON (a.th_owner_id=b.id AND b.unused=1)");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,obj_id=NULL,dot_id=NULL,pl_owner_id=NULL,th_owner_id=NULL,is_dropped=NULL WHERE id=%d", id);
			mysql_query(gMysql, request);
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//предварительная коррекция базы
	//возвращаем в игру потерянные обекты (те, у которых: хотябы одна координата NULL и нету владельцев и нету точки появления и указан непустой is_dropped (и unused, разумеется, 0))
	//эта коррекция нужна для того, чтобы сохранить id прежнего владельца для подобранных вещей. если же is_dropped устанавливать в NULL при подборе вещи, то потери вещей можно избежать
	//вообще же объект теряется при установке всех владельцев в NULL без указания координат появления объекта в игровом мире
    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET is_dropped=NULL WHERE (placex IS NULL OR placey IS NULL OR placez IS NULL) AND pl_owner_id IS NULL AND th_owner_id IS NULL AND dot_id IS NULL AND is_dropped IS NOT NULL AND unused=0");
	mysql_query(gMysql, request);
	
	//корректируем точки появления объектов (обНУЛЛяем все точки, объекты которых не содержат точек появления)
	mysql_format(gMysql, request, sizeof(request), "SELECT a.id FROM object_dot a INNER JOIN objects b ON a.object_id=b.id WHERE b.dot_id IS NULL");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL WHERE id=%d", id);
			mysql_query(gMysql, request);
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//начинаем расстановку
	//сперва загружаем и расставляем из objects всё, где dot_id IS NOT NULL (?)
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,c.placex,c.placey,c.placez,a.add_rotx,a.add_roty,a.add_rotz,b.rotatable,c.id as dot_id,b.inworld_id,b.posx,b.posy,b.posz,b.height,b.def_value,b.is_vehicle FROM objects a INNER JOIN (things b, object_dot c) ON (a.dot_id=c.id AND a.thing_id=b.id) WHERE a.dot_id IS NOT NULL AND a.unused=0");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
			x = cache_get_field_content_float(i, "placex", gMysql);
			y = cache_get_field_content_float(i, "placey", gMysql);
			z = cache_get_field_content_float(i, "placez", gMysql);
			rotx = cache_get_field_content_float(i, "posx", gMysql);
			roty = cache_get_field_content_float(i, "posy", gMysql);
			rotz = cache_get_field_content_float(i, "posz", gMysql);
			add_rotx = cache_get_field_content_float(i, "add_rotx", gMysql);
			add_roty = cache_get_field_content_float(i, "add_roty", gMysql);
			add_rotz = cache_get_field_content_float(i, "add_rotz", gMysql);
			rotatable = cache_get_field_content_int(i, "rotatable", gMysql);
			inworld_id = cache_get_field_content_int(i, "inworld_id", gMysql);
			height = cache_get_field_content_float(i, "height", gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			dot_id = cache_get_field_content_int(i, "dot_id", gMysql);
			def_value = cache_get_field_content_int(i, "def_value", gMysql);
			is_vehicle = cache_get_field_content_int(i, "is_vehicle", gMysql);

			if(is_vehicle == 1)
			{
			    create_new_vehicle(id, x, y, z, rotz, inworld_id, def_value);
			}
			else
			{
			    if(rotatable && add_rotx == 0 && add_roty == 0)
			    {
			        Alpha = rotz;
			        //получаем углы поворота
					ret = MapAndreas_Valid_Z_Coordinate(x,y,z,Z_coord,Alpha,Beta);
					if(ret == 0)
					{
					    add_rotx = Beta;
						add_roty = Alpha;
					}
					else
					{
					    add_rotx = 0.001;
					    add_roty = 0.001;
					}
			    }
			    if(rotatable && ret == 0)
					obj_id = CreateObject(inworld_id, x, y, z+height, rotx+add_rotx, roty+add_roty, rotz+add_rotz, OBJECT_DISTANCE);
				else
					obj_id = CreateObject(inworld_id, x, y, z+height, rotx, roty, rotz, OBJECT_DISTANCE);
				//переделать все vector под VectorSize()
				//...
			    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,obj_id=%d,pl_owner_id=NULL,th_owner_id=NULL,value=%d WHERE id=%d", VectorSize(x,y,z), x, y, z, add_rotx, add_roty, obj_id, def_value, id);
				mysql_query(gMysql, request);
			}
		    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=%d WHERE id=%d", id, dot_id);
			mysql_query(gMysql, request);
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//расставляем перемещённые персонажами объекты
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.value,a.placex,a.placey,a.placez,a.add_rotx,a.add_roty,a.add_rotz,b.is_vehicle,b.rotatable,b.inworld_id,b.posx,b.posy,b.posz,b.height FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.dot_id IS NULL and a.pl_owner_id IS NULL and a.th_owner_id IS NULL and a.is_dropped IS NOT NULL AND a.unused=0");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
			x = cache_get_field_content_float(i, "placex", gMysql);
			y = cache_get_field_content_float(i, "placey", gMysql);
			z = cache_get_field_content_float(i, "placez", gMysql);
			rotx = cache_get_field_content_float(i, "posx", gMysql);
			roty = cache_get_field_content_float(i, "posy", gMysql);
			rotz = cache_get_field_content_float(i, "posz", gMysql);
			add_rotx = cache_get_field_content_float(i, "add_rotx", gMysql);
			add_roty = cache_get_field_content_float(i, "add_roty", gMysql);
			add_rotz = cache_get_field_content_float(i, "add_rotz", gMysql);
			inworld_id = cache_get_field_content_int(i, "inworld_id", gMysql);
			height = cache_get_field_content_float(i, "height", gMysql);
			rotatable = cache_get_field_content_int(i, "rotatable", gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			is_vehicle = cache_get_field_content_int(i, "is_vehicle", gMysql);

			if(is_vehicle == 1)
			{
			    create_old_vehicle(id, x, y, z, add_rotz, inworld_id);
			}
			else
			{
				//сохраняем для объектов координаты персонажа и разницу высот
				if(rotatable == 1)
					obj_id = CreateObject(inworld_id, x, y, z+height, rotx+add_rotx, roty+add_roty, rotz+add_rotz, OBJECT_DISTANCE);
				else
					obj_id = CreateObject(inworld_id, x, y, z+height, rotx, roty, rotz, OBJECT_DISTANCE);
			    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET obj_id=%d,dot_id=NULL,pl_owner_id=NULL,th_owner_id=NULL WHERE id=%d", obj_id, id);
				mysql_query(gMysql, request);
			}
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//затем расставляем свободные/новые объекты на свободные места
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,IFNULL(a.prev_id, 0) as prev_id,b.is_vehicle,b.inworld_id,b.posx,b.posy,b.posz,b.height,b.type_id,b.rotatable,b.def_value FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.dot_id IS NULL AND a.pl_owner_id IS NULL AND a.th_owner_id IS NULL AND a.is_dropped IS NULL AND a.unused=0 ORDER BY RAND()");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
			rotx = cache_get_field_content_float(i, "posx", gMysql);
			roty = cache_get_field_content_float(i, "posy", gMysql);
			rotz = cache_get_field_content_float(i, "posz", gMysql);
			rotatable = cache_get_field_content_int(i, "rotatable", gMysql);
			inworld_id = cache_get_field_content_int(i, "inworld_id", gMysql);
			height = cache_get_field_content_float(i, "height", gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			type_id = cache_get_field_content_int(i, "type_id", gMysql);
			def_value = cache_get_field_content_int(i, "def_value", gMysql);
			is_vehicle = cache_get_field_content_int(i, "is_vehicle", gMysql);
			prev_id = cache_get_field_content_int(i, "prev_id", gMysql);

			//получаем свободное место
		    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.placex,a.placey,a.placez FROM object_dot a INNER JOIN dot_type b ON a.id=b.dot_id WHERE a.object_id IS NULL and b.type_id=%d ORDER BY last_time,RAND() LIMIT 1", type_id);
			mysql_query(gMysql, request);
			cache_get_data(rows1, fields1, gMysql);
			if(rows1 == 0)
			{
			    continue;
			}
			else
			{
				x = cache_get_field_content_float(0, "placex", gMysql);
				y = cache_get_field_content_float(0, "placey", gMysql);
				z = cache_get_field_content_float(0, "placez", gMysql);
				dot_id = cache_get_field_content_int(0, "id", gMysql);

				//расставляем
				if(is_vehicle == 1)
				{
					//не забыть: освобождая вещи от владельцев, очищать и список инвентаря транспортных средств
					//...
					create_new_vehicle(id, x, y, z, rotz, inworld_id, def_value);
				    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET dot_id=%d WHERE id=%d", dot_id, id);
					mysql_query(gMysql, request);
				}
				else
				{
				    add_rotx = 0;
				    add_roty = 0;
				    if(rotatable)
				    {
				        Alpha = rotz;
				        //получаем углы поворота
						ret = MapAndreas_Valid_Z_Coordinate(x,y,z,Z_coord,Alpha,Beta);
						if(ret == 0)
						{
						    add_rotx = Beta;
							add_roty = Alpha;
						}
						else
						{
						    add_rotx = 0.001;
						    add_roty = 0.001;
						}
				    }
				    if(rotatable && ret == 0)
				    {
						obj_id = CreateObject(inworld_id, x, y, z+height, rotx+add_rotx, roty+add_roty, rotz, OBJECT_DISTANCE);
					}
					else
					{
						obj_id = CreateObject(inworld_id, x, y, z+height, rotx, roty, rotz, OBJECT_DISTANCE);
					}
					res = composit_thing(request, main_thing_id);
					if(res > 0 && main_thing_id > 0 && prev_id == 0) //для устранения 'бесконечных' используемых вещей
					    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET prev_id=%d,vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,obj_id=%d,dot_id=%d,pl_owner_id=NULL,th_owner_id=NULL,value=%d WHERE id=%d", main_thing_id, VectorSize(x,y,z), x, y, z, add_rotx, add_roty, obj_id, dot_id, def_value, id);
					else
					    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,obj_id=%d,dot_id=%d,pl_owner_id=NULL,th_owner_id=NULL,value=%d WHERE id=%d", VectorSize(x,y,z), x, y, z, add_rotx, add_roty, obj_id, dot_id, def_value, id);
					mysql_query(gMysql, request);
				}
				//время (last_time) здесь не должно редактироваться! (?)
				//mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=%d WHERE id=%d", id, dot_id);
			    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=%d,last_time=now() WHERE id=%d", id, dot_id);
				mysql_query(gMysql, request);
			}
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//самое главное обнулять/заполнять поля obj_id,pl_owner_id,th_owner_id,dot_id,is_dropped,unused при операциях с объектами
}

public set_new_objects_on_places()
{
	new request[1024];
	new Float:x, Float:y, Float:z, Float:rotx, Float:roty, Float:rotz, Float:height;
	new Float:add_rotx, Float:add_roty, Float:Z_coord, Float:Alpha, Float:Beta;
	new rows, fields, rows1, fields1, i, inworld_id, id, obj_id, type_id, dot_id, def_value, is_vehicle;
	new rotatable, res, main_thing_id, prev_id, ret;
	new Cache:dots, Cache:result;

	//затем расставляем свободные/новые объекты на свободные места
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,IFNULL(a.prev_id, 0) as prev_id,b.is_vehicle,b.inworld_id,b.posx,b.posy,b.posz,b.height,b.type_id,b.rotatable,b.def_value FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.dot_id IS NULL AND a.pl_owner_id IS NULL AND a.th_owner_id IS NULL AND a.is_dropped IS NULL AND a.unused=0 ORDER BY RAND()");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		dots = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(dots, gMysql);
			rotx = cache_get_field_content_float(i, "posx", gMysql);
			roty = cache_get_field_content_float(i, "posy", gMysql);
			rotz = cache_get_field_content_float(i, "posz", gMysql);
			rotatable = cache_get_field_content_int(i, "rotatable", gMysql);
			inworld_id = cache_get_field_content_int(i, "inworld_id", gMysql);
			height = cache_get_field_content_float(i, "height", gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			type_id = cache_get_field_content_int(i, "type_id", gMysql);
			def_value = cache_get_field_content_int(i, "def_value", gMysql);
			is_vehicle = cache_get_field_content_int(i, "is_vehicle", gMysql);
			prev_id = cache_get_field_content_int(i, "prev_id", gMysql);

			//получаем свободное место
		    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.placex,a.placey,a.placez FROM object_dot a INNER JOIN dot_type b ON a.id=b.dot_id WHERE a.object_id IS NULL and b.type_id=%d ORDER BY last_time,RAND() LIMIT 1", type_id);
			mysql_query(gMysql, request);
			cache_get_data(rows1, fields1, gMysql);
			if(rows1 == 0)
			{
			    continue;
			}
			else
			{
				x = cache_get_field_content_float(0, "placex", gMysql);
				y = cache_get_field_content_float(0, "placey", gMysql);
				z = cache_get_field_content_float(0, "placez", gMysql);
				dot_id = cache_get_field_content_int(0, "id", gMysql);

				//расставляем
				if(is_vehicle == 1)
				{
					//не забыть: освобождая вещи от владельцев, очищать и список инвентаря транспортных средств
					//...
					create_new_vehicle(id, x, y, z, rotz, inworld_id, def_value);
				    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET dot_id=%d WHERE id=%d", dot_id, id);
					mysql_query(gMysql, request);
				}
				else
				{
				    add_rotx = 0;
				    add_roty = 0;
				    if(rotatable)
				    {
				        Alpha = rotz;
				        //получаем углы поворота
						ret = MapAndreas_Valid_Z_Coordinate(x,y,z,Z_coord,Alpha,Beta);
						if(ret == 0)
						{
						    add_rotx = Beta;
							add_roty = Alpha;
						}
						else
						{
						    add_rotx = 0.001;
						    add_roty = 0.001;
						}
				    }
				    if(rotatable && ret == 0)
				    {
						obj_id = CreateObject(inworld_id, x, y, z+height, rotx+add_rotx, roty+add_roty, rotz, OBJECT_DISTANCE);
					}
					else
					{
						obj_id = CreateObject(inworld_id, x, y, z+height, rotx, roty, rotz, OBJECT_DISTANCE);
					}
					res = composit_thing(request, main_thing_id);
					if(res > 0 && main_thing_id > 0 && prev_id == 0) //для устранения 'бесконечных' используемых вещей
					    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET prev_id=%d,vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,obj_id=%d,dot_id=%d,pl_owner_id=NULL,th_owner_id=NULL,value=%d WHERE id=%d", main_thing_id, VectorSize(x,y,z), x, y, z, add_rotx, add_roty, obj_id, dot_id, def_value, id);
					else
					    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,obj_id=%d,dot_id=%d,pl_owner_id=NULL,th_owner_id=NULL,value=%d WHERE id=%d", VectorSize(x,y,z), x, y, z, add_rotx, add_roty, obj_id, dot_id, def_value, id);
					mysql_query(gMysql, request);
				}
				//время (last_time) здесь не должно редактироваться! (?)
				//mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=%d WHERE id=%d", id, dot_id);
			    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=%d,last_time=now() WHERE id=%d", id, dot_id);
				mysql_query(gMysql, request);
			}
		}
		if(cache_is_valid(dots, gMysql))
			cache_delete(dots, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//удаление всех объектов из игры
public destroy_objects()
{
	new request[256];
	new obj_id, id, rows, fields, i, is_vehicle;
	new Cache:objs;
	
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.obj_id,b.is_vehicle FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE obj_id IS NOT NULL");
	mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		objs = cache_save(gMysql);
		for(i = 0; i < rows; ++i)
		{
		    cache_set_active(objs, gMysql);
			obj_id = cache_get_field_content_int(i, "obj_id", gMysql);
			id = cache_get_field_content_int(i, "id", gMysql);
			is_vehicle = cache_get_field_content_int(i, "is_vehicle", gMysql);

			if(is_vehicle == 1)
			{
			    //а вот тута сохраняем количество оставшегося топлива
			    if(obj_id <= MAX_VEHICLES && gVeh[obj_id][4] == 1) //если двигатель не заглушен
				{
					save_vehicle_state(INVALID_PLAYER_ID, obj_id);
				}
			}
			else
			{
				DestroyObject(obj_id);
			}
		    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET obj_id=NULL,pl_owner_id=NULL,th_owner_id=NULL WHERE id=%d", id);
			mysql_query(gMysql, request);
		}
		if(cache_is_valid(objs, gMysql))
			cache_delete(objs, gMysql);
	}
	
	//удаляем все модели авто
	for(i = 0; i < MAX_VEHICLES; ++i)
	{
//	    if(gVeh[i][0] > 0)
			DestroyVehicle(i);
	}
}

//заполнить ячейки меню объектов
public set_objects_cells(playerid, Float:range)
{
	new request[256];
	new rows, fields;
	new i, j;
	new Float:x, Float:y, Float:z;
	new Float:placex, Float:placey, Float:placez;
	new Cache:result;
	new Float:vector, Float:distance;
	
	GetPlayerPos(playerid, x, y, z);

	vector = VectorSize(x, y, z);
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,b.invent_id,a.placex,a.placey,a.placez FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.vector IS NOT NULL AND (a.vector-%f)<%f AND (a.vector-%f)>-%f LIMIT 100", vector, range, vector, range);

	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);

	for(i = 0; i < 6; ++i)
	{
	    gObj[playerid][i] = -1;
	    gObjThing[playerid][i] = -1;
	}

	if(rows == 0)
	{
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	    return;
	}
	
	for(i = 0, j = 0; i < rows; ++i)
	{
		placex = cache_get_field_content_float(i, "placex", gMysql);
		placey = cache_get_field_content_float(i, "placey", gMysql);
		placez = cache_get_field_content_float(i, "placez", gMysql);

//		distance = floatsqroot( floatadd( floatadd( floatmul(floatsub(placex,x),floatsub(placex,x)), floatmul(floatsub(placey,y),floatsub(placey,y)) ), floatmul(floatsub(placez,z),floatsub(placez,z)) ) );
		distance = VectorSize(placex-x, placey-y, placez-z);

	    if(distance < range)
		{
			gObj[playerid][j] = cache_get_field_content_int(i, "invent_id", gMysql);
			gObjThing[playerid][j] = cache_get_field_content_int(i, "id", gMysql);
			if(j >= 5)
			    break;
			j++;
		}
	}
	
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//заполнить ячейки меню транспорта
public set_vehicle_cells(vehicleid)
{
	new i, inv[MAX_INVENTORY_ON_VEHICLE], obj[MAX_INVENTORY_ON_VEHICLE];
	
	load_vehicle_inventory(vehicleid, inv, obj);
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
		gVehObj[vehicleid][i] = inv[i];
		gVehMod[vehicleid][i] = obj[i];
	}
}

//определить, лежит ли какой-нибудь объект в указанном радиусе от заданной точки
public is_any_object_in_range(Float:x, Float:y, Float:z, Float:range)
{
	new request[256];
	new rows, fields;
	new i;
	new Float:placex, Float:placey, Float:placez;
	new Cache:result;
	new Float:vector, Float:distance;
	new ret_value;

	ret_value = 0;
	vector = VectorSize(x, y, z);
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,b.invent_id,a.placex,a.placey,a.placez FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.vector IS NOT NULL AND (a.vector-%f)<%f AND (a.vector-%f)>-%f LIMIT 80", vector, range, vector, range);

	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);

	if(rows == 0)
	{
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	    return ret_value;
	}
	
	for(i = 0; i < rows; ++i)
	{
		placex = cache_get_field_content_float(i, "placex", gMysql);
		placey = cache_get_field_content_float(i, "placey", gMysql);
		placez = cache_get_field_content_float(i, "placez", gMysql);

		distance = VectorSize(placex-x, placey-y, placez-z);

	    if(distance < range)
		{
			ret_value = 1;
			break;
		}
	}

	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	return ret_value;
}

//положить объект в ячейку инвентаря
//этой функцией можно освобождать ячейку (передать object_id=-1)
public set_character_inventory_cell(playerid, cell, object_id)
{
	new request[256];
	new rows, fields;
	new obj_id;
	new Cache:result;
//	new row_name[16];
//	new result[64];
//	new res;
/*
	//получаем прежний объект из ячейки
	strdel(row_name, 0, sizeof(row_name));
	strdel(result, 0, sizeof(result));
	format(row_name, sizeof(row_name), "inv%d", cell+1);
	get_character_value(playerid, row_name, "inventory", result);

	res = strval(result);
	if(res == object_id)
	    return;

	//если ячейка не пустая - выкладываем прежний объект из инвентаря
	if(res != -1)
	{
		//удаляем вледельца для объекта, обнуляем поле игрового объекта, точки появления объекта и флаг неиспользования объекта (объект "виснет" в базе до следующего перезапуска сервера)
		//это всё-же лучше, чем если объект окажется в неявном владении персонажем
		//позже лучше заменить эту функцию на обычное выкладывание объекта из инвентаря в игровой мир
		//...
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET placex=NULL,placey=NULL,placez=NULL,pl_owner_id=NULL,th_owner_id=NULL,dot_id=NULL,unused=0 WHERE id=%d", object_id);
		mysql_query(gMysql, request);
	}
*/
	if(object_id == -1)
	{
		//освобождаем ячейку
	    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET inv%d=-1 WHERE player_id=%d", cell+1, gPlayersID[playerid]);
		mysql_query(gMysql, request);

	    gInv[playerid][cell] = -1;
	    gInvThing[playerid][cell] = -1;
	    return -1;
	}

	//начальная инициализация модели объекта
	obj_id = INVALID_OBJECT_ID;
	//невозможно при помощи этой функции забрать вещь из чужого инвентаря!
	mysql_format(gMysql, request, sizeof(request), "SELECT c.id,c.invent_id,IFNULL(b.obj_id,0) as obj_id FROM objects b INNER JOIN things c ON b.thing_id=c.id WHERE b.id=%d AND b.pl_owner_id IS NULL LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
		//освобождаем ячейку
	    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET inv%d=-1 WHERE player_id=%d", cell+1, gPlayersID[playerid]);
		mysql_query(gMysql, request);

	    gInv[playerid][cell] = -1;
	    gInvThing[playerid][cell] = -1;
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	    return -1;
	}
	else
	{
		obj_id = cache_get_field_content_int(0, "obj_id", gMysql);
		if(obj_id == 0)
		    obj_id = INVALID_OBJECT_ID;
	}
	gInv[playerid][cell] = cache_get_field_content_int(0, "invent_id", gMysql);
	gInvThing[playerid][cell] = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	if(obj_id != INVALID_OBJECT_ID && IsValidObject(obj_id))
	    DestroyObject(obj_id);

	//занимаем ячейку
    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET inv%d=%d WHERE player_id=%d", cell+1, object_id, gPlayersID[playerid]);
	mysql_query(gMysql, request);

	//оптимизация!
	//объединить раздельные UPDATE-ы в один
	//...

	//указываем вледельца для объекта и обнуляем поле игрового объекта, точки появления объекта и флаг неиспользования объекта
	//если объект до этого принадлежал другому персонажу, сохраняем id прежнего владельца
    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=%d,th_owner_id=NULL,obj_id=NULL,dot_id=NULL,unused=0 WHERE id=%d", gPlayersID[playerid], object_id);
	mysql_query(gMysql, request);

	//обНУЛЛяем объект для точки появления и обновляем время обновления точки
    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL,last_time=now() WHERE object_id=%d", object_id);
	mysql_query(gMysql, request);
	
	return 1;
}

//положить объект в ячейку инвентаря транспорта
//этой функцией можно освобождать ячейку (передать object_id=-1)
public set_vehicle_inventory_cell(vehicleid, cell, object_id)
{
	new request[256];
	new rows, fields;
	new obj_id;
	new Cache:result, Cache:res;

	//занимаем ячейку
    mysql_format(gMysql, request, sizeof(request), "UPDATE veh_invent SET inv%d=%d WHERE object_id=%d", cell+1, object_id, gVeh[vehicleid][0]);
	mysql_query(gMysql, request);

	if(object_id == -1)
	{
	    gVehObj[vehicleid][cell] = -1;
	    gVehMod[vehicleid][cell] = -1;
	    return;
	}

	//начальная инициализация модели объекта
	obj_id = INVALID_OBJECT_ID;
	//сначала ищем с obj_id
	mysql_format(gMysql, request, sizeof(request), "SELECT c.id,c.invent_id,b.obj_id FROM veh_invent a INNER JOIN (objects b, things c) ON (a.inv%d=b.id AND b.thing_id=c.id) WHERE a.object_id=%d AND b.obj_id IS NOT NULL LIMIT 1", cell+1, gVeh[vehicleid][0]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
		//затем пробуем найти без obj_id
		mysql_format(gMysql, request, sizeof(request), "SELECT c.id,c.invent_id FROM veh_invent a INNER JOIN (objects b, things c) ON (a.inv%d=b.id AND b.thing_id=c.id) WHERE a.object_id=%d AND b.obj_id IS NULL LIMIT 1", cell+1, gVeh[vehicleid][0]);
		res = mysql_query(gMysql, request);
		cache_get_data(rows, fields, gMysql);
		if(rows == 0)
		{
		    gVehMod[vehicleid][cell] = -1;
		    gVehObj[vehicleid][cell] = -1;
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
			if(cache_is_valid(res, gMysql))
				cache_delete(res);
		    return;
		}
	}
	else
	{
		obj_id = cache_get_field_content_int(0, "obj_id", gMysql);
	}
	gVehMod[vehicleid][cell] = cache_get_field_content_int(0, "invent_id", gMysql);
	gVehObj[vehicleid][cell] = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	if(obj_id != INVALID_OBJECT_ID && IsValidObject(obj_id))
	    DestroyObject(obj_id);

	//указываем вледельца для объекта и обнуляем поле игрового объекта, точки появления объекта и флаг неиспользования объекта
	//если объект до этого принадлежал другому персонажу, сохраняем id прежнего владельца
    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=%d,obj_id=NULL,dot_id=NULL,unused=0 WHERE id=%d", gVeh[vehicleid][0], object_id);
	mysql_query(gMysql, request);

	//обНУЛЛяем объект для точки появления и обновляем время обновления точки
    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL,last_time=now() WHERE object_id=%d", object_id);
	mysql_query(gMysql, request);
}

//выложить объект из ячейки инвентаря
//функция подходит для "разбрасывания" объектов инвентаря (при obj_cell равном -1)
public drop_character_inventory_cell(playerid, cell, obj_cell)
{
	new request[432];
	new rows, fields;
	new modelid, rotatable;
	new Float:x, Float:y, Float:z, Float:Z_coord, Float:ang, Float:old_ang, Float:dx, Float:dy;
	new Float:dx1, Float:dy1, Float:Z_coord1;
	new Float:dx2, Float:dy2, Float:Z_coord2;
	new Float:Alpha, Float:Beta, Float:Alpha1, Float:Beta1, Float:Alpha2, Float:Beta2;
	new Float:rotx, Float:roty, Float:rotz;
	new Float:height;
	new obj_id;
	new ret;
	new Cache:result;
	new bool:flag;
	new is_any;
	new iteration;
	new Float:iter_val;
	new found;
	new invent_id, id;

	if(cell < 0)
	    return;

	if(obj_cell >= 0)
	{
		//защита от дурака
	    if(gInv[playerid][cell] == -1 || gObj[playerid][obj_cell] != -1)
	        return;
	}

	//получаем свойства объекта
    mysql_format(gMysql, request, sizeof(request), "SELECT c.posx,c.posy,c.posz,c.height,c.inworld_id,c.invent_id,b.id,c.rotatable FROM inventory a INNER JOIN (objects b, things c) ON (a.inv%d=b.id AND b.thing_id=c.id) WHERE a.player_id=%d LIMIT 1", cell+1, gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    gInv[playerid][cell] = -1;
	    gInvThing[playerid][cell] = -1;
	    return;
	}
	rotx = cache_get_field_content_float(0, "posx", gMysql);
	roty = cache_get_field_content_float(0, "posy", gMysql);
	rotz = cache_get_field_content_float(0, "posz", gMysql);
	height = cache_get_field_content_float(0, "height", gMysql);
	modelid = cache_get_field_content_int(0, "inworld_id", gMysql);
	rotatable = cache_get_field_content_int(0, "rotatable", gMysql);
	invent_id = cache_get_field_content_int(0, "invent_id", gMysql);
	id = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	if(obj_cell >= 0)
	{
		gObj[playerid][obj_cell] = invent_id;
		gObjThing[playerid][obj_cell] = id;
	}
	
	//освобождаем ячейку
    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET inv%d=-1 WHERE player_id=%d", cell+1, gPlayersID[playerid]);
	mysql_query(gMysql, request);
	gInv[playerid][cell] = -1;
    gInvThing[playerid][cell] = -1;

	//если объект - оружие в руках, то сохраняем количество патронов и убираем из рук
	if(id == gPlayerWeapon[playerid][0])
	{
	    //сохраняем количество патронов
	    save_character_ammo(playerid);

	    //убираем оружие из рук
	    ResetPlayerWeapons(playerid);
	    gPlayerWeapon[playerid][0] = -1;
	    gPlayerWeapon[playerid][1] = -1;
	}

	GetPlayerPos(playerid, x, y, z);

	GetPlayerFacingAngle(playerid, ang);
	ang = floatdiv(floatmul(floatadd(ang,90.0),3.14), 180.0);
	old_ang = ang;
	dx = x + floatmul(floatcos(ang),DROP_DISTANCE_FROM_PLAYER);
	dy = y + floatmul(floatsin(ang),DROP_DISTANCE_FROM_PLAYER);
	Alpha = rotz; //в градусах!

	//эту проверку можно запихнуть в Valid_Z_Coordinate (?) //оптимизация!
	//надо бы создать отдельную функцию для определения координат, чтобы с её помощью разбрасывать вещи из авто
	//...
	ret = MapAndreas_Valid_Z_Coordinate(dx,dy,z,Z_coord,Alpha,Beta); //ищем место, куда бы выложить объект
    dx1 = dx;
    dy1 = dy;
    Z_coord1 = Z_coord;
    flag = false;
    iteration = 1;
    iter_val = 1.0;
    found = 0;
    //пока место плохое или рядом лежит другой объект, и при этом не сделан полный оборот вокруг персонажа
    while( (ret == 1 || (is_any = is_any_object_in_range(dx1,dy1,Z_coord1+height+1.0,1.11))) && (ang > floatsub(old_ang,3.14)) )
    {
        //оптимизация!
        //вычислять место исходя из координат персонажа и лежащих объектов (намного быстрей, чем десяток обращений к базе)
        //...
		if(ret == 0 && is_any == 1)
		{
	        dx2 = dx1;
	        dy2 = dy1;
	        Z_coord2 = Z_coord1;
	        Alpha2 = Alpha1;
	        Beta2 = Beta1;
		    found = 1;
		}
        flag = true;
	    dx = dx1;
	    dy = dy1;
	    Z_coord = Z_coord1;
	    Alpha1 = rotz;
	    //раскачиваем место как маятник
        ang = floatsub(old_ang,(iteration%2 == 0)?floatmul(-0.348,iter_val):floatmul(0.348,iter_val)); //проверяем места с шагом в 20 градусов
        if(iteration%2 == 0)
	        iter_val = floatadd(iter_val,1.0);
		iteration++;
		dx1 = x + floatmul(floatcos(ang),DROP_DISTANCE_FROM_PLAYER);
		dy1 = y + floatmul(floatsin(ang),DROP_DISTANCE_FROM_PLAYER);
		ret = MapAndreas_Valid_Z_Coordinate(dx1,dy1,z,Z_coord1,Alpha1,Beta1); //ищем новое место для объекта
    }

    if(flag) //если был заход в цикл
	{
	    if(ret == 0 && is_any == 0) //если последнее место хорошее - помещаем объект на него
        {
	        dx = dx1;
	        dy = dy1;
	        Z_coord = Z_coord1;
	        Alpha = Alpha1;
	        Beta = Beta1;
		}
		else //иначе - размещаем позади персонажа
		{
		    if(found == 1)
		    {
		        dx = dx2;
		        dy = dy2;
		        Z_coord = Z_coord2;
		        Alpha = Alpha2;
		        Beta = Beta2;
		    }
		    else
		    {
			    Z_coord = floatsub(z,1.0);
				ang = floatadd(old_ang,3.14);
				dx = x + floatmul(floatcos(ang),DROP_DISTANCE_FROM_PLAYER);
				dy = y + floatmul(floatsin(ang),DROP_DISTANCE_FROM_PLAYER);
			}
		}
    }

	if(rotatable == 1 && ret == 0)
	{
	    //если место вполне подходит
	    obj_id = CreateObject(modelid, dx, dy, Z_coord+height+1.0, rotx+Beta, roty+Alpha, rotz, OBJECT_DISTANCE);
	}
	else
	{
	    //если место находится под навесом, либо рядом со стеной
		obj_id = CreateObject(modelid, dx, dy, Z_coord+height+1.0, rotx, roty, rotz, OBJECT_DISTANCE);
	}

	//указываем координаты объекта, обнуляем поля владельца и точки появления объекта, указываем кто выложил и obj_id
	if(rotatable == 1)
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,add_rotz=%f,pl_owner_id=NULL,th_owner_id=NULL,obj_id=%d,dot_id=NULL,is_dropped=%d WHERE id=%d", VectorSize(dx,dy,Z_coord+1.0), dx, dy, Z_coord+1.0, Beta, Alpha, 0.0, obj_id, gPlayersID[playerid], id);
	else
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=NULL,obj_id=%d,dot_id=NULL,is_dropped=%d WHERE id=%d", VectorSize(dx,dy,Z_coord+1.0), dx, dy, Z_coord+1.0, obj_id, gPlayersID[playerid], id);
	mysql_query(gMysql, request);

	//обНУЛЛяем объект для точки появления и обновляем время обновления точки
    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL,last_time=now() WHERE object_id=%d", id);
	mysql_query(gMysql, request);
	
	if(obj_cell >= 0)
	{
		//для обновления стримера
		SetPlayerPos(playerid, x+0.001, y, z);
	}
}

//переложить объект из ячейки инвентаря транспорта в инвентарь игрока
public take_vehicle_inventory_cell(playerid, vehicleid, cell, veh_cell)
{
	new request[256];
	new rows, fields;
	new thing_id;
	new Cache:result;

	//защита от дурака
    if(gInv[playerid][cell] != -1 || gVehObj[vehicleid][veh_cell] == -1)
        return;

    mysql_format(gMysql, request, sizeof(request), "SELECT thing_id FROM objects WHERE id=%d LIMIT 1", gVehObj[vehicleid][veh_cell]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    gInv[playerid][cell] = -1;
	    gInvThing[playerid][cell] = -1;
	    return;
	}
	thing_id = cache_get_field_content_int(0, "thing_id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//перемещаем объект в инвентарь персонажа
    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory a,objects b,veh_invent c SET c.inv%d=-1,\
																								  a.inv%d=%d,\
																								  b.pl_owner_id=%d,\
																								  b.th_owner_id=NULL \
																								  WHERE a.player_id=%d \
																								  AND b.id=%d \
																								  AND c.object_id=%d",
												                                                  veh_cell+1,
																								  cell+1,gVehObj[vehicleid][veh_cell],
																								  gPlayersID[playerid],
																								  gPlayersID[playerid],
																								  gVehObj[vehicleid][veh_cell],
																								  gVeh[vehicleid][0]);
	mysql_query(gMysql, request);
	
	gInv[playerid][cell] = gVehMod[vehicleid][veh_cell];
    gInvThing[playerid][cell] = thing_id;
	gVehObj[vehicleid][veh_cell] = -1;
    gVehMod[vehicleid][veh_cell] = -1;
}

//переложить объект из инвентаря игрока в ячейку инвентаря транспорта
public give_vehicle_inventory_cell(playerid, vehicleid, cell, veh_cell)
{
	new request[256];
	new inv[6];

	//защита от дурака
    if(gInv[playerid][cell] == -1 || gVehObj[vehicleid][veh_cell] != -1)
        return;

	//получаем id объекта
	load_player_inventory(playerid, inv);
	
	//если объект - оружие в руках, то сохраняем количество патронов и убираем из рук
	if(inv[cell] == gPlayerWeapon[playerid][0])
	{
	    //сохраняем количество патронов
	    save_character_ammo(playerid);

	    //убираем оружие из рук
	    ResetPlayerWeapons(playerid);
	    gPlayerWeapon[playerid][0] = -1;
	    gPlayerWeapon[playerid][1] = -1;
	}

	//перемещаем объект в инвентарь транспорта
    mysql_format(gMysql, request, sizeof(request), "UPDATE veh_invent a,objects b,inventory c SET a.inv%d=%d,c.inv%d=-1,b.pl_owner_id=NULL,b.th_owner_id=%d WHERE a.object_id=%d AND b.id=%d AND c.player_id=%d", veh_cell+1,inv[cell],cell+1,gVeh[vehicleid][0],gVeh[vehicleid][0],inv[cell],gPlayersID[playerid]);
	mysql_query(gMysql, request);

	gVehObj[vehicleid][veh_cell] = inv[cell];
    gVehMod[vehicleid][veh_cell] = gInv[playerid][cell];
	gInv[playerid][cell] = -1;
    gInvThing[playerid][cell] = -1;
}

//переложить объект в инвентаре на пустое место
//опасная функция, т.к. обнуляет ячейку без проверки содержимого (!)
public move_character_inventory_cell(playerid, cell1, cell2)
{
	new request[128];
	new Cache:result;

    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET inv%d=inv%d,inv%d=-1 WHERE player_id=%d", cell2+1, cell1+1, cell1+1, gPlayersID[playerid]);
	result = mysql_query(gMysql, request);

    gInv[playerid][cell2] = gInv[playerid][cell1];
	gInv[playerid][cell1] = -1;
    gInvThing[playerid][cell2] = gInvThing[playerid][cell1];
	gInvThing[playerid][cell1] = -1;
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//переложить объект в инвентаре транспорта на пустое место
//опасная функция, т.к. обнуляет ячейку без проверки содержимого (!)
public move_vehicle_inventory_cell(vehicleid, cell1, cell2)
{
	new request[128];

    mysql_format(gMysql, request, sizeof(request), "UPDATE veh_invent SET inv%d=inv%d,inv%d=-1 WHERE object_id=%d", cell2+1, cell1+1, cell1+1, gVeh[vehicleid][0]);
	mysql_query(gMysql, request);

    gVehMod[vehicleid][cell2] = gVehMod[vehicleid][cell1];
	gVehMod[vehicleid][cell1] = -1;
    gVehObj[vehicleid][cell2] = gVehObj[vehicleid][cell1];
	gVehObj[vehicleid][cell1] = -1;
}

//получить свойства содержимого ячейки инвентаря
public get_inventory_properties_cell(playerid, cell, inv_isrot[3], Float:inv_deg[3], &Float:inv_zoom, &is_auto)
{
	new request[384];
	new row_name[16];
	new rows, fields, unused, object_id;
	new Cache:result;

    format(row_name, sizeof(row_name), "inv%d", cell+1);
    mysql_format(gMysql, request, sizeof(request), "SELECT a.player_id,c.is_auto,c.invent_id,c.rotx,c.roty,c.rotz,c.inventx,c.inventy,c.inventz,c.zoom,c.id,b.unused,b.id as object_id FROM inventory a INNER JOIN (objects b, things c) ON (a.%s=b.id AND b.thing_id=c.id) WHERE a.player_id=%d LIMIT 1", row_name, gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    gInv[playerid][cell] = -1;
	    gInvThing[playerid][cell] = -1;
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
		is_auto = 0;
	    return -1;
	}
	unused = cache_get_field_content_int(0, "unused", gMysql);
	if(unused == 1)
	{
		object_id = cache_get_field_content_int(0, "object_id", gMysql);
		//удаляем неиспользуемый объект из инвентаря
	    set_character_inventory_cell(playerid, cell, -1);
		//корректируем базу
		//обнуляем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,pl_owner_id=NULL,th_owner_id=NULL,obj_id=NULL,dot_id=NULL WHERE id=%d", object_id);
		mysql_query(gMysql, request);
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
		is_auto = 0;
		return -1;
	}
	gInv[playerid][cell] = cache_get_field_content_int(0, "invent_id", gMysql);
	gInvThing[playerid][cell] = cache_get_field_content_int(0, "id", gMysql);
	inv_isrot[0] = cache_get_field_content_int(0, "rotx", gMysql);
	inv_isrot[1] = cache_get_field_content_int(0, "roty", gMysql);
	inv_isrot[2] = cache_get_field_content_int(0, "rotz", gMysql);
	inv_deg[0] = cache_get_field_content_float(0, "inventx", gMysql);
	inv_deg[1] = cache_get_field_content_float(0, "inventy", gMysql);
	inv_deg[2] = cache_get_field_content_float(0, "inventz", gMysql);
	inv_zoom = cache_get_field_content_float(0, "zoom", gMysql);
	is_auto = cache_get_field_content_int(0, "is_auto", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	return 1;
}

//получить свойства содержимого ячейки инвентаря транспорта
public get_vehicle_properties_cell(vehicleid, cell, inv_isrot[3], Float:inv_deg[3], &Float:inv_zoom)
{
	new request[320];
	new row_name[16];
	new rows, fields, unused, object_id;
	new Cache:result, Cache:res;

    format(row_name, sizeof(row_name), "inv%d", cell+1);
    mysql_format(gMysql, request, sizeof(request), "SELECT a.object_id as vehicleid,c.invent_id,c.rotx,c.roty,c.rotz,c.inventx,c.inventy,c.inventz,c.zoom,c.id,b.unused,b.id as object_id FROM veh_invent a INNER JOIN (objects b, things c) ON (a.%s=b.id AND b.thing_id=c.id) WHERE a.object_id=%d LIMIT 1", row_name, gVeh[vehicleid][0]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    gVehMod[vehicleid][cell] = -1;
	    gVehObj[vehicleid][cell] = -1;
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	    return -1;
	}
	unused = cache_get_field_content_int(0, "unused", gMysql);
	if(unused == 1)
	{
		object_id = cache_get_field_content_int(0, "object_id", gMysql);
		//удаляем неиспользуемый объект из инвентаря
	    set_vehicle_inventory_cell(vehicleid, cell, -1);
		//корректируем базу
		//обнуляем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=NULL,obj_id=NULL,dot_id=NULL WHERE id=%d", object_id);
		res = mysql_query(gMysql, request);
		if(cache_is_valid(res, gMysql))
			cache_delete(res);
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
		return -1;
	}
	gVehMod[vehicleid][cell] = cache_get_field_content_int(0, "invent_id", gMysql);
	gVehObj[vehicleid][cell] = cache_get_field_content_int(0, "object_id", gMysql);
	inv_isrot[0] = cache_get_field_content_int(0, "rotx", gMysql);
	inv_isrot[1] = cache_get_field_content_int(0, "roty", gMysql);
	inv_isrot[2] = cache_get_field_content_int(0, "rotz", gMysql);
	inv_deg[0] = cache_get_field_content_float(0, "inventx", gMysql);
	inv_deg[1] = cache_get_field_content_float(0, "inventy", gMysql);
	inv_deg[2] = cache_get_field_content_float(0, "inventz", gMysql);
	inv_zoom = cache_get_field_content_float(0, "zoom", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	return 1;
}

//получить свойства всех объектов из второстепенного меню (не из инвентаря!!!)
public get_objects_properties(playerid, inv_isrot[6][3], Float:inv_deg[6][3], Float:inv_zoom[6], Float:range)
{
	new request[512];
	new rows, fields;
	new i, unused, obj_id, object_id;
	new Cache:result, Cache:res;

    set_objects_cells(playerid, range);

	for(i = 0; i < 6; ++i)
	{
	    if(gObj[playerid][i] < 0)
	        break;
	        
	    mysql_format(gMysql, request, sizeof(request), "SELECT c.rotx,c.roty,c.rotz,c.inventx,c.inventy,c.inventz,c.zoom,b.unused,b.obj_id,b.id as object_id FROM objects b INNER JOIN things c ON b.thing_id=c.id WHERE b.id=%d LIMIT 1", gObjThing[playerid][i]);
		result = mysql_query(gMysql, request);
		cache_get_data(rows, fields, gMysql);
		if(rows == 0)
		{
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		    continue;
		}
		unused = cache_get_field_content_int(0, "unused", gMysql);
		if(unused == 1)
		{
			object_id = cache_get_field_content_int(0, "object_id", gMysql);
			obj_id = cache_get_field_content_int(0, "obj_id", gMysql);
			//корректируем базу
			//обнуляем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта
		    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=NULL,obj_id=NULL,dot_id=NULL WHERE id=%d", object_id);
			res = mysql_query(gMysql, request);
			if(IsValidObject(obj_id))
			{
			    DestroyObject(obj_id);
			}
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
			if(cache_is_valid(res, gMysql))
				cache_delete(res);
		    continue;
		}
		inv_isrot[i][0] = cache_get_field_content_int(0, "rotx", gMysql);
		inv_isrot[i][1] = cache_get_field_content_int(0, "roty", gMysql);
		inv_isrot[i][2] = cache_get_field_content_int(0, "rotz", gMysql);
		inv_deg[i][0] = cache_get_field_content_float(0, "inventx", gMysql);
		inv_deg[i][1] = cache_get_field_content_float(0, "inventy", gMysql);
		inv_deg[i][2] = cache_get_field_content_float(0, "inventz", gMysql);
		inv_zoom[i] = cache_get_field_content_float(0, "zoom", gMysql);
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	}
}

//получить свойства всех объектов из меню транспорта
public get_vehicle_objects_properties(vehicleid, inv_isrot[MAX_INVENTORY_ON_VEHICLE][3], Float:inv_deg[MAX_INVENTORY_ON_VEHICLE][3], Float:inv_zoom[MAX_INVENTORY_ON_VEHICLE])
{
	new request[512];
	new rows, fields;
	new i, unused, obj_id, object_id;
	new Cache:result, Cache:res;

	if(vehicleid <= 0)
	    return -1;

    set_vehicle_cells(vehicleid);

	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
	    if(gVehObj[vehicleid][i] < 0)
	        continue;

	    mysql_format(gMysql, request, sizeof(request), "SELECT c.rotx,c.roty,c.rotz,c.inventx,c.inventy,c.inventz,c.zoom,b.unused,b.obj_id,b.id as object_id FROM objects b INNER JOIN things c ON b.thing_id=c.id WHERE b.id=%d LIMIT 1", gVehObj[vehicleid][i]);
		result = mysql_query(gMysql, request);
		cache_get_data(rows, fields, gMysql);
		if(rows == 0)
		{
		    print("no data: get_vehicle_objects_properties()");
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		    continue;
		}
		unused = cache_get_field_content_int(0, "unused", gMysql);
		if(unused == 1)
		{
			object_id = cache_get_field_content_int(0, "object_id", gMysql);
			obj_id = cache_get_field_content_int(0, "obj_id", gMysql);
			//корректируем базу
			//обнуляем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта
		    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=NULL,obj_id=NULL,dot_id=NULL WHERE id=%d", object_id);
			res = mysql_query(gMysql, request);
			if(IsValidObject(obj_id))
			{
			    DestroyObject(obj_id);
			}
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
			if(cache_is_valid(res, gMysql))
				cache_delete(res);
		    continue;
		}
		
		inv_isrot[i][0] = cache_get_field_content_int(0, "rotx", gMysql);
		inv_isrot[i][1] = cache_get_field_content_int(0, "roty", gMysql);
		inv_isrot[i][2] = cache_get_field_content_int(0, "rotz", gMysql);
		inv_deg[i][0] = cache_get_field_content_float(0, "inventx", gMysql);
		inv_deg[i][1] = cache_get_field_content_float(0, "inventy", gMysql);
		inv_deg[i][2] = cache_get_field_content_float(0, "inventz", gMysql);
		inv_zoom[i] = cache_get_field_content_float(0, "zoom", gMysql);
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	}
	
	return 1;
}

//получить свойства содержимого инвентаря персонажа
public get_inventory_properties(playerid, inv_isrot[6][3], Float:inv_deg[6][3], Float:inv_zoom[6], is_auto[6])
{
	new request[512];
	new row_name[16];
	new i, rows, fields, object_id, obj_id, unused;
	new Cache:result, Cache:res;

	for(i = 0; i < 6; ++i)
	{
	    format(row_name, sizeof(row_name), "inv%d", i+1);
	    mysql_format(gMysql, request, sizeof(request), "SELECT a.player_id,b.id as object_id,b.obj_id,c.is_auto,c.invent_id,c.rotx,c.roty,c.rotz,c.inventx,c.inventy,c.inventz,c.zoom,c.id as thing_id,b.unused FROM inventory a INNER JOIN (objects b, things c) ON (a.%s=b.id AND b.thing_id=c.id) WHERE a.player_id=%d LIMIT 1", row_name, gPlayersID[playerid]);
		result = mysql_query(gMysql, request);
		cache_get_data(rows, fields, gMysql);
		if(rows == 0)
		{
		    gInv[playerid][i] = -1;
		    gInvThing[playerid][i] = -1;
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		    continue;
		}
		unused = cache_get_field_content_int(0, "unused", gMysql);
		if(unused == 1)
		{
			//удаляем неиспользуемый объект из инвентаря
		    set_character_inventory_cell(playerid, i, -1);
			//корректируем базу
			//обнуляем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта
		    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=NULL,obj_id=NULL,dot_id=NULL WHERE id=%d", object_id);
			res = mysql_query(gMysql, request);
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
			if(cache_is_valid(res, gMysql))
				cache_delete(res);
		    continue;
		}
		gInv[playerid][i] = cache_get_field_content_int(0, "invent_id", gMysql);
		gInvThing[playerid][i] = cache_get_field_content_int(0, "thing_id", gMysql);
		inv_isrot[i][0] = cache_get_field_content_int(0, "rotx", gMysql);
		inv_isrot[i][1] = cache_get_field_content_int(0, "roty", gMysql);
		inv_isrot[i][2] = cache_get_field_content_int(0, "rotz", gMysql);
		inv_deg[i][0] = cache_get_field_content_float(0, "inventx", gMysql);
		inv_deg[i][1] = cache_get_field_content_float(0, "inventy", gMysql);
		inv_deg[i][2] = cache_get_field_content_float(0, "inventz", gMysql);
		inv_zoom[i] = cache_get_field_content_float(0, "zoom", gMysql);
		object_id = cache_get_field_content_int(0, "object_id", gMysql); //BD object id
		obj_id = cache_get_field_content_int(0, "obj_id", gMysql); //ingame object
		is_auto[i] = cache_get_field_content_int(0, "is_auto", gMysql); //auto object
		if(cache_is_valid(result, gMysql))
			cache_delete(result);

		if(IsValidObject(obj_id))
		    DestroyObject(obj_id);

		//немного корректируем базу
		//указываем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=%d,th_owner_id=NULL,obj_id=NULL,dot_id=NULL WHERE id=%d", gPlayersID[playerid], object_id);
		res = mysql_query(gMysql, request);
		if(cache_is_valid(res, gMysql))
			cache_delete(res);

		//обНУЛЛяем объект для точки появления и обновляем время обновления точки
	    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL,last_time=now() WHERE object_id=%d", object_id);
		res = mysql_query(gMysql, request);
		if(cache_is_valid(res, gMysql))
			cache_delete(res);
	}
}

//получить имя и значение вещи объекта
public get_object_data(playerid, cell, area, name[128], &value)
{
	new request[256];
	new rows, fields;
	new Cache:result;

	strdel(name, 0, sizeof(name));

	if(cell < 0)
	    return -1;
	
	switch(area)
	{
	    case INVENTORY_AREA:
	    {
	        mysql_format(gMysql, request, sizeof(request), "SELECT a.value,b.name FROM objects a INNER JOIN (things b, inventory c) ON (a.thing_id=b.id AND a.id=c.inv%d) WHERE c.player_id=%d", cell+1, gPlayersID[playerid]);
//	        printf("%s",request); //отладка!!!
		}
		case OBJECTS_AREA:
		{
			if(gObjectsMenuShow[playerid] == 0)
			    return -1;
	        mysql_format(gMysql, request, sizeof(request), "SELECT a.value,b.name FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.id=%d", gObjThing[playerid][cell]);
		}
		case VEHICLE_AREA:
		{
			if(gVehicleMenuShow[playerid] == 0)
			    return -1;
	        mysql_format(gMysql, request, sizeof(request), "SELECT a.value,b.name FROM objects a INNER JOIN (things b, veh_invent c, objects d, things e) ON (a.thing_id=b.id AND a.id=c.inv%d AND d.id=c.object_id AND d.thing_id=e.id) WHERE d.id=%d AND e.is_vehicle=1", cell+1, gVeh[gVehicleMenuShow[playerid]][0]);
		}
		default: return -1;
	}

	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		cache_get_field_content(0, "name", name, gMysql);
		value = cache_get_field_content_int(0, "value", gMysql);
	}
	else
	    return -1;
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
    return 1;
}

//задаёт текущее оружие персонажа и количество патронов
public give_character_weapon(playerid, cell)
{
	new request[128];
	new rows, fields;
	new object_id, ammo;
	new Cache:result;

    mysql_format(gMysql, request, sizeof(request), "SELECT b.id,b.value FROM inventory a INNER JOIN objects b ON b.id=a.inv%d WHERE a.player_id=%d AND b.pl_owner_id=%d LIMIT 1", cell+1, gPlayersID[playerid], gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		object_id = cache_get_field_content_int(0, "id", gMysql);
		ammo = cache_get_field_content_int(0, "value", gMysql);
		
		if(gPlayerWeapon[playerid][0] > 0 && gPlayerWeapon[playerid][0] != object_id)
			save_character_ammo(playerid);

		gPlayerWeapon[playerid][0] = object_id;
		gPlayerWeapon[playerid][1] = ammo;
		
		//обновляем количество патронов в обоймах (на всякий случай, можно этого и не делать)
		save_character_ammo(playerid);
	}
	else
	{
		save_character_ammo(playerid);
		gPlayerWeapon[playerid][0] = -1;
		gPlayerWeapon[playerid][1] = -1;
		//отнимаем у персонажа что там у него незаконно лежит
	    mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET inv%d=-1 WHERE player_id=%d", cell+1, gPlayersID[playerid]);
		mysql_query(gMysql, request);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//сохраняем количество патронов в самом объекте-оружии
//(а разбрасываем это количество по коробкам только при разборе оружия)
public save_character_ammo(playerid)
{
	new request[128];
	new rows, fields;
	new ammo, object_id, value, row;
	new Cache:res, Cache:result;

	//задаём количество патронов в самом оружии
	if(gPlayerWeapon[playerid][0] != -1)
	{
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET value=%d WHERE id=%d", gPlayerWeapon[playerid][1], gPlayerWeapon[playerid][0]);
		mysql_query(gMysql, request);
	}
	else
	    return;
	
	//количество патронов для распределения
	value = gPlayerWeapon[playerid][1];

	//корректируем расходуемые объекты в составе оружия
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.value FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.th_owner_id=%d AND b.is_consumble=1", gPlayerWeapon[playerid][0]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		res = cache_save(gMysql);

	    if(value <= 0)
	    {
			//освобождаем пустые обоймы
			for(row = 0; row < rows; ++row)
			{
			    cache_set_active(res, gMysql);
				object_id = cache_get_field_content_int(row, "id", gMysql);
			    free_object_from_owner(object_id);
			}
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
			return;
		}

	    row = 0;
		while(value > 0 && row < rows) //пока есть патроны для распределения
		{
		    cache_set_active(res, gMysql);
			ammo = cache_get_field_content_int(row, "value", gMysql);
			object_id = cache_get_field_content_int(row, "id", gMysql);
			
			if(value > ammo)
			{
			    value = value - ammo;
			    row++;
			    continue;
			}
			else
			{
				if(value < ammo)
				{
				    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET value=%d WHERE id=%d", value, object_id);
					mysql_query(gMysql, request);
				}
				//освобождаем пустые обоймы
				for(row = row+1; row < rows; ++row)
				{
				    cache_set_active(res, gMysql);
					object_id = cache_get_field_content_int(row, "id", gMysql);
				    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET value=%d WHERE id=%d", value, object_id);
					mysql_query(gMysql, request);
				    free_object_from_owner(object_id);
				}
			    break;
			}
			row++;
		}
		
		if(value < 0)
		{
		    //а тут можно античит поставить на патроны (кик там, или бан)
		    //...
		}
		
		if(cache_is_valid(res, gMysql))
			cache_delete(res, gMysql);
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
	}
}

//сохранить состояние и координаты авто
public save_vehicle_state(playerid, vehicleid)
{
	new request[512];
	new is_working, is_engine, is_lights, is_alarm, is_doors, is_bonnet, is_boot, is_objective, panels, doors, lights, tires, engine_id, patrol;
	new Float:x, Float:y, Float:z, Float:ang;
	new rows, fields;
	new bool:is_dotted;
	new Cache:result;

//	gVeh[vehicleid][0] - id объекта

	if(gVeh[vehicleid][0] <= 0)
	{
	    //античит
	    if(!IsPlayerAdmin(playerid))
	    {
	        SendClientMessage(playerid, 0xFFAACCFF, "It was an admin's vehicle!");
	        DestroyVehicle(vehicleid);
	    }
		return;
	}
		
	is_dotted = false;

	//если авто находится на точке респавна - выходим, т.к. обновлять нечего!
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,b.is_working,b.patrol FROM objects a INNER JOIN veh_data b ON a.id=b.object_id WHERE id=%d AND dot_id IS NOT NULL LIMIT 1", gVeh[vehicleid][0]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		is_working = cache_get_field_content_int(0, "is_working", gMysql);
		patrol = cache_get_field_content_int(0, "patrol", gMysql);
		if(cache_is_valid(result, gMysql))
			cache_delete(result);
		if(is_working == gVeh[vehicleid][4] && gVeh[vehicleid][3] == patrol)
		{
			return;
		}
		is_dotted = true;
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	GetVehicleParamsEx(vehicleid, is_engine, is_lights, is_alarm, is_doors, is_bonnet, is_boot, is_objective);
	if(gVeh[vehicleid][3] <= 0 && gVeh[vehicleid][5] > 0)
	{
	    gVeh[vehicleid][3] = 0;
	    gVeh[vehicleid][4] = 0;
		update_vehicle_sensors(playerid);
	}
	if(gVeh[vehicleid][2] < 0)
	{
	    gVeh[vehicleid][2] = 0;
	    gVeh[vehicleid][4] = 0;

	    //"освобождение" двигателя от авто
	    mysql_format(gMysql, request, sizeof(request), "SELECT engine_id FROM veh_data WHERE object_id=%d AND engine_id > 0 LIMIT 1", gVeh[vehicleid][0]);
		mysql_query(gMysql, request);
		cache_get_data(rows, fields, gMysql);
		if(rows > 0)
		{
			engine_id = cache_get_field_content_int(0, "engine_id", gMysql);
			//осторожно! объект может не удалиться при перезагрузке сервера (повторное появление после удаления всех объектов)
			//выход - не вызывать функцию save_vehicle_state при закрытии сервера (создать отдельную функцию для этого, которая не вызывает free_object_from_owner())
			free_object_from_owner(engine_id);
			//думаю вместо поля 'value' объекта-авто использовать 'value' объекта-двигателя (?)
		    mysql_format(gMysql, request, sizeof(request), "UPDATE veh_data SET engine_id=NULL WHERE object_id=%d LIMIT 1", gVeh[vehicleid][0]);
			mysql_query(gMysql, request);
		}
	}
	SetVehicleParamsEx(vehicleid, gVeh[vehicleid][4], is_lights, is_alarm, is_doors, is_bonnet, is_boot, is_objective);
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	GetVehiclePos(vehicleid, x, y, z);
	GetVehicleZAngle(vehicleid, ang);

	if(playerid != INVALID_PLAYER_ID)
	{
	    if(is_dotted)
	    {
			//сохраняем состояние авто
		    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, veh_data b SET a.vector=%f,a.placex=%f,a.placey=%f,a.placez=%f,a.add_rotz=%f,a.value=%d,b.panels=%d,b.doors=%d,b.light=%d,b.tires=%d,b.patrol=%d,b.is_working=%d WHERE a.id=%d AND b.object_id=a.id", VectorSize(x,y,z), x, y, z, ang, gVeh[vehicleid][2], panels, doors, lights, tires, gVeh[vehicleid][3], gVeh[vehicleid][4], gVeh[vehicleid][0]);
	    }
	    else
	    {
			//задаём поле 'value' для владельца
		    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, veh_data b SET a.vector=%f,a.placex=%f,a.placey=%f,a.placez=%f,a.add_rotz=%f,a.dot_id=NULL,is_dropped=%d,a.value=%d,b.panels=%d,b.doors=%d,b.light=%d,b.tires=%d,b.patrol=%d,b.is_working=%d WHERE a.id=%d AND b.object_id=a.id", VectorSize(x,y,z), x, y, z, ang, gPlayersID[playerid], gVeh[vehicleid][2], panels, doors, lights, tires, gVeh[vehicleid][3], gVeh[vehicleid][4], gVeh[vehicleid][0]);
		}
	}
	else
	{
		//задаём поле 'value' для владельца
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, veh_data b SET a.vector=%f,a.placex=%f,a.placey=%f,a.placez=%f,a.add_rotz=%f,a.dot_id=NULL,a.value=%d,b.panels=%d,b.doors=%d,b.light=%d,b.tires=%d,b.patrol=%d,b.is_working=%d WHERE a.id=%d AND b.object_id=a.id", VectorSize(x,y,z), x, y, z, ang, gVeh[vehicleid][2], panels, doors, lights, tires, gVeh[vehicleid][3], gVeh[vehicleid][4], gVeh[vehicleid][0]);
	}
	
	result = mysql_query(gMysql, request);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//обязательное сохранение состояния авто
public save_vehicle_position(vehicleid)
{
	new request[512];
	new Float:x, Float:y, Float:z, Float:ang;
	new is_engine, is_lights, is_alarm, is_doors, is_bonnet, is_boot, is_objective, panels, doors, lights, tires;
	new Cache:result;
	
	GetVehicleParamsEx(vehicleid, is_engine, is_lights, is_alarm, is_doors, is_bonnet, is_boot, is_objective);
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
	GetVehiclePos(vehicleid, x, y, z);
	GetVehicleZAngle(vehicleid, ang);

    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, veh_data b SET a.vector=%f,a.placex=%f,a.placey=%f,a.placez=%f,a.add_rotz=%f,a.dot_id=NULL,b.panels=%d,b.doors=%d,b.light=%d,b.tires=%d WHERE a.id=%d AND b.object_id=a.id", VectorSize(x,y,z), x, y, z, ang, panels, doors, lights, tires, gVeh[vehicleid][0]);
	result = mysql_query(gMysql, request);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//создать новое авто
public create_new_vehicle(object_id, Float:x, Float:y, Float:z, Float:rotz, inworld_id, def_value)
{
	new i, vehicleid, color1, color2, cells, panels, doors, light, tires, patrol, patrol_cons, max_patrol;
	new request[432];
	new inv[MAX_INVENTORY_ON_VEHICLE], obj[MAX_INVENTORY_ON_VEHICLE];
	new rows, fields;
	new Cache:result;

	mysql_format(gMysql, request, sizeof(request), "SELECT object_id FROM veh_data WHERE object_id=%d", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows <= 0)
	{
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO veh_data(object_id) values(%d)", object_id);
		mysql_query(gMysql, request);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	mysql_format(gMysql, request, sizeof(request), "SELECT object_id FROM veh_invent WHERE object_id=%d", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows <= 0)
	{
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO veh_invent(object_id) values(%d)", object_id);
		mysql_query(gMysql, request);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	mysql_format(gMysql, request, sizeof(request), "UPDATE veh_data SET panels=0,doors=0,light=0,tires=0,patrol=0,is_working=0 WHERE object_id=%d", object_id);
	mysql_query(gMysql, request);

	mysql_format(gMysql, request, sizeof(request), "SELECT a.cells,a.def_panels,a.def_doors,a.def_light,a.def_tires,a.def_patrol,a.patrol_cons,a.max_patrol,b.color1,b.color2 FROM objects c INNER JOIN (vehicles a, veh_data b) ON (c.id=b.object_id AND c.thing_id=a.thing_id) WHERE c.id=%d LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		color1 = cache_get_field_content_int(0, "color1", gMysql);
		color2 = cache_get_field_content_int(0, "color2", gMysql);
		cells = cache_get_field_content_int(0, "cells", gMysql);
		panels = cache_get_field_content_int(0, "def_panels", gMysql);
		doors = cache_get_field_content_int(0, "def_doors", gMysql);
		light = cache_get_field_content_int(0, "def_light", gMysql);
		tires = cache_get_field_content_int(0, "def_tires", gMysql);
		patrol = cache_get_field_content_int(0, "def_patrol", gMysql);
		patrol_cons = cache_get_field_content_int(0, "patrol_cons", gMysql);
		max_patrol = cache_get_field_content_int(0, "max_patrol", gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
	vehicleid = CreateVehicle(inworld_id, x, y, z, rotz, color1, color2, -1); //don't respawn the vehicle
	UpdateVehicleDamageStatus(vehicleid, panels, doors, light, tires);
	gVeh[vehicleid][0] = object_id;
	gVeh[vehicleid][1] = cells; //количество ячеек инвентаря транспорта
	gVeh[vehicleid][2] = def_value; //состояние двигателя по-умолчанию
	gVeh[vehicleid][3] = patrol; //количество бензина по-умолчанию
	gVeh[vehicleid][4] = 0; //двигатель заглушен
	gVeh[vehicleid][5] = patrol_cons; //расход топлива
	gVeh[vehicleid][6] = max_patrol; //объём бака
	SetVehicleParamsEx(vehicleid, 0, 0, 0, 0, 0, 0, 0); //а вот тут можно включить/выключить фары
	
	//начальная инициализация инвентаря
	load_vehicle_inventory(vehicleid, inv, obj);
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
	    gVehMod[vehicleid][i] = obj[i];
		gVehObj[vehicleid][i] = inv[i];
	}
	
	//сохраняем состояние авто
	mysql_format(gMysql,request,sizeof(request),"UPDATE objects a,veh_data b SET a.unused=0,a.add_rotx=0,a.add_roty=0,a.pl_owner_id=NULL,a.th_owner_id=NULL,a.is_dropped=NULL,a.obj_id=%d,a.vector=%f,a.placex=%f,a.placey=%f,a.placez=%f,a.add_rotz=%f,a.value=%d,b.panels=%d,b.doors=%d,b.light=%d,b.tires=%d,b.patrol=%d,b.is_working=%d WHERE a.id=%d AND b.object_id=a.id",vehicleid,VectorSize(x,y,z),x,y,z,rotz,def_value,panels,doors,light,tires,patrol,0,object_id);
	mysql_query(gMysql, request);
//	printf("%s",request); //отладка!!!
}

//создать перемещённое авто
public create_old_vehicle(object_id, Float:x, Float:y, Float:z, Float:rotz, inworld_id)
{
	new i, vehicleid, value, color1, color2, cells, panels, doors, light, tires, patrol, patrol_cons, max_patrol, is_working;
	new request[512];
	new rows, fields;
	new inv[MAX_INVENTORY_ON_VEHICLE], obj[MAX_INVENTORY_ON_VEHICLE];
	new Cache:result;

	mysql_format(gMysql, request, sizeof(request), "SELECT object_id FROM veh_data WHERE object_id=%d", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows <= 0)
	{
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO veh_data(object_id) values(%d)", object_id);
		mysql_query(gMysql, request);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	mysql_format(gMysql, request, sizeof(request), "SELECT object_id FROM veh_invent WHERE object_id=%d", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows <= 0)
	{
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO veh_invent(object_id) values(%d)", object_id);
		mysql_query(gMysql, request);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	mysql_format(gMysql, request, sizeof(request), "SELECT c.value,a.cells,a.patrol_cons,a.max_patrol,b.color1,b.color2,b.panels,b.doors,b.light,b.tires,b.patrol,b.is_working FROM objects c INNER JOIN (vehicles a, veh_data b) ON (c.id=b.object_id AND c.thing_id=a.thing_id) WHERE c.id=%d LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		color1 = cache_get_field_content_int(0, "color1", gMysql);
		color2 = cache_get_field_content_int(0, "color2", gMysql);
		cells = cache_get_field_content_int(0, "cells", gMysql);
		panels = cache_get_field_content_int(0, "panels", gMysql);
		doors = cache_get_field_content_int(0, "doors", gMysql);
		light = cache_get_field_content_int(0, "light", gMysql);
		tires = cache_get_field_content_int(0, "tires", gMysql);
		patrol = cache_get_field_content_int(0, "patrol", gMysql);
		patrol_cons = cache_get_field_content_int(0, "patrol_cons", gMysql);
		is_working = cache_get_field_content_int(0, "is_working", gMysql);
		value = cache_get_field_content_int(0, "value", gMysql);
		max_patrol = cache_get_field_content_int(0, "max_patrol", gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
	vehicleid = CreateVehicle(inworld_id, x, y, z, rotz, color1, color2, -1); //don't respawn the vehicle
	SetVehicleParamsEx(vehicleid, is_working, 0, 0, 0, 0, 0, 0); //а вот тут можно включить/выключить фары
	UpdateVehicleDamageStatus(vehicleid, panels, doors, light, tires);
	gVeh[vehicleid][0] = object_id;
	gVeh[vehicleid][1] = cells; //количество ячеек инвентаря транспорта
	gVeh[vehicleid][2] = value; //состояние двигателя
	gVeh[vehicleid][3] = patrol; //количество бензина
	gVeh[vehicleid][4] = is_working; //двигатель работает/заглушен
	gVeh[vehicleid][5] = patrol_cons; //расход топлива
	gVeh[vehicleid][6] = max_patrol; //объём бака

	//начальная инициализация инвентаря
	load_vehicle_inventory(vehicleid, inv, obj);
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
	    gVehMod[vehicleid][i] = obj[i];
		gVehObj[vehicleid][i] = inv[i];
	}

	mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET obj_id=%d WHERE id=%d", vehicleid, object_id);
	mysql_query(gMysql, request);
}

//удалить авто из точки появления, если оно не находится в этой точке
public drop_vehicle_from_dot(playerid, vehicleid)
{
	new request[512];
	new Float:x, Float:y, Float:z;

	GetVehiclePos(vehicleid, x, y, z);

	mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot a, objects b SET a.last_time=NOW(),a.object_id=NULL,b.dot_id=NULL,b.is_dropped=%d WHERE a.object_id=%d AND b.id=%d AND ((a.placex-%f)*(a.placex-%f)+(a.placey-%f)*(a.placey-%f)+(a.placez-%f)*(a.placez-%f))>10", gPlayersID[playerid], gVeh[vehicleid][0], gVeh[vehicleid][0], x, x, y, y, z, z);
	mysql_query(gMysql, request);
}

//присвоить объект новому объекту-владельцу
//т.к. объект становится присвоенным объекту, то он больше не может находиться в инвентаре
//но (!) эта функция не меняет инвентарь персонажа (необходимо позаботиться об этом при вызове данной функции)
//зато это позволяет вызывать функцию для всех объектов, а не только тех, что в инвентаре
public set_object_owner(th_owner_id, object_id)
{
	new request[512];
	new rows, fields;
	new value;
	new Cache:result;

    mysql_format(gMysql, request, sizeof(request), "SELECT value FROM objects WHERE id=%d LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		value = cache_get_field_content_int(0, "value", gMysql);
	}
	else
	    value = 0;
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//задаём поле 'value' для владельца
    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET value=value+%d WHERE id=%d", value, th_owner_id);
	mysql_query(gMysql, request);
	
	//немного корректируем базу
	//указываем вледельца для объекта и обнуляем поле игрового объекта и точки появления объекта, а также  указываем unused=0
	//сохраняем id прежнего владельца, если таковой был (!)
    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=%d,obj_id=NULL,dot_id=NULL,unused=0 WHERE id=%d", th_owner_id, object_id);
	mysql_query(gMysql, request);

	//обНУЛЛяем объект для точки появления и обновляем время обновления точки
    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=NULL,last_time=now() WHERE object_id=%d", object_id);
	mysql_query(gMysql, request);
}

//получить из базы свободный (unused=true) объект заданного типа, либо создать новый такой
//используется для создания композитных объектов-владельцев из комбинации некоторых существующих объектов
public get_unused_object(thing_id)
{
	new request[256];
	new rows, fields;
	new object_id;
	new Cache:result, Cache:res;

    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM objects WHERE thing_id=%d AND unused=1 LIMIT 1", thing_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO objects(thing_id,next_id,unused) values(%d,%d,1)", thing_id, thing_id);
		mysql_query(gMysql, request);

	    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM objects WHERE thing_id=%d AND unused=1 LIMIT 1", thing_id);
		res = mysql_query(gMysql, request);
		cache_get_data(rows, fields, gMysql);
		if(rows == 0)
		{
		    return -1;
		}
		if(cache_is_valid(res, gMysql))
			cache_delete(res);
	}
	object_id = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	return object_id;
}

//создать резидентный объект (для объектов, помеченных как '!' в файле описания вещей)
public create_resident_object(object_id, old_thing_id, new_thing_id)
{
	new request[256];

	//если вещи совпадают - сохраняем value!
	if(old_thing_id == new_thing_id)
	{
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET thing_id=%d,prev_id=%d WHERE id=%d", new_thing_id, old_thing_id, object_id);
	}
	else
	{
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, things b SET a.thing_id=%d,a.prev_id=%d,a.value=b.def_value WHERE a.id=%d AND b.id=%d", new_thing_id, old_thing_id, object_id, new_thing_id);
	}
	mysql_query(gMysql, request);
}

//создать новую составную вещь
public create_composite_object(playerid, &cell)
{
	new i,res;
	new request[256];
	new thing[8];
	new owner_cell; //ячейка, где будет находиться созданный объект
	new owner_id; //id объекта-владельца
	new inventory[6];
	new no_selected;
	new main_thing_id;

	owner_id = -1;
	owner_cell = -1;
	
	cell = -1;

	//для начала определяем, какие ячейки инвентаря выбраны
	//и генерируем запрос
	strdel(request, 0, sizeof(request));
	for(i = 0, no_selected = 1; i < 6; ++i)
	{
	    if(gRotate[playerid][i] > 0)
	    {
	        if(strlen(request) != 0)
	            strcat(request, ",");
			strdel(thing, 0, sizeof(thing));
	        format(thing, sizeof(thing), "%d", gInvThing[playerid][i]);
			strcat(request, thing);
			no_selected = 0;
		}
	}

	if(no_selected == 1)
	    return 0;

	res = composit_thing(request, main_thing_id);
	
//	printf("res=%d,main_thing_id=%d,request=%s",res,main_thing_id,request); //отладка!!!

	if(res > 0) //если объект создаётся
	{
		//получаем инвентарь персонажа
		load_player_inventory(playerid, inventory);

	    //создаём объект-владелец (если он уже находится среди выбранных объектов)
		for(i = 0; i < 6; ++i)
		{
		    //если объект выбран
            if(gRotate[playerid][i] > 0)
            {
				//если в правиле прописан резидентный объект (значок '!')
				//задаём приоритет резидентному объекту
                if(main_thing_id != 0)
                {
				    //если объект резидентный
				    if( main_thing_id == gInvThing[playerid][i] || gInvThing[playerid][i] == -main_thing_id )
				    {
						//заменяем id вещи этого объекта на id вещи создаваемого
						//при этом запоминаем прежний id в поле objects.prev_id
			            create_resident_object(inventory[i], main_thing_id, res);
				        //оставляем этот объект в его ячейке как есть (т.е делаем его владельцем)
				        owner_cell = i;
				        owner_id = inventory[i];
				        break;
					}
                }
                else if(gInvThing[playerid][i] == res) //если тип объекта совпадает с типом создаваемой вещи
			    {
			        //оставляем этот объект в его ячейке как есть (т.е делаем его владельцем)
			        owner_cell = i;
			        owner_id = inventory[i];
			        break;
				}
			}
		}

		//создаём объект-владелец
		if(owner_id < 0)
		{
		    //получаем свободный объект с заданного типа (либо создаём новый свободный этого типа)
		    owner_id = get_unused_object(res);
		    if(owner_id < 0)
		    {
				print("no free unused object!");
		        return 0;
			}
		}
		
		if(owner_id == gPlayerWeapon[playerid][0])
		{
			//сохраним патрончики
    		save_character_ammo(playerid);
		}

	    //присваиваем выбранные объекты (попутно освобождая инвентарь)
		for(i = 0; i < 6; ++i)
		{
		    //если ячейка выбрана и не является ячейкой объекта-владельца
		    if(gRotate[playerid][i] > 0 && i != owner_cell)
		    {
		        //присваиваем объекту-владельцу
		        set_object_owner(owner_id, inventory[i]);
		        //удаляем из инвентаря (сбрасываем gInv и gInvThing для текущей ячейки)
		        set_character_inventory_cell(playerid, i, -1);
			}
		}
		
		//выбираем для объекта-владельца самую старшую ячейку (если ячейка для него ещё не выбрана)
        if(owner_cell < 0) //если объект-владелец ещё не находится в ячейке
        {
			//ищем самую старшую освобождённую ячейку
			for(i = 0; i < 6; ++i)
			{
			    if(gRotate[playerid][i] > 0)
			    {
			    	owner_cell = i;
			    	break;
				}
			}
			
			//помещаем объект-владелец в найденную ячейку
			//эта функция автоматически установит unused=0
			set_character_inventory_cell(playerid, owner_cell, owner_id);
		}
//		SendClientMessage(playerid, 0x00FF00, "ok!"); //отладка!!!
	}
	else
	{
	    //по-сути, тут надо бы организовать добавление элементов к композитному объекту
	    return 0;
	}

	cell = owner_cell;
	
	if(owner_id == gPlayerWeapon[playerid][0])
	    return 1; //если обновилось активное оружие игрока, сообщаем об этом
	else
		return 0; //owner_cell; //возвращаем номер ячейки с новым объектом
}

//разобрать композитный объект
public disassemble_resident_object(object_id, prev_id)
{
	new request[256];

	//если объект неразбираемый, либо не резидентный - выходим!
	if(prev_id <= 0)
	    return;

	//value можно также сохранять, как и thing_id
	//а вообще можно устанавливать value в def_value прежнего объекта(прямо в этом же запросе)
	//...
//    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET prev_id=thing_id,thing_id=%d,value=0 WHERE id=%d", prev_id, object_id);
    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, things b SET a.prev_id=NULL,a.thing_id=%d,a.value=b.def_value WHERE a.id=%d AND b.id=%d", prev_id, object_id, prev_id);
	mysql_query(gMysql, request);
}

//разобрать композитный объект
//подразумевает появление составляющих объектов в инвентаре игрока, либо рядом с игроком
public disassemble_cell_object(playerid, cell)
{
	new request[256];
	new rows, fields;
	new str_prev_id[16];
	new object_id, pl_owner_id, thing_id, prev_id;
	new i, j, count, pl_id;
	new col_name[16], result[64];
	new th_owner_id;
	new Cache:ids;
	
	//ищем объект (можно найти все объекты инвентаря и исходя из этого искать свободные ячейки) (?)
	strdel(col_name, 0, sizeof(col_name));
	format(col_name, sizeof(col_name), "inv%d", cell+1);
	get_character_value(playerid, col_name, "inventory", result);
	th_owner_id = strval(result);
	if(th_owner_id < 0)
	    return -1;
	//ищем владельца объекта и тип вещи
    mysql_format(gMysql, request, sizeof(request), "SELECT pl_owner_id,thing_id,prev_id FROM objects WHERE id=%d AND unused=0 LIMIT 1", th_owner_id);
	mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		pl_owner_id = cache_get_field_content_int(0, "pl_owner_id", gMysql);
		thing_id = cache_get_field_content_int(0, "thing_id", gMysql);
		strdel(str_prev_id, 0, sizeof(str_prev_id));
		cache_get_field_content(0, "prev_id", str_prev_id, gMysql);
		prev_id = strval(str_prev_id);
	}
	else
	    return -1;

	//если это неразбираемый композитный объект - выходим!
	if(prev_id < 0)
	    return -1;

	//а вот тута необходимо проверить, разбираемый (композитный) это объект или нет!
	if(is_composit(thing_id) < 0)
	    return -1;

	//ищем playerid игрока
	pl_id = INVALID_PLAYER_ID;
	for(i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(gPlayersID[i] == pl_owner_id)
	        pl_id = i;
	}
	//надо сверить playerid и pl_id. если не совпадают, удалить объект pl_owner_id у playerid из ячейки cell
	if(pl_id != playerid)
	{
	    print("error: wrong playerid!");
	    //удаляем объект
	    //...
	    return -1;
	}

	//если такого игрока не существует - выходим
	if(pl_id == INVALID_PLAYER_ID)
	{
	    print("error: invalid playerid!");
	    return -1;
 	}
	    
	//сохраним-ка мы тут патрончики, не помешает
	save_character_ammo(playerid);

	//ищем все объекты, для которых разбираемый объект является хозяином
    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM objects WHERE th_owner_id=%d AND unused=0 ORDER BY id DESC", th_owner_id);
	mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		//сосчитаем свободные ячейки инвентаря персонажа
		if(prev_id > 0)
		    count = 0; //ячейка не освободится
		else
			count = 1; //одна освободится из-под композитного объекта
			
		for(i = 0; i < 6; ++i)
		{
		    if(gInv[playerid][i] == -1)
		        count++;
		}
		//если количество ячеек меньше количества составных частей - выходим
		if(count < rows)
		    return -1;

		//сохраняем предыдущий результат перед новым обращением к БД
		ids = cache_save(gMysql);

		if(prev_id == 0)
		{
			//полностью освобождаем ячейку с композитным/резидентным объектом
			set_character_inventory_cell(playerid, cell, -1);
		}

		for(i = 0; i < rows; ++i)
		{
			//ищем свободную ячейку
			//...
			for(j = 0; j < 6; ++j)
			{
				if(gInv[playerid][j] == -1)
				{
				    cache_set_active(ids, gMysql);
					object_id = cache_get_field_content_int(i, "id", gMysql);
					set_character_inventory_cell(playerid, j, object_id);
					break;
				}
			}
		}
		//удаляем результат поиска
		if(cache_is_valid(ids, gMysql))
			cache_delete(ids, gMysql);
	}
	else
	{
		//удаляем результат поиска
		if(cache_is_valid(ids, gMysql))
			cache_delete(ids, gMysql);

	    //если композитный нерезидентный объект не является хозяином ни для одного другого объекта - выходим!
		if(prev_id == 0)
		    return -1;
	}
	
	if(prev_id > 0)
	{
	    //разбираем резидентный объект
	    disassemble_resident_object(th_owner_id, prev_id);
	}
	else if(prev_id == 0)
	{
		//т.к. у композитного объекта больше нету составляющих, он удаляется
		mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,pl_owner_id=NULL,th_owner_id=NULL,obj_id=NULL,dot_id=NULL,value=0,unused=1 WHERE id=%d", th_owner_id);
		mysql_query(gMysql, request);
	}
	
	if(th_owner_id == gPlayerWeapon[playerid][0])
	{
		//убираем оружие из рук
		ResetPlayerWeapons(playerid);
		gPlayerWeapon[playerid][0] = -1;
		gPlayerWeapon[playerid][1] = -1;
	}

	//остаётся только обновить меню у вызвавшего игрока (делается это в вызвавшей функции)

	return 0;
}

//разобрать объект
public disassemble_inventory_object(object_id)
{
	new request[256];
	new rows, fields, i;
	new playerid, cell, pl_owner_id;
	new inv[6];
	new Cache:result;

	if(object_id <= 0)
	    return -1;

    mysql_format(gMysql, request, sizeof(request), "SELECT pl_owner_id FROM objects WHERE id=%d LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return -1;
	pl_owner_id = cache_get_field_content_int(0, "pl_owner_id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	playerid = INVALID_PLAYER_ID;
	for(i = 0; i < MAX_PLAYERS; ++i)
	{
	    if(gPlayersID[i] == pl_owner_id)
	    {
	        playerid = i;
	        break;
	    }
	}
	if(playerid == INVALID_PLAYER_ID)
	    return -1;
	load_player_inventory(playerid, inv);
	
	cell = -1;
	for(i = 0; i < 6; ++i)
	{
	    if(inv[i] == object_id)
	    {
	        cell = i;
	        break;
		}
	}
	if(cell < 0)
	    return -1;

	disassemble_cell_object(playerid, cell);
	
	return cell;
}

//выковырять объект из композитного объекта
//подразумевает появление объекта в инвентаре, либо рядом с игроком
public pick_object_from_owner(obj_id)
{
}

//освободить объект, находящийся в ячейке инвентаря
public free_cell_from_owner(playerid, cell)
{
	new inventory[6];
	
	load_player_inventory(playerid, inventory);
	free_object_from_owner(inventory[cell]);
}

//освободить объект от всех владельцев
public clear_all_owners(object_id)
{
	new request[256];
	new rows, fields, i, playerid;
	new pl_owner_id, th_owner_id, is_consumble;
	new col_name[16];
	new invent_object_id;
	new Cache:ids, Cache:result, Cache:res;

	if(object_id < 0)
	    return;

	//ищем владельца объекта и тип вещи
    mysql_format(gMysql, request, sizeof(request), "SELECT IFNULL(a.pl_owner_id, 0) as pl_owner,IFNULL(a.th_owner_id, 0) as th_owner,b.is_consumble FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.id=%d AND a.unused=0 LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		pl_owner_id = cache_get_field_content_int(0, "pl_owner", gMysql);
		th_owner_id = cache_get_field_content_int(0, "th_owner", gMysql);
		is_consumble = cache_get_field_content_int(0, "is_consumble", gMysql);

		if(pl_owner_id > 0)
		{
			//удаляем вещь из инвентаря владельца
		    mysql_format(gMysql, request, sizeof(request), "SELECT inv1,inv2,inv3,inv4,inv5,inv6 FROM inventory WHERE player_id=%d LIMIT 1", pl_owner_id);
			res = mysql_query(gMysql, request);
			cache_get_data(rows, fields, gMysql);
			if(rows > 0)
			{
				//сохраняем предыдущий результат перед новым обращением к БД
				ids = cache_save(gMysql);
			    for(i = 0; i < 6; ++i)
			    {
				    cache_set_active(ids, gMysql);
				    strdel(col_name, 0, sizeof(col_name));
					format(col_name, sizeof(col_name), "inv%d", i+1);
			        invent_object_id = cache_get_field_content_int(0, col_name, gMysql);
			        if(object_id == invent_object_id)
			        {
						mysql_format(gMysql, request, sizeof(request), "UPDATE inventory SET %s=-1 WHERE player_id=%d", col_name, pl_owner_id);
						mysql_query(gMysql, request);
						
						for(playerid = 0; playerid < MAX_PLAYERS; ++playerid)
						{
						    if(gPlayersID[playerid] == pl_owner_id)
						    {
						        gInv[playerid][i] = -1;
						        gInvThing[playerid][i] = -1;
						        break;
							}
						}
			        }
			    }
				//удаляем результат поиска
				if(cache_is_valid(ids, gMysql))
					cache_delete(ids, gMysql);
				if(cache_is_valid(res, gMysql))
					cache_delete(res);
			}
		}
		
		if(th_owner_id > 0)
		{
			//удаляем вещь из инвентаря владельца-авто
		    mysql_format(gMysql, request, sizeof(request), "SELECT inv1,inv2,inv3,inv4,inv5,inv6,inv7,inv8,inv9,inv10,inv11,inv12 FROM veh_invent WHERE object_id=%d LIMIT 1", th_owner_id);
			res = mysql_query(gMysql, request);
			cache_get_data(rows, fields, gMysql);
			if(rows > 0)
			{
				//сохраняем предыдущий результат перед новым обращением к БД
				ids = cache_save(gMysql);
			    for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
			    {
				    cache_set_active(ids, gMysql);
				    strdel(col_name, 0, sizeof(col_name));
					format(col_name, sizeof(col_name), "inv%d", i+1);
			        invent_object_id = cache_get_field_content_int(0, col_name, gMysql);
			        if(object_id == invent_object_id)
			        {
						mysql_format(gMysql, request, sizeof(request), "UPDATE veh_invent SET %s=-1 WHERE object_id=%d", col_name, th_owner_id);
						mysql_query(gMysql, request);
			        }
			    }
				//удаляем результат поиска
				if(cache_is_valid(ids, gMysql))
					cache_delete(ids, gMysql);
				if(cache_is_valid(res, gMysql))
					cache_delete(res);
			}
			
			//если объект расходуемый
			if(is_consumble == 1 && th_owner_id > 0)
			{
			    //отнимаем значение 'value' объекта от 'value' объекта-владельца
			    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, objects b SET a.value=a.value-b.value WHERE a.id=%d AND b.id=%d", th_owner_id, object_id);
				mysql_query(gMysql, request);
			}
		}
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
	
	mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET pl_owner_id=NULL,th_owner_id=NULL,is_dropped=NULL WHERE id=%d", object_id);
	mysql_query(gMysql, request);
}

//освободить объект от владельца (для таких заканчивающихся объектов, как патроны, колёса, двигатели и т.д.)
//освободить все объекты, зависимые от данного
//подразумевает появление объекта на свободном месте в игровом мире
public full_free_object_from_owner(object_id)
{
	new rows, fields, i, id;
	new request[512];
	new Cache:result;

	//освободим объект от владельца
	clear_all_owners(object_id);

	//находим и освобождаем всё, для чего объект является владельцем
	//не разбираются вложенные композитные объекты!!!
    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM objects WHERE th_owner_id=%d", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
	{
		for(i = 0; i < rows; ++i)
		{
			id = cache_get_field_content_int(i, "id", gMysql);
			//освободим объект от владельца
			clear_all_owners(id);
			put_object_on_free_place(id);
		}
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//размещаем объект в свободном месте (эта функция обНУЛЛяет многие поля объекта)
	put_object_on_free_place(object_id);
}

//освободить объект от владельца (для таких заканчивающихся объектов, как патроны, колёса, двигатели и т.д.)
//подразумевает появление объекта на свободном месте в игровом мире
public free_object_from_owner(object_id)
{
	//освободим объект от владельца
	clear_all_owners(object_id);
	//размещаем объект в свободном месте (эта функция обНУЛЛяет многие поля объекта)
	put_object_on_free_place(object_id);
}

stock put_object_on_free_place(object_id)
{
	new rows, fields, dot_id, obj_id, inworld_id, type_id, def_value, is_vehicle, rotatable, ret;
	new request[512];
	new Float:x, Float:y, Float:z, Float:rotx, Float:roty, Float:rotz, Float:height, Float:add_rotx, Float:add_roty;
	new Float:Z_coord, Float:Alpha, Float:Beta;
	new Cache:result;

	if(object_id <= 0)
	    return;

	//получаем высоту, тип вещи, вращение и модель объекта
    mysql_format(gMysql, request, sizeof(request), "SELECT b.posx,b.posy,b.posz,b.height,b.inworld_id,b.type_id,b.rotatable,b.def_value,b.is_vehicle,a.obj_id FROM objects a INNER JOIN things b ON a.thing_id=b.id WHERE a.id=%d LIMIT 1", object_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    print("error in put_object_on_free_place(): no such object");
	    return;
	}
	rotx = cache_get_field_content_float(0, "posx", gMysql);
	roty = cache_get_field_content_float(0, "posy", gMysql);
	rotz = cache_get_field_content_float(0, "posz", gMysql);
	height = cache_get_field_content_float(0, "height", gMysql);
	inworld_id = cache_get_field_content_int(0, "inworld_id", gMysql);
	type_id = cache_get_field_content_int(0, "type_id", gMysql);
	rotatable = cache_get_field_content_int(0, "rotatable", gMysql);
	def_value = cache_get_field_content_int(0, "def_value", gMysql);
	is_vehicle = cache_get_field_content_int(0, "is_vehicle", gMysql);
	obj_id = cache_get_field_content_int(0, "obj_id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//получаем координаты
    mysql_format(gMysql, request, sizeof(request), "SELECT a.id,a.placex,a.placey,a.placez FROM object_dot a INNER JOIN dot_type b ON a.id=b.dot_id WHERE object_id IS NULL AND b.type_id=%d ORDER BY last_time LIMIT 1", type_id);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
		//обновляем БД
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=NULL,placex=NULL,placey=NULL,placez=NULL,add_rotx=0,add_roty=0,add_rotz=0,obj_id=NULL,dot_id=NULL,pl_owner_id=NULL,th_owner_id=NULL,value=%d,unused=0 WHERE id=%d", def_value, object_id);
		mysql_query(gMysql, request);
	    return;
	}
	dot_id = cache_get_field_content_int(0, "id", gMysql);
	x = cache_get_field_content_float(0, "placex", gMysql);
	y = cache_get_field_content_float(0, "placey", gMysql);
	z = cache_get_field_content_float(0, "placez", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	//создаём саму модель
	if(is_vehicle == 1)
	{
	    for(new i = 0; i < MAX_VEHICLES; ++i)
	    {
	        if(gVeh[i][0] == object_id)
	        {
		        DestroyVehicle(i);
				for(new k = 0; k < VEHICLE_BUFFER_DATA_NUMBER; ++k)
				{
			        gVeh[i][k] = 0;
				}
		        break;
	        }
		}
		create_new_vehicle(object_id, x, y, z, rotz, inworld_id, def_value);
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET dot_id=%d,unused=0 WHERE id=%d", dot_id, object_id);
		mysql_query(gMysql, request);
	}
	else
	{
	    add_rotx = 0;
	    add_roty = 0;
	    if(rotatable)
	    {
	        Alpha = rotz;
	        //получаем углы поворота
			ret = MapAndreas_Valid_Z_Coordinate(x,y,z,Z_coord,Alpha,Beta);
			if(ret == 0)
			{
			    add_rotx = Beta;
			    add_roty = Alpha;
			}
			else
			{
			    add_rotx = 0.001;
			    add_roty = 0.001;
			}
	    }
	    if(rotatable)
			obj_id = CreateObject(inworld_id, x, y, z+height, rotx+add_rotx, roty+add_roty, rotz, OBJECT_DISTANCE);
	    else
			obj_id = CreateObject(inworld_id, x, y, z+height, rotx, roty, rotz, OBJECT_DISTANCE);
	    mysql_format(gMysql, request, sizeof(request), "UPDATE objects SET vector=%f,placex=%f,placey=%f,placez=%f,add_rotx=%f,add_roty=%f,add_rotz=0,obj_id=%d,dot_id=%d,pl_owner_id=NULL,th_owner_id=NULL,value=%d,unused=0 WHERE id=%d", VectorSize(x,y,z), x, y, z, add_rotx, add_roty, obj_id, dot_id, def_value, object_id);
		mysql_query(gMysql, request);
	}

	//обновляем БД
    mysql_format(gMysql, request, sizeof(request), "UPDATE object_dot SET object_id=%d WHERE id=%d", object_id, dot_id);
	mysql_query(gMysql, request);
}

//получить значение поля таблицы
public get_character_value(playerid, row_name[], table_name[], result[64])
{
	new num;
	new request[128];
	new Cache:res;

	if( (strcmp(table_name, "characters", true, 10) != 0) && (strcmp(table_name, "inventory", true, 9) != 0) )
	    return;

    mysql_format(gMysql, request, sizeof(request), "SELECT %s FROM %s WHERE player_id=%d", row_name, table_name, gPlayersID[playerid]);
	res = mysql_query(gMysql, request);
	mysql_store_result();

	num = mysql_num_rows();
	if(num == 0)
	{
		mysql_free_result();
		return;
	}
	else
	{
	    if(mysql_retrieve_row())
	    {
	        mysql_fetch_field_row(result,row_name,gMysql);
	    }
	}
	mysql_free_result();
	
	if(cache_is_valid(res, gMysql))
		cache_delete(res);
}

//получить значение поля таблицы
public get_vehicle_value(vehicleid, row_name[], table_name[], result[64])
{
	new num;
	new request[128];
	new Cache:res;

	if( (strcmp(table_name, "veh_data", true, 8) != 0) && (strcmp(table_name, "veh_invent", true, 10) != 0) )
	    return;

    mysql_format(gMysql, request, sizeof(request), "SELECT %s FROM %s WHERE object_id=%d", row_name, table_name, gVeh[vehicleid][0]);
	res = mysql_query(gMysql, request);
	mysql_store_result();

	num = mysql_num_rows();
	if(num == 0)
	{
		mysql_free_result();
		return;
	}
	else
	{
	    if(mysql_retrieve_row())
	    {
	        mysql_fetch_field_row(result,row_name,gMysql);
	    }
	}
	mysql_free_result();

	if(cache_is_valid(res, gMysql))
		cache_delete(res);
}

public get_thing_field(field_name[], thing[], out_data[64])
{
	new rows, fields;
	new request[512];
	new Cache:result;

	if(strlen(field_name) == 0 || strlen(thing) == 0)
	{
	    print("error in get_thing_value(): empty request");
	    return;
	}

	//получаем значение поля
    mysql_format(gMysql, request, sizeof(request), "SELECT %s FROM things WHERE name=\"%s\" LIMIT 1", field_name, thing);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	{
	    print("error in get_thing_value(): no such thing");
	    printf("%s",request);
	    return;
	}
	cache_get_field_content(0, field_name, out_data, gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

public set_character_cell_value(playerid, cell, value)
{
	new request[512];

    mysql_format(gMysql, request, sizeof(request), "UPDATE objects a, inventory b SET a.value=%d WHERE a.id=b.inv%d AND b.player_id=%d", value, cell+1, gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

public get_character_state(playerid, &health, &hunger, &thirst, &wound)
{
	new rows, fields;
	new request[512];
	new Cache:result;

	//получаем высоту, тип вещи, вращение и модель объекта
    mysql_format(gMysql, request, sizeof(request), "SELECT health,hunger,thirst,wound FROM characters WHERE player_id=%d LIMIT 1", gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return 0;
	health = cache_get_field_content_int(0, "health", gMysql);
	hunger = cache_get_field_content_int(0, "hunger", gMysql);
	thirst = cache_get_field_content_int(0, "thirst", gMysql);
	wound = cache_get_field_content_int(0, "wound", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	return 1;
}

public set_character_state(playerid, health, hunger, thirst, wound)
{
	new request[512];

	//получаем высоту, тип вещи, вращение и модель объекта
    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET health=%d,hunger=%d,thirst=%d,wound=%d WHERE player_id=%d LIMIT 1", health, hunger, thirst, wound, gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

public get_character_health(playerid)
{
	new result[64];
	
	strdel(result, 0, sizeof(result));
    get_character_value(playerid,"health","characters",result);
	return strval(result);
}

public set_character_health(playerid, health)
{
	new request[128];

    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET health=%d WHERE player_id=%d LIMIT 1", (health>MAX_HEALTH_VALUE)?MAX_HEALTH_VALUE:health, gPlayersID[playerid]);
	mysql_query(gMysql, request);
	
	update_sensor_health(playerid, health);
}

public get_character_hunger(playerid)
{
	new result[64];

	strdel(result, 0, sizeof(result));
    get_character_value(playerid,"hunger","characters",result);
	return strval(result);
}

public set_character_hunger(playerid, hunger)
{
	new request[128];

    mysql_format(gMysql, request, sizeof(request), "update characters set hunger=%d where player_id=%d LIMIT 1", (hunger>MAX_HUNGER_VALUE)?MAX_HUNGER_VALUE:hunger, gPlayersID[playerid]);
	mysql_query(gMysql, request);

	update_sensor_hunger(playerid, hunger);
}

public get_character_thirst(playerid)
{
	new result[64];

	strdel(result, 0, sizeof(result));
    get_character_value(playerid,"thirst","characters",result);
	return strval(result);
}

public set_character_thirst(playerid, thirst)
{
	new request[128];

    mysql_format(gMysql, request, sizeof(request), "update characters set thirst=%d where player_id=%d LIMIT 1", (thirst>MAX_THIRST_VALUE)?MAX_THIRST_VALUE:thirst, gPlayersID[playerid]);
	mysql_query(gMysql, request);

	update_sensor_thirst(playerid, thirst);
}

public get_character_wound(playerid)
{
	new result[64];

	strdel(result, 0, sizeof(result));
    get_character_value(playerid,"wound","characters",result);
	return strval(result);
}

public set_character_wound(playerid, wound)
{
	new request[128];

    mysql_format(gMysql, request, sizeof(request), "update characters set wound=%d where player_id=%d LIMIT 1", wound, gPlayersID[playerid]);
	mysql_query(gMysql, request);

	update_sensor_wound(playerid, wound);
}

public get_character_scores(playerid)
{
	new result[64];

	strdel(result, 0, sizeof(result));
    get_character_value(playerid,"scores","characters",result);
	return strval(result);
}

public set_character_scores(playerid, scores)
{
	new request[128];

    mysql_format(gMysql, request, sizeof(request), "update characters set scores=%d where player_id=%d LIMIT 1", scores, gPlayersID[playerid]);
	mysql_query(gMysql, request);
//	update_sensor_scores(playerid, scores);
}

public get_character_killer_name(playerid, name[64])
{
	new rows, fields;
	new request[128];
	new Cache:result;

    mysql_format(gMysql, request, sizeof(request), "SELECT a.name FROM players a INNER JOIN characters b ON a.id=b.killer_id WHERE b.player_id=%d LIMIT 1", gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows > 0)
		cache_get_field_content(0, "name", name, gMysql);
	else
	    format(name, sizeof(name), "no killer");
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

//запомнить последнего попавшего
public set_character_c_killer(playerid)
{
	new request[128];

	if(playerid < 0)
		return;

	//c_killer - последний персонаж, который нанёс урон
	if(gKiller[playerid] < 0)
	    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET c_killer=NULL WHERE player_id=%d LIMIT 1", gPlayersID[playerid]);
	else
	    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET c_killer=%d WHERE player_id=%d LIMIT 1", gKiller[playerid], gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

//запомнить убийцу
public set_character_killer(playerid)
{
	new request[128];

	if(playerid < 0)
		return;

	//c_killer - последний персонаж, который нанёс урон
	if(gKiller[playerid] < 0)
	    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET c_killer=NULL WHERE player_id=%d LIMIT 1", gPlayersID[playerid]);
	else
	    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET killer_id=%d,c_killer=NULL WHERE player_id=%d LIMIT 1", gKiller[playerid], gPlayersID[playerid]);
	mysql_query(gMysql, request);

	gKiller[playerid] = -1;
	update_statistic_data(playerid);
}

//увеличить очки персонажа
public upscore_character(db_playerid)
{
	new request[128];
	new playerid;
	
	if(db_playerid < 0)
		return;

    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET scores=scores+1 WHERE player_id=%d LIMIT 1", db_playerid);
	mysql_query(gMysql, request);

	for(playerid = 0; playerid < MAX_PLAYERS; ++playerid)
	{
	    if(gPlayersID[playerid] == db_playerid)
	    {
			gScores[playerid]++;
			SetPlayerScore(playerid, gScores[playerid]);
			update_statistic_data(playerid);
			break;
	    }
	}
}

public save_player_position(playerid)
{
	new request[256];
	new Float:x, Float:y, Float:z, Float:ang;
	
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, ang);

    mysql_format(gMysql, request, sizeof(request), "update characters set placex=%f,placey=%f,placez=%f,angle=%f where player_id=%d LIMIT 1", x, y, z, ang, gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

public load_player_position(playerid)
{
	new rows, fields;
	new request[128];
	new Float:x, Float:y, Float:z, Float:ang;
	new Cache:result;

	set_player_id(playerid);
	
    mysql_format(gMysql, request, sizeof(request), "SELECT placex,placey,placez,angle FROM characters WHERE player_id=%d LIMIT 1", gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	x = cache_get_field_content_float(0, "placex", gMysql);
	y = cache_get_field_content_float(0, "placey", gMysql);
	z = cache_get_field_content_float(0, "placez", gMysql);
	ang = cache_get_field_content_float(0, "angle", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	if(z <= 0)
		SetPlayerPosFindZ(playerid, x, y, z);
	else
		SetPlayerPos(playerid, x, y, z);

	SetPlayerFacingAngle(playerid, ang);
	SetCameraBehindPlayer(playerid);
	
	//для античита
	gNonCheaters[playerid][0] = x;
	gNonCheaters[playerid][1] = y;
	gNonCheaters[playerid][2] = z;
}

public save_state_and_position(playerid)
{
	new request[256];
	new Float:x, Float:y, Float:z, Float:ang;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, ang);

    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET placex=%f,placey=%f,placez=%f,angle=%f,health=%d,hunger=%d,thirst=%d,wound=%d,killer_id=%d WHERE player_id=%d LIMIT 1", x, y, z, ang, gHealth[playerid], gHunger[playerid], gThirst[playerid], gWound[playerid], gKiller[playerid], gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

public load_player_inventory(playerid, inv[6])
{
	new rows, fields;
	new request[128];
	new Cache:result;

	set_player_id(playerid);

    mysql_format(gMysql, request, sizeof(request), "SELECT inv1,inv2,inv3,inv4,inv5,inv6 FROM inventory WHERE player_id=%d LIMIT 1", gPlayersID[playerid]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	inv[0] = cache_get_field_content_int(0, "inv1", gMysql);
	inv[1] = cache_get_field_content_int(0, "inv2", gMysql);
	inv[2] = cache_get_field_content_int(0, "inv3", gMysql);
	inv[3] = cache_get_field_content_int(0, "inv4", gMysql);
	inv[4] = cache_get_field_content_int(0, "inv5", gMysql);
	inv[5] = cache_get_field_content_int(0, "inv6", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
}

public load_vehicle_inventory(vehicleid, inv[MAX_INVENTORY_ON_VEHICLE], obj[MAX_INVENTORY_ON_VEHICLE])
{
	new i, k, rows, fields;
	new request[128];
	new column[16];
	new Cache:result;

//	set_vehicle_id(vehicleid);

	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
	    inv[i] = -1;
	    obj[i] = -1;
	}

    mysql_format(gMysql, request, sizeof(request), "SELECT inv1,inv2,inv3,inv4,inv5,inv6,inv7,inv8,inv9,inv10,inv11,inv12 FROM veh_invent WHERE object_id=%d LIMIT 1", gVeh[vehicleid][0]);
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	for(k = 0; k < MAX_INVENTORY_ON_VEHICLE; ++k)
	{
	    format(column, sizeof(column), "inv%d", k+1);
		inv[k] = cache_get_field_content_int(0, column, gMysql);
	}
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
	
	for(i = 0; i < MAX_INVENTORY_ON_VEHICLE; ++i)
	{
		if(inv[i] > 0)
		{
		    mysql_format(gMysql, request, sizeof(request), "SELECT a.invent_id FROM things a INNER JOIN objects b ON a.id=b.thing_id WHERE b.id=%d LIMIT 1", inv[i]);
			result = mysql_query(gMysql, request);
			cache_get_data(rows, fields, gMysql);
			if(rows > 0)
				obj[i] = cache_get_field_content_int(0, "invent_id", gMysql);
			else
			    print("error in load_vehicle_inventory()");
			if(cache_is_valid(result, gMysql))
				cache_delete(result);
		}
		else
		    obj[i] = -1;
	}
}

#if defined LOAD_DEBUG
public update_player_position(playerid)
{
	new request[256];
	new Float:x, Float:y, Float:z, Float:ang;
	
	x = -1420.64;
	y = -2897.94;
	z = 48.0911;
	ang = 38.8824;

    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET placex=%f,placey=%f,placez=%f,angle=%f WHERE player_id=%d LIMIT 1", x, y, z, ang, gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

public add_spawn_place(playerid)
{
	new request[128];
	new Float:x, Float:y, Float:z, Float:ang;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, ang);

    mysql_format(gMysql, request, sizeof(request), "insert into spawns(placex, placey, placez, angle) values(%f,%f,%f,%f)", x, y, z, ang);
	mysql_query(gMysql, request);
}

//добавить новую точку появления вещи
public add_dot_place(playerid, type)
{
	new rows, fields, id;
	new request[128];
	new Float:x, Float:y, Float:z;
	new Cache:result;

	GetPlayerPos(playerid, x, y, z);

    mysql_format(gMysql, request, sizeof(request), "insert into object_dot(placex, placey, placez) values(%f,%f,%f)", x, y, z);
	mysql_query(gMysql, request);
	
	if(type == 0)
		return;

    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM object_dot ORDER BY id DESC LIMIT 1");
	result = mysql_query(gMysql, request);
	mysql_store_result();
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return;
	id = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

    mysql_format(gMysql, request, sizeof(request), "insert into dot_type(dot_id, type_id) values(%d,%d)", id, type);
	mysql_query(gMysql, request);
}

//добавить тип вещи к последней созданной точке
public upd_dot_place(type)
{
	new rows, fields, id;
	new request[128];
	new Cache:result;

    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM object_dot ORDER BY id DESC LIMIT 1");
	result = mysql_query(gMysql, request);
	mysql_store_result();
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return;
	id = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

	if(type < 0)
	{
	    //удаляем тип
	    mysql_format(gMysql, request, sizeof(request), "DELETE FROM dot_type WHERE dot_id=%d AND type_id=%d LIMIT 1", id, -type);
		mysql_query(gMysql, request);
	}
	else
	{
	    //добавляем тип
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO dot_type(dot_id, type_id) VALUES(%d,%d)", id, type);
		mysql_query(gMysql, request);
	}
}

//добавить объект в игровой мир
public add_objects_to_gm(type, dup)
{
	new request[128];

	if(type < 0 || dup <= 0)
	{
	    return;
	}

	for(new i = 0; i < dup; ++i)
	{
	    //добавляем объекты
	    mysql_format(gMysql, request, sizeof(request), "INSERT INTO objects(thing_id,next_id) VALUES(%d,%d)", type, type);
		mysql_query(gMysql, request);
	}
}

public add_car_place(playerid)
{
	new rows, fields, id;
	new request[128];
	new Float:x, Float:y, Float:z;
	new Cache:result;

	GetPlayerPos(playerid, x, y, z);

    mysql_format(gMysql, request, sizeof(request), "insert into object_dot(placex, placey, placez) values(%f,%f,%f)", x, y, z);
	mysql_query(gMysql, request);

    mysql_format(gMysql, request, sizeof(request), "SELECT id FROM object_dot ORDER BY id DESC LIMIT 1");
	result = mysql_query(gMysql, request);
	mysql_store_result();
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return;
	id = cache_get_field_content_int(0, "id", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);

    mysql_format(gMysql, request, sizeof(request), "insert into dot_type(dot_id, type_id) values(%d,11)", id);
	mysql_query(gMysql, request);
}

public go_to_thing_place(playerid, thing_type[])
{
	new rows, fields;
	new request[256];
	new Float:x, Float:y, Float:z;
	new Cache:result;

    mysql_format(gMysql, request, sizeof(request), "SELECT a.placex,a.placey,a.placez FROM objects a INNER JOIN (things b, thing_type c) ON (a.thing_id=b.id AND b.type_id=c.id) WHERE c.name='%s' ORDER BY RAND() DESC LIMIT 1", thing_type);
	result = mysql_query(gMysql, request);
	mysql_store_result();
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return;
	x = cache_get_field_content_float(0, "placex", gMysql);
	y = cache_get_field_content_float(0, "placey", gMysql);
	z = cache_get_field_content_float(0, "placez", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
		
	SetPlayerPos(playerid, x+1, y+1, z+2);
}

#endif

public get_spawn_place(playerid)
{
	new rows, fields, id;
	new request[256];
	new Float:x, Float:y, Float:z, Float:ang;
	new Cache:result;


    mysql_format(gMysql, request, sizeof(request), "SELECT id,placex,placey,placez,angle FROM spawns ORDER BY last_time LIMIT 1");
	result = mysql_query(gMysql, request);
	cache_get_data(rows, fields, gMysql);
	if(rows == 0)
	    return;
	id = cache_get_field_content_int(0, "id", gMysql);
	x = cache_get_field_content_float(0, "placex", gMysql);
	y = cache_get_field_content_float(0, "placey", gMysql);
	z = cache_get_field_content_float(0, "placez", gMysql);
	ang = cache_get_field_content_float(0, "angle", gMysql);
	if(cache_is_valid(result, gMysql))
		cache_delete(result);
	
	strdel(request, 0, sizeof(request));
	mysql_format(gMysql, request, sizeof(request), "UPDATE spawns SET last_time=NOW() WHERE id=%d", id);
	mysql_query(gMysql, request);
	
    mysql_format(gMysql, request, sizeof(request), "UPDATE characters SET placex=%f,placey=%f,placez=%f,angle=%f WHERE player_id=%d LIMIT 1", x, y, z, ang, gPlayersID[playerid]);
	mysql_query(gMysql, request);
}

//совственно обезглавить персонаж
public kill_character(playerid)
{
	destroy_sensors(playerid);
	SetPlayerHealth(playerid, 0.0);
}

