#include <gst/gst.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include "test_camera.h"


#define LAPTOP_TEST         (true)
#define SAVE_TEST_FILES     (true)
#define SAVE_TEST_FILE_EXT  ".flv"
#define LOCAL_DISPLAY       (true)

#define REMOTE_DISPLAY      (true)
#define SERVER_IP_ADDRESS  "192.168.0.189"
#define SERVER_PORT         5000

//Remote PC
//gst-launch-1.0 udpsrc port=5000 caps="application/x-rtp, media=(string)video, encoding-name=(string)JPEG, payload=(int)26" ! rtpjpegdepay ! jpegdec ! autovideosink

char format_list[][10] = {"I420", "YV12", "YUY2", "UYVY", "NV12", "NV21", "RGB", "BGR", "MJPEG"};
typedef struct
{
    int width;
    int height;
}resolution_t;
resolution_t resolution_list[] = {{320, 240}, {640, 480}, {1280, 720}, {1920, 1080}};
int frameRate_list[] = {30, 60, 24, 25};
const char* autofocus_list[] = {"Off", "On"};

char driverCompatibility[] = "Driver Compatibility Test";
char firmwareFunctionality[] = "Firmware Functionality Test";
char apisAndLibraries[] = "APIs and Libraries Test";
char autofocus[] = "Autofocus Test";
char exposureControl[] = "Exposure Control Test";
char whiteBalance[] = "White Balance Test";
char videoBalance[] = "Video Balance Test";
char frameRate[] = "Frame Rate Test";
char lensDistortion[] = "Lens Distortion Test";
char latency[] = "Latency Test";
char rotateCamera[] = "Rotate camera angle Test";
char powerConsumption[] = "Power Consumption Test";
char resolution[] = "Resolution Test";
char imageQuality[] = "Image Quality Tests (Color Accuracy, Sharpness, Noise Levels, Dynamic Range)";
char stressTest[] = "Stress Test";
char camera_environmental[] = "Environmental Tests (Temperature, Humidity, Vibration and Shock)";

camera_instance_t camera_instance;
char output_path[] = "./output/camera";
char gstreamer_command[2048] = {0};

// Initialize GStreamer
int init_gstreamer(void*arg) {
    gst_init(NULL, NULL);
}

int deinit_gstreamer(void*arg) {
    /*TO DO*/
    return 0;
}
char filename[100];

char* get_date_time(void)
{
    // Get the current date and time
    time_t rawtime;
    struct tm *timeinfo;
    static char datetime[20]; // Buffer to hold date and time string

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(datetime, sizeof(datetime), "%Y%m%d_%H%M%S", timeinfo);
    return datetime;
}

int select_video_rotate_camera(void)
{
    char rotateangle_str[32];
    const char* rotateangle_list[] = {
        "None (0 degrees)",
        "Counterclockwise 90 degrees",
        "Rotate 180 degrees",
        "Clockwise 90 degrees",
        "Horizontal flip (mirror)",
        "Upper-left to lower-right diagonal flip",
        "Vertical flip",
        "Upper-right to lower-left diagonal flip"
    };

    printf("\nSelect rotation angle:\n");
    for (long unsigned int i = 1; i <= sizeof(rotateangle_list)/sizeof(rotateangle_list[0]); i++)
    {
        printf("%ld] %s\n", i, rotateangle_list[i - 1]);
    }

    scanf("%s", rotateangle_str);
    int rotateangle_value = atoi(rotateangle_str);
    if (rotateangle_value > 0 && rotateangle_value <= sizeof(rotateangle_list)/sizeof(rotateangle_list[0]))
    {
        return rotateangle_value - 1;
    }

    return 0; // Default to no rotation
}

char * select_video_format(void)
{
    char format_str[32];
    printf("\nSelect format_list number:\n");
    for(long unsigned int i = 1; i <= sizeof(format_list)/sizeof(format_list[0]); i++)
    {
        printf("%ld] %s\n", i, format_list[i - 1]);
    }
    scanf("%s", format_str);
    int format_value = atoi(format_str);
    if(format_value > 0 && format_value <= sizeof(format_list)/sizeof(format_list[0]))
    {
        return format_list[format_value - 1];
    }

    return "YUY2";
}

