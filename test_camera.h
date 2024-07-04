#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct test_camera
{
    /* data */
}camera_instance_t;


extern char driverCompatibility[];
extern char firmwareFunctionality[];
extern char apisAndLibraries[];
extern char autofocus[];
extern char exposureControl[];
extern char whiteBalance[];
extern char videoBalance[];
extern char frameRate[];
extern char lensDistortion[];
extern char latency[];
extern char rotateCamera[];
extern char powerConsumption[];
extern char resolution[];
extern char imageQuality[];
extern char stressTest[];
extern char camera_environmental[];

extern camera_instance_t camera_instance;

int init_gstreamer(void*arg);
int deinit_gstreamer(void*arg);
int test_driver_compatibility(void*arg);
int test_firmware_functionality(void*arg);
int test_apis_and_libraries(void*arg);
int test_autofocus(void*arg);
int test_exposure_control(void*arg);
int test_white_balance(void*arg);
int test_video_balance(void*arg);
int test_frame_rate(void*arg);
int test_lens_distortion(void*arg);
int test_latency(void*arg);
int test_video_rotate_camera(void*arg);
int test_power_consumption(void*arg);
int test_resolution(void*arg);
int test_image_quality(void*arg);
int test_stress(void*arg);
int test_environmental_conditions(void*arg);

#endif // TEST_CAMERA_H