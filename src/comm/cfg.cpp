#include "cfg.h"

#include <ccm/clog.h>
#include <ccm/pugixml.hpp>


using namespace pugi;


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
			if(!run_id_) {	
        		SVC_LOG((LM_ERROR, "new error"));
        		return false;
        	}
			memset(run_id_, 0, strlen(id.as_string())+1);
			strcpy(run_id_, id.as_string());
		}
        printf("run id: %s\n", run_id_);
	
		xml_node log = common.child("log");
		if (!log.empty()){
			xml_attribute level = log.attribute("level");
			log_level_ = level.as_int();
            printf("log level: %d\n", log_level_);
		}

		xml_node thread = common.child("thread");
		if (!thread.empty()){
			xml_attribute count = thread.attribute("count");
			threads_ = count.as_int();
            printf("threads: %d\n", threads_);
		}

        xml_node dev = common.child("dev");
		if (!dev.empty()){
			xml_attribute types = dev.attribute("types");
			dev_types_ = types.as_int();
            printf("max dev types: %d\n", dev_types_);
		}
	}

    bool bret = DownManage::inst().init(dev_types());
    if (!bret){
        SVC_LOG((LM_ERROR, "int down manage error"));
        return false;
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
            if (!listen_ip_) {	
        		SVC_LOG((LM_ERROR, "new error"));
        		return false;
        	}
            memset(listen_ip_, 0, strlen(ip.as_string())+1);
            strcpy(listen_ip_, ip.as_string());
			listen_port_ = port.as_int();
            printf("ip: %s, port: %d\n", listen_ip_, listen_port_);
		}
	}
	
	xml_node downFirmware = root.child("downFirmware");
	if (!downFirmware.empty())
	{
		xml_node firmware = downFirmware.child("firmware");
		while (!firmware.empty())
		{
		    xml_attribute dev_type = firmware.attribute("dev_type");
			xml_attribute path = firmware.attribute("path");
			xml_attribute name = firmware.attribute("name"); 
			xml_attribute version = firmware.attribute("version"); 
            
		    DownInfo *down_info = new DownInfo;
            if (!down_info) {	
        		SVC_LOG((LM_ERROR, "new error"));
        		return false;
        	}

            down_info->dev_type = dev_type.as_int();

            int len = strlen(path.as_string())+1;
            down_info->firmware_path = new char[len];
            if (!down_info->firmware_path) return false;
            memset(down_info->firmware_path, 0, len);
            strcpy(down_info->firmware_path, path.as_string());
            if (down_info->firmware_path[len-2] != '/'){	
        		SVC_LOG((LM_ERROR, "firmware path format error"));
        		return false;
        	}

            len = strlen(name.as_string())+1;
            down_info->firmware_name = new char[len];
            if (!down_info->firmware_name) {	
        		SVC_LOG((LM_ERROR, "new error"));
        		return false;
        	}
            memset(down_info->firmware_name, 0, len);
            strcpy(down_info->firmware_name, name.as_string());

            len = strlen(path.as_string())+strlen(name.as_string())+1;
            down_info->firmware_file = new char[len];
            if (!down_info->firmware_file) {	
        		SVC_LOG((LM_ERROR, "new error"));
        		return false;
        	}
            memset(down_info->firmware_file, 0, len);
            sprintf(down_info->firmware_file, "%s%s", path.as_string(), name.as_string());
            
            down_info->firmware_version = version.as_int();
            
            printf("dev_type: %d, path: %s, name: %s, file: %s, version: %d\n", 
                down_info->dev_type, down_info->firmware_path, down_info->firmware_name, 
                down_info->firmware_file, down_info->firmware_version);

            bool bret = DownManage::inst().insert(down_info);
            if (!bret) {	
        		SVC_LOG((LM_ERROR, "insert down info error"));
        		return false;
        	}
            
		    firmware = firmware.next_sibling("firmware");

		}
	}

	return true;
}

