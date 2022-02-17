// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	zVEC3 GetRgb(zSTRING color);
	zVEC3 String2Vec3(const zSTRING& s);
	COLORREF GetColorRef(zSTRING str);
	const char NAME_VALUE_SEPARATOR = ':';

	int zCPropEle::GetIconID()
	{
		
		return (int)GetPropType();
	};


	zCPropEle::zCPropEle(const char* c)
		: zSTRING(c)
	{
		
		checked = false;
	}


	zCPropEle::zCPropEle()
		: zSTRING()
	{
		
		checked = false;
	}


	TPropEditType zCPropEle::ParseDataType(const zSTRING& dtw)
	{
		
		if (dtw.Search("enum") >= 0) return PETenum;
		else if (dtw.Search("bool") >= 0) return PETbool;
		else if (dtw.Search("string") >= 0) return PETstring;
		else if (dtw.Search("int") >= 0) return PETint;
		else if (dtw.Search("float") >= 0) return PETfloat;
		else if (dtw.Search("raw") >= 0) return PETraw;
		else if (dtw.Search("vec3") >= 0) return PETvec3;
		else if (dtw.Search("color") >= 0) return PETcolor;
		return PETint;
	}

	COLORREF GetColorRef(zSTRING str)
	{
		
		return RGB(GetRgb(str).n[0], GetRgb(str).n[1], GetRgb(str).n[2]);
	}

	inline zVEC3 GetRgb(zSTRING color)
	{
		
		zVEC3 clr; clr = String2Vec3(color);

		return clr;
	}

	zVEC3 String2Vec3(const zSTRING& s) {
		
		zVEC3 v;
		v[0] = s.PickWord(1, " ", " ").ToFloat();
		v[1] = s.PickWord(2, " ", " ").ToFloat();
		v[2] = s.PickWord(3, " ", " ").ToFloat();
		return v;
	};

	TPropEditType zCPropEle::GetPropType()
	{
		
		if (Search("[") != -1)
		{
			if (Search("[]") != -1) return PETchunkend;
			else return PETclass;
		}
		else
		{
			zSTRING secWord = PickWord(2, "=", "=");
			if (secWord.Search("groupBegin") != -1 ||
				secWord.Search("groupEnd") != -1)
			{
				if (Search("End") != -1) return PETchunkend;
				else return PETgroup;
			}
			else
			{
				zSTRING thirdWord = PickWord(2, ":", ":");
				return ParseDataType(secWord);
			}
		}
		return PETint;
	}


	CString zCPropEle::GetName()
	{
		
		if (GetPropType() == PETclass)
		{
			zSTRING lStr = PickWord(1, " %[]", " %[]");
			return CString(lStr.ToChar());
		}
		else
			return CString(PickWord(1, "=", "=").ToChar());
	}


	CString zCPropEle::GetDescription()
	{
		
		CString description = GetName() + NAME_VALUE_SEPARATOR;
		TPropEditType type = GetPropType();
		if (type != PETclass &&
			type != PETgroup)
		{
			if (type == PETenum)
			{
				description += GetEnumVal();
			}
			else
				if (type == PETbool && GetIntVal() == 0)
				{
					description += CString("FALSE");
				}
				else
					if (type == PETbool && GetIntVal() == 1)
					{
						description += CString("TRUE");
					}
					else
					{
						description += GetStrVal();
					}
		}
		return description;
	}


	int zCPropEle::GetIntVal()
	{
		
		return PickWord(2, ":", ":").ToInt();
	}


	float zCPropEle::GetFloatVal()
	{
		
		return PickWord(2, ":", ":").ToFloat();
	}


	CString zCPropEle::GetStrVal()
	{
		
		int index = this->Search(":");
		if (index >= 0)
			return CString(Copied(index + 1).ToChar());
		else
			return "";
	}


	zVEC3 zCPropEle::GetVecVal()
	{
		
		zSTRING v = PickWord(2, ":", ":");
		return zVEC3(v.PickWord(1, " ", " ").ToFloat(),
			v.PickWord(2, " ", " ").ToFloat(),
			v.PickWord(3, " ", " ").ToFloat());
	}


	CString	zCPropEle::GetEnumVal()
	{
		
		zSTRING v = PickWord(2, "=:", "=:");
		return CString(v.PickWord(GetIntVal() + 2, ";", ";").ToChar());
	}


	void zCPropEle::GetEnumList(CComboBox& list)
	{
		
		list.ResetContent();
		zSTRING v = PickWord(2, "=:", "=:");
		int z = 2;
		zSTRING w = v.PickWord(z, ";", ";");
		while (!w.IsEmpty())
		{
			list.AddString(w.ToChar());
			z++;
			w = v.PickWord(z, ";", ";");
		}
	}


	CMFCPropertyGridProperty* zCPropEle::GetEnumList()
	{
		
		CMFCPropertyGridProperty* pEnum = AddProp(GetName(), GetEnumVal(), _T(""));
		zSTRING v = PickWord(2, "=:", "=:");
		int z = 2;
		zSTRING w = v.PickWord(z, ";", ";");
		while (!w.IsEmpty())
		{
			pEnum->AddOption(w.ToChar());
			z++;
			w = v.PickWord(z, ";", ";");
		}
		pEnum->AllowEdit(FALSE);
		return pEnum;
	}


	COLORREF zCPropEle::GetColor()
	{
		
		return RGB(100, 200, 0);
	}


	void zCPropEle::SetByInt(int v)
	{
		
		assign(zSTRING(PickWord(1, ":", ":") + zSTRING(":") + zSTRING(v)).ToChar());
	}


	void zCPropEle::SetByFloat(float v)
	{
		
		assign(PickWord(1, ":", ":").ToChar());
	}


	void zCPropEle::SetByStr(CString v)
	{
		
		assign(zSTRING(PickWord(1, ":", ":") + zSTRING(":") + zSTRING(v.GetBuffer(0))).ToChar());
	}


	void zCPropEle::SetByVec(zVEC3 v)
	{
		
		assign(zSTRING(PickWord(1, ":", ":") + zSTRING(":") +
			zSTRING(v[0]) + zSTRING(" ") + zSTRING(v[1]) + zSTRING(" ") + zSTRING(v[2])).ToChar());
	}


	void zCPropEle::SetByColor(COLORREF col)
	{
		
		assign
		(
			zSTRING(
				PickWord(1, ":", ":")
				+ ":"
				+ zSTRING(GetRValue(col)) + " "
				+ zSTRING(GetGValue(col)) + " "
				+ zSTRING(GetBValue(col)) + " 255"
			).ToChar()
		);
	}


	CString zCPropEle::PickValue(CString text)
	{
		
		int index = text.Find(NAME_VALUE_SEPARATOR, 0);
		text.Delete(0, index + 1);
		text.TrimLeft(' '); text.TrimRight(' ');
		return text;
	};


	CString zCPropEle::PickName(CString text)
	{
		
		int index = text.Find(NAME_VALUE_SEPARATOR, 0);
		text.Delete(index, text.GetLength() - index);
		text.TrimLeft(' '); text.TrimRight(' ');
		return text;
	};


	void zCPropEle::ToggleBoolVal()
	{
		
		SetByInt(1 - GetIntVal());
	};


	void zCPropEle::SetByEnum(CString v)
	{
		
		zSTRING text = PickWord(2, "=:", "=:");
		int wortNr = 2;
		int count = 0;
		zSTRING w = text.PickWord(wortNr++, ";", ";");
		while (!w.IsEmpty())
		{
			if (v == CString(w.ToChar()))
			{
				SetByInt(count);
				return;
			};
			w = text.PickWord(wortNr++, ";", ";");
			count++;
		}
	};


	void zCPropEle::ToggleEnumVal()
	{
		
		CString current = GetEnumVal();
		zSTRING v = PickWord(2, "=:", "=:");
		int wortNr = 2;
		zSTRING first = v.PickWord(wortNr++, ";", ";");
		zSTRING next = first;
		bool pickNext = false;
		while (!next.IsEmpty())
		{
			if (current == CString(next.ToChar())) pickNext = true;
			next = v.PickWord(wortNr++, ";", ";");
			if (pickNext && !next.IsEmpty())
			{
				SetByEnum(next.ToChar());
				return;
			}
		}
		SetByEnum(first.ToChar());
	}


	_variant_t zCPropEle::GetValue()
	{
		
		switch (GetPropType())
		{
		case PETbool:
			return _variant_t((short)GetIntVal(), VT_BOOL);
			break;
		case PETenum:
			return (_variant_t)GetEnumVal();
			break;
		case PETint:
			return (_variant_t)GetIntVal();
			break;
		case PETfloat:
			return (_variant_t)GetFloatVal();
			break;
		case PETstring:
			return (_variant_t)GetStrVal();
			break;
		case PETvec3:
			return (_variant_t)GetStrVal();
			break;
		case PETcolor:
			return (_variant_t)GetColorRef(GetStrVal().GetBuffer(0));
			break;
		case PETraw:
			return (_variant_t)GetStrVal();
			break;
		default:
			return (_variant_t)NULL;
			break;
		}
	};


	CPropList::CPropList()
	{
		
	};


	CPropList::~CPropList()
	{
		
	};


	zCPropEle* CPropList::SearchElementByString(CString item)
	{
		
		uint i = 0;
		while (i < GetNum())
		{
			if (operator[](i)->GetName() == item) return operator[](i);
			i++;
		};
		return 0;
	};

}