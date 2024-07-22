#include <map>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_console.h"
#include "esp_log.h"

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

    static const ConsoleCommand Cmd_GetPCDVersion("get-pcd-version", "get PCD firmware version", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "NFC Get PCD version ...");
            NFC_GetVersion();

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