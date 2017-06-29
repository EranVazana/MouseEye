#include <opencv2\opencv.hpp>
#include "opencv2/videoio.hpp"

using namespace std;

typedef cv::Size Resolution;

//-Class used to work with the user's webcam.
class CaptureDevice {
public:
	CaptureDevice();
	CaptureDevice(Resolution);
	~CaptureDevice();

	cv::Mat retriveFrame();
	void mirrorFrame();

	Resolution _resolution;

private:
	void init();

	cv::VideoCapture _capture_device;
	bool _mirror_frame;

	//======================================================================
	//-Custom exceptions:

	class NoDeviceFoundException : public exception {
		virtual const char* what() const throw() {
			return "--(!)An Error occurred while opening video capture.";
		}
	};

	class DeviceDisconnectedException: public exception {
		virtual const char* what() const throw() {
			return "--(!)An Error occurred while capturing frames: Capture device disconnected.";
		}
	};
};