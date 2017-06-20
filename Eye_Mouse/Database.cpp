#include "Database.h"

//======================================================================

char* zErrMsg = nullptr;

string _returnValue;

//======================================================================

using namespace DatabaseConstants;

//-Contructor.
Database::Database(){
	if (!checkExist()){ //-If the Database doesn't exists, create it and create the tables.
		openDatabase();

		//-Create settings table.
		executeCommand("create table user_settings(setting_name text not null, setting_value integer not null)");

		//-Init settings.
		vector<Setting> settings = { Setting(BRIGHTNESS_NAME, DEFAULT_BRIGHTNESS), Setting(CONTRAST_NAME, DEFAULT_CONTRAST), Setting(THRESHOLD_NAME, DEFAULT_THRESHOLD),
									 Setting(BLUR_KSIZE_X_NAME, DEFAULT_BLUR_KSIZE_X), Setting(BLUR_KSIZE_Y_NAME, DEFAULT_BLUR_KSIZE_Y),
									 Setting(EYEBROW_REMOVE_NAME, DEFAULT_EYEBROW_REMOVE),
									 Setting(FACE_POSITION_SENSETIVITY_NAME, DEFAULT_FACE_POSITION_SENSETIVITY), Setting(EYES_POSITION_SENSETIVITY_NAME, DEFAULT_EYES_POSITION_SENSETIVITY) };

		for (unsigned int i = 0; i < settings.size(); ++i){
			string sqlCommand = "INSERT INTO user_settings(setting_name,setting_value) VALUES('" + settings[i].first + "','" + to_string(settings[i].second) + "')";
			executeCommand(sqlCommand);
		}
	}
	else
		openDatabase(); //-Else, just open it.
}

//-Destructor.
Database::~Database(){
	sqlite3_close(db);
}

// Input: Setting name.
// Output: Value of the setting in the database.
int Database::getSetting(string setting_name){
	_returnValue.clear();

	string sqlCommand = "SELECT setting_value FROM user_settings WHERE setting_name = '" + setting_name + "'";
	executeCommand(sqlCommand, callbackSetting);

	if (_returnValue.empty())
		throw exception(getCustomMsg(string("The setting '" + setting_name + "' wasn't found.")));

	return stoi(_returnValue);
}

// Input: Setting name and a new value.
//-Changes the setting value in the database.
void Database::changeSetting(string setting_name, int setting_value){
	string sqlCommand = "UPDATE user_settings SET setting_value = " + to_string(setting_value) + " WHERE setting_name = '" + setting_name + "'";
	executeCommand(sqlCommand);
}

// Output: True if the database file exist, false if it doesn't.
//-Check if the file exists without opening it.
bool Database::checkExist() {
	struct stat buffer;
	return (stat(DATABASE_NAME, &buffer) == 0);
}

//-Open the database file.
void Database::openDatabase() {
	int rc = sqlite3_open(DATABASE_NAME, &db);

	if (rc) {
		sqlite3_close(db);
		throw exception(getCustomMsg(CANNOT_OPEN_DATABASE_ERROR));
	}
}

// Input: Command, callback function (can be null).
//-Execute the command and throws an exception if the command returns an error.
void Database::executeCommand(string sqlCommand, int(*callback)(void*, int, char**, char**)) {
	int rc = sqlite3_exec(db, sqlCommand.c_str(), callback, nullptr, &zErrMsg);

	if (rc != SQLITE_OK)
		throw exception(getZErrMsg());
}

//-Callback function.
int Database::callbackSetting(void* notUsed, int argc, char** argv, char** azCol){
	_returnValue = string(argv[0]);
	return SQLITE_OK;
}

// Output: Returns the full zErrMsg.
char* Database::getZErrMsg() {
	char* msg = new char[strlen(DATABASE_ERROR_MESSAGE_INTRO) + strlen(zErrMsg) + 1];

	strcat(msg, DATABASE_ERROR_MESSAGE_INTRO);
	strcat(msg, zErrMsg);

	return msg;
}

// Output: Returns the full custom message.
char* Database::getCustomMsg(string s) {
	const char* c_string = s.c_str();
	char* msg = new char[strlen(DATABASE_ERROR_MESSAGE_INTRO) + strlen(c_string) + 1];

	strcat(msg, DATABASE_ERROR_MESSAGE_INTRO);
	strcat(msg, c_string);

	return msg;
}