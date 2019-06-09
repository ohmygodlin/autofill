// KeyboardSimulator.cpp: implementation of the KeyboardSimulator class.
//
//////////////////////////////////////////////////////////////////////

#include "KeyboardSimulator.h"
#include<string.h>
#include<stdio.h>
#include<windows.h>
#include "winio.h"

#define MAPVK_VK_TO_VSC 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KeyboardSimulator* KeyboardSimulator::_kbSimulator = NULL;

KeyboardSimulator::KeyboardSimulator()
{
	memset(toPrintChars, 0, CHAR_MAX);
}

KeyboardSimulator::~KeyboardSimulator()
{

}

KeyboardSimulator* KeyboardSimulator::getInstance()
{
	if(_kbSimulator == NULL) {
		_kbSimulator = new KeyboardSimulator();
	}
	return _kbSimulator;
}

/* Get characters to be simulated as keyboard, put them in toPrintChars.
** if success return true;
** else return false;
*/
bool KeyboardSimulator::getCharsFromFile(FILE* chFile)
{
//	freopen(fileName, "r", stdin);
	memset(toPrintChars, 0, CHAR_MAX);
	if(fgets(toPrintChars, CHAR_MAX, chFile) != NULL) {
//	if(fscanf(chFile, "%s",toPrintChars) != EOF) {
		printf("The toPrintChars is %s\n", toPrintChars);
		return true;
	}
	else return false;
}

/* Set the character with scan code to $60
** scanCode: Character to be set
*/
void KeyboardSimulator::_setChar(char scanCode)
{
	_asm {
wait4IBE1:
		in al, 0x64
		and al, 10b
		//Wait entil OBF = 0, 
		//this should be done no matter setting command to $64, or setting data to $60
		jnz wait4IBE1
		// Set keyboard out command to $64
		mov al, 0xD2	//0xD2 for keyboard
		//mov al, 0xD3	//0xD3 for mouse
		out 0x64, al

wait4IBE2:
		in al, 0x64
		and al, 10b
		//Wait entil OBF = 0
		jnz wait4IBE2
		// Set scanCode of toPrintChar to $60
		mov al, scanCode
		out 0x60, al
	}
}

/* Simulated keyboard as the characters storing in toPrintChars.
** if success return true;
** else return false;
*/
bool KeyboardSimulator::setChars()
{
	// Initialize the WinIo library.
	bool result = InitializeWinIo();
	if(result) {
		// To set characters in toPrintChars
		int charLen = strlen(toPrintChars);
		if(charLen == 0) {
			printf("No Characters to print!\n");
		}
		for(int i = 0; i < charLen; i ++)
		{
			// ASKII 2 Virtual Code
			UINT vkCode = char2VK(toPrintChars[i]);
			// Virtual Code to Scan Code
			UINT scanCode = ::MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
			_setChar(scanCode);				// Send key down
			_setChar(scanCode | 0x80);		// Send key up
		}
		ShutdownWinIo();
	} else {
		printf("Error during initialization of WinIo. \n");
	}
	return result;
}

/* Simulated keyboard as the characters storing in vkCode, as virtual key code.
** if success return true;
** else return false;
*/
bool KeyboardSimulator::setChars(UINT vkCode)
{
	bool result = InitializeWinIo();
	if(result) {
		UINT scanCode = ::MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
			_setChar(scanCode);				// Send key down
			_setChar(scanCode | 0x80);		// Send key up 
			ShutdownWinIo();
	} else {
		printf("Error during initialization of WinIo. \n");
	}
	return result;
}

/* Convert some askii to Virtual key. (Not ALL!)
** ch: askii code
** return: Virtual Key
*/
UINT KeyboardSimulator::char2VK(char ch)
{
	if(!__isascii(ch)) 
	{
		printf("Not a ASkii!\n");
	} else 
	{
		if(islower(ch)) {
			ch = toupper(ch);
		}
		switch(ch) {
		case '\t': ch = VK_TAB;
			break;
		case '\n': ch = VK_RETURN;
			break;
		case ' ':	ch = VK_SPACE;
			break;
		}
	}
	return ch;
}