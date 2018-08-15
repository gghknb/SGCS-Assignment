#include <stdio.h>
#include "list.h"
#include "bitmap.h"
#include "hash.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int i,j;
    int index,index2;
    char str[100];
    char mode1[30],mode2[30];
    char list_name[30],list_name2[30];
    char hash_name[30],hash_data_string[30];
    char bitmap_name[30];
    char bitmap_data_string[30];
    struct list_array new_list[10];
    struct list_elem* elem_temp,*elem_temp2,*elem_temp3;
    struct list_item* item_temp;
    struct hash_array new_hash[10];
    struct hash_iterator hash_temp;
    struct hash_item *hash_item_temp;
    struct bitmap_array new_bitmap[10];
    int list_num = 0,hash_num = 0,bitmap_num = 0;
    int list_flag = 0,hash_flag = 0,bitmap_flag = 0;
    int ss;
    int list_index,list_index2,list_index3;
    int list_data;
    int hash_index;
    int hash_data;
    int value_of_return;
    int size;
    int temp;
    int bitmapsize;
    int bitmap_index;
    int bitmap_data,bitmap_data2;
    while(1)
    {

        fgets(str,sizeof(str),stdin);
        sscanf(str,"%s",mode1);
        if(strcmp(mode1,"list_insert") == 0)    //List
        {
            sscanf(str,"%s %s %d %d",mode1,list_name,&list_index,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index  = i;
                    break;
                }
            }
            elem_temp = list_begin(&new_list[index].lst);
            for(i = 0 ; i < list_index ; i++)
            {
                elem_temp = list_next(elem_temp);
            }
            item_temp = (struct list_item*)malloc(sizeof(struct list_item));
            item_temp->data = list_data;
            list_insert(elem_temp,&item_temp->item);

        }
        else if(strcmp(mode1,"list_splice") == 0)
        {
            sscanf(str,"%s %s %d %s %d %d",mode1,list_name,&list_index,list_name2,&list_index2,&list_index3);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i ;
                    break;
                }
            }
            for(i = 0; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name2) == 0)
                {
                    index2 = i;
                    break;
                }
            }
            elem_temp = list_begin(&new_list[index].lst);
            elem_temp2 = list_begin(&new_list[index2].lst);
            elem_temp3 = list_begin(&new_list[index2].lst);
            for(i = 0 ; i < list_index ; i++)
            {
                elem_temp = list_next(elem_temp);
            }
            for(i = 0 ; i < list_index2 ; i++)
            {
                elem_temp2 = list_next(elem_temp2);
            }
            for(i = 0 ; i < list_index3 ; i++)
            {
                elem_temp3 = list_next(elem_temp3);
            }

            list_splice(elem_temp,elem_temp2,elem_temp3);
        }
        else if(strcmp(mode1,"list_push") == 0);
        else if(strcmp(mode1,"list_push_front") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            item_temp = (struct list_item*)malloc(sizeof(struct list_item));
            item_temp->data = list_data;
            list_push_front(&new_list[index].lst,&item_temp->item);
        }
        else if(strcmp(mode1,"list_push_back") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            item_temp = (struct list_item*)malloc(sizeof(struct list_item));
            item_temp->data = list_data;
            list_push_back(&new_list[index].lst,&item_temp->item);
        }
        else if(strcmp(mode1,"list_remove") == 0)
        {
            sscanf(str,"%s %s %d %d",mode1,list_name,&list_index,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index  = i;
                    break;
                }
            }
            elem_temp = list_begin(&new_list[index].lst);
            for(i = 0 ; i < list_index ; i++)
            {
                elem_temp = list_next(elem_temp);
            }
            list_remove(elem_temp);
        }
        else if(strcmp(mode1,"list_pop_front") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            list_pop_front(&new_list[index].lst);
        }
        else if(strcmp(mode1,"list_pop_back") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            list_pop_back(&new_list[index].lst);
        }
        else if(strcmp(mode1,"list_front") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            elem_temp = list_front(&new_list[index].lst);
            item_temp = list_entry(elem_temp,struct list_item,item); 
            printf("%d\n",item_temp->data);

        }
        else if(strcmp(mode1,"list_back") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            elem_temp = list_back(&new_list[index].lst);
            item_temp = list_entry(elem_temp,struct list_item,item); 
            printf("%d\n",item_temp->data);
        }
        else if(strcmp(mode1,"list_size") == 0)
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            size = list_size(&new_list[index].lst);
            printf("%d\n",size);
        }
        else if(strcmp(mode1,"list_empty") == 0)
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            printf("%s\n",list_empty(&new_list[index].lst) ? "true" : "false");
        }
        else if(strcmp(mode1,"list_reverse") == 0)
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            list_reverse(&new_list[index].lst);
        }
        else if(strcmp(mode1,"list_sort") == 0)
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            list_sort(&new_list[index].lst,&list_compare,NULL);
        }
        else if(strcmp(mode1,"list_insert_ordered") == 0)
        {
            sscanf(str,"%s %s %d",mode1,list_name,&list_data);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            item_temp = (struct list_item*)malloc(sizeof(struct list_item));
            item_temp->data = list_data;
            list_insert_ordered(&new_list[index].lst,&item_temp->item,&list_compare,NULL);
        }
        else if(strcmp(mode1,"list_unique") == 0)
        {
            ss = sscanf(str,"%s %s %s",mode1,list_name,list_name2);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(ss == 3 )
            {
                for(i = 0 ; i < list_num ; i++)
                {
                    if(strcmp(new_list[i].name,list_name2) == 0)
                    {
                        index2 = i;
                        break;
                    }
                }
                list_unique(&new_list[index].lst,&new_list[index2].lst,&list_compare,NULL);
            }
            else if(ss ==2)
            {
                list_unique(&new_list[index].lst,NULL,&list_compare,NULL);
            }
        }
        else if(strcmp(mode1,"list_max") == 0)
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            elem_temp = list_max(&new_list[index].lst,&list_compare,NULL);
            item_temp = list_entry(elem_temp,struct list_item,item); 
            printf("%d\n",item_temp->data);
        }
        else if(strcmp(mode1,"list_min") == 0)
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            elem_temp = list_min(&new_list[index].lst,&list_compare,NULL);
            item_temp = list_entry(elem_temp,struct list_item,item); 
            printf("%d\n",item_temp->data);

        }
        else if(strcmp(mode1,"list_swap") == 0)
        {
            sscanf(str,"%s %s %d %d",mode1,list_name,&list_index,&list_index2);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(list_index > list_index2)
            {
                temp = list_index;
                list_index = list_index2;
                list_index2 = temp;
            }
            elem_temp = list_begin(&new_list[index].lst);
            elem_temp2 = list_begin(&new_list[index].lst);
            for(i = 0 ; i < list_index ; i++)
                elem_temp = list_next(elem_temp);
            for(i = 0 ; i < list_index2 ; i++)
                elem_temp2 = list_next(elem_temp2);

            list_swap(elem_temp,elem_temp2);
        }
        else if(strcmp(mode1,"list_shuffle") == 0)	
        {
            sscanf(str,"%s %s",mode1,list_name);
            for(i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,list_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            list_shuffle(&new_list[index].lst);
        }
        else if(strcmp(mode1,"hash_insert") == 0)   //Hashtable
        {
            sscanf(str,"%s %s %d",mode1,hash_name,&hash_data);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            hash_item_temp = (struct hash_item*)malloc(sizeof(struct hash_item));
            hash_item_temp->data = hash_data;
            hash_insert(&new_hash[index].hsh,&hash_item_temp->item);
        }
        else if(strcmp(mode1,"hash_replace") == 0)
        {
            sscanf(str,"%s %s %d",mode1,hash_name,&hash_data);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            hash_item_temp = (struct hash_item*)malloc(sizeof(struct hash_item));
            hash_item_temp->data = hash_data;
            hash_replace(&new_hash[index].hsh,&hash_item_temp->item);
        }
        else if(strcmp(mode1,"hash_find") == 0)
        {
            sscanf(str,"%s %s %d",mode1,hash_name,&hash_data);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            hash_item_temp = (struct hash_item*)malloc(sizeof(struct hash_item));
            hash_item_temp->data = hash_data;
            if(hash_find(&new_hash[index].hsh,&hash_item_temp->item) != NULL)
                printf("%d\n",hash_data);

        }
        else if(strcmp(mode1,"hash_delete") == 0)
        {
            sscanf(str,"%s %s %d",mode1,hash_name,&hash_data);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            hash_item_temp = (struct hash_item*)malloc(sizeof(struct hash_item));
            hash_item_temp->data = hash_data;
            hash_delete(&new_hash[index].hsh,&hash_item_temp->item);
        }
        else if(strcmp(mode1,"hash_clear") == 0)
        {
            sscanf(str,"%s %s",mode1,hash_name);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            hash_clear(&new_hash[index].hsh,NULL);
        }
        else if(strcmp(mode1,"hash_size") == 0)
        {
            sscanf(str,"%s %s",mode1,hash_name);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            size = hash_size(&new_hash[index].hsh);
            printf("%d\n",size);
        }
        else if(strcmp(mode1,"hash_empty") == 0)
        {
            sscanf(str,"%s %s",mode1,hash_name);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(hash_empty(&new_hash[index].hsh) == 0)
                printf("false\n");
            else printf("true\n");
        }
        else if(strcmp(mode1,"hash_apply") == 0)
        {
            sscanf(str,"%s %s %s",mode1,hash_name,hash_data_string);
            for(i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,hash_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(hash_data_string,"square") == 0)
                hash_apply(&new_hash[index].hsh,hash_square);
            else if(strcmp(hash_data_string,"triple") == 0)
                hash_apply(&new_hash[index].hsh,hash_triple);
        }
        else if(strcmp(mode1,"bitmap_size") == 0)   //Bitmap
        {
             sscanf(str,"%s %s",mode1,bitmap_name);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            printf("%d\n",bitmap_size(new_bitmap[index].btmap));

        }
        else if(strcmp(mode1,"bitmap_set") == 0)
        {
            sscanf(str,"%s %s %d %s",mode1,bitmap_name,&bitmap_data,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                bitmap_set(new_bitmap[index].btmap,bitmap_data,true);
            else if(strcmp(bitmap_data_string,"false") == 0)
                bitmap_set(new_bitmap[index].btmap,bitmap_data,false);
        }
        else if(strcmp(mode1,"bitmap_mark") == 0)
        {
            sscanf(str,"%s %s %d",mode1,bitmap_name,&bitmap_data);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            bitmap_mark(new_bitmap[index].btmap,bitmap_data);
        }
        else if(strcmp(mode1,"bitmap_reset") == 0)
        {
             sscanf(str,"%s %s %d",mode1,bitmap_name,&bitmap_data);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            bitmap_reset(new_bitmap[index].btmap,bitmap_data);

        }
        else if(strcmp(mode1,"bitmap_flip") == 0)
        {
            sscanf(str,"%s %s %d",mode1,bitmap_name,&bitmap_data);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            bitmap_flip(new_bitmap[index].btmap,bitmap_data);
        }
        else if(strcmp(mode1,"bitmap_test") == 0)
        {
            sscanf(str,"%s %s %d",mode1,bitmap_name,&bitmap_data);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            printf("%s\n",bitmap_test(new_bitmap[index].btmap,bitmap_data) ? "true" : "false");

        }
        else if(strcmp(mode1,"bitmap_set_all") == 0)
        {   
            sscanf(str,"%s %s %s",mode1,bitmap_name,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                bitmap_set_all(new_bitmap[index].btmap,true);
            else if(strcmp(bitmap_data_string,"false") == 0)
                bitmap_set_all(new_bitmap[index].btmap,false); 
        }
        else if(strcmp(mode1,"bitmap_set_multiple") == 0)
        {
            sscanf(str,"%s %s %d %d %s",mode1,bitmap_name,&bitmap_data,&bitmap_data2,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                bitmap_set_multiple(new_bitmap[index].btmap,bitmap_data,bitmap_data2,true);
            else if(strcmp(bitmap_data_string,"false") == 0)
                bitmap_set_multiple(new_bitmap[index].btmap,bitmap_data,bitmap_data2,false);

        }
        else if(strcmp(mode1,"bitmap_count") == 0)
        {
            sscanf(str,"%s %s %d %d %s",mode1,bitmap_name,&bitmap_data,&bitmap_data2,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                printf("%d\n",bitmap_count(new_bitmap[index].btmap,bitmap_data,bitmap_data2,true));
            else if(strcmp(bitmap_data_string,"false") == 0)
                printf("%d\n",bitmap_count(new_bitmap[index].btmap,bitmap_data,bitmap_data2,false));


        }
        else if(strcmp(mode1,"bitmap_contains") == 0)
        {
            sscanf(str,"%s %s %d %d %s",mode1,bitmap_name,&bitmap_data,&bitmap_data2,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                printf("%s\n",bitmap_contains(new_bitmap[index].btmap,bitmap_data,bitmap_data2,true) ? "true" : "false");
            else if(strcmp(bitmap_data_string,"false") == 0)
                printf("%s\n",bitmap_contains(new_bitmap[index].btmap,bitmap_data,bitmap_data2,false) ? "true" : "false");

        }
        else if(strcmp(mode1,"bitmap_any") == 0)
        {   
            sscanf(str,"%s %s %d %d",mode1,bitmap_name,&bitmap_data,&bitmap_data2);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(bitmap_any(new_bitmap[index].btmap,bitmap_data,bitmap_data2) != 0)
                printf("true\n");
            else printf("false\n");
        }
        else if(strcmp(mode1,"bitmap_none") == 0)
        {
            sscanf(str,"%s %s %d %d",mode1,bitmap_name,&bitmap_data,&bitmap_data2);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            printf("%s\n",bitmap_none(new_bitmap[index].btmap,bitmap_data,bitmap_data2) ? "true" : "false");

        }
        else if(strcmp(mode1,"bitmap_all") == 0)
        {
            sscanf(str,"%s %s %d %d",mode1,bitmap_name,&bitmap_data,&bitmap_data2);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(bitmap_all(new_bitmap[index].btmap,bitmap_data,bitmap_data2) != 0)
                printf("true\n");
            else printf("false\n");
        }
        else if(strcmp(mode1,"bitmap_scan") == 0)
        {
            sscanf(str,"%s %s %d %d %s",mode1,bitmap_name,&bitmap_data,&bitmap_data2,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                printf("%u\n",bitmap_scan(new_bitmap[index].btmap,bitmap_data,bitmap_data2,true));
            else if(strcmp(bitmap_data_string,"false") == 0)
                printf("%u\n",bitmap_scan(new_bitmap[index].btmap,bitmap_data,bitmap_data2,false));

        }
        else if(strcmp(mode1,"bitmap_scan_and_flip") == 0)
        {
            sscanf(str,"%s %s %d %d %s",mode1,bitmap_name,&bitmap_data,&bitmap_data2,bitmap_data_string);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            if(strcmp(bitmap_data_string,"true") == 0)
                printf("%u\n",bitmap_scan_and_flip(new_bitmap[index].btmap,bitmap_data,bitmap_data2,true));
            else if(strcmp(bitmap_data_string,"false") == 0)
                printf("%u\n",bitmap_scan_and_flip(new_bitmap[index].btmap,bitmap_data,bitmap_data2,false));


        }
        else if(strcmp(mode1,"bitmap_dump") == 0)
        {
            sscanf(str,"%s %s",mode1,bitmap_name);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            bitmap_dump(new_bitmap[index].btmap);

        }
        else if(strcmp(mode1,"bitmap_expand") == 0)
        {
            sscanf(str,"%s %s %d",mode1,bitmap_name,&bitmap_data);
            for(i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,bitmap_name) == 0)
                {
                    index = i;
                    break;
                }
            }
            bitmapsize = bitmap_size(new_bitmap[index].btmap);
            new_bitmap[index].btmap = bitmap_expand(new_bitmap[index].btmap,bitmapsize+bitmap_data);

        }
        else if(strcmp(mode1,"create") == 0)
        {
            sscanf(str,"%s %s",mode1,mode2);
            if(strcmp(mode2,"list") ==0)
            {
                sscanf(str,"%s %s %s",mode1,mode2,list_name);
                strcpy(new_list[list_num].name,list_name);
                list_init(&new_list[list_num].lst);
                list_num++;
            }
            else if(strcmp(mode2,"hashtable") == 0)
            {
                sscanf(str,"%s %s %s",mode1,mode2,hash_name);
                strcpy(new_hash[hash_num].name,hash_name);
                hash_init(&new_hash[hash_num].hsh,&compute_return_hash_value,&hash_compare,NULL);
                hash_num++;
            }
            else if(strcmp(mode2,"bitmap") == 0)
            {
                sscanf(str,"%s %s %s %d",mode1,mode2,bitmap_name,&bitmapsize);
                strcpy(new_bitmap[bitmap_num].name,bitmap_name);
                new_bitmap[bitmap_num].btmap = bitmap_create(bitmapsize);
                bitmap_num++;
            }
        }
        else if(strcmp(mode1,"delete") == 0)
        {
            list_flag = 0;
            hash_flag = 0;
            bitmap_flag = 0;
            sscanf(str,"%s %s",mode1,mode2);
            for( i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,mode2) == 0)
                {
                    list_index = i;
                    list_flag = 1;
                    break;
                }
            }
            for( i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,mode2) == 0)
                {
                    hash_index = i;
                    hash_flag = 1;
                    break;
                }
            }
            for( i = 0 ; i < bitmap_num; i++)
            {
                if(strcmp(new_bitmap[i].name,mode2) == 0)
                {
                    bitmap_index = i;
                    bitmap_flag =1;
                    break;
                }
            }
            if(list_flag == 1)
            {
                for( i = list_index ; i < list_num - 1 ; i++)
                {
                    strcpy(new_list[i].name, new_list[i+1].name);
                    new_list[i] = new_list[i+1];
                }
                list_num = list_num - 1;
            }
            else if(hash_flag == 1)
            {
                for( i = hash_index ; i < hash_num -1 ; i++)
                {
                    strcpy(new_hash[i].name,new_hash[i+1].name);
                    new_hash[i] = new_hash[i+1];
                }
                hash_num = hash_num - 1;
            }
            else if(bitmap_flag == 1)
            {     
                bitmap_destroy(new_bitmap[bitmap_index].btmap);
                for( i = bitmap_index ; i < bitmap_num -1 ; i++)
                {
                    strcpy(new_bitmap[i].name,new_bitmap[i+1].name);
                    new_bitmap[i] = new_bitmap[i+1];
                }
                bitmap_num = bitmap_num - 1;
            }

        }
        else if(strcmp(mode1,"dumpdata") == 0)
        {
            list_flag = 0;
            hash_flag = 0;
            bitmap_flag = 0;
            sscanf(str,"%s %s",mode1,mode2);
            for( i = 0 ; i < list_num ; i++)
            {
                if(strcmp(new_list[i].name,mode2) == 0)
                {
                    list_index = i; 
                    list_flag = 1;
                    break;
                }
            }
            for( i = 0 ; i < hash_num ; i++)
            {
                if(strcmp(new_hash[i].name,mode2) == 0)
                {
                    hash_index = i;
                    hash_flag = 1;
                    break;
                }
            }
            for( i = 0 ; i < bitmap_num ; i++)
            {
                if(strcmp(new_bitmap[i].name,mode2) == 0)
                {
                    bitmap_index = i;
                    bitmap_flag = 1;
                    break;
                }
            }
            if(list_flag == 1)
            {
                if(list_begin(&new_list[list_index].lst) == list_end(&new_list[list_index].lst) )
                    continue;
                for(elem_temp = list_begin(&new_list[list_index].lst) ; elem_temp != list_end(&new_list[list_index].lst); elem_temp = list_next(elem_temp))
                {
                    item_temp = list_entry(elem_temp,struct list_item,item);
                    printf("%d ",item_temp->data);
                }
                printf("\n");
            }
            else if(hash_flag == 1)
            {
                for(hash_first(&hash_temp,&new_hash[hash_index].hsh) ; hash_next(&hash_temp) !=0 ;)
                {
                    printf("%d ",hash_entry(hash_temp.elem,struct hash_item,item)->data);
                }
                printf("\n");
            }
            else if(bitmap_flag == 1)
            {
                bitmapsize = bitmap_size(new_bitmap[bitmap_index].btmap);
                for(i = 0 ; i < bitmapsize;i++)
                {
                
                      value_of_return = bitmap_test(new_bitmap[bitmap_index].btmap,i);
                      printf("%d",value_of_return);
                }
                printf("\n");
            }
        }
        else if(strcmp(mode1,"quit") == 0)
            break;
    }

    return 0;
}












