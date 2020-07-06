#include "LinkedListApi.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct _ll_{
  ll_node *head;
  unsigned int node_count;
  int (*comparison_fn)(void*, void*);
  int (*order_comparison_fn)(void*, void*);
};

struct _ll_node{
  void* data;
  struct _ll_node *next;
};

ll_t*
get_singly_ll(ll_t* ll){
  return ll;
}

ll_node*
get_singly_ll_head(ll_t* ll){
  return ll->head;
}

ll_node*
get_next_node(ll_node* node){
  return node->next;
}

ll_node*
get_node_data(ll_node* node){
  return node->data;
}

void
decrease_node_count(ll_t* ll){
  ll->node_count--;
}
void
increase_node_count(ll_t* ll){
  ll->node_count++;
}

ll_t*
init_singly_ll(){
  ll_t *head = calloc(1, sizeof(ll_t));
  return head;
}

int
add_node_by_val(ll_t *ll , void *data){
  ll_node *node = calloc(1, sizeof(ll_node));
  node->data = data;
  if(!ll->head){
    ll->head = node;
    ll->node_count++;
    return 0;
  }
  node->next = ll->head;
  ll->node_count++;
  return 0;
}

bool_t
is_ll_empty(ll_t *ll){
  if(!ll) assert(0);
  if(ll->node_count == 0)
    return LL_TRUE;
  return LL_FALSE;
}

int
ll_remove_node(ll_t* ll, ll_node *node){
  if(!ll || !ll->head ) return 0;
  if(!node){
    printf("%s(%d) : Error: node is NULL\n", __FUNCTION__, __LINE__);
      return -1;
  }
  ll_node *head = ll->head;
  ll_node *prev = NULL;
  if(head == node){
    ll->head = ll->head->next;
    ll->node_count--;
    node->next = NULL;
    return 0;
  }
  prev = head;
  head = head->next;
  for(int i=1; i< ll->node_count; i++){
    if(head != node){
      prev = head;
      head = head->next;
      continue;
    }
    prev->next = head->next;
    head->next = NULL;
    ll->node_count--;
    return 0;
  }
  printf("%s(%d) : Error: node is NULL\n", __FUNCTION__, __LINE__);
  return -1;
}

int
ll_add_node(ll_t *ll, ll_node *node){
  if(!ll) return -1;
  if(!node) return -1;
  if(!ll->head){
    ll->head = node;
    ll->node_count++;
    return 0;
  }
  node->next = ll->head;
  ll->head = node;
  ll->node_count++;
  return 0;
}

int
ll_delete_node(ll_t *ll, ll_node *node){
  if(!ll) return -1;
  if(!ll->head || !node) return 0;
  ll_node *trav = NULL;
  if(node->next){
    ll_node *temp = NULL;
    node->data = node->next->data;
    temp = node->next;
    node->next = node->next->next;
    free(temp);
    ll->node_count--;
    return 0;
  }
  if(ll->node_count == 1 && ll->head == node){
    free(node);
    ll->head = NULL;
    ll->node_count--;
    return 0;
  }
  trav = ll->head;
  while(trav->next != node){
    trav = trav->next;
    continue;
  }
  trav->next = NULL;
  free(node);
  ll->node_count--;
  return 0;
}
