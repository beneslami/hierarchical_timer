#ifndef LINKEDLISTAPI_H
#define LINKEDLISTAPI_H

typedef struct _ll_node ll_node;  /* opeque structure */
typedef struct _ll_ ll_t;         /* opeque structure */
typedef enum _bool_{
  LL_FALSE,
  LL_TRUE
} bool_t;

/* function declarations */
ll_t*
init_singly_ll();
int
add_node_by_val(ll_t*, void*);
bool_t
is_ll_empty(ll_t*);
int
ll_remove_node(ll_t*, ll_node*);
int
ll_add_node(ll_t*, ll_node*);
int
ll_delete_node(ll_t*, ll_node*);
ll_node*
get_singly_ll_head(ll_t*);
ll_node*
get_next_node(ll_node*);
ll_node*
get_node_data(ll_node*);
ll_t*
get_singly_ll(ll_t*);
void
decrease_node_count(ll_t*);
void
increase_node_count(ll_t*);

/* Macros */
#define ITERATE_LIST_BEGIN(list_ptr, node_ptr)      \
  {                                                 \
    ll_node *node = NULL;                           \
    node = get_singly_ll_head(list_ptr)             \
    for(; node != NULL; node = node_ptr){           \
      node = node_ptr->next;

#define ITERATE_LIST_END(list_ptr, node_ptr) }}

#define ITERATE_LIST_BEGIN_ENHANCED(list_ptr, node_ptr, prev)   \
  {                                                             \
    ll_node *_node_ptr = NULL;                                  \
    ll_node *prev = NULL;                                       \
    node_ptr = get_singly_ll_head(list_ptr);                    \
    for(;node_ptr != NULL; node_ptr = _node_ptr){               \
      _node_ptr = get_next_node(node_ptr);

#define ITERATE_LIST_CONTINUE_ENHANCED(list_ptr, node_ptr, prev)  \
  {                                                               \
    if(node_ptr) prev = node_ptr;                                 \
    continue;}

#define ITERATE_LIST_BREAK(list_ptr, node_ptr, prev)   break;

#define ITERATE_LIST_NODE_DELETE_ENHANCED(list_ptr, node_ptr, prev)   \
  {if(node_ptr && prev == NULL){                                      \
    get_singly_ll_head(list_ptr) = get_next_node(node_ptr);           \
  }                                                                   \
  else if(node_ptr && prev){                                          \
      get_next_node(prev) = get_next_node(node_ptr);                  \
  }                                                                   \
  free(node_ptr);                                                     \
  decrease_node_count(list_ptr);                                      \
  node_ptr = NULL;}

#define ITERATE_LIST_END_ENHANCED(list_ptr, node_ptr, prev)      \
  if(node_ptr) prev = node_ptr; }}


#endif
