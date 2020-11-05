#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"jval.h"

void jrb_print_db(JRB tree)
    {
        JRB tmp;
        jrb_traverse(tmp, tree)
            printf("Name: %s - Phone: %ld\n", jval_s(tmp->key), jval_l(tmp->val));
    }

JRB jrb_find_num(JRB tree, Jval value)
    {
        JRB tmp;
        jrb_traverse(tmp, tree)
            {
                if(jval_l(tmp->key) == jval_l(value))
                    return tmp;
            }
        return NULL;
    }

int main()
    {
        JRB phonebook, node,s;
        phonebook = make_jrb();

        jrb_insert_str(phonebook, "Chung", new_jval_l(99912335));
        jrb_insert_str(phonebook, "Manh", new_jval_l(9818888));
        jrb_insert_str(phonebook, "Chung", new_jval_l(979997));
        jrb_insert_str(phonebook, "Tuan", new_jval_l(913997));
        jrb_insert_str(phonebook, "Lien", new_jval_l(945997));
        jrb_insert_str(phonebook, "Tung", new_jval_l(979327));
        jrb_insert_str(phonebook, "Thang", new_jval_l(973497));

        jrb_print_db(phonebook);

        s = jrb_find_num(phonebook, new_jval_l(913997) );
        printf("%s\n", s->key);
    }