resolution_t select_video_resolution(void)
{
    resolution_t ret = {640, 480};
    char resolution_str[32];
    printf("\nSelect resolution_list number:\n");
    for(long unsigned int i = 1; i <= sizeof(resolution_list)/sizeof(resolution_list[0]); i++)
    {
        printf("%ld] width %d x height %d\n", i, resolution_list[i - 1].width, resolution_list[i - 1].height);
    }
    scanf("%s", resolution_str);
    char resolution_value = atoi(resolution_str);
    if(resolution_value > 0 && resolution_value <= sizeof(resolution_list)/sizeof(resolution_list[0]))
    {
        ret = resolution_list[resolution_value - 1];
    }
    return ret;
}

int select_video_frameRate(void)
{
    char frameRate_str[32];
    printf("\nSelect frameRate_list number:\n");
    for(long unsigned int i = 1; i <= sizeof(frameRate_list)/sizeof(frameRate_list[0]); i++)
    {
        printf("%ld] %d\n", i, frameRate_list[i - 1]);
    }
    scanf("%s", frameRate_str);
    char frameRate_value = atoi(frameRate_str);
    if(frameRate_value > 0 && frameRate_value <= sizeof(frameRate_list)/sizeof(frameRate_list[0]))
    {
        return frameRate_list[frameRate_value - 1];
    }

    return 30;
}

// Function to select brightness
int select_video_brightness(void)
{
    char brightness_str[32];
    printf("\nEnter brightness level (0-100) OR Default (-1):\n");
    scanf("%s", brightness_str);
    int brightness_value = atoi(brightness_str);
    if (brightness_value >= 0 && brightness_value <= 100)
    {
        return brightness_value;
    }
    else if(brightness_value < 0)
    {
        return -1;
    }
    return 50; // Default brightness
}

// Function to select contrast
int select_video_contrast(void)
{
    char contrast_str[32];
    printf("\nEnter contrast level (0-100) OR Default (-1):\n");
    scanf("%s", contrast_str);
    int contrast_value = atoi(contrast_str);
    if (contrast_value >= 0 && contrast_value <= 100)
    {
        return contrast_value;
    }
    else if(contrast_value < 0)
    {
        return -1;
    }
    return 50; // Default contrast
}

// Function to select saturation
int select_video_saturation(void)
{
    char saturation_str[32];
    printf("\nEnter saturation level (0-100) OR Default (-1):\n");
    scanf("%s", saturation_str);
    int saturation_value = atoi(saturation_str);
    if (saturation_value >= 0 && saturation_value <= 100)
    {
        return saturation_value;
    }
    else if(saturation_value < 0)
    {
        return -1;
    }
    return 50; // Default saturation
}

// Function to select sharpness
int select_video_sharpness(void)
{
    char sharpness_str[32];
    printf("\nEnter sharpness level (0-100) OR Default (-1):\n");
    scanf("%s", sharpness_str);
    int sharpness_value = atoi(sharpness_str);
    if (sharpness_value >= 0 && sharpness_value <= 100)
    {
        return sharpness_value;
    }
    else if(sharpness_value < 0)
    {
        return -1;
    }
    return 50; // Default sharpness
}

// Function to select gamma
int select_video_gamma(void)
{
    char gamma_str[32];
    printf("\nEnter gamma level (0-100) OR Default (-1):\n");
    scanf("%s", gamma_str);
    int gamma_value = atoi(gamma_str);
    if (gamma_value >= 0 && gamma_value <= 100)
    {
        return gamma_value;
    }
    else if(gamma_value < 0)
    {
        return -1;
    }
    return 50; // Default gamma
}

// Function to select hue
int select_video_hue(void)
{
    char hue_str[32];
    printf("\nEnter hue level (0-100) OR Default (-1):\n");
    scanf("%s", hue_str);
    int hue_value = atoi(hue_str);
    if (hue_value >= 0 && hue_value <= 100)
    {
        return hue_value;
    }
    else if(hue_value < 0)
    {
        return -1;
    }
    return 50; // Default hue
}

// Function to select exposure
int select_video_exposure(void)
{
    char exposure_str[32];
    printf("\nEnter exposure value (0-100) OR Default (-1):\n");
    scanf("%s", exposure_str);
    int exposure_value = atoi(exposure_str);
    if (exposure_value >= 0 && exposure_value <= 100)
    {
        return exposure_value;
    }
    else if(exposure_value < 0)
    {
        return -1;
    }
    return 50; // Default exposure
}

