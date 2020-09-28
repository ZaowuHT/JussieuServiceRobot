#ifndef STREAMBUILDER_H
#define STREAMBUILDER_H

#include "system.h"

// 数据流构造器相关常量声明区域

#define BYTE 0
#define INT16_BYTE 1
#define INT32_BYTE 2
#define FLOAT_BYTE_Q8 3
#define FLOAT_BYTE_Q16 4
#define CHECKSUM_ERROR 0
#define CHECKSUM_OK 1

#define BIT_24 16777216l
#define BIT_16 65536l
#define BIT_8 256

// 数据流构造器方法声明区域

uint8_t StreamBuilder_AddInt(int32_t dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType);
uint8_t StreamBuilder_AddUnsignedInt(uint32_t dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType);
uint8_t StreamBuilder_AddFloat(float dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType);
uint8_t StreamBuilder_AddUnsignedFloat(float dataNum, uint8_t* dataStream, uint16_t addPos, uint8_t builderType);
uint8_t StreamBuilder_AddCheckSum(uint8_t* dataStream, uint16_t startPos, uint16_t endPos, uint16_t addPos);
uint8_t StreamBuilder_VerifyCheckSum(uint8_t* dataStream, uint16_t startPos, uint16_t endPos, uint8_t checkSum);
int32_t StreamBuilder_GetInt(uint8_t* dataStream, uint16_t getPos, uint8_t builderType);
uint32_t StreamBuilder_GetUnsignedInt(uint8_t* dataStream, uint16_t getPos, uint8_t builderType);
float StreamBuilder_GetFloat(uint8_t* dataStream, uint16_t getPos, uint8_t builderType);
float StreamBuilder_GetUnsignedFloat(uint8_t* dataStream, uint16_t getPos, uint8_t builderType);

#endif
