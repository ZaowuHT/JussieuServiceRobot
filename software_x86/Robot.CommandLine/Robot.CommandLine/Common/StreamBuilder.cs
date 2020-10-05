using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Robot.CommandLine.Common
{
    public class StreamBuilder
    {
        public enum BuilderType
        { BYTE = 0, INT16_BYTE = 1, INT32_BYTE = 2, FLOAT_BYTE_Q8 = 3, FLOAT_BYTE_Q16 = 4 };
        
        public enum ChecksumResult
        { CHECKSUM_ERROR = 0, CHECKSUM_OK = 1 };

        static public int BIT_24 = 16777216;
        static public int BIT_16 = 65536;
        static public int BIT_8 = 256;

        static public int AddInt(Int32 dataNum, byte[] dataStream, UInt16 addPos, BuilderType builderType)
        {
			int retValue = 0;
			if (builderType == BuilderType.BYTE)
			{
				sbyte _data = (sbyte)dataNum;
				dataStream[addPos] = (byte) (_data & 0xff);
				retValue = 1;
			}
			else if (builderType == BuilderType.INT16_BYTE)
			{
				Int16 _data = (Int16)dataNum;
				dataStream[addPos] = (byte) (_data & 0xff);
				dataStream[addPos + 1] = (byte) ((_data >> 8) & 0xff);
				retValue = 2;
			}
			else if (builderType == BuilderType.INT32_BYTE)
			{
				Int32 _data = (Int32)dataNum;
				dataStream[addPos] = (byte)(_data & 0xff);
				dataStream[addPos + 1] = (byte)((_data >> 8) & 0xff);
				dataStream[addPos + 2] = (byte)((_data >> 16) & 0xff);
				dataStream[addPos + 3] = (byte)((_data >> 24) & 0xff);
				retValue = 4;
			}
			return retValue;
		}

		static public int AddUnsignedInt(UInt32 dataNum, byte[] dataStream, UInt16 addPos, BuilderType builderType)
		{
			int retValue = 0;
			if (builderType == BuilderType.BYTE)
			{
				byte _data = (byte)dataNum;
				dataStream[addPos] = (byte)(_data & 0xff);
				retValue = 1;
			}
			else if (builderType == BuilderType.INT16_BYTE)
			{
				UInt16 _data = (UInt16)dataNum;
				dataStream[addPos] = (byte)(_data & 0xff);
				dataStream[addPos + 1] = (byte)((_data >> 8) & 0xff);
				retValue = 2;
			}
			else if (builderType == BuilderType.INT32_BYTE)
			{
				UInt32 _data = (UInt32)dataNum;
				dataStream[addPos] = (byte)(_data & 0xff);
				dataStream[addPos + 1] = (byte)((_data >> 8) & 0xff);
				dataStream[addPos + 2] = (byte)((_data >> 16) & 0xff);
				dataStream[addPos + 3] = (byte)((_data >> 24) & 0xff);
				retValue = 4;
			}
			return retValue;
		}

		static public int AddFloat(float dataNum, byte[] dataStream, UInt16 addPos, BuilderType builderType)
		{
			Int32 _data = 0;
			if (builderType == BuilderType.FLOAT_BYTE_Q8) _data = (Int32)(dataNum * (1 << 8));
			else if (builderType == BuilderType.FLOAT_BYTE_Q16) _data = (Int32)(dataNum * (1 << 16));
			return AddInt(_data, dataStream, addPos, BuilderType.INT32_BYTE);
		}

		static public int AddUnsignedFloat(float dataNum, byte[] dataStream, UInt16 addPos, BuilderType builderType)
		{
			UInt32 _data = 0;
			if (builderType == BuilderType.FLOAT_BYTE_Q8) _data = (UInt32)(dataNum * (1 << 8));
			else if (builderType == BuilderType.FLOAT_BYTE_Q16) _data = (UInt32)(dataNum * (1 << 16));
			return AddUnsignedInt(_data, dataStream, addPos, BuilderType.INT32_BYTE);
		}

		static public int AddCheckSum(byte[] dataStream, UInt16 startPos, UInt16 endPos, UInt16 addPos)
		{
			byte _checksum = 0x00;
			for (int i = startPos; i <= endPos; i++)
				_checksum ^= dataStream[i];
			dataStream[addPos] = _checksum;
			return 1;
		}

		static public ChecksumResult VerifyCheckSum(byte[] dataStream, UInt16 startPos, UInt16 endPos, byte checkSum)
		{
			ChecksumResult retValue = ChecksumResult.CHECKSUM_ERROR;
			byte _checksum = 0x00;
			for (int i = startPos; i <= endPos; i++)
				_checksum ^= dataStream[i];
			if (checkSum == _checksum) retValue = ChecksumResult.CHECKSUM_OK;
			return retValue;
		}

		static public Int32 GetInt(byte[] dataStream, UInt16 getPos, BuilderType builderType)
		{
			Int32 retValue = 0;
			if (builderType == BuilderType.BYTE) retValue = (Int32)dataStream[getPos];
			else if (builderType == BuilderType.INT16_BYTE) retValue = (Int32)(dataStream[getPos] + dataStream[getPos + 1] * BIT_8);
			else if (builderType == BuilderType.INT32_BYTE) retValue = (Int32)(dataStream[getPos] + dataStream[getPos + 1] * BIT_8
				+ dataStream[getPos + 2] * BIT_16 + dataStream[getPos + 3] * BIT_24);
			return retValue;
		}

		static public UInt32 GetUnsignedInt(byte[] dataStream, UInt16 getPos, BuilderType builderType)
		{
			UInt32 retValue = 0;
			if (builderType == BuilderType.BYTE) retValue = (UInt32)dataStream[getPos];
			else if (builderType == BuilderType.INT16_BYTE) retValue = (UInt32)(dataStream[getPos] + dataStream[getPos + 1] * BIT_8);
			else if (builderType == BuilderType.INT32_BYTE) retValue = (UInt32)(dataStream[getPos] + dataStream[getPos + 1] * BIT_8
				+ dataStream[getPos + 2] * BIT_16 + dataStream[getPos + 3] * BIT_24);
			return retValue;
		}

		static public float GetFloat(byte[] dataStream, UInt16 getPos, BuilderType builderType)
		{
			float retValue = 0;
			if (builderType == BuilderType.FLOAT_BYTE_Q8)
				retValue = (float)(GetInt(dataStream, getPos, BuilderType.INT32_BYTE) * 1.0f / (1 << 8));
			else if (builderType == BuilderType.FLOAT_BYTE_Q16)
				retValue = (float)(GetInt(dataStream, getPos, BuilderType.INT32_BYTE) * 1.0f / (1 << 16));
			return retValue;
		}

		static public float GetUnsignedFloat(byte[] dataStream, UInt16 getPos, BuilderType builderType)
		{
			float retValue = 0;
			if (builderType == BuilderType.FLOAT_BYTE_Q8)
				retValue = (float)(GetUnsignedInt(dataStream, getPos, BuilderType.INT32_BYTE) * 1.0f / (1 << 8));
			else if (builderType == BuilderType.FLOAT_BYTE_Q16)
				retValue = (float)(GetUnsignedInt(dataStream, getPos, BuilderType.INT32_BYTE) * 1.0f / (1 << 16));
			return retValue;
		}
	};
};
