// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw ERROR_BITFIELD_SIZE;
	this->MemLen = len;
	this->BitLen = ((int)(len / TBitField::GetBitInField())+1)*sizeof(TELEM)*8;
	this->pMem = new TELEM[BitLen/ TBitField::GetBitInField()];
	for (size_t i = 0; i < GetCountField(); i++)
	{
		this->pMem[i] = 0;
	}
}


TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
	this->pMem = new TELEM[BitLen / TBitField::GetBitInField()];
	for (size_t i = 0; i < GetCountField(); i++)
	{
		this->pMem[i] = bf.pMem[i];
	}
	
}

TBitField::~TBitField()
{
	delete this->pMem;
}

unsigned int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n >= this->MemLen)
		throw ERROR_BITFIELD_INDEX;
	return (unsigned int)(n/this->GetBitInField());
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= this->MemLen)
		throw ERROR_BITFIELD_INDEX;
	return (1 << n%TBitField::GetBitInField());
}

int TBitField::GetBitInField()
{
	return sizeof(TELEM) * 8;
}

int TBitField::GetCountField() const
{
	return this->BitLen / TBitField::GetBitInField();
}


// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->MemLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= this->MemLen)
		throw ERROR_BITFIELD_INDEX;
	this->pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= this->MemLen)
		throw ERROR_BITFIELD_INDEX;
	this->pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= this->MemLen)
		throw ERROR_BITFIELD_INDEX;
  return (this->pMem[GetMemIndex(n)] >> n)&1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if(this->MemLen != bf.MemLen)
		throw ERROR_BITFIELD_SIZE;
	delete this->pMem;
	this->MemLen = bf.MemLen;
	this->BitLen = bf.BitLen;
	this->pMem = new TELEM[BitLen / TBitField::GetBitInField()];
	for (size_t i = 0; i < GetCountField(); i++)
	{
		this->pMem[i] = bf.pMem[i];
	}
	return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this->MemLen != bf.MemLen)
		throw ERROR_BITFIELD_SIZE;
	bool check = true;
	for (int i = 0; i < this->GetCountField(); i++)
		if (this->pMem[i] != bf.pMem[i])
			check = false;
  return check;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !operator==(bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (this->MemLen != bf.MemLen)
		throw ERROR_BITFIELD_SIZE;
	TBitField temp(this->MemLen);
	for (size_t i = 0; i < this->GetCountField(); i++)
	{
		temp.pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return temp;
}
TBitField& TBitField::operator|=(const TBitField &bf) // операция "или"
{
	if (this->MemLen != bf.MemLen)
		throw ERROR_BITFIELD_SIZE;
	for (size_t i = 0; i < this->GetCountField(); i++)
	{
		this->pMem[i] = this->pMem[i] | bf.pMem[i];
	}
	return *this;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (this->MemLen != bf.MemLen)
		throw ERROR_BITFIELD_SIZE;
	TBitField temp(this->MemLen);
	for (size_t i = 0; i < this->GetCountField(); i++)
	{
		temp.pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return temp;
}

TBitField& TBitField::operator&=(const TBitField &bf) // операция "и"
{
	if (this->MemLen != bf.MemLen)
		throw ERROR_BITFIELD_SIZE;
	for (size_t i = 0; i < this->GetCountField(); i++)
	{
		this->pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return *this;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(this->MemLen);
	TELEM temp_tel = temp.pMem[this->GetCountField() - 1];
	for (size_t i = 0; i < this->GetCountField(); i++)
	{
		temp.pMem[i] = ~this->pMem[i];
	}
	int CountBitNotUsed = this->BitLen - this->MemLen;
	TELEM LastFieldBitMask = 0;
	for (size_t i = 0; i < CountBitNotUsed; i++)
		LastFieldBitMask |= 1 << i;
	
	temp.pMem[this->GetCountField() - 1] ^= LastFieldBitMask;
	return temp;
}

// ввод/вывод

istream &operator >> (istream &istr, TBitField &bf) // ввод
{
	if (bf.MemLen < istr.gcount())
		throw ERROR_BITFIELD_SIZE;
	char temp;
	int index = 0;
	while(istr.get(temp))
	{
		if (temp == '\n')
			break;
		if (temp == ' ')
			continue;
		if (temp != '1' && temp != '0')
			throw ERROR_BITFIELD_VALUE;
		if (temp == '1')
			bf.SetBit(index++);
		else
			bf.ClrBit(index++);
	} 
	
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.GetCountField(); i++)
	{
		for (size_t z = 0; z < TBitField::GetBitInField(); z++)
		{
			if (z % 8 == 0 && z != 0)
				ostr << " ";
			ostr << ((bf.pMem[i] & 1 << z) ? 1 : 0);			
		}
		ostr<< endl;
	}
	return ostr;
}
