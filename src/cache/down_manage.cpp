#include "down_manage.h"
#include <ccm/clog.h>


DownManage::DownManage(){}


bool DownManage::init(unsigned int max_items)
{
    max_items_ = max_items;
    pthread_mutex_init(&mutex_, NULL);
    lhash_dev_type_ = NULL;
    lhash_dev_type_ = lh_new(dev_hash, dev_cmp);
    if (!lhash_dev_type_) {
        SVC_LOG((LM_ERROR, "init down manage error"));
        return false;
    }
    return true;
}


bool DownManage::insert(DownInfo *down_info)
{
    if (!down_info) return false;
    
    if (this->items() >= max_items_){
        SVC_LOG((LM_ERROR, "cache full error; dev_type:%d", 
                            down_info->dev_type));
        return false;
    }
    
    if (!lhash_dev_type_){
        SVC_LOG((LM_ERROR, "cache null error; dev_type:%d", 
                            down_info->dev_type));
        return false;
    }
    
    pthread_mutex_lock(&mutex_);
    lh_insert(lhash_dev_type_, down_info);
    pthread_mutex_unlock(&mutex_);
    
    return true;
}


bool DownManage::del(int dev_type)
{
    DownInfo down_info;
    down_info.dev_type = dev_type;
    
    pthread_mutex_lock(&mutex_);
    void *ret 
        = lh_delete(lhash_dev_type_, &down_info);
    pthread_mutex_unlock(&mutex_);
    
    if (!ret) {
        SVC_LOG((LM_ERROR, "del error; dev_type:%d", dev_type));
        return false;
    }
    
    DownInfo *p = (DownInfo *)ret;
    p->free();
    delete p;
        
    return true;
}


bool DownManage::update(DownInfo *down_info)
{
    bool bret = del(down_info->dev_type);
    if (!bret) return false;
    bret = insert(down_info);
    if (!bret) return false;
    return true;
}



bool DownManage::find(int dev_type, DownInfo **down_info)
{
    DownInfo tmp_down_info;
    tmp_down_info.dev_type = dev_type;    
    void *ret = lh_retrieve(lhash_dev_type_, &tmp_down_info); 

    if (!ret) {
        SVC_LOG((LM_ERROR, "find error; dev_type:%d", dev_type));
        return false;
    }
    
    *down_info = (DownInfo*)ret;
    return true;
}


void DownManage::clear()
{
    return;
}


unsigned int DownManage::items()
{
    return lh_num_items(lhash_dev_type_);
}



unsigned long DownManage::dev_hash(const void *buffer)
{
    DownInfo *down_info = (DownInfo *)buffer;
    char tmp_buffer[32] = {0};
    sprintf(tmp_buffer, "%016d", down_info->dev_type);
    unsigned long value = lh_strhash(tmp_buffer);
    return value;
}


int DownManage::dev_cmp(const void *buffer1, 
                            const void *buffer2)
{
    if ((!buffer1) || (!buffer2))
        return -1;
    
    DownInfo *down_info1 = (DownInfo *)buffer1;
    DownInfo *down_info2 = (DownInfo *)buffer2;
    
    if (down_info1->dev_type != down_info2->dev_type) 
        return -1;
    
    return 0;
}

