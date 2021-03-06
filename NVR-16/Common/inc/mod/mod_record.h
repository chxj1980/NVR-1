#ifndef _MOD_RECORD_H_
#define _MOD_RECORD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <sys/time.h>


/*************提供给业务层的*****************/

typedef enum			//分辨率枚举
{
	EM_REC_ENC_D1 = 0,		//D1
	EM_REC_ENC_4CIF,		//4CIF
	EM_REC_ENC_HALF_D1,		//HD1
	EM_REC_ENC_CIF,			//Cif
	EM_REC_ENC_QCIF,		//Qcif
	EM_REC_ENC_N_D1 = 10,	//N制D1
	EM_REC_ENC_N_4CIF,		//N制4CIF
	EM_REC_ENC_N_HALF_D1,	//N制HD1
	EM_REC_ENC_N_CIF,		//N制Cif
	EM_REC_ENC_N_QCIF,		//N制Qcif
	EM_REC_ENC_720P = 20,	//720P
	EM_REC_ENC_1080P,		//1080P
	EM_REC_ENC_960H,		//960H
	EM_REC_ENC_N_960H,		//N制960H//csp modify
}EMRECENCRESOL;

typedef struct mr_recordpara
{
	//u8 nChn;						//通道号
	u32 nBitRate;					//编码比特率(单位为k)
	EMRECENCRESOL emResolution;		//编码分辨率
	u8 nFrameRate;					//编码帧率
} SModRecordRecPara;

typedef enum				//业务层发送消息类型枚举
{
	EM_RECALARM_V_MOTION = 0,			//移动侦测
	EM_RECALARM_V_ALARM,				//报警输入
	EM_RECALARM_V_IPCEXTALARM,				//报警输入	
	EM_RECALARM_V_BIND,					//视频遮挡
	EM_RECALARM_V_LOST,					//视频丢失
	EM_RECALARM_NETLINK_LOST,			//网络断连
	EM_RECALARM_ABNORMAL,				//异常
} EMRECALARMTYPE;

typedef enum					//录像布防类型枚举
{
	EM_REC_SCH_WEEK_DAY = 0,
	EM_REC_SCH_MONTH_DAY,
	EM_REC_SCH_EVERY_DAY,
	EM_REC_SCH_ONCE_DAY,
}EMRECSCHTYPE;

typedef struct mr_schtime
{
	u32 nStartTime;	//每个时间段的起始时间
	u32 nStopTime;	//结束时间
}SModRecSchTime;

typedef enum
{
    EM_REC_MASK_MANUAL = (1 << 0),
    EM_REC_MASK_TIMING = (1 << 1),
    EM_REC_MASK_MOTION = (1 << 2),
    EM_REC_MASK_SENSORALARM = (1 << 3),
} EMRECTYPEMASK;

#define MAX_TIME_SEGMENTS 12	//每天可设置的时间段的最大数目

typedef enum					//褰曞儚甯冮槻绫诲瀷鏋氫妇
{
	EM_REC_SCH_REC_TIMER = 0,
	EM_REC_SCH_REC_VMOTION,
	EM_REC_SCH_REC_ALARM,
}EMRECSCHRECTYPE;

typedef struct mr_schpara
{
	u8 nChn;										//閫氶亾鍙�
	EMRECSCHTYPE nTimeType;							//褰曞儚甯冮槻鐨勬椂闂寸被鍨�
	u32 nValue;										//瀵瑰簲绫诲瀷涓嬬殑鍙栧�硷紝闇�瑕佹寜浣嶆搷浣滐紝锛堝彲浠ュ疄鐜拌〃绀�31澶╋級
	EMRECSCHRECTYPE nRecType;					
	SModRecSchTime nSchTime[31][MAX_TIME_SEGMENTS];	//鍚勬甯冮槻鏃堕棿
}SModRecSchPara;

/*
typedef struct mr_schpara
{
	u8 nChn;										//通道号
	EMRECSCHTYPE nTimeType;							//录像布防的时间类型
	u32 nValue;										//对应类型下的取值，需要按位操作，（可以实现表示31天）
	u8 nRecType[31][MAX_TIME_SEGMENTS];				//录像类型(位运算，由低到高位分别代表
													//定时、移动侦测、报警输入、遮挡、丢失、.....、其它)
	SModRecSchTime nSchTime[31][MAX_TIME_SEGMENTS];	//各段布防时间
}SModRecSchPara;
*/

