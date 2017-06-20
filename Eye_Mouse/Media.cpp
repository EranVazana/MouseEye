#include "Media.h"

//======================================================================

Mat Media::_no_eye_image = imread(NEF_IMAGE_LOCATION);
HICON Media::_program_icon = (HICON)LoadImage(NULL, PROGRAM_ICON_LOCATION, IMAGE_ICON, ICON_SIZE, ICON_SIZE, LR_LOADFROMFILE);