#include <map>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_console.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "Board_Pn5180_CustomDev.h"

#include "nfc.h"

#include "console_helper.hpp"
#include "console_cmd.hpp"

static const char *TAG = "Console";
static esp_console_repl_t *s_repl = nullptr;

Console *Console::single = nullptr;

static void registerCustomCommands()
{
    const std::vector<CommandArgs> no_args;
    static const ConsoleCommand Cmd_Testing("test", "testing cmd", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "testing cmd completed");

            return 0; 
        }
    );

    const struct GpioOutput {
        GpioOutput(): pinNum(INT1, nullptr, nullptr, "<pinNum>", "pin number"), 
                    level(INT1, nullptr, nullptr, "<level>", "pin output"){}
        CommandArgs pinNum;
        CommandArgs level;
    } led_state_args;
    static const ConsoleCommand Cmd_ToggleLED("pin-set", "set pin output level", &led_state_args, sizeof(led_state_args),
        [&](ConsoleCommand *c){
            gpio_num_t pinNum = (gpio_num_t)(c->get_int_of(&GpioOutput::pinNum));
            auto level = c->get_int_of(&GpioOutput::level)>0?true:false;
            ESP_LOGI(TAG, "set GPIO-%d = %d", pinNum, level);
            gpio_set_level(pinNum, level);
            return 0; 
        }
    );    
    
    const struct PN5180ConfigArgs {
        PN5180ConfigArgs(): config(INT1, nullptr, nullptr, "<config>", "config register"), 
                            value(INT1, nullptr, nullptr, "<value>", "config value"){}
        CommandArgs config;
        CommandArgs value;
    } args_pn5180_register;
    static const ConsoleCommand Cmd_SetN5180Config("config", "set config register", &args_pn5180_register, sizeof(args_pn5180_register),
        [&](ConsoleCommand *c){
            auto config = c->get_int_of(&PN5180ConfigArgs::config);
            auto value = c->get_int_of(&PN5180ConfigArgs::value);

            NFC_UpdateConfig(config, value);
            return 0; 
        }
    );
    
    static const ConsoleCommand Cmd_SoftRestart("restart", "reboot esp cmd", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "Rebooting esp ....");
	        esp_restart();
            return 0; 
        }
    );    
    
    static const ConsoleCommand Cmd_GetMem("mem", "memory info", no_args,
        [&](ConsoleCommand *c){
            uint32_t free_dram = esp_get_free_heap_size();
            uint32_t free_iram = heap_caps_get_free_size(MALLOC_CAP_INTERNAL) - free_dram;
            uint32_t free_mini_heap = heap_caps_get_minimum_free_size(MALLOC_CAP_DEFAULT);
            printf("free memory=%d (DRAM/heap), %d (IRAM)\n", free_dram, free_iram);
            printf("free mini. heap=%d\n", free_mini_heap);

            return 0; 
        }
    );    
    
    static const ConsoleCommand Cmd_RTOS_GetTask("rtos-get-task", "rtos task", no_args,
        [&](ConsoleCommand *c){
            const size_t bytes_per_task = 60;//40; /* see vTaskList description */
            char *task_list_buffer = (char*) malloc(uxTaskGetNumberOfTasks() * bytes_per_task);
            if (task_list_buffer)
            {
                vTaskList(task_list_buffer);
                fputs("Task Name\tStatus\tPrio\tHWM\tTask#", stdout);
        #ifdef CONFIG_FREERTOS_VTASKLIST_INCLUDE_COREID
                fputs("\tAffinity", stdout);
        #endif
                fputs("\n", stdout);
                fputs(task_list_buffer, stdout);
                free(task_list_buffer);
            }
            return 0; 
        }
    );

    static const ConsoleCommand Cmd_NfcInit("nfc-init", "init nfc", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "NFC starting ...");
            NFC_Run();

            return 0; 
        }
    );

    static const ConsoleCommand Cmd_NfcIrq("irq-nfc", "send irq to nfc", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "IRQ NFC ...");
            NFC_SendISR();

            return 0; 
        }
    );

    static const ConsoleCommand Cmd_ReadVerByEEP("read-version", "read firmware version by eep", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "NFC Read PCD version eep ...");
            NFC_ReadE2Prom_Version();
            return 0; 
        }
    );
}

Console::Console()
{
    // init console REPL environment
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_config.task_priority = 0;
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &s_repl));

    registerCustomCommands();

    // start console REPL
    ESP_ERROR_CHECK(esp_console_start_repl(s_repl));
    // ESP_LOGI(TAG, "constructor");
}

Console::~Console()
{
    s_repl->del(s_repl);

    // ESP_LOGI(TAG, "~deconstructor");
}

void Console::start()
{
    if (single == nullptr)
    {
        single = new Console();
    }
}

/* Wrapping function for C domain */
#ifdef __cplusplus
extern "C" void ConsoleStart(void)
{
    Console::start();
}
#endif