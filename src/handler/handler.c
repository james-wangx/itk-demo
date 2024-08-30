#include <epm/epm.h>
#include <tc/tc_startup.h>
#include <tccore/custom.h>
#include <ict/ict_userservice.h>

static int test(EPM_action_message_t msg)
{
    printf("\nStart action: test");

    // do something

    printf("\nEnd action: test");

    return 0;
}

extern DLLAPI int handler_register_handlers(int* decision, va_list args)
{
    *decision = ALL_CUSTOMIZATIONS;

    if (EPM_register_action_handler("test", "test", test) != ITK_ok)
    {
        printf("register test fail!");
        return -1;
    }
        
    printf("register test success!");

    return ITK_ok;
}

extern DLLAPI int handler_register_callbacks(void)
{
    return CUSTOM_register_exit("handler", "USER_gs_shell_init_module", (CUSTOM_EXIT_ftn_t)handler_register_handlers);
}
