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
//	CgaAttr cAttr(CgaAttr::Color::BLACK, CgaAttr::Color::BLACK, false);
//	int j = 1;
//
//	for(int i = 0; i <= 79; i++)
//	{
//		setCursor(i, j);
//		setAttr(cAttr);
//
//		if(i == 79)
//		{
//			i = 1;
//			j++;
//		}
//
//		if(j == 26)
//		{
//			i = 81;
//		}
//	}
}

void CgaScreen::scroll()
{
//	int j = 2;
//
//	for(int i = 0; i <= 79; i++)
//	{
//		setCursor(i, j - 1);
//		setAttr(getCursor(i, j));
//
//		if(i == 79)
//		{
//			i = 1;
//			j++;
//		}
//
//		if(j == 26)
//		{
//			i = 81;
//		}
//	}
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
