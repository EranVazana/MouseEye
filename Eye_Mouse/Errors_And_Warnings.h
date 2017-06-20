#pragma once

//-Static class that contains strings of errors.
class Errors_And_Warnings {
public:
	//-Database Errors:
	#define DATABASE_ERROR_MESSAGE_INTRO "--(!)An Error occurred in Database: "
	#define CANNOT_OPEN_DATABASE_ERROR "Cannot open the Database."

	//-Cascade errors:
	#define FACE_CASCADE_LOAD_ERROR "--(!)An Error occurred while loading face cascade."
	#define EYE_CASCADE_LOAD_ERROR "--(!)An Error occurred while loading eyes cascade."

	//-Video capture errors:
	#define CANNOT_OPEN_CAPTURE_DEVICE "--(!)An Error occurred while opening video capture."
	#define DEVICE_DISCONNECTED_ERROR "--(!)An Error occurred while capturing frames: Capture device disconnected."

	//-Keybored Hook errors:
	#define HOOK_SETUP_ERROR "--(!)Failed to setup the keyboard listner."

	//-Zoom Area Selctor GUI errors:
	#define ZOOM_AREA_GUI_SETUP_ERROR "--(!) An Error occurred while opening the Area Selection GUI."
};