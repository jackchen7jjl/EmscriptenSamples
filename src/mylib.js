mergeInto(LibraryManager.library, {

  GetTheDeviceInfo:function(outPtr) 
  {
	//var str = Pointer_stringify(outPtr);
	var deviceInfo = "userAgent:" + navigator.userAgent + ",appName:" + navigator.appName + ",appCodeName:" + navigator.appCodeName + ",platform: " + navigator.platform;
	stringToUTF8(deviceInfo, outPtr, 1024);
  },
  
});