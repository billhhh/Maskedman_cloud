function getServiceInfo(){
	var serviceName = "<SN>人流轨迹分析服务</SN>";
	var servicePath = "<SP>./data/service_proc/QtTrajectoryAnalysis.exe</SP>";
	var serviceIcon = "<SI>./data/service_icon/statisticsdialog.jpg</SI>";
	var serviceData = "<SD>video</SD>";
	return serviceName + servicePath + serviceIcon + serviceData;
}

function getServiceName(){
	var serviceName = "轨迹分析服务";
	return serviceName;
}

function getServicePath(){
	var servicePath = "/data/service_proc/QtTrajectoryAnalysis.exe";
	return servicePath;
}

function getServiceIcon(){
	var serviceIcon = "/data/service_icon/trajectoryAnalyzing.png";
	return serviceIcon;
}

function getServiceData(){
	var serviceData = "video";
	return serviceData;
}
