#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.c"
#include"jval.h"

int main()
    {
        JRB phonebook, node;
        phonebook = make_jrb();

        jrb_insert_str(phonebook, "Chung", new_jval_l(99912335));
        jrb_insert_str(phonebook, "Manh", new_jval_l(9818888));
        jrb_insert_str(phonebook, "Chung", new_jval_l(979997));
        jrb_insert_str(phonebook, "Tuan", new_jval_l(913997));
        jrb_insert_str(phonebook, "Lien", new_jval_l(945997));
        jrb_insert_str(phonebook, "Tung", new_jval_l(979327));
        jrb_insert_str(phonebook, "Thang", new_jval_l(973497));

        jrb_print_tree(phonebook, 1);
    }