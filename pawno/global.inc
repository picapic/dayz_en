#if defined _GLABAL_INC_
	#endinput
#endif

#define _GLOBAL_INC_

#include<a_samp>
#include"defines.inc"

new gMysql;

new gPlayerPasswordRequest[MAX_PLAYERS];
new gPlayerPasswordCheckCount[MAX_PLAYERS];

new gPlayersID[MAX_PLAYERS];

new gInventoryMenuShow[MAX_PLAYERS];
new gObjectsMenuShow[MAX_PLAYERS];
new gVehicleMenuShow[MAX_PLAYERS];

new gHideMapZone;

new Float:gNonCheaters[MAX_PLAYERS][3];

new gVehicleDataShow[MAX_PLAYERS];

new PlayerText:gTdInventory[MAX_PLAYERS][6][18]; 		//текстдравы меню инвентар€
new PlayerText:gTdObject[MAX_PLAYERS][6][18]; 			//текстдравы меню объектов
new PlayerText:gTdVehicle[MAX_PLAYERS][MAX_INVENTORY_ON_VEHICLE][18];	 		//текстдравы меню транспорта
new PlayerText:gTdMenu[MAX_PLAYERS][TD_COUNT]; 			//дополнительные текстдравы меню
new PlayerText:gTdDataVehicle[MAX_PLAYERS][TD_COUNT_VEHICLE]; 	//текстдравы данных транспорта
new PlayerText:gStatisticData[MAX_PLAYERS][TD_STATISTIC_DATA];	//текстдравы статистики игрока

new gIndex[MAX_PLAYERS][6]; 		//активный текстдрав дл€ каждой €чейки инвентар€
new gRotate[MAX_PLAYERS][6]; 		//активность €чейки (выбрана или нет)
new gIndexObject[MAX_PLAYERS][6]; 	//активный текстдрав объекта (тот, который виден)
new gRotateObject[MAX_PLAYERS][6]; 	//активна€ €чейка объекта (выбрана или нет)
new gIndexVehicle[MAX_VEHICLES][MAX_INVENTORY_ON_VEHICLE]; 	//активный текстдрав объекта (тот, который виден)
new gRotateVehicle[MAX_VEHICLES][MAX_INVENTORY_ON_VEHICLE]; 	//активна€ €чейка объекта (выбрана или нет)

new gTimerid; 			//таймер обновлени€ инвентар€
new gTimeridUpdateSensors; 	//таймер обновлени€ сенсоров
new gTimeridSaveSensors; 	//таймер сохранени€ сенсоров и позиции персонажа в Ѕƒ
new gTimeridUpdateVehicles; 	//таймер обновлени€ состо€ни€ всех авто
new gTimerNonCheaters;		//таймер поиска читеров

new gUnoccupiedUpdateTimer;	//таймер обновлени€ состо€ни€ незан€тых авто
new gUpdateWeaponTimer; //таймер обновлени€ количества патронов

new gInv[MAX_PLAYERS][6]; 	//содержит идентификатор модели в samp-мире
new gInvThing[MAX_PLAYERS][6]; 	//содержит id вещи
new gObj[MAX_PLAYERS][6]; 	//содержит идентификатор модели в samp-мире
new gObjThing[MAX_PLAYERS][6]; 	//содержит id объекта
new gVehMod[MAX_VEHICLES][MAX_INVENTORY_ON_VEHICLE]; 	//содержит идентификатор модели в samp-мире
new gVehObj[MAX_VEHICLES][MAX_INVENTORY_ON_VEHICLE]; 	//содержит id объектов инвентар€ транспорта

new gVeh[MAX_VEHICLES][VEHICLE_BUFFER_DATA_NUMBER]; //содержит данные о транспортных средствах в игре 
			   //0-id объекта
			   //1-кол-во €чеек
			   //2-двигатель
			   //3-количество топлива
			   //4-работает ли двигатель
			   //5-расход топлива
			   //6-объЄм бака

new gUnoccupiedVehData[MAX_VEHICLES]; //перечень "зан€тых" авто
new gWeaponUpdate[MAX_PLAYERS]; //перечень игроков, дл€ которых требуетс€ обновление количества патронов

new gPlayerWeapon[MAX_PLAYERS][2]; //хранит id объекта-оружи€ и количество оставшихс€ патронов

new gHealth[MAX_PLAYERS];
new gHunger[MAX_PLAYERS];
new gThirst[MAX_PLAYERS];
new gWound[MAX_PLAYERS];
new gKiller[MAX_PLAYERS];
new gScores[MAX_PLAYERS];

new gPlayerLang[MAX_PLAYERS][4]; //€зык каждого игрока
new gAllLangs[2][4]={"en","ru"};
new gLangsNumber;


#pragma unused gIndex
#pragma unused gKiller
#pragma unused gScores
#pragma unused gTdObject
#pragma unused gLangsNumber
#pragma unused gIndexObject
#pragma unused gIndexVehicle
#pragma unused gTdDataVehicle
#pragma unused gVehicleDataShow
#pragma unused gVehicleMenuShow
#pragma unused gUpdateWeaponTimer
#pragma unused gInventoryMenuShow
#pragma unused gTimeridUpdateSensors
#pragma unused gTimeridUpdateVehicles
#pragma unused gPlayerPasswordCheckCount
#pragma unused gPlayerPasswordRequest
#pragma unused gUnoccupiedUpdateTimer
#pragma unused gTimeridSaveSensors
#pragma unused gUnoccupiedVehData
#pragma unused gTimerNonCheaters
#pragma unused gObjectsMenuShow
#pragma unused gStatisticData
#pragma unused gRotateVehicle
#pragma unused gRotateObject
#pragma unused gWeaponUpdate
#pragma unused gNonCheaters
#pragma unused gTdInventory
#pragma unused gHideMapZone
#pragma unused gPlayerLang
#pragma unused gTdVehicle
#pragma unused gAllLangs
#pragma unused gTimerid
#pragma unused gTdMenu
