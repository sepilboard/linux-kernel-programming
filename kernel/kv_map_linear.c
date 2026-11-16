#include <linux/syscalls.h>
#include <linux/list.h>

#define KV_VALUE_SIZE 128

struct kv_entry
{
    unsigned long long key;
    char val[128];
    struct list_head list;
};

LIST_HEAD(kv_map);

SYSCALL_DEFINE2(kv_put_linear, unsigned long long, key, char*, usr_val)
{
    char value[KV_VALUE_SIZE];
    // unsigned long len = copy_from_user(value, usr_val, sizeof(value));
    unsigned long len = strncpy_from_user(value, usr_val, sizeof(value));

    // if(len) return -1;
    if(len < 0) return -1;
    if(len >= KV_VALUE_SIZE) return -1;

    struct kv_entry *entry;
    list_for_each_entry(entry, &kv_map, list){
        if(entry->key == key){
            strcpy(entry->val, value);
            return len;
        }
    }

    entry = kmalloc(sizeof(struct kv_entry), GFP_KERNEL);
    if(!entry) return -1;

    entry->key = key;
    strcpy(entry->val, value);
    list_add_tail(&entry->list, &kv_map);
    return len;
}

SYSCALL_DEFINE3(kv_get_linear, unsigned long long, key, char*, usr_buf, size_t, buf_size)
{
    int len = 0;
    struct kv_entry *entry;

    list_for_each_entry(entry, &kv_map, list){
        if(entry->key == key){
            len = strlen(entry->val)+1;
            if(buf_size < len) return -1;

            if(copy_to_user(usr_buf, entry->val, len)) return -1;
            return len;
        }
    }

    return -1;
}


