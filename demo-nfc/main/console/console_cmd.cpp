#include <map>
#include <vector>

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
    static const ConsoleCommand Dummy("dum", "just a dum cmd", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "dummy cmd completed");

            return 0; 
        }
    );

    static const ConsoleCommand NFC_INIT("nfc-init", "init nfc", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "NFC starting ...");
            NFC_Run();

            return 0; 
        }
    );

    static const ConsoleCommand GET_PCD_VER("get-pcd-version", "get PCD firmware version", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "NFC Get PCD version ...");
            NFC_GetVersion();

            return 0; 
        }
    );
    
    static const ConsoleCommand NFC_READ_E2P("read-version", "read firmware version by eep", no_args,
        [&](ConsoleCommand *c){
            ESP_LOGI(TAG, "NFC Read PCD version ...");
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