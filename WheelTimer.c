#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "WheelTimer.h"
#include "LinkedListApi.h"

struct _wheel_timer_elem_{
  int time_interval;
  int execute_cycle_no_hour;
  int execute_cycle_no_minute;
  int execute_cycle_no_second;
  app_call_back app_callback;
  void *arg;
  int arg_size;
  char is_recurrence;
};

struct _wheel_timer_{
  int current_clock_tic;
  int clock_tic_interval;
  int wheel_size;
  int current_cycle_no;
  ll_t *slots[0];
};

struct _timer_{
  wheel_timer_t *hour;
  wheel_timer_t *minute;
  wheel_timer_t *second;
  pthread_t thread;
};

struct _time_value_{
  int hour;
  int minute;
  int second;
};

static void*
wheel_fn(void *arg){
  timer_t *wt = (timer_t*)arg;
  wheel_timer_elem_t *wt_elem = NULL;
  ll_t *slot_list = NULL;
  ll_node *head = NULL, *prev_node = NULL;
  while(1){
    wt->second->current_clock_tic++;
    if(wt->second->current_clock_tic > 60){
      wt->second->current_clock_tic = 1;
      wt->second->current_cycle_no++;
      wt->minute->current_clock_tic++;
      if(wt->minute->current_clock_tic > 60){
        wt->minute->current_clock_tic = 0;
        wt->minute->current_cycle_no++;
        wt->hour->current_clock_tic++;
        if(wt->hour->current_clock_tic > 24){
          wt->hour->current_clock_tic = 0;
          wt->hour->current_cycle_no++;
        }
      }
    }
    sleep(1);
    slot_list = wt->second->slots[wt->second->current_clock_tic];
    printf("Wheel Timer Time = %d hour(s), %d minute(s), %d second(s): ", wt->hour->current_clock_tic, wt->minute->current_clock_tic, wt->second->current_clock_tic);
    if(is_ll_empty(slot_list)){
      printf("\n");
    }
    ITERATE_LIST_BEGIN_ENHANCED(slot_list, head, prev_node){
      wt_elem = (wheel_timer_elem_t*)get_node_data(head);
      if(wt->hour->current_clock_tic == wt_elem->execute_cycle_no_hour){
        if(wt->minute->current_clock_tic == wt_elem->execute_cycle_no_minute){
          if(wt->second->current_clock_tic == wt_elem->execute_cycle_no_second){
            wt_elem->app_callback(wt_elem->arg, wt_elem->arg_size);     //Invoke the application event through fn pointer
            if(wt_elem->is_recurrence){ // After invocation, check if the event needs to be rescheduled again in future
              time_value_t *current_time = calloc(1, sizeof(time_value_t*));
              GET_WT_CURRENT_TIME(wt, current_time);

              int hour_cycle_no = (int)((wt_elem->time_interval/3600)) + current_time->hour;
              int min_cycle_no  = (int)(((wt_elem->time_interval - (hour_cycle_no*3600))/60)) + current_time->minute;
              if(min_cycle_no > 60){
                min_cycle_no = min_cycle_no % 60;
                hour_cycle_no++;
              }
              int sec_cycle_no = ( wt_elem->time_interval - (hour_cycle_no*3600) - (min_cycle_no*60) ) + current_time->second;
              if(sec_cycle_no > 60){
                sec_cycle_no = sec_cycle_no % 60;
                min_cycle_no++;
                if(min_cycle_no >= 60){
                  min_cycle_no = min_cycle_no % 60;
                  hour_cycle_no++;
                }
                if(hour_cycle_no >= 24){
                  hour_cycle_no = hour_cycle_no % 24;
                }
              }

              free(current_time);
            	int slot_no  = sec_cycle_no;
              wt_elem->execute_cycle_no_hour = hour_cycle_no;
              wt_elem->execute_cycle_no_minute = min_cycle_no;
              wt_elem->execute_cycle_no_second = sec_cycle_no;
              if(sec_cycle_no == wt->second->current_clock_tic){
                ITERATE_LIST_CONTINUE_ENHANCED(slot_list, head, prev_node);
              }
              ll_remove_node(slot_list, head);
              ll_add_node(wt->second->slots[slot_no], head);
            }
            else{
              free_wheel_timer_element((wheel_timer_elem_t*)get_node_data(head));
              ll_delete_node(slot_list, head);
            }
          }
        }
      }
      printf("%d\t%d\t%d\n", wt_elem->execute_cycle_no_hour, wt_elem->execute_cycle_no_minute, wt_elem->execute_cycle_no_second);
    }ITERATE_LIST_END_ENHANCED(slot_list, head, prev_node);
  }
  return NULL;
}