typedef struct rec_header
{
	u8	nChn;					//帧的通道号
	u8	nStreamType;				//码流类型（0:视频流、1:视音频流、2: snap）
	u8	nMediaType; 				//编码方式（0:H264、10:PCMU、11:ADPCM）
	u8	nFrameType; 				//是否关键帧(0:否、1:是) FRAME_TYPE_P = 0,FRAME_TYPE_I = 1/3/5
	u32 nBitRate;					//比特率
								//nFrameType 在Snap模块用做抓图类型EM_SNAP_TYPE
	EMRECENCRESOL emResolution; 	//编码分辨率
	u64 nPts;						//时间戳（微妙）
								//nPts 在Snap模块用做struct timeval {time_t tv_sec; suseconds_t tv_usec}; gettimeofday(); )
	u64 nTimeStamp; 				//帧时间戳（毫秒）
	u8* nDate;					//指向帧数据的指针
	u32 nDataLength;				//帧数据的真实长度
	u16 width;
	u16 height;
}SModRecRecordHeader;



typedef void (*FPMODRECSTATUS)(void* status);
typedef void* RecHddInfo;
typedef void (*FPMODRECEXCEPTION)(u8 nType);

s32 ModRecordInit(u8 nChnNum, SModRecordRecPara* para, FPMODRECSTATUS getrecstatus, RecHddInfo hddinfo, FPMODRECEXCEPTION RecExceptCB);
s32 ModRecordDeInit();
s32 ModRecordStartManual(u8 nChn);
s32 ModRecordStopManual(u8 nChn);
s32 ModRecordStop(u8 nChn);
s32 ModRecordStopAll();
s32 ModRecordPause(u8 nChn);
s32 ModRecordResume(u8 nChn);
s32 ModRecordSetPreTime(u8 nChn, u32 time);
s32 ModRecordSetDelayTime(u8 nChn, u32 time);
//s32 ModRecordSetRecChnTriggered(u8 chn, EMRECALARMTYPE emType, u64 data);
s32 ModRecordSetTrigger(u8 chn, EMRECALARMTYPE emType, u8 key);
s32 ModRecordSetSchedulePara(u8 nChn, SModRecSchPara* para);
s32 ModRecordWriteOneFrame(u8 nChn, SModRecRecordHeader* header);
void ModRecordSetDealHardDiskFull(u8 nCover); //硬盘满时是否覆盖:0否 1是
void ModRecordSetWorkingEnable(u8 nEnable); //设置模块是否工作:0否 1是
s32 ModRecordBufRelease(void);//释放录像缓冲区，解放内存
s32 ModRecordBufResume(void); //恢复由上个接口释放掉的缓冲区

s32 ModRecordSetTimeZoneOffset(int nOffset);
s32 ModRecordGetTimeZoneOffset();

//yaogang modify 20141225
/*抓图类型	bit_mask	value
定时			0		
移动侦测	1
报警			2 //海康只有外部报警和IPC外部报警才抓图
动测或报警			(1<<1) |(1<<2)			用于备份
动测且报警			(1<<1) |(1<<2) |(1<<7)	同上
手动			3
/////////////////智能侦测	4
手动截图	4
////////////////////////////////回放截图	6
全部类型			0xff
预录			5		因为报警后要上传发生报警前n秒的图片
日常			6		(每天两个时间点上传深广)
全部类型	0xff
*/

typedef struct
{
	u8 chn;
	u8 pic_type;
	u32 data_size;
	u32 width;
	u32 height;
	time_t tv_sec;
	suseconds_t tv_usec;
}SModRecSnapHeader;

int ModRecordSnapshotToFile(u8 chn, u8 pic_type, time_t tv_sec, suseconds_t tv_usec, void *pdata, u32 data_size, u32 width, u32 height);
int ModRecordSnapshotToBuf(u8 chn, u8 pic_type, time_t tv_sec, suseconds_t tv_usec, void *pdata, u32 data_size, u32 width, u32 height);

//预录
int ModRecordPreSnapToFile(u8 chn, time_t tv_sec, void *pdata, u32 data_size, u32 width, u32 height);
int ModRecordPreSnapFromFile(u8 chn, time_t tv_sec, void *pdata, u32 * pdata_size);


/********************业务层提供结束***************************/

#ifdef __cplusplus
}
#endif

#endif //_MOD_RECORD_H_

