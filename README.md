# Classic linked list
This is my realization of (cache-**un**friendly) doubly-linked list.

BTW, I have cache-frienly version of [linked list](https://github.com/ThreadJava800/LinkedList).

I keep pointer to the first (technical) value of list in my [structure](https://github.com/ThreadJava800/ClassicList/blob/main/list.h#L62). It contains pointers to first and last values of list.

This approach allows me to circle my list and avoid if-else constructions.

Also, you can check out my debug tools. I have text dump and graphics dump (as html file). I used graphviz library for list visualization.

# Functionality
Every push i calloc new block for value, and free it after remove (or after dtor).

- insert(List_t*, Elem_t, long index) - inserts value to logical index of list
- insertPhys(List_t*, Elem_t, ListElement_t *index) - inserts value to physical index of list *works o(1)*
- pushBack(List_t*, Elem_t value) - pushes value to tail of list
- pushFront(List_t*, Elem_t value) - pushes value to head of list
- remove(List_t*, Elem_t, long index) - removes value from logical index of list
- removePhys(List_t*, Elem_t, ListElement_t *index) - removes value from physical index of list *works o(1)*
- logicToPhysic(List_t*, long index) - returns pointer to ListElement_t* from logics index
- visualGraph(List_t*, [optional]const char* comment) and DUMP(List_t*) - graphical and text dumps. 