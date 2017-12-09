#pragma once

#include "stdafx.h"
#include "ScreenManager.h"

class BatteryManager {
public:
	BatteryManager();
	~BatteryManager();

	BOOL getBatteryInfo();
	BOOL bACLineMode;										//���������� � ���� ��� � �������
	ScreenManager screenManager;							

	SYSTEM_POWER_STATUS systemPowerStatus;
	DWORD dwBatteryLifePercent;
	DWORD dwBatteryLifeTime;
	DWORD dwBatteryLifeTimeMinutes;
	DWORD dwBatteryLifeTimeHours;

	BOOL  bCurrentlyCharging;								//� ������ ������ ���������� ��� ���

	void  calculateLifeTime();
};

BatteryManager::BatteryManager() {
	ZeroMemory(&systemPowerStatus, sizeof(systemPowerStatus));
	bACLineMode = FALSE;
	bCurrentlyCharging = FALSE;

	screenManager.getMonitorBrightness();
	screenManager.getPowerPolicy();
}

BatteryManager::~BatteryManager() {

}

BOOL BatteryManager::getBatteryInfo() {
	ZeroMemory(&systemPowerStatus, sizeof(systemPowerStatus));

	GetSystemPowerStatus(&systemPowerStatus);					//�������� ������� ������ ��������������

	bACLineMode = systemPowerStatus.ACLineStatus;
	dwBatteryLifeTime = systemPowerStatus.BatteryLifeTime;
	dwBatteryLifePercent = systemPowerStatus.BatteryLifePercent;

	if (dwBatteryLifePercent < FULL_CHARGE_PERCENTS) {
		bCurrentlyCharging = TRUE;
	}
	else {
		bCurrentlyCharging = FALSE;
	}

	if (bACLineMode == FALSE) {
		calculateLifeTime();
	}

	return TRUE;
}

void BatteryManager::calculateLifeTime() {
	if (bACLineMode == FALSE) {
		dwBatteryLifeTimeHours = dwBatteryLifeTime / 3600;
		dwBatteryLifeTimeMinutes = dwBatteryLifeTime / 60 - dwBatteryLifeTimeHours * 60;
	}
}