// Function to select white balance
int select_video_white_balance(void)
{
    char white_balance_str[32];
    printf("\nEnter white balance temperature (2000-8000K) OR Default (-1):\n");
    scanf("%s", white_balance_str);
    int white_balance_value = atoi(white_balance_str);
    if (white_balance_value >= 2000 && white_balance_value <= 8000)
    {
        return white_balance_value;
    }
    else if(white_balance_value < 0)
    {
        return -1;
    }
    return 4000; // Default white balance temperature
}

// Function to select noise level
int select_video_noise_level(void)
{
    char noise_level_str[32];
    printf("\nEnter noise level (0-100) OR Default (-1):\n");
    scanf("%s", noise_level_str);
    int noise_level_value = atoi(noise_level_str);
    if (noise_level_value >= 0 && noise_level_value <= 100)
    {
        return noise_level_value;
    }
    else if(noise_level_value < 0)
    {
        return -1;
    }
    return 50; // Default noise level
}

// Function to select dynamic range
int select_video_dynamic_range(void)
{
    char dynamic_range_str[32];
    printf("\nEnter dynamic range (0-100) OR Default (-1):\n");
    scanf("%s", dynamic_range_str);
    int dynamic_range_value = atoi(dynamic_range_str);
    if (dynamic_range_value >= 0 && dynamic_range_value <= 100)
    {
        return dynamic_range_value;
    }
    else if(dynamic_range_value < 0)
    {
        return -1;
    }
    return 50; // Default dynamic range
}

// Function to select autofocus
int select_video_autofocus(void)
{
    char autofocus_str[32];
    printf("\nSelect autofocus mode:\n");
    for (long unsigned int i = 1; i <= sizeof(autofocus_list)/sizeof(autofocus_list[0]); i++)
    {
        printf("%ld] %s\n", i, autofocus_list[i - 1]);
    }
    scanf("%s", autofocus_str);
    int autofocus_value = atoi(autofocus_str);
    if (autofocus_value > 0 && autofocus_value <= sizeof(autofocus_list)/sizeof(autofocus_list[0]))
    {
        return autofocus_value - 1;
    }
    return 0; // Default to autofocus off
}

char * get_gstreamer_command(const char *pipeline_description, int duration_seconds, const char *filename)
{
    memset(gstreamer_command, 0, sizeof(gstreamer_command));
    snprintf(gstreamer_command, sizeof(gstreamer_command), "timeout %d gst-launch-1.0 %s ! tee name=t ", duration_seconds, pipeline_description);
    
    if (SAVE_TEST_FILES == true)
    {
        char store_cmd[1000] = {0};
        char *encoder = NULL;
        if (LAPTOP_TEST == true)
        {
            encoder = "x264enc";
        }
        else
        {
            encoder = "vpuenc_h264 ! h264parse";
        }
        snprintf(store_cmd, sizeof(store_cmd), " t. ! queue ! videoconvert ! %s ! flvmux ! filesink location=%s async=false ", encoder, filename);
        strcat(gstreamer_command, store_cmd);
    }
    if(REMOTE_DISPLAY == true)
    {
        char remote_cmd[1000] = {0};
        snprintf(remote_cmd, sizeof(remote_cmd), " t. ! queue ! videoconvert ! jpegenc ! rtpjpegpay ! udpsink host=%s port=%d ", SERVER_IP_ADDRESS, SERVER_PORT);
        strcat(gstreamer_command, remote_cmd);
    }

    if(LOCAL_DISPLAY == true)
    {
        char local_cmd[] = " t. ! queue ! videoconvert ! autovideosink ";
        strcat(gstreamer_command, local_cmd);
    }
    return gstreamer_command;
}

