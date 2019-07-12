#include "aut_interface.h"

AUT_Interface::AUT_Interface(QObject *parent)
	: QObject(parent)
{

}

AUT_Interface::~AUT_Interface()
{

}

#define Base_Part
void AUT_Interface::JSPT_CreateWorkbench()
{

}

void AUT_Interface::JSPT_DestroyWorkbench()
{

}

bool AUT_Interface::JSPT_SetScanState(const int nState)
{
	return true;
}

bool AUT_Interface::JSPT_DownloadConfig()
{
	return true;
}

bool AUT_Interface::JSPT_GetTemperature(float &fInnerTemp, float &fWedgeTemp)
{
	fInnerTemp = 0.0;
	fWedgeTemp = 0.0;

	return true;
}

bool AUT_Interface::JSPT_SetTrigMode(const int nMode)
{
	return true;
}

#define Encoder_Part
bool AUT_Interface::JSPT_SetEncoderInitial()
{
	return true;
}

bool AUT_Interface::JSPT_SetEncoderStart()
{
	return true;
}

int AUT_Interface::JSPT_SetEncoderPause()
{
	int nPulseNum = 0;

	return nPulseNum;
}

int AUT_Interface::JSPT_SetEncoderStop()
{
	int nPulseNum = 0;

	return nPulseNum;
}

bool AUT_Interface::JSPT_SetEncoderCalibratStart(const int nEncoder)
{
	return true;
}

int AUT_Interface::JSPT_SetEncoderCalibratStop(const int nEncoder)
{
	int nPulseNum = 0;

	return nPulseNum;
}

bool AUT_Interface::JSPT_SetEncoderSensity(const int nEncoder)
{
	return true;
}

int AUT_Interface::JSPT_GetEncoderSensity(const int nEncoder)
{
	int nPulseNum = 0;

	return nPulseNum;
}

bool AUT_Interface::JSPT_SetSingleEncoderTrig(const int nTotalNum, const int nEncoder, const float fInterval)
{
	return true;
}

bool AUT_Interface::JSPT_GetSingleEncoderTrig(int &nTotalNum, int &nEncoder, float &fInterval)
{
	nTotalNum = 0;
	nEncoder = 0;
	fInterval = 0.0;

	return true;
}

double AUT_Interface::JSPT_GetPosition(const int nEncoder)
{
	double dPosition = 0.0;

	return dPosition;
}

#define Detect_Part
bool AUT_Interface::JSPT_SetVelocity(const int nLongVel, const int nTranVel)
{
	return true;
}

bool AUT_Interface::JSPT_SetPA_DDF(const int nFlag)
{
	return true;
}

bool AUT_Interface::JSPT_SetProbe(const int nProbeNum, const int nType, const float fFreq, 
								  const int nCount, const float fPitch, const int nChannel)
{
	return true;
}

bool AUT_Interface::JSPT_SetPA_Wedge(const int nWedgeNum, const float fAngle, const int nVel, 
									 const float fHeight)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroup_PRF(const int nFreq)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroupCount(const int nCount)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroupType(const int nGroupNum, const int nTxProbe, const int nRxProbe, 
									  const int nTxWedge, const int nRxWedge)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroupPulseWidth(const int nGroupNum, const int nWidth)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroup_Transmit(const int nGroupNum, const int nFirstEle, const int nActiveEle,
										   const float fAngle, const float fFocus, const int nWaveType)
{
	return true;
}

bool AUT_Interface::JSPT_SetPA_Receive(const int nGroupNum, const int nFirstEle, const int nActiveEle,
									   const float fAngle, const float fFocus, const int nWaveType)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroup_Window(const int nGroupNum, const float fStart, const float fWidth)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroup_MeanValue(const int nGroupNum)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroup_Gain(const int nGroupNum, const float fGain)
{
	return true;
}

bool AUT_Interface::JSPT_SetGroup_Filter(const int nGroupNum, const int nBP)
{
	return true;
}

bool AUT_Interface::JSPT_GetData()
{
	return true;
}