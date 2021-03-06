// dog.cpp : 定义 DLL 应用程序的导出函数。
//
#include <Windows.h>
#include "com_zzwtec_camera_fx_service_watchdog_WatchDogService.h"

#define Memory_Name L"Global\\ZZWTEC_SHARED_MEMORY"

typedef struct DogFood {
	//程序运行状态
	char status;
	//时间戳
	long long timestamp;
}DogFood;

DogFood * dogFood;

int  CreateDogFood();

/**
	嘿喂狗
*/
JNIEXPORT jint JNICALL Java_com_zzwtec_camera_fx_service_watchdog_WatchDogService_FeedDog(JNIEnv * env, jobject thisObj, jobject jDogFood) {
	if (dogFood == NULL) {
		int code = CreateDogFood();
		if (code == -1) {
			return -1;
		}
	}
	jclass  jDogFoodClass = (*env)->GetObjectClass(env,jDogFood);
	jmethodID getTimestampMID = (*env)->GetMethodID(env,jDogFoodClass,"getTimestamp","()J");
	jmethodID getStatusMID = (*env)->GetMethodID(env,jDogFoodClass,"getStatus","()I");
	jlong jtimestamp = (*env)->CallLongMethod(env,jDogFood,getTimestampMID);
	jint jstatus = (*env)->CallIntMethod(env,jDogFood,getStatusMID);
	dogFood->status = (char)jstatus;
	dogFood->timestamp = jtimestamp;

	return 0;
}


//狗粮快递
int CreateDogFood() {
	HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,Memory_Name);
	int rst = GetLastError();
	if (rst) {
		int errorCode = GetLastError();
		printf("内存申请失败!%d\n",errorCode);
		return -1;
	}
	//获取狗粮
	dogFood = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DogFood));
	printf("初始化状态码:%d\n",dogFood->status);
	printf("初始化时间戳:%lld\n", dogFood->timestamp);
	return 0;
}

