#include "struct.h"

struct list_entry *add_node(struct list_entry *,struct list_entry *);

struct list_entry *remove_node(struct list_entry *,struct neighbor *);

struct flood_entry *init_fe(struct data_index *,struct list_entry *,struct flood_entry *);

struct flood *init_flood(struct flood_entry *,struct flood_entry *);

struct flood *init_data();

struct flood_entry *get_flood(struct flood *,struct data_index *data);

void add_entry(struct flood *flood, struct data_index *data,char *msg,struct list_entry *entry);

void rm_entry(struct flood *flood,struct flood_entry *entry);

void free_flood(struct flood *flood);

void free_list(struct list_entry *entry);

