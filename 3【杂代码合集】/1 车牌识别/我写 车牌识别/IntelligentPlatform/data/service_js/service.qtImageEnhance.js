function getServiceInfo(){
	var serviceName = "<SN>图像增强服务</SN>";
	var servicePath = "<SP>./data/service_proc/QtImageEnhance.exe</SP>";
	var serviceIcon = "<SI>./data/service_icon/imageenhance.jpg</SI>";
	var serviceData = "<SD>image</SD>";
	return serviceName + servicePath + serviceIcon + serviceData;
}

function getServiceName(){
	var serviceName = "图像增强服务";
	return serviceName;
}

function getServicePath(){
	var servicePath = "/data/service_proc/QtImageEnhance.exe";
	return servicePath;
}

function getServiceIcon(){
	var serviceIcon = "/data/service_icon/imageEnhance.png";
	return serviceIcon;
}

function getServiceData(){
	var serviceData = "image";
	return serviceData;
}
