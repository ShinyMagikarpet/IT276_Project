#include "gf2d_list.h"
#include "simple_logger.h"

List *gf2d_list_new() {

	return gf2d_list_new_size(16);

}

List *gf2d_list_new_size(Uint32 count) {

	List *l;
	if (!count) {
		slog("cannot make new list of zero");
		return NULL;
	}
	l = (List *)malloc(sizeof(List));
	if (!l) {
		slog("failed to allocate memory for list");
		return NULL;
	}
	memset(l, 0, sizeof(List));
	l->size = count;
	l->element = (ListElement*)malloc(sizeof(ListElement)*count);
	if (!l->element) {
		slog("failed to allocate space for element");
		free(1);
		return NULL;
	}
	memset(l->element, 0, sizeof(ListElement)*count);
	return l;

}

void gf2d_list_delete(List *list) {

	if (!list)return;
	if (list->element) {
		free(list->element);
	}

	free(list);

}

List *gf2d_list_get_nth(List *list, Uint32 n) {

	if (!list) {
		slog("No list provided");
		return NULL;
	}

	if ((n >= list->count || n >= list->size))return NULL;
	return list->element[n].data;

}

List *gf2d_list_expand(List *list) {

	List *l;
	if (!list) {
		slog("no list provided");
		return NULL;
	}
	if (!list->size)list->size = 8;
	l = gf2d_list_new_size(list->size * 2);
	if (!l)return list;
	if (list->count > 0) {
		memcpy(l->element, list->element, sizeof(ListElement)*list->count);
	}
	l->count = list->count;
	gf2d_list_delete(list);
	return l;
}

List *gf2d_list_append(List *list, void *data) {

	if (!list) {
		slog("No list provided");
		return NULL;
	}
	if (list->count >= list->size) {
		list = gf2d_list_expand(list);
		if (!list) {
			slog("append failed due to lack of memory");
			return NULL;
		}
	}
	list->element[list->count++].data = data;
	return list;
}

List *gf2d_list_concat(List *a, List *b)
{
	int i, count;
	void *data;
	if ((!a) || (!b))
	{
		slog("missing list data");
		return NULL;
	}
	count = gf2d_list_get_count(b);
	for (i = 0; i < count; i++)
	{
		data = gf2d_list_get_nth(b, i);
		a = gf2d_list_append(a, data);
		if (a == NULL)return NULL;
	}
	return a;
}

List *gf2d_list_concat_free(List *a, List *b)
{
	a = gf2d_list_concat(a, b);
	if (a == NULL)return NULL;
	gf2d_list_delete(b);
	return a;
}

List *gf2d_list_prepend(List *list, void *data) {
	return gf2d_list_insert(list, data, 0);
}

List *gf2d_list_insert(List *list, void *data, Uint32 n) {

	if (!list) {
		slog("No list provided");
		return NULL;
	}
	if (n > list->size + 1) {
		slog("attempting to insert element beyond list length");
		return NULL;
	}
	if (list->count >= list->size) {
		list = gf2d_list_expand(list);
		if (!list)return NULL;
	}
	memmove(&list->element[n + 1], &list->element[n], sizeof(ListElement)*(list->count - n)); //copy all elements after n
	list->count++;
	return list;
}

List *gf2d_list_delete_nth(List *list, Uint32 n) {

	if (!list) {
		slog("list not provided");
		return NULL;
	}
	if (n > list->count) {
		slog("attempting to delete beyond list");
		return list;
	}
	if (n == (list->count - 1)) {
		list->count--;
		return list;
	}
	memmove(&list->element[n], &list->element[n + 1], sizeof(ListElement)*(list->count - n));
	list->count--;
	return list;
}



List *gf2d_list_delete_first(List *list) {
	
	if (!list)return NULL;
	return gf2d_list_delete_nth(list, 0);
}

List *gf2d_list_delete_last(List *list) {

	if (!list)return NULL;
	return gf2d_list_delete_nth(list, list->count - 1);
}


int gf2d_list_delete_data(List *list, void *data) {

	int i;
	if (!list) {
		slog("no list provided");
		return -1;
	}
	for (i = 0; i < list->count; i++) {
		if (list->element[i].data == data) {
			gf2d_list_delete_nth(list, i);
			return 0;
		}
	}
	slog("Data not found");
	return -1;
}

Uint32 gf2d_list_get_count(List *list) {

	if (!list)return 0;
	return list->count;
}

void gf2d_list_foreach(List *list, void(*function)(void *data, void *context), void *contextData) {

	int i;
	if (!list) {
		slog("no list provided");
		return;
	}
	if (!function) {
		slog("no function provided");
		return;
	}
	for (i = 0; i < list->count; i++) {
		function(list->element[i].data, contextData);
	}
}