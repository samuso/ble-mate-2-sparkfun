/*
  sam.cpp - Library for testing libraries in Arduino.
  Created by BLElib Fallahi 10 March, 2015.
*/

#include <Arduino.h>
#include "BLElib.h"

// Works correctly.
BLElib::BLElib(Stream *serial_stream)
{
	stream = serial_stream;
}

// Works correctly.
void BLElib::sendCMD(String command)
{
	stream->println(command);
    stream->print("\r");
    stream->flush();
}

// Works correctly.
bool BLElib::cmdIsValid(String buffer)
{
	int len = buffer.length();
	for (int i = 0; i < len - 1; i++)
	{
		if (buffer.charAt(i) == 'O' && buffer.charAt(i+1) == 'K'){
			return true;
		}
	}
	return false;
}

// Works correctly.
/*
*	Tries seding the command 10 times
*   if successful then returns the 
*	clean buffer, otherwise returns ERR.
*	Secure way to send commands.
*/ 
String BLElib::cmd(String input)
{
	String output = "";
	for (int i = 0; i < 10; i++)
	{
		sendCMD(input);
		output = getBuffer();
		if (cmdIsValid(output))
		{
			output = cleanBuffer(output);
			return output;	
		}
	}
	return "ERR\n";
}


// Gets rid of the OK and ERR at the end of each buffer
// Works correctly
String BLElib::cleanBuffer(String buffer) 
{
	String clean_buffer = "";
	int len = buffer.length();
	if (len >= 9)
	{
		for (int i = 0; i < len - 10; i++) 
		{
			if (!(buffer.charAt(i) == '\n') || !(i == (len - 11))) 
			{
				clean_buffer.concat(buffer.charAt(i));
			}
		}
	}
	return clean_buffer;
}

// Works correctly.
bool BLElib::bufferIsEmpty()
{
	if (stream->available() > 0)
	{
		return false;
	}
	return true;
}

// Works correctly.
String BLElib::getBuffer()
{
	String buffer = "";
	while (!bufferIsEmpty())
	{
		buffer.concat(char(stream->read())); 
		// IDK why I put this there ...
		delay(1);
    }
    return buffer;
} 

void BLElib::resetDevice()
{
	sendCMD("RST");
	// Gives device time to boot up.
	delay(10000);
	emptyBuffer();
}

void BLElib::emptyBuffer()
{
	while(!bufferIsEmpty())
	{
		stream->read();
	}
}

// Works correctly.
bool BLElib::contains(String str, String sub_str)
{
	int counter = 0;
	for (int i = 0; i < str.length(); i++) 
	{
		if (str.charAt(i) == sub_str.charAt(counter))
		{
			counter++;
			if (counter == sub_str.length())
			{
				return true;
			}
		}
		else
		{
			counter = 0;
		}
	}
	return false;
}

// Works Correctly.
bool BLElib::setShortName(String sName)
{
	if (sName.length() > 7 || sName.length() < 1)
	{
		emptyBuffer();
		return false;
	}
	cmd("SET SNAME=" + sName);
	delay(1);
	if (contains(cmd("GET SNAME"),sName))
	{
		cmd("WRT");
		emptyBuffer();
		return true;
	}
	emptyBuffer();
	return false;
}

// Works correctly.
bool BLElib::isDiscoverable()
{
	if (contains(cmd("STS"), "P ADV"))
	{
		return true;
	}
	return false;
}

// Works correctly.
/*
* Is supposed to be used only once from
* the devices default state, otherwise
* user will have to send custom cmds. 
*/
void BLElib::makeDiscoverable()
{

	cmd("SCN OFF");
	cmd("SET CENT=OFF");
	cmd("ADV ON");
}

void BLElib::autoconnect()
{
	cmd("ADV OFF");
	cmd("SET CENT=ON");
	cmd("SET ACON=ON");
	cmd("SET CCON=ON");
	cmd("SCN ON");
	emptyBuffer();
	cmd("SND CONNECTED");
}