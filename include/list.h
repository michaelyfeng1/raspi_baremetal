#pragma once


typedef struct ListEntry
{
    struct ListEntry* next;
}ListEntry_t;

typedef struct HeadList
{
    ListEntry_t* next;
    ListEntry_t* tail;
}HeadList_t;

void append_list_to_tail();
ListEntry_t* remove_list_head();
u8 is_list_empty();