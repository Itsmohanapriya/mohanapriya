#ifndef BLACK_H
#define BLACK_H

#include <xc.h>

typedef enum
{
            dashboard,
            main_menu,
            viewlog,
            downloadlog,
            clearlog,
            setlog
}pos_t;
    extern pos_t state;
    
    void view_dashboard(void);
    
    void view_menu(void);
    
    void view_log(void);
    
    void clear_log(void);
    
    void download_log(void);
    
    void set_log(void);
    
    void read_event();
    
    void print_clcd();

#endif