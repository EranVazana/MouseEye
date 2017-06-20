#pragma once
#include <string>
#include <vector>

#include <Windows.h>
#include "sqlite3.h"

#include "Errors_And_Warnings.h"

using namespace std;

//-Constants vars for the database.
namespace DatabaseConstants {

	//-Default values of the settings.
	enum DefaultSettingsValues {
		DEFAULT_BRIGHTNESS = 40, DEFAULT_CONTRAST = 7, DEFAULT_THRESHOLD = 60,
		DEFAULT_BLUR_KSIZE_X = 11, DEFAULT_BLUR_KSIZE_Y = 11,
		DEFAULT_EYEBROW_REMOVE = 7,
		DEFAULT_FACE_POSITION_SENSETIVITY = 10, DEFAULT_EYES_POSITION_SENSETIVITY = 7
	};

	#define BRIGHTNESS_NAME "Brightness"
	#define CONTRAST_NAME "Contrast"
	#define THRESHOLD_NAME "Threshold"
	#define BLUR_KSIZE_X_NAME "Blur_Ksize_X"
	#define BLUR_KSIZE_Y_NAME "Blur_Ksize_Y"
	#define EYEBROW_REMOVE_NAME "Eyebrow_Remove"
	#define FACE_POSITION_SENSETIVITY_NAME "Face_Position_Sensetivity"
	#define EYES_POSITION_SENSETIVITY_NAME "Eyes_Position_Sensetivity"
}

//-Class used for working with the program Database.
class Database {
public:
	Database();
	~Database();

	int getSetting(string);
	void changeSetting(string ,int);

private:
	#define DATABASE_NAME "EyeMouseDB.db"

	typedef pair<string, int> Setting;

	sqlite3* db;

	bool checkExist();
	void openDatabase();

	void executeCommand(string, int(*)(void*, int, char**, char**) = nullptr);
	static int callbackSetting(void* ,int ,char** ,char**);

	char* getZErrMsg();
	char* getCustomMsg(string);
};