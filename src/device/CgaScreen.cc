#include <device/CgaScreen.h>
#include "lib/tools.h"

CgaScreen::CgaScreen()
	: mAttr(),
	  index(Ports::P1),
	  data(Ports::P2),
	  screen((CgaChar*) Offset0)
{
	clear();
}

CgaScreen::CgaScreen(CgaAttr attr)
	: mAttr(attr),
	  index(Ports::P1),
	  data(Ports::P2),
	  screen((CgaChar*) Offset0)
{
	clear();
}

void CgaScreen::clear()
{
	setAttr(CgaAttr());

	// leert den gesamten Video RAM
	for (int row = 0; row < Rows; ++row)
	{
		for (int col = 0; col < Columns; ++col)
		{
			setCursor(col, row);
			show('\0');
		}
	}

	setCursor(0, 0);
}

void CgaScreen::scroll()
{
	// scrolle den Bildschirm und alles darunter im Video RAM
	memcpy((void*) Video::Offset0, (void*) (Video::Offset0 + Columns * 2), 2 * Columns * Rows);

	int row, col;
	getCursor(col, row);
	if (row > 0)
	{
		setCursor(0, row - 1);
	}
}

void CgaScreen::setAttr(const CgaAttr &attr)
{
	mAttr = attr;
}

void CgaScreen::getAttr(CgaAttr &attr)
{
	attr = mAttr;
}

void CgaScreen::setCursor(int column, int row)
{
	if (column < 0 || column >= Columns)
	{
		column = 0;
	}

	if (row < 0 || row >= Rows)
	{
		row = 0;
	}

	unsigned offset = row * Columns + column;

	index.write(I2);
	data.write(offset);
	index.write(I1);
	data.write(offset>>8);
}

void CgaScreen::getCursor(int &column, int &row)
{
	index.write(Cursor::I1);
	unsigned offset = data.read();

	offset <<= 8;
	index.write(Cursor::I2);
	offset |= data.read();

	/* off	0000 0000 1111 1111
	 * <<8	1111 1111 0000 0000
	 * | 	0000 0000 0100 1001
	 * =	1111 1111 0100 1001
	 */

	column = offset % Screen::Columns;
	row = offset / Screen::Columns;
}

void CgaScreen::show(char ch, const CgaAttr &attr)
{
	int row, col;
	getCursor(col, row);

	int offset = 2 * (row * Columns + col);
	screen = (CgaChar*) (Offset0 + offset);
	screen->setChar(ch);
	screen->setAttr(attr);
}

void CgaScreen::show(char ch)
{
	show(ch, mAttr);
}
