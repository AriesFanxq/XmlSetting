#ifndef AUT_INTERFACE_H
#define AUT_INTERFACE_H

#include <QObject>

class AUT_Interface : public QObject
{
	Q_OBJECT

public:
	AUT_Interface(QObject *parent);
	~AUT_Interface();

	/* ---------------------------- 基础部分 ---------------------------- */
	//SetOpen
	static void JSPT_CreateWorkbench();
	//SetClose
	static void JSPT_DestroyWorkbench();
	//SetStart、SetStop、SetPause
	static bool JSPT_SetScanState(const int nState);		//start/stop/pause都是这个函数，需不需要分开
	//DownloadConfig
	static bool JSPT_DownloadConfig();
	//GetTemperature
	static bool JSPT_GetTemperature(float &fInnerTemp, float &fWedgeTemp);
	//SetTrigMode
	static bool JSPT_SetTrigMode(const int nMode);

	/* ---------------------------- 编码器部分 ---------------------------- */
	//SetEncoderInitial
	static bool JSPT_SetEncoderInitial();
	//SetEncoderStart
	static bool JSPT_SetEncoderStart();
	//SetEncoderPause
	static int JSPT_SetEncoderPause();
	//SetEncoderStop
	static int JSPT_SetEncoderStop();
	//SetEncoderCalibratStart
	static bool JSPT_SetEncoderCalibratStart(const int nEncoder);
	//SetEncoderCalibratStop
	static int JSPT_SetEncoderCalibratStop(const int nEncoder);
	//SetEncoderSensity
	static bool JSPT_SetEncoderSensity(const int nEncoder);
	//GetEncoderSensity
	static int JSPT_GetEncoderSensity(const int nEncoder);
	//SetSingleEncoderTrig
	static bool JSPT_SetSingleEncoderTrig(const int nTotalNum, const int nEncoder, const float fInterval);
	//GetSingleEncoderTrig
	static bool JSPT_GetSingleEncoderTrig(int &nTotalNum, int &nEncoder, float &fInterval);
	//GetPosition
	static double JSPT_GetPosition(const int nEncoder);

	/* ---------------------------- 检测设置部分 ---------------------------- */
	//SetVelocity
	static bool JSPT_SetVelocity(const int nLongVel, const int nTranVel);
	//SetPA_DDF	nFlag = 0开启，nFlag = 1关闭
	static bool JSPT_SetPA_DDF(const int nFlag);
	//SetProbe
	static bool JSPT_SetProbe(const int nProbeNum, const int nType, const float fFreq,
		const int nCount, const float fPitch, const int nChannel);
	//SetPA_Wedge
	static bool JSPT_SetPA_Wedge(const int nWedgeNum, const float fAngle, const int nVel,
		const float fHeight);
	//SetGroup_PRF
	static bool JSPT_SetGroup_PRF(const int nFreq);
	//SetGroupCount
	static bool JSPT_SetGroupCount(const int nCount);
	//SetGroupType
	static bool JSPT_SetGroupType(const int nGroupNum, const int nTxProbe, const int nRxProbe,
		const int nTxWedge, const int nRxWedge);
	//SetGroupPulseWidth
	static bool JSPT_SetGroupPulseWidth(const int nGroupNum, const int nWidth);
	//SetGroup_Transmit
	static bool JSPT_SetGroup_Transmit(const int nGroupNum, const int nFirstEle, const int nActiveEle,
		const float fAngle, const float fFocus, const int nWaveType);
	//SetPAreceive
	static bool JSPT_SetPA_Receive(const int nGroupNum, const int nFirstEle, const int nActiveEle,
		const float fAngle, const float fFocus, const int nWaveType);
	//SetGroup_Window
	static bool JSPT_SetGroup_Window(const int nGroupNum, const float fStart, const float fWidth);
	//SetGroup_MeanValue
	static bool JSPT_SetGroup_MeanValue(const int nGroupNum);
	//SetGroup_Gain
	static bool JSPT_SetGroup_Gain(const int nGroupNum, const float fGain);
	//SetGroup_Filter 带通BP(band-pass)
	static bool JSPT_SetGroup_Filter(const int nGroupNum, const int nBP);
	//GetData	还没有确定
	static bool JSPT_GetData();
};

#endif // AUT_INTERFACE_H