// Helper function to create a pipeline and run it for a short time
gboolean run_pipeline(const char *pipeline_description, int duration_seconds, const char *filename) {
    char *gstreamer_cmd = get_gstreamer_command(pipeline_description, duration_seconds, filename);
    printf("\nCMD :%s\n",gstreamer_cmd);
    int ret = system(gstreamer_cmd);

    if (ret == -1) {
        perror("system");
        return EXIT_FAILURE;
    }

    return WEXITSTATUS(ret);
    /*
    GstElement *pipeline = gst_parse_launch(select_output_mode(pipeline_description, filename), NULL);
    if (!pipeline) {
        g_printerr("Failed to create pipeline: %s\n", pipeline_description);
        return FALSE;
    }

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(pipeline);
        return FALSE;
    }

    g_usleep(duration_seconds * 1000000);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    */
    return TRUE;
}

// Driver Compatibility Test
int test_driver_compatibility(void*arg) {
    // Check if the v4l2src element is available
    GstElement *src = gst_element_factory_make("v4l2src", "src");
    if (src) {
        printf("Driver compatibility test - Pass\n");
        gst_object_unref(src);
    } else {
        printf("Driver compatibility test - Fail\n");
    }
}

// Firmware Functionality Test
int test_firmware_functionality(void*arg) {
    // Assuming firmware is part of the driver compatibility for laptop cameras
    test_driver_compatibility(NULL);
}

// APIs and Libraries Test
int test_apis_and_libraries(void*arg) {
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline("v4l2src ! videoconvert", 5, filename)) {
        printf("APIs and libraries test - Pass\n");
    } else {
        printf("APIs and libraries test - Fail\n");
    }
}

// Autofocus Test
int test_autofocus(void*arg) {
    // Test the autofocus by running the pipeline and visually inspecting
    char command[1000] = "v4l2src ! video/x-raw,framerate=30/1,";
    int autofocus = select_video_autofocus();
    char autofocus_str[512] = {0};

    if (autofocus == 0 || autofocus == 1)
    {
        sprintf(autofocus_str, "autofocus=%d", autofocus);
        strcat(command, autofocus_str);
    }
    strcat(command, " ! videoconvert");
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline(command, 15, filename)) {
        printf("Autofocus test - Placeholder (visual inspection required)\n");
    } else {
        printf("Autofocus test - Fail\n");
    }
}

// Exposure Control Test
int test_exposure_control(void*arg) {
    char command[1000] = "v4l2src ! video/x-raw,framerate=30/1 ! videoconvert ! videoflip method=0";
    int exposure = select_video_exposure();
    char exposure_str[512] = {0};

    if (exposure > 0)
    {
        sprintf(exposure_str, " ! videoconvert ! video/x-raw, exposure=%d", exposure);
        strcat(command, exposure_str);
    }
    strcat(command, " ! videoconvert");
    // Test the exposure control by running the pipeline and visually inspecting
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline(command, 5, filename)) {
        printf("Exposure control test - Placeholder (visual inspection required)\n");
    } else {
        printf("Exposure control test - Fail\n");
    }
}

// White Balance Test
int test_white_balance(void*arg) {
    char command[1000] = "v4l2src ! video/x-raw,framerate=30/1 ! videoconvert ! videoflip method=0";
    int white_balance = select_video_white_balance();
    char white_balance_str[512] = {0};

    if (white_balance > 0)
    {
        sprintf(white_balance_str, " ! videoconvert ! video/x-raw, white-balance=%d", white_balance);
        strcat(command, white_balance_str);
    }
    strcat(command, " ! videoconvert");

    // Test the white balance by running the pipeline and visually inspecting
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline(command, 5, filename))
    {
        printf("White balance test - Placeholder (visual inspection required)\n");
    }
    else
    {
        printf("White balance test - Fail\n");
    }
}

// Video Balance Test
int test_video_balance(void*arg) {
    char command[1000] = "v4l2src ! video/x-raw,framerate=30/1 ! videoconvert ! videoflip method=0";
    int brightness = select_video_brightness();
    int contrast = select_video_contrast();
    int saturation = select_video_saturation();
    int gamma = select_video_gamma();
    int hue = select_video_hue();
    int sharpness = -1; //select_video_sharpness();
    char videobalance[512] = " ! videobalance";

    if (brightness > 0)
    {
        char brightness_str[100] = {0};
        sprintf(brightness_str, " brightness=%f",brightness / 100.0);
        strcat(videobalance, brightness_str);
    }
    if (contrast > 0)
    {
        char contrast_str[100] = {0};
        sprintf(contrast_str, " contrast=%f",contrast / 100.0);
        strcat(videobalance, contrast_str);
    }
    if (saturation > 0)
    {
        char saturation_str[100] = {0};
        sprintf(saturation_str, " saturation=%f",saturation / 100.0);
        strcat(videobalance, saturation_str);
    }
    if (hue > 0)
    {
        char hue_str[100] = {0};
        sprintf(hue_str, " hue=%f",hue / 100.0);
        strcat(videobalance, hue_str);
    }
    if (gamma > 0)
    {
        char gamma_str[100] = {0};
        sprintf(gamma_str, " gamma=%f",gamma / 100.0);
        strcat(videobalance, gamma_str);
    }
    if (sharpness > 0)
    {
        char sharpness_str[100] = {0};
        sprintf(sharpness_str, " sharpness=%f",sharpness / 100.0);
        strcat(videobalance, sharpness_str);
    }
    if (brightness > 0 || contrast > 0 || saturation > 0 || hue > 0 || gamma > 0 || sharpness > 0)
    {
        strcat(command, videobalance);
    }
    strcat(command, " ! videoconvert");
    //sprintf(command, "v4l2src ! video/x-raw,framerate=30/1 ! videoconvert ! videoflip method=0 ! videobalance brightness=%f contrast=%f saturation=%f ! videoconvert",
    //    brightness / 100.0, contrast / 100.0, saturation / 100.0);

    // Test the Video balance by running the pipeline and visually inspecting
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline(command, 5, filename))
    {
        printf("Video balance test - Placeholder (visual inspection required)\n");
    }
    else
    {
        printf("Video balance test - Fail\n");
    }
}

// Frame Rate Test
int test_frame_rate(void*arg) {
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);

    int framerate = select_video_frameRate();
    char command[1000] = {0};
    sprintf(command, "v4l2src ! video/x-raw,framerate=%d/1 ! videoconvert", framerate);

    if (run_pipeline(command, 5, filename)) {
        printf("Frame rate test - Pass\n");
    } else {
        printf("Frame rate test - Fail\n");
    }
}

// Lens Distortion Test
int test_lens_distortion(void*arg) {
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    // Test the lens distortion by running the pipeline and visually inspecting
    if (run_pipeline("v4l2src ! videoconvert", 5, filename)) {
        printf("Lens distortion test - Placeholder (visual inspection required)\n");
    } else {
        printf("Lens distortion test - Fail\n");
    }
}

// Latency Test
int test_latency(void*arg) {
    struct timeval start, end;
    long mtime, seconds, useconds;    

    GstElement *pipeline = gst_parse_launch("v4l2src ! videoconvert", NULL);
    if (!pipeline) {
        printf("Latency test - Fail (pipeline creation failed)\n");
        return 0;
    }

    gettimeofday(&start, NULL);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_usleep(1000000);  // Let it run for 1 second
    gettimeofday(&end, NULL);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    printf("Latency test - Time: %ld ms\n", mtime);
    return 0;
}

// Rotate camera angle Test
int test_video_rotate_camera(void*arg) {
    // Test the Rotate camera angle by running the pipeline
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);

    int rotateangle = select_video_rotate_camera();
    char command[1000] = {0};
    sprintf(command, "v4l2src ! video/x-raw,framerate=30/1 ! videoconvert ! videoflip method=%d ! videoconvert", rotateangle);

    if (run_pipeline(command, 5, filename)) {
        printf("Rotate camera angle test - Pass\n");
    } else {
        printf("Rotate camera angle test - Fail\n");
    }
}

// Power Consumption Test
int test_power_consumption(void*arg) {
    // Power consumption requires hardware measurement tools, here we just run the pipeline
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline("v4l2src ! videoconvert", 5, filename)) {
        printf("Power consumption test - Placeholder (hardware measurement required)\n");
    } else {
        printf("Power consumption test - Fail\n");
    }
}

// Resolution Test
int test_resolution(void*arg) {
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);

    char *format = select_video_format();
    resolution_t resolution = select_video_resolution();
    int framerate = select_video_frameRate();
    char command[1000] = {0};
    sprintf(command, "v4l2src ! video/x-raw,format=%s,width=%d,height=%d,framerate=%d/1 ! videoconvert",
        format, resolution.width, resolution.height, framerate);

    if (run_pipeline(command, 5, filename)) {
        printf("Resolution test - Pass\n");
    } else {
        printf("Resolution test - Fail\n");
    }
}

