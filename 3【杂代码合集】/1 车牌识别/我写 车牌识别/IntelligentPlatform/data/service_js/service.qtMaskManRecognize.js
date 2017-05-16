function getServiceInfo(){
	var serviceName = "<SN>蒙面人检测服务/SN>";
	var servicePath = "<SP>./data/service_proc/QtMaskManRecognize.exe</SP>";
	var serviceIcon = "<SI>./data/service_icon/maskManDetect.png</SI>";
	var serviceData = "<SD>video</SD>";
	return serviceName + servicePath + serviceIcon + serviceData;
}

function getServiceName(){
	var serviceName = "蒙面检测服务";
	return serviceName;
}

function getServicePath(){
	var servicePath = "/data/service_proc/QtMaskManRecognize.exe";
	return servicePath;
}

function getServiceIcon(){
	var serviceIcon = "/data/service_icon/maskManDetect.png";
	return serviceIcon;
}

function getServiceData(){
	var serviceData = "video";
	return serviceData;
}
