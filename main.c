#include <stdio.h>
#include <string.h>
#include "WheelTimer.h"
#include "LinkedListApi.h"

void
print_(char *str){
    printf("%s\n", str);
}
void wrapper_print(void *arg, int arg_size){
  char *str = (char*)arg;
  print_(str);
}

int
main(int argc, char **argv){
  timer_t *wt = init_wheel_timer(); //initialize wall clock timer
  /* Register the events to be triggered with Wheel timer now. */
  wheel_timer_elem_t *wt_elem = register_app_event(wt, wrapper_print, "ben_event", strlen("ben_event"),12633, 1);
  wt_elem = register_app_event(wt, wrapper_print, "Oana_event", strlen("Oana_event"), 5, 1);  // special case when ben_event and Oana_event is triggered at 15th slot!
  start_wheel_timer(wt); /* Wheel timer has started running in a separte thread. */

  scanf("\n");
  return 0;
}
