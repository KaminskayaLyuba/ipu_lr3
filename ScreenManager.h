#pragma once

#include "stdafx.h"
#include <ntddvdeo.h>

class ScreenManager {
public:
	ScreenManager();
	~ScreenManager();

	void getMonitorBrightness();							
	void setMonitorBrightness(DWORD);
	void restoreMonitorBrightness();

	void getPowerPolicy();
	void setPowerPolicy(DWORD);
	void restorePowerPolicy();

	HANDLE hMonitor;
	DWORD  dwOldBrightness;
	DWORD  dwOldScreenTimeout;

	SYSTEM_POWER_POLICY powerPolicy;							//политика управлени€ питанием
	DISPLAY_BRIGHTNESS  displayBrightness;						//€ркость экрана
};

ScreenManager::ScreenManager() {

	hMonitor = CreateFileA("//./LCD",							
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
}

ScreenManager::~ScreenManager() {
	CloseHandle(hMonitor);
}

void ScreenManager::getMonitorBrightness() {
	BOOL  bResult;
	DWORD dwBytesReturned;

	bResult = DeviceIoControl(hMonitor,						//получить текущие параметры €ркости экрана
		IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS,
		NULL, 0, &displayBrightness,
		sizeof(displayBrightness),
		&dwBytesReturned, NULL);

	dwOldBrightness = displayBrightness.ucDCBrightness;
}

void ScreenManager::getPowerPolicy() {
	if (CallNtPowerInformation(SystemPowerPolicyDc, NULL, 0, &powerPolicy, sizeof(powerPolicy)) == STATUS_SUCCESS)			//получить инфорамцию о электропитании
	{
		dwOldScreenTimeout = powerPolicy.VideoTimeout;
	}
}

void ScreenManager::setMonitorBrightness(DWORD brightnessLevel) {
	BOOL  bResult;
	DWORD dwBytesReturned;

	displayBrightness.ucACBrightness = (UCHAR)brightnessLevel;
	displayBrightness.ucDCBrightness = (UCHAR)brightnessLevel;

	bResult = DeviceIoControl(hMonitor,
		IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS,
		&displayBrightness, sizeof(displayBrightness),
		NULL, 0, &dwBytesReturned, NULL);
	if (bResult == FALSE) {
//		cout << "Screen::setMonitorBrightness(DWORD) error: " << GetLastError() << endl;
	}
}

void ScreenManager::setPowerPolicy(DWORD seconds) {
	NTSTATUS statusResult;
	powerPolicy.VideoTimeout = seconds;
	statusResult = CallNtPowerInformation(SystemPowerPolicyDc, &powerPolicy, sizeof(powerPolicy), NULL, 0);
	if (statusResult != STATUS_SUCCESS) {
//		cout << "Screen::getPowerPolicy() error: " << GetLastError() << endl;
	}
}

void ScreenManager::restorePowerPolicy() {
	setPowerPolicy(dwOldScreenTimeout);
}

void ScreenManager::restoreMonitorBrightness() {
	setMonitorBrightness(dwOldBrightness);
}
