#include "streambuilder.h"

uint8_t StreamBuilder_AddInt(int32_t dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType)
{
	uint8_t retValue = 0;
	if (builderType == BYTE)
	{
		int8_t _data = (int8_t) dataNum;
		dataStream[addPos] = _data;
		retValue = 1;
	}
	else if (builderType == INT16_BYTE)
	{
		int16_t _data = (int16_t) dataNum;
		dataStream[addPos] = _data & 0xff;
		dataStream[addPos + 1] = (_data >> 8) & 0xff;
		retValue = 2;
	}
	else if (builderType == INT32_BYTE)
	{
		int32_t _data = (int32_t) dataNum;
		dataStream[addPos] = _data & 0xff;
		dataStream[addPos + 1] = (_data >> 8) & 0xff;
		dataStream[addPos + 2] = (_data >> 16) & 0xff;
		dataStream[addPos + 3] = (_data >> 24) & 0xff;
		retValue = 4;
	}
	return retValue;
}

uint8_t StreamBuilder_AddUnsignedInt(uint32_t dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType)
{
	uint8_t retValue = 0;
	if (builderType == BYTE)
	{
		uint8_t _data = (uint8_t) dataNum;
		dataStream[addPos] = _data;
		retValue = 1;
	}
	else if (builderType == INT16_BYTE)
	{
		uint16_t _data = (uint16_t) dataNum;
		dataStream[addPos] = _data & 0xff;
		dataStream[addPos + 1] = (_data >> 8) & 0xff;
		retValue = 2;
	}
	else if (builderType == INT32_BYTE)
	{
		uint32_t _data = (uint32_t) dataNum;
		dataStream[addPos] = _data & 0xff;
		dataStream[addPos + 1] = (_data >> 8) & 0xff;
		dataStream[addPos + 2] = (_data >> 16) & 0xff;
		dataStream[addPos + 3] = (_data >> 24) & 0xff;
		retValue = 4;
	}
	return retValue;
}

uint8_t StreamBuilder_AddFloat(float dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType)
{
	int32_t _data = 0;
	if (builderType == FLOAT_BYTE_Q8) _data = (int32_t) (dataNum * (1 << 8));
	else if (builderType == FLOAT_BYTE_Q16) _data = (int32_t) (dataNum * (1 << 16));
	return StreamBuilder_AddInt(_data, dataStream, addPos, INT32_BYTE);
}

uint8_t StreamBuilder_AddUnsignedFloat(float dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType)
{
	uint32_t _data = 0;
	if (builderType == FLOAT_BYTE_Q8) _data = (uint32_t) (dataNum * (1 << 8));
	else if (builderType == FLOAT_BYTE_Q16) _data = (uint32_t) (dataNum * (1 << 16));
	return StreamBuilder_AddUnsignedInt(_data, dataStream, addPos, INT32_BYTE);
}


uint8_t StreamBuilder_AddCheckSum(uint8_t* dataStream, uint16_t startPos, uint16_t endPos, uint16_t addPos)
{
	uint8_t _checksum = 0x00;
	for (int i = startPos; i <= endPos; i++)
		_checksum ^= dataStream[i];
	dataStream[addPos] = _checksum;
	return 1;
}

uint8_t StreamBuilder_VerifyCheckSum(uint8_t* dataStream, uint16_t startPos, uint16_t endPos, uint8_t checkSum)
{
	uint8_t retValue = CHECKSUM_ERROR;
	uint8_t _checksum = 0x00;
	for (int i = startPos; i <= endPos; i++)
		_checksum ^= dataStream[i];
	if (checkSum == _checksum) retValue = CHECKSUM_OK;
	return retValue;
}

int32_t StreamBuilder_GetInt(uint8_t* dataStream, uint16_t getPos, uint8_t builderType)
{
	int32_t retValue = 0;
	if (builderType == BYTE) retValue = (int32_t) dataStream[getPos];
	else if (builderType == INT16_BYTE) retValue = (int32_t) (dataStream[getPos] + dataStream[getPos + 1] * BIT_8);
	else if (builderType == INT32_BYTE) retValue = (int32_t) (dataStream[getPos] + dataStream[getPos + 1] * BIT_8
		+ dataStream[getPos + 2] * BIT_16 + dataStream[getPos + 3] * BIT_24);
	return retValue;
}

uint32_t StreamBuilder_GetUnsignedInt(uint8_t* dataStream, uint16_t getPos, uint8_t builderType)
{
	uint32_t retValue = 0;
	if (builderType == BYTE) retValue = (uint32_t) dataStream[getPos];
	else if (builderType == INT16_BYTE) retValue = (uint32_t) (dataStream[getPos] + dataStream[getPos + 1] * BIT_8);
	else if (builderType == INT32_BYTE) retValue = (uint32_t) (dataStream[getPos] + dataStream[getPos + 1] * BIT_8
		+ dataStream[getPos + 2] * BIT_16 + dataStream[getPos + 3] * BIT_24);
	return retValue;
}

float StreamBuilder_GetFloat(uint8_t* dataStream, uint16_t getPos, uint8_t builderType)
{
	float retValue = 0;
	if (builderType == FLOAT_BYTE_Q8)
		retValue = (float) (StreamBuilder_GetInt(dataStream, getPos, INT32_BYTE) * 1.0f / (1 << 8));
	else if (builderType == FLOAT_BYTE_Q16)
		retValue = (float) (StreamBuilder_GetInt(dataStream, getPos, INT32_BYTE) * 1.0f / (1 << 16));
	return retValue;
}

float StreamBuilder_GetUnsignedFloat(uint8_t* dataStream, uint16_t getPos, uint8_t builderType)
{
	float retValue = 0;
	if (builderType == FLOAT_BYTE_Q8)
		retValue = (float) (StreamBuilder_GetUnsignedInt(dataStream, getPos, INT32_BYTE) * 1.0f / (1 << 8));
	else if (builderType == FLOAT_BYTE_Q16)
		retValue = (float) (StreamBuilder_GetUnsignedInt(dataStream, getPos, INT32_BYTE) * 1.0f / (1 << 16));
	return retValue;
}
