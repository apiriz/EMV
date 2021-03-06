#include <stdio.h>
#include <string.h>
#include "OperationEventWithValue.h"

OperationEventWithValue::OperationEventWithValue(void): 
	valueType(OPEVENT_NONE),
	stringValue(NULL),
	byteValue(NULL),
	length (-1)
{
}

OperationEventWithValue::~OperationEventWithValue(void)
{
}


// Retreives a data type of the value stored by this object
int OperationEventWithValue::getValueType(void)
{
	return valueType;
}


// Sets the long value of the object
int OperationEventWithValue::setLong(long Value)
{
	if (getValueType() != OPEVENT_NONE)
		releaseAllResources();
	valueType = OPEVENT_LONG;
	longValue = Value;
	length = 0;
	set_value();
	return setSuccess();
}

// Returns a long value stored in the object.
int OperationEventWithValue::getLong(long* Value)
{
	// Check if the value is set
	if (!IsValueSet())
		return ERR_OPEVNT__VALUE_IS_NOT_SET;

	// Check the value type; must be CNFG_DWORD.
	if (getValueType() != OPEVENT_LONG)
		return ERR_UNEXPECTED_TYPE;

	// Argument to this function must be a poinetr to an actual value
	if (!Value)
		return ERR_BAD_ARGUMENT;

	*Value = longValue;
	return SUCCESS;
}

// Sets a value of the object to string
int OperationEventWithValue::setString(const char* pString)
{
	releaseAllResources();
	
	if (pString)
	{
		stringValue = (char*) allocateResource (sizeof (char), (int)strlen(pString) + 1);
		if (!stringValue)
		{
			return setError(ERR_MEMORY_ALLOC, SETTING_OPERATION_EVENT);
		}
		strcpy (stringValue, pString);
		length = (int)strlen(stringValue);
	}
	else
		length = 0;

	valueType = OPEVENT_STRING;
	set_value();
	return setSuccess();
}


// Returns a pointer to the string kontained in this object 
int OperationEventWithValue::getString(char **string)
{
	// Check if the value is set
	if (!IsValueSet())
		return ERR_OPEVNT__VALUE_IS_NOT_SET;

	if (getValueType() != OPEVENT_STRING )
		return ERR_UNEXPECTED_TYPE;

	if (string == NULL)
		return ERR_BAD_ARGUMENT;

	*string = stringValue;
	return SUCCESS;
}

// Sets a value of this object to a byte string
int OperationEventWithValue::setByteString(const byte *pString, int len)
{
	releaseAllResources();

	if (len < 0)
		return setError(ERR_BAD_ARGUMENT, SETTING_OPERATION_EVENT);
	
	if (!pString && len > 0)
		return setError(ERR_BAD_ARGUMENT, SETTING_OPERATION_EVENT);
	
	if (len > 0)
	{
		byteValue = (byte*) allocateResource (sizeof(byte), len);
		if (!byteValue)
		{
			return setError(ERR_MEMORY_ALLOC, SETTING_OPERATION_EVENT);
		}
		//Manually copy bytes of data.
		// Don't use memcpy here, it will cause an error when run outside of development
		// environment! 
		// This is due to the face that buffer is allocated in a process Heap, and memcpy
		// cannot access that memory.
		for (int i = 0; i < len; i++)
			byteValue[i] = pString[i];
	}

	valueType = OPEVENT_BINARY;
	length = len;
	set_value();
	return setSuccess();
}

// Retreives a byte string stored by this object
int OperationEventWithValue::getByteString(byte** Value)
{
	// Check if the value is set
	if (!IsValueSet())
		return ERR_OPEVNT__VALUE_IS_NOT_SET;

	if (getValueType() != OPEVENT_BINARY)
		return ERR_UNEXPECTED_TYPE;

	if (Value == NULL)
		return ERR_BAD_ARGUMENT;

	*Value = byteValue;
	return SUCCESS;
}

// Retreives the length of the string stored by this object
int OperationEventWithValue::getLength(void)
{
	// Check if the value is set
	if (!IsValueSet())
		return ERR_OPEVNT__VALUE_IS_NOT_SET;

	return length;
}

void OperationEventWithValue::releaseAllResources()
{
	if (stringValue)
	{
		releaseResource((void**)&stringValue);
		stringValue = 0;
		length = -1;
	}
	if (byteValue)
	{
		releaseResource((void**)&byteValue);
		byteValue = 0;
		length = -1;
	}
	valueType = OPEVENT_NONE;
}

void OperationEventWithValue::resetEvent(bool withStop)
{
	releaseAllResources();
	OperationEvent::resetEvent(withStop);
}
