#ifndef __GF2D_LIST_H__
#define __GF2D_LIST_H__

#include <SDL.h>

typedef struct {

	void *data;

}ListElement;


typedef struct {
	ListElement *element;
	Uint32 size;
	Uint32 count;
}List;

List *gf2d_list_new();

List *gf2d_list_new_size(Uint32 count);

void gf2d_list_delete(List *list);

List *gf2d_list_get_nth(List *list, Uint32 n);

List *gf2d_list_append(List *list, void *data);

List *gf2d_list_insert(List *list, void *data, Uint32 n);

List *gf2d_list_delete_nth(List *list, Uint32 n);

int gf2d_list_delete_data(List *list, void *data);

Uint32 gf2d_list_get_count(List *list);

List *gf2d_list_concat(List *a, List *b);

List *gf2d_list_concat_free(List *a, List *b);

void gf2d_list_foreach(List *list, void(*function)(void *data, void *context), void *contextData);



#endif
