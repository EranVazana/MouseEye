#include "CaptureDevice.h"

//-Constructor.
CaptureDevice::CaptureDevice() {
	//-Init device.
	init();

	//-Get device resolution values.
	_resolution = Resolution((unsigned int)_capture_device.get(cv::CAP_PROP_FRAME_WIDTH),
							 (unsigned int)_capture_device.get(cv::CAP_PROP_FRAME_HEIGHT));

	_mirror_frame = true;
}

//-Custom resolution constuctor.
CaptureDevice::CaptureDevice(Resolution resolution) {
	//-Init device.
	init();

	_resolution = resolution;
	_capture_device.set(cv::CAP_PROP_FRAME_WIDTH, resolution.width);
	_capture_device.set(cv::CAP_PROP_FRAME_WIDTH, resolution.height);

	_mirror_frame = true;
}

//-Destructor.
CaptureDevice::~CaptureDevice() {
	_capture_device.release();
}

// Output: frame from the capture device.
//-Retriving frame from the capture device, mirror the image(unless the user told not to) for better tracking results.
//-Throws exception if the capture device disconnected.
cv::Mat CaptureDevice::retriveFrame() {
	cv::Mat new_frame;

	//-Capture frame from the capture device.
	if (!_capture_device.retrieve(new_frame))
		throw DeviceDisconnectedException();

	//-Mirror the image.
	if (_mirror_frame)
		cv::flip(new_frame, new_frame, 1);

	return new_frame;
}

//-Set the capture device to mirror the frames.
void CaptureDevice::mirrorFrame() {
	_mirror_frame = !_mirror_frame;
}

//-Try opening X amount of devices until one is open.
//-If no device was found, throw an exception to the main function.
void CaptureDevice::init() {
	const unsigned int max_devies = 10;
	for (unsigned int device = 0; device < max_devies; ++device) {
		_capture_device.open(device);
		if (_capture_device.isOpened())
			return;
		_capture_device.release();
	}

	//-Throw exception if no device is open.
	throw NoDeviceFoundException();
}