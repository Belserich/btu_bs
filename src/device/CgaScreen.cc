#include <device/CgaScreen.h>

CgaScreen::CgaScreen()
	: index(Ports::P1),
	  data(Ports::P2),
	  mAttr()
{
	clear();
}

CgaScreen::CgaScreen(CgaAttr attr)
	: index(Ports::P1),
	  data(Ports::P2),
	  mAttr(attr)
{
	clear();
}

void CgaScreen::clear()
{
	// leert den gesamten Video RAM
	for (int row = 0; row < Rows * Pages; ++row)
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
	CgaChar* dest;

	// scrolle den Bildschirm und alles darunter im Video RAM
	for (int row = 1; row < Rows * Pages; ++row)
	{
		for (int col = 0; col < Columns; ++col)
		{
			int off1 = 2 * ((row - 1) * Columns + col);
			int off2 = 2 * (row * Columns + col);
			dest = (CgaChar*) (Offset0 + off1);
			screen = (CgaChar*) (Offset0 + off2);

			dest->setChar(screen->getChar());
			dest->setAttr(screen->getAttr());
		}
	}

	// leere die letzte Zeile
//	int off = (Rows - 1) * Columns;
//	screen = (CgaChar*) (Offset0 + off);
//	CgaAttr attr;
//
//	for (int col = 0; col < Columns; col++)
//	{
//		screen->setChar('\0');
//		screen->setAttr(attr);
//		screen++;
//	}

	int row, col;
	getCursor(col, row);
	if (row > 0)
	{
		setCursor(0, Rows - 1);
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

	if (row < 0 || row >= Rows * Pages)
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
