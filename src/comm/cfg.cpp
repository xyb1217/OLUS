#include "cfg.h"


bool Cfg::init(const char *cfg_file)
{
    bool bret = load(cfg_file);
    if (!bret){
        SVC_LOG((LM_ERROR, "load error"));
        return false;
    }
    return true;
}


bool Cfg::load(const char *cfg_file)
{
	xml_document doc;	
	doc.load_file(cfg_file);	
	xml_node root = doc.first_child();	
	if (root.empty()){	
		SVC_LOG((LM_ERROR, "root is empty"));
		return false;
	}

	xml_node common = root.child("common");
	if (!common.empty())	
	{
		xml_node run = common.child("run");
		if (!run.empty()){
			xml_attribute id = run.attribute("id");
			run_id_ = new char[strlen(id.as_string())+1];
			if(!run_id_) return false;
			memset(run_id_, 0, strlen(id.as_string())+1);
			strcpy(run_id_, id.as_string());
		}
	
		xml_node log = common.child("log");
		if (!log.empty()){
			xml_attribute level = log.attribute("level");
			log_level_ = level.as_int();
		}

		xml_node thread = common.child("thread");
		if (!thread.empty()){
			xml_attribute count = thread.attribute("count");
			threads_ = count.as_int();
		}

        xml_node conn = common.child("conn");
		if (!conn.empty()){
			xml_attribute count = conn.attribute("count");
			conns_ = count.as_int();
		}
	}

    xml_node localAddr = root.child("localAddr");
	if (!localAddr.empty())
	{
		xml_node addr = localAddr.child("addr");
		if(!addr.empty())
		{
			xml_attribute ip = addr.attribute("ip");
			xml_attribute port= addr.attribute("port"); 
            listen_ip_ = new char[strlen(ip.as_string())+1];
            if (!listen_ip_) return false;
            memset(listen_ip_, 0, strlen(ip.as_string())+1);
            strcpy(listen_ip_, ip.as_string());
			listen_port_ = port.as_int();
		}
	}
	
	xml_node downFirmware = root.child("downFirmware");
	if (!downFirmware.empty())
	{
		xml_node addr = downFirmware.child("addr");
		if (!addr.empty())
		{
			xml_attribute path = addr.attribute("path");
			xml_attribute name = addr.attribute("name"); 
			xml_attribute version = addr.attribute("version"); 
            
            firmware_path_ = new char[strlen(path.as_string())+1];
            if (!firmware_path_) return false;
            memset(firmware_path_, 0, strlen(path.as_string())+1);
            strcpy(firmware_path_, path.as_string());

            firmware_name_ = new char[strlen(name.as_string())+1];
            if (!firmware_name_) return false;
            memset(firmware_name_, 0, strlen(name.as_string())+1);
            strcpy(firmware_name_, name.as_string());

            firmware_version_ = new char[strlen(version.as_string())+1];
            if (!firmware_version_) return false;
            memset(firmware_version_, 0, strlen(version.as_string())+1);
            strcpy(firmware_version_, version.as_string());

            
            int len = strlen(path.as_string())+strlen(name.as_string())+1;
            firmware_file_ = new char[len];
            if (!firmware_file_) return false;
            memset(firmware_file_, 0, len);
            sprintf(firmware_file_, "%s%s", path.as_string(), name.as_string());

		}
	}

	return true;
}