timer_t*
init_wheel_timer(){
  timer_t *timer = calloc(1, sizeof(timer_t));
  wheel_timer_t *wt_second = calloc(1, sizeof(wheel_timer_t) + 60*sizeof(ll_t*));
  wt_second->clock_tic_interval = 1;
  wt_second->current_clock_tic = 0;
  wt_second->current_cycle_no = 0;
  wt_second->wheel_size = 60;
  for(int i = 0; i<= 60; i++)
    wt_second->slots[i] = init_singly_ll();
  timer->second = wt_second;

  wheel_timer_t *wt_minutes = calloc(1, sizeof(wheel_timer_t));
  wt_minutes->clock_tic_interval = 60 * wt_second->clock_tic_interval;
  wt_minutes->wheel_size = 60;
  wt_minutes->current_clock_tic = 0;
  wt_minutes->current_cycle_no = 0;
  timer->minute = wt_minutes;

  wheel_timer_t *wt_hour = calloc(1, sizeof(wheel_timer_t));
  wt_hour->clock_tic_interval = 3600 * wt_second->clock_tic_interval;
  wt_hour->wheel_size = 24;
  wt_hour->current_clock_tic = 0;
  wt_hour->current_cycle_no = 0;
  timer->hour = wt_hour;

  memset(&(timer->thread), 0, sizeof(timer_t));
  return timer;
}

void
start_wheel_timer(timer_t *wt){
  pthread_t *thread = &(wt->thread);
  if(pthread_create(thread, NULL, wheel_fn, (void*)wt)){
    printf("Wheel timer thread initialization failed, exiting ...\n");
    exit(0);
  }
}

wheel_timer_elem_t*
register_app_event(timer_t *wt, app_call_back call_back, void *arg, int arg_size, int time_interval, char is_recursive){
    if(!wt || !call_back) return NULL;
    wheel_timer_elem_t *wt_elem = calloc(1, sizeof(wheel_timer_elem_t));
    wt_elem->time_interval = time_interval;
    wt_elem->app_callback = call_back;
    wt_elem->arg = calloc(1, arg_size);
    memcpy(wt_elem->arg, arg, arg_size);
    wt_elem->arg_size = arg_size;
    wt_elem->is_recurrence = is_recursive;

  	int hour_cycle_no = (int)((wt_elem->time_interval/3600));
    int min_cycle_no  = (int)(((wt_elem->time_interval - (hour_cycle_no*3600))/60));
    if(min_cycle_no >= 60){
      min_cycle_no = min_cycle_no % 60;
      hour_cycle_no++;
    }
    int sec_cycle_no = ( wt_elem->time_interval - (hour_cycle_no*3600) - (min_cycle_no*60) );
    if(sec_cycle_no >= 60){
      sec_cycle_no = sec_cycle_no % 60;
      min_cycle_no++;
      if(min_cycle_no >= 60){
        min_cycle_no = min_cycle_no % 60;
        hour_cycle_no++;
      }
      if(hour_cycle_no >= 24){
        hour_cycle_no = hour_cycle_no % 24;
      }
    }

  	int slot_no  = sec_cycle_no;
  	wt_elem->execute_cycle_no_hour = hour_cycle_no;
    wt_elem->execute_cycle_no_minute = min_cycle_no;
    wt_elem->execute_cycle_no_second = sec_cycle_no;

    if(add_node_by_val(wt->second->slots[slot_no], wt_elem) != 0){
      printf("error in adding event to the linked list\n");
      return NULL;
    }
    return wt_elem;
}

void
free_wheel_timer_element(wheel_timer_elem_t *wt_elem){
	free(wt_elem->arg);
	free(wt_elem);
}
