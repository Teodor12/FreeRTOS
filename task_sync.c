#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "rom/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

#define SAMPLES 10

SemaphoreHandle_t xSemaphore = NULL;
TaskHandle_t print_task_handler = NULL;
TaskHandle_t adc_1_task_handler = NULL;
static portMUX_TYPE spinlock = portMUX_INITIALIZER_UNLOCKED;
static volatile unsigned buffer[SAMPLES];

void IRAM_ATTR timer_0_callback(void *param)
{
    static unsigned num = 1;
    static unsigned idx = 0;
    BaseType_t task_woken = pdFALSE;
    if (idx < SAMPLES)
    {
        buffer[idx++] = num++;
    }
    if (idx >= SAMPLES)
    {

        printf("The buffer is ready\n");
        num = 1;
        idx = 0;
        vTaskNotifyGiveFromISR(print_task_handler, &task_woken);
    }
    if (task_woken)
    {
        portYIELD_FROM_ISR();
    }
}

void timer_0_setup()
{
    const esp_timer_create_args_t my_timer_args = {.callback = &timer_0_callback, .name = "Timer0"};
    esp_timer_handle_t timer_handler;
    ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 100000));
}

void print_task(void *arg)
{
    while (1)
    {

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Task entered at: %ld\n", xTaskGetTickCount());
        for (int i = 0; i < SAMPLES; i++)
        {
            printf("%d ", buffer[i]);
        }
        printf("\n");
        
        portENTER_CRITICAL(&spinlock);
        xSemaphoreGive(xSemaphore);
        portEXIT_CRITICAL(&spinlock);
        
    }
}

void IRAM_ATTR timer_1_callback(void *args)
{

    static volatile bool led_state = false;
    led_state = !led_state;
    printf("The led state is changed.\n");
    gpio_set_level(2, led_state);
}

void timer_1_setup()
{
    const esp_timer_create_args_t my_timer_args = {.callback = &timer_1_callback, .name = "Timer1"};
    esp_timer_handle_t timer_handler;
    ESP_ERROR_CHECK(esp_timer_create(&my_timer_args, &timer_handler));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handler, 1000000));
}

void adc_1_ch4_setup(){
    adc1_config_channel_atten(ADC1_CHANNEL_1, ADC_ATTEN_DB_11);//setting the input voltage range up to approx. 2.5 V
    //CALLIBRATION
    esp_adc_cal_characteristics_t adc1_chars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    //SETTING THE RESOLUTION OF THE ADC 1
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);

}

void adc_1_task(){
    while (1)
    {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);
        int adc_value = adc1_get_raw(ADC1_CHANNEL_4);
        printf("ADC Value: %d", adc_value);
        printf("\n");
        xSemaphoreGive(xSemaphore);
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
    
}

void app_main(void)
{
    gpio_pad_select_gpio(2);
    gpio_set_direction(2, GPIO_MODE_OUTPUT);

    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);

    xTaskCreatePinnedToCore(adc_1_task, "adc_1_task", 2048, NULL, 2, &adc_1_task_handler, app_cpu);
    xTaskCreatePinnedToCore(print_task, "print_task", 2048, NULL, 1, &print_task_handler, app_cpu);
    

    timer_0_setup();
    timer_1_setup();
    vTaskDelete(NULL);
}