// Image Quality Tests (Color Accuracy, Sharpness, Noise Levels, Dynamic Range)
int test_image_quality(void*arg) {
    // Test image quality by running the pipeline and visually inspecting
    char command[1000] = "v4l2src ! video/x-raw,framerate=30/1 ! videoconvert ! videoflip method=0";
    int noise_level = -1; //select_video_noise_level();
    int dynamic_range = -1; //select_video_dynamic_range();
    char videobalance[512] = " ! videoconvert";

    if (noise_level > 0)
    {
        char noise_level_str[100] = {0};
        sprintf(noise_level_str, " ! videonoise level=%d", noise_level);
        strcat(videobalance, noise_level_str);
    }
    if (dynamic_range > 0)
    {
        char dynamic_range_str[100] = {0};
        sprintf(dynamic_range_str, " ! videobalance dynamic-range=%f",dynamic_range / 100.0);
        strcat(videobalance, dynamic_range_str);
    }

    if (noise_level > 0 || dynamic_range > 0)
    {
        strcat(command, videobalance);
    }
    strcat(command, " ! videoconvert");
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline(command, 5, filename)) {
        printf("Color accuracy, sharpness, noise levels, dynamic range tests - Placeholder (visual inspection required)\n");
    } else {
        printf("Color accuracy, sharpness, noise levels, dynamic range tests - Fail\n");
    }
}

// Stress Test
int test_stress(void*arg) {
    // Stress test by running the pipeline for an extended period
    sprintf(filename, "%s/%s_%s%s", output_path, __FUNCTION__, get_date_time(), SAVE_TEST_FILE_EXT);
    if (run_pipeline("v4l2src ! videoconvert", 60, filename)) {
        printf("Stress test - Pass\n");
    } else {
        printf("Stress test - Fail\n");
    }
}

// Environmental Tests (Temperature, Humidity, Vibration and Shock)
int test_environmental_conditions(void*arg) {
    // Placeholder for environmental condition tests
    printf("Temperature, humidity, vibration and shock tests - Placeholder (external hardware required)\n");
}

// Automated Tests
/*
int main()
{
    char* format = select_video_format();
    char* resolution = select_video_resolution();
    int framerate = select_video_frameRate();
    int rotateangle = select_video_rotate_camera();
    int brightness = select_video_brightness();
    int contrast = select_video_contrast();
    int saturation = select_video_saturation();
    int sharpness = select_video_sharpness();
    int gamma = select_video_gamma();
    int hue = select_video_hue();
    int exposure = select_video_exposure();
    int white_balance = select_video_white_balance();

    char command[1000] = {0};
    sprintf(command, "v4l2src ! video/x-raw,format=%s,width=%d,height=%d,framerate=%d/1 ! videoconvert ! videoflip method=%d ! videobalance brightness=%f contrast=%f saturation=%f hue=%f gamma=%f ! videoconvert ! videobalance sharpness=%f ! video/x-raw, exposure=%d, white-balance=%d",
        format, atoi(resolution), atoi(resolution+5), framerate, rotateangle, brightness / 100.0, contrast / 100.0, saturation / 100.0, hue / 100.0, gamma / 100.0, sharpness / 100.0, exposure, white_balance);

    const char* filename = "output_file"; // Replace with your actual filename
    if (run_pipeline(command, 5, filename))
    {
        printf("Pipeline executed successfully.\n");
    }
    else
    {
        printf("Pipeline execution failed.\n");
    }

    return 0;
}


Explanation of Tests

    Driver Compatibility: Checks if the v4l2src element is available.
    Firmware Functionality: Uses the driver compatibility test as a proxy.
    APIs and Libraries: Runs a simple pipeline to verify GStreamer functionality.
    Autofocus, Exposure Control, White Balance: These tests require visual inspection.
    Frame Rate: Verifies the camera can handle 30 fps.
    Lens Distortion: Placeholder for visual inspection.
    Latency: Measures the time taken to start and stop a pipeline.
    Throughput: Tests data handling capacity.
    Power Consumption: Placeholder for hardware measurement.
    Resolution: Verifies the camera outputs at 1080p.
    Image Quality: Placeholder for visual inspection of various quality metrics.
    Stress: Runs the pipeline for an extended period.
    Environmental Conditions: Placeholder for external hardware testing.
*/

