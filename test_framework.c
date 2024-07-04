#include <stdio.h>
#include "test_framework.h"
#include "test_camera.h"
#include <gst/gst.h>

test_case_t test_table[] =
{
    {"camera", 1, driverCompatibility, &camera_instance, &init_gstreamer, &test_driver_compatibility, &deinit_gstreamer},
    {"camera", 2, firmwareFunctionality, &camera_instance, &init_gstreamer, &test_firmware_functionality, &deinit_gstreamer},
    {"camera", 3, apisAndLibraries, &camera_instance, &init_gstreamer, &test_apis_and_libraries, &deinit_gstreamer},
    {"camera", 4, autofocus, &camera_instance, &init_gstreamer, &test_autofocus, &deinit_gstreamer},
    {"camera", 5, exposureControl, &camera_instance, &init_gstreamer, &test_exposure_control, &deinit_gstreamer},
    {"camera", 6, whiteBalance, &camera_instance, &init_gstreamer, &test_white_balance, &deinit_gstreamer},
    {"camera", 7, videoBalance, &camera_instance, &init_gstreamer, &test_video_balance, &deinit_gstreamer},
    {"camera", 8, frameRate, &camera_instance, &init_gstreamer, &test_frame_rate, &deinit_gstreamer},
    {"camera", 9, lensDistortion, &camera_instance, &init_gstreamer, &test_lens_distortion, &deinit_gstreamer},
    {"camera", 10, latency, &camera_instance, &init_gstreamer, &test_latency, &deinit_gstreamer},
    {"camera", 11, rotateCamera, &camera_instance, &init_gstreamer, &test_video_rotate_camera, &deinit_gstreamer},
    {"camera", 12, powerConsumption, &camera_instance, &init_gstreamer, &test_power_consumption, &deinit_gstreamer},
    {"camera", 13, resolution, &camera_instance, &init_gstreamer, &test_resolution, &deinit_gstreamer},
    {"camera", 14, imageQuality, &camera_instance, &init_gstreamer, &test_image_quality, &deinit_gstreamer},
    {"camera", 15, stressTest, &camera_instance, &init_gstreamer, &test_stress, &deinit_gstreamer},
    {"camera", 16, camera_environmental, &camera_instance, &init_gstreamer, &test_environmental_conditions, &deinit_gstreamer},
    {"speaker", 1, NULL, NULL, NULL, NULL, NULL},
    {"speaker", 2, NULL, NULL, NULL, NULL, NULL},
    {"microphone", 1, NULL, NULL, NULL, NULL, NULL},
    {"microphone", 2, NULL, NULL, NULL, NULL, NULL},
};

const char component_list[][32] = {"camera", "speaker", "microphone"};

test_case_t* get_test_case_param(const char* component_type, unsigned int test_number)
{
    //printf("get_test_case_param %s, %d\n", component_type, test_number);
    for(long unsigned int i = 0; i < sizeof(test_table)/sizeof(test_table[0]); i++)
    {
       if(strcmp(component_type, test_table[i].component_type) == 0 && test_table[i].test_case_number == test_number)
       {
           return &test_table[i];
       }
    }
    return NULL;
}

int run_tests(const char* component_type, unsigned int test_number)
{
    test_case_t* test = get_test_case_param(component_type, test_number);
    if (test != NULL)
    {
        printf("TEST %s, %d\n", component_type, test_number);
        if(test->test_description != NULL)
            printf("%s\n",test->test_description);
        if(test->init != NULL)
            test->init(test->arg);
        if(test->run != NULL)
            test->run(test->arg);
        if(test->exit != NULL)
            test->exit(test->arg);
        return 0;
    }
   return -1;
}

// Main function with argument parsing//int argc, char *argv[]
int main(void)
{
    int ret = 0;
    int test_number = 1;
    int component_number = 0;
    char component_str[32] = {0};
    int max_components = sizeof(component_list)/sizeof(component_list[0]);
    //if (argc < 3) {
        //printf("Usage: %s <component type> <test_number(s) or 'all'>\n", argv[0]);
        //return 1;
    //}

    do
    {
        printf("\nSelect component number OR 'exit'\n");
        for(long unsigned int i = 0; i < sizeof(component_list)/sizeof(component_list[0]); i++)
        {
            printf("%ld] %s\n",i+1, component_list[i]);
        }

        scanf("%s", component_str);
        if (strcmp(component_str, "exit") == 0)
        {
            return 1;
        }
        else
        {
            component_number = atoi(component_str);
            if(component_number <= 0)
            {
                printf("Invalid Selected component: Please try again with correct number. or 'exit'\n");
                continue;
            }
        }
        component_number--;

        if(component_number < max_components)
        {
            do
            {
                char test_number_str[32] = {0};
                printf("\nSelect test case number OR 'all' OR 'exit':\n");
                for(long unsigned int i = 0; i < sizeof(test_table)/sizeof(test_table[0]); i++)
                {
                    if(strcmp(component_list[component_number], test_table[i].component_type) == 0)
                    {
                        printf("%d] %s\n", test_table[i].test_case_number, test_table[i].test_description);
                    }
                }

                scanf("%s", test_number_str);
                if (strcmp(test_number_str, "all") == 0)
                {
                    test_number = 1;
                    do
                    {
                        ret = run_tests(component_list[component_number], test_number);
                        test_number++;
                    } while (ret == 0);
                }
                else if (strcmp(test_number_str, "exit") == 0)
                {
                    break;
                }
                else
                {
                    test_number = atoi(test_number_str);
                    ret = run_tests(component_list[component_number], test_number);
                    if(ret != 0)
                    {
                        printf("Invalid test case number\n");
                    }
                }
            } while (1);
        }
        else
        {
            printf("Invalid Selected component: Please try again with correct number. or 'exit'\n");
        }
    } while(1);
    return 0;
}
