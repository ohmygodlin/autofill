// KeyboardSimulator.h: interface for the KeyboardSimulator class.
// This Class simulates the Keyboard operation.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KEYBOARDSIMULATOR_H__9742506F_AA07_45A3_BEDB_EF8BC2982288__INCLUDED_)
#define AFX_KEYBOARDSIMULATOR_H__9742506F_AA07_45A3_BEDB_EF8BC2982288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<TCHAR.h>
#include<windows.h>

#define CHAR_MAX 1000

class KeyboardSimulator  
{
public:
	virtual ~KeyboardSimulator();
	static KeyboardSimulator* getInstance();

	/* Get characters to be simulated as keyboard, put them in toPrintChars.
	** if success return true;
	** else return false;
	*/
	bool getCharsFromFile(FILE* fileName);
	/* Simulated keyboard as the characters storing in toPrintChars.
	** if success return true;
	** else return false;
	*/
	bool setChars();

	/* Simulated keyboard as the characters storing in vkCode, as virtual key code.
	** if success return true;
	** else return false;
	*/
	bool setChars(UINT vkCode);

protected:
	KeyboardSimulator();
	/* Set the character with scan code to $60
	** scanCode: Character to be set
	*/
	void _setChar(char scanCode);

	/* Convert some askii to Virtual key. (Not ALL!)
	** ch: askii code
	** return: Virtual Key
	*/
	UINT char2VK(char ch);

private:
	static KeyboardSimulator* _kbSimulator;
	TCHAR toPrintChars[CHAR_MAX];

};

#endif // !defined(AFX_KEYBOARDSIMULATOR_H__9742506F_AA07_45A3_BEDB_EF8BC2982288__INCLUDED_)
