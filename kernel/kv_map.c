#include <linux/syscalls.h>
#include <linux/rbtree.h>

#define KV_VALUE_SIZE 128

struct kv_entry
{
    unsigned long long key;
    char val[128];
    struct list_head list;
    struct rb_node node;
};

static struct rb_root kv_map = RB_ROOT;

static int kv_cmp(const void *key, const struct rb_node *other)
{
    unsigned long long k = *(const unsigned long long *)key;
    unsigned long long other_k = rb_entry(other, struct kv_entry, node)->key;
    
    if(k == other_k) return 0;
    else if(k > other_k) return 1;
    else return -1;
}

static bool kv_less(struct rb_node *a, const struct rb_node *b)
{
    struct kv_entry *x = rb_entry(a, struct kv_entry, node);
    struct kv_entry *y = rb_entry(b, struct kv_entry, node);

    return x->key < y->key;
}

SYSCALL_DEFINE2(kv_put, unsigned long long, key, char*, usr_val)
{
    char value[KV_VALUE_SIZE];
    unsigned long len = strncpy_from_user(value, usr_val, sizeof(value));

    if(len < 0) return -1;
    if(len >= KV_VALUE_SIZE) return -1;

    struct kv_entry *entry;
    struct rb_node *node = rb_find(&key, &kv_map, kv_cmp);

    if(node){
        entry = rb_entry(node, struct kv_entry, node);
        strcpy(entry->val, value);
        return len;
    }

    entry = kmalloc(sizeof(struct kv_entry), GFP_KERNEL);
    if(!entry) return -1;

    entry->key = key;
    strcpy(entry->val, value);
    rb_add(&entry->node, &kv_map, kv_less);
    return len;
}

SYSCALL_DEFINE3(kv_get, unsigned long long, key, char*, usr_buf, size_t, buf_size)
{
    int len = 0;
    struct kv_entry *entry;
    struct rb_node *node = rb_find(&key, &kv_map, kv_cmp);

    if(node){
        entry = rb_entry(node, struct kv_entry, node);
        len = strlen(entry->val)+1;
        if(buf_size < len) return -1;

        if(copy_to_user(usr_buf, entry->val, len)) return -1;
        return len;
    }

    return -1;
}