/*
// Function to run all tests
void run_all_tests() {
    test_driver_compatibility();
    test_firmware_functionality();
    test_apis_and_libraries();
    test_autofocus();
    test_exposure_control();
    test_white_balance();
    test_frame_rate();
    test_lens_distortion();
    test_latency();
    test_throughput();
    test_power_consumption();
    test_resolution();
    test_image_quality();
    test_stress();
    test_environmental_conditions();
}

typedef int (* state_callback)(void*arg);

typedef struct
{
    unsigned int test_case_number,
    char* component_type,
    char* vender,
    unsigned int retry_count,
    unsigned int retry_timeout,
    unsigned int state,
    unsigned int sub_state,
    void*arg,
    state_callback init,
    state_callback run,
    state_callback wait_resp,
    state_callback exit,
}test_instance_t;

test_instance_t test_table[] =
{
   {1, "camera", "webcam", 0, 0, 0, 0, NULL, NULL, NULL, NULL},
   {2, "camera", "webcam", 0, 0, 0, 0, NULL, NULL, NULL, NULL},
   {3, "speaker", "webcam", 0, 0, 0, 0, NULL, NULL, NULL, NULL},
   {4, "speaker", "XYZ", 0, 0, 0, 0, NULL, NULL, NULL, NULL},
   {5, "Microphone", "laptop", 0, 0, 0, 0, NULL, NULL, NULL, NULL},
}

test_instance_t* set_test_case_param(test_instance_t test_setup)
{
    if (get_test_case_param(test_setup.test_case_number) == NULL)
    {
        
    }
}


test_instance_t* get_test_case_param(test_number)
{
    for(int i = 0; i < sizeof(test_table)/sizeof(test_instance_t); i++)
    {
       if(test_table[i].test_case_number == test_number)
       {
           return test_table[i];
       }
    }
    return NULL;
}

int run_tests(int test_number[], int numOfTest)
{
   int ret = 0;
   unsigned int retry_count = 0,

   for(int t = 0; t < numOfTest; t++)
   {
        test_instance_t* instance = get_test_case_param(test_number[t]);

        if (test_number != NULL)
        {
            if(instance->init != NULL)
                instance->init(instance->arg);
            if(instance->run != NULL)
                instance->run(instance->arg)
            if(instance->wait_resp != NULL)
                //Start timeout timer
                timeout = 0;
                while(1)
                {
                    ret = instance->wait_resp(instance->arg)
                    if(ret != 0 && timeout == 1 && retry_count < instance->retry_count)
                    {
                        if(instance->run != NULL)
                            instance->run(instance->arg)
                        timeout = 0;
                        retry_count++;
                    }
                    else
                    {
                        break;
                    }
                };
                
            if(instance->exit != NULL)
                instance->exit()
        }

   }

   return 0;
}


// Main function with argument parsing
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <component type> <vender> <test_number(s) or 'all'>\n", argv[0]);
        return 1;
    }

    init_gstreamer();

    if (strcmp(argv[1], "all") == 0) {
        run_all_tests();
    } else {
        for (int i = 1; i < argc; i++) {
            int test_number = atoi(argv[i]);
            switch (test_number) {
                case 1:
                    test_driver_compatibility();
                    break;
                case 2:
                    test_firmware_functionality();
                    break;
                case 3:
                    test_apis_and_libraries();
                    break;
                case 4:
                    test_autofocus();
                    break;
                case 5:
                    test_exposure_control();
                    break;
                case 6:
                    test_white_balance();
                    break;
                case 7:
                    test_frame_rate();
                    break;
                case 8:
                    test_lens_distortion();
                    break;
                case 9:
                    test_latency();
                    break;
                case 10:
                    test_throughput();
                    break;
                case 11:
                    test_power_consumption();
                    break;
                case 12:
                    test_resolution();
                    break;
                case 13:
                    test_image_quality();
                    break;
                case 14:
                    test_stress();
                    break;
                case 15:
                    test_environmental_conditions();
                    break;
                default:
                    printf("Invalid test number: %d\n", test_number);
                    break;
            }
        }
    }

    return 0;
}

*/
