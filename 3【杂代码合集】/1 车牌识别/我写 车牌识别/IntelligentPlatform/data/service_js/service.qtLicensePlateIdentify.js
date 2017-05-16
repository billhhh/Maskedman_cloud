function getServiceInfo(){
	var serviceName = "<SN>车牌识别服务/SN>";
	var servicePath = "<SP>./data/service_proc/QtLicensePlateIdentify.exe</SP>";
	var serviceIcon = "<SI>./data/service_icon/licensePlateIdentify.png</SI>";
	var serviceData = "<SD>image</SD>";
	return serviceName + servicePath + serviceIcon + serviceData;
}

function getServiceName(){
	var serviceName = "车牌识别服务";
	return serviceName;
}

function getServicePath(){
	var servicePath = "/data/service_proc/QtLicensePlateIdentify.exe";
	return servicePath;
}

function getServiceIcon(){
	var serviceIcon = "/data/service_icon/licensePlateIdentify.png";
	return serviceIcon;
}

function getServiceData(){
	var serviceData = "image";
	return serviceData;
}
