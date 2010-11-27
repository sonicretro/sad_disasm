/* List of functions:
	main() - Creates struct definitions required for other functions.
	MakeModelStructs(address) - Creates structs for the OBJECT at address.
	ModelPointers() - Calls MakeModelStructs for every pointer in the selected area.
	MakeAttachStructs(address) - Creates structs for the ATTACH at address.
	MakeLandTableStructs(address) - Creates structs for the LandTable at address (calls MakeModelStructs MakeAnimStructs MakeTexListStructs)..
	LandTablePointers() - Calls MakeLandTableStructs for every pointer in the selected area.
	MakeAnimStructs(address) - Creates structs for the AnimHead at address.
	AnimPointers() - Calls MakeAnimStructs for every pointer in the selected area.
	MakeTexListStructs(address) - Creates structs for the TexListHead at address.
	TexListPointers() - Calls MakeTexListStructs for every pointer in the selected area.
	All functions return 1 on completion.
*/

#define MS_0TYPE 0x00F00000L            // Mask for 1st arg typing
#define FF_0VOID 0x00000000L            // Void (unknown)?
#define FF_0NUMH 0x00100000L            // Hexadecimal number?
#define FF_0NUMD 0x00200000L            // Decimal number?
#define FF_0CHAR 0x00300000L            // Char ('x')?
#define FF_0SEG  0x00400000L            // Segment?
#define FF_0OFF  0x00500000L            // Offset?
#define FF_0NUMB 0x00600000L            // Binary number?
#define FF_0NUMO 0x00700000L            // Octal number?
#define FF_0ENUM 0x00800000L            // Enumeration?
#define FF_0FOP  0x00900000L            // Forced operand?
#define FF_0STRO 0x00A00000L            // Struct offset?
#define FF_0STK  0x00B00000L            // Stack variable?

#define MS_1TYPE 0x0F000000L            // Mask for 2nd arg typing
#define FF_1VOID 0x00000000L            // Void (unknown)?
#define FF_1NUMH 0x01000000L            // Hexadecimal number?
#define FF_1NUMD 0x02000000L            // Decimal number?
#define FF_1CHAR 0x03000000L            // Char ('x')?
#define FF_1SEG  0x04000000L            // Segment?
#define FF_1OFF  0x05000000L            // Offset?
#define FF_1NUMB 0x06000000L            // Binary number?
#define FF_1NUMO 0x07000000L            // Octal number?
#define FF_1ENUM 0x08000000L            // Enumeration?
#define FF_1FOP  0x09000000L            // Forced operand?
#define FF_1STRO 0x0A000000L            // Struct offset?
#define FF_1STK  0x0B000000L            // Stack variable?

// The following macros answer questions like
//   'is the 1st (or 2nd) operand of instruction or data of the given type'?
// Please note that data items use only the 1st operand type (is...0)

#define isDefArg0(F)    ((F & MS_0TYPE) != FF_0VOID)
#define isDefArg1(F)    ((F & MS_1TYPE) != FF_1VOID)
#define isDec0(F)       ((F & MS_0TYPE) == FF_0NUMD)
#define isDec1(F)       ((F & MS_1TYPE) == FF_1NUMD)
#define isHex0(F)       ((F & MS_0TYPE) == FF_0NUMH)
#define isHex1(F)       ((F & MS_1TYPE) == FF_1NUMH)
#define isOct0(F)       ((F & MS_0TYPE) == FF_0NUMO)
#define isOct1(F)       ((F & MS_1TYPE) == FF_1NUMO)
#define isBin0(F)       ((F & MS_0TYPE) == FF_0NUMB)
#define isBin1(F)       ((F & MS_1TYPE) == FF_1NUMB)
#define isOff0(F)       ((F & MS_0TYPE) == FF_0OFF)
#define isOff1(F)       ((F & MS_1TYPE) == FF_1OFF)
#define isChar0(F)      ((F & MS_0TYPE) == FF_0CHAR)
#define isChar1(F)      ((F & MS_1TYPE) == FF_1CHAR)
#define isSeg0(F)       ((F & MS_0TYPE) == FF_0SEG)
#define isSeg1(F)       ((F & MS_1TYPE) == FF_1SEG)
#define isEnum0(F)      ((F & MS_0TYPE) == FF_0ENUM)
#define isEnum1(F)      ((F & MS_1TYPE) == FF_1ENUM)
#define isFop0(F)       ((F & MS_0TYPE) == FF_0FOP)
#define isFop1(F)       ((F & MS_1TYPE) == FF_1FOP)
#define isStroff0(F)    ((F & MS_0TYPE) == FF_0STRO)
#define isStroff1(F)    ((F & MS_1TYPE) == FF_1STRO)
#define isStkvar0(F)    ((F & MS_0TYPE) == FF_0STK)
#define isStkvar1(F)    ((F & MS_1TYPE) == FF_1STK)

//
//      Bits for DATA bytes
//

#define DT_TYPE 0xF0000000L             // Mask for DATA typing

#define FF_BYTE 0x00000000L             // byte
#define FF_WORD 0x10000000L             // word
#define FF_DWRD 0x20000000L             // dword
#define FF_QWRD 0x30000000L             // qword
#define FF_TBYT 0x40000000L             // tbyte
#define FF_ASCI 0x50000000L             // ASCII ?
#define FF_STRU 0x60000000L             // Struct ?
#define FF_OWRD 0x70000000L             // octaword (16 bytes)
#define FF_FLOAT 0x80000000L            // float
#define FF_DOUBLE 0x90000000L           // double
#define FF_PACKREAL 0xA0000000L         // packed decimal real
#define FF_ALIGN    0xB0000000L         // alignment directive

//
//      Bits for CODE bytes
//

#define MS_CODE 0xF0000000L
#define FF_FUNC 0x10000000L             // function start?
#define FF_IMMD 0x40000000L             // Has Immediate value ?
#define FF_JUMP 0x80000000L             // Has jump table

static main()
{
	auto strID;
	strID = AddStrucEx(-1, "Vector3", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "X", 0x0, FF_FLOAT, -1, 4);
		AddStrucMember(strID, "Y", 0x4, FF_FLOAT, -1, 4);
		AddStrucMember(strID, "Z", 0x8, FF_FLOAT, -1, 4);
	}
	strID = AddStrucEx(-1, "Rotation3", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "X", 0x0, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Y", 0x4, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Z", 0x8, FF_DWRD, -1, 4);
	}
	strID = AddStrucEx(-1, "LandTable", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "COLCount", 0x0, FF_WORD|FF_0NUMD, -1, 2);
		AddStrucMember(strID, "AnimCount", 0x2, FF_WORD|FF_0NUMD, -1, 2);
		AddStrucMember(strID, "Unknown_0", 0x4, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Unknown_1", 0x8, FF_DWRD, -1, 4);
		AddStrucMember(strID, "COL", 0xC, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "AnimData", 0x10, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "TexName", 0x14, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "TexList", 0x18, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Unknown_4", 0x1C, FF_DWRD, 0, 4);
		AddStrucMember(strID, "Unknown_5", 0x20, FF_DWRD, 0, 4);
	}
	strID = AddStrucEx(-1, "COL", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "Center", 0x0, FF_STRU, GetStrucIdByName("Vector3"), 12);
		AddStrucMember(strID, "Radius", 0xC, FF_FLOAT, -1, 4);
		AddStrucMember(strID, "Padding", 0x10, FF_DWRD, -1, 8);
		AddStrucMember(strID, "Object", 0x18, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "anonymous_6", 0x1C, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Flags", 0x20, FF_DWRD, -1, 4);
	}
	strID = AddStrucEx(-1, "OBJECT", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "Flags", 0x0, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Attach", 0x4, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Position", 0x8, FF_STRU, GetStrucIdByName("Vector3"), 12);
		AddStrucMember(strID, "Rotation", 0x14, FF_STRU, GetStrucIdByName("Rotation3"), 12);
		AddStrucMember(strID, "Scale", 0x20, FF_STRU, GetStrucIdByName("Vector3"), 12);
		AddStrucMember(strID, "Child", 0x2C, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Relate", 0x30, FF_DWRD|FF_0OFF, 0, 4);
	}
	strID = AddStrucEx(-1, "ATTACH", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "Vertex", 0x0, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Normal", 0x4, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "VertexTotal", 0x8, FF_DWRD|FF_0NUMD, -1, 4);
		AddStrucMember(strID, "Mesh", 0xC, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Material", 0x10, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "MeshTotal", 0x14, FF_WORD|FF_0NUMD, -1, 2);
		AddStrucMember(strID, "MaterialTotal", 0x16, FF_WORD|FF_0NUMD, -1, 2);
		AddStrucMember(strID, "Center", 0x18, FF_STRU, GetStrucIdByName("Vector3"), 12);
		AddStrucMember(strID, "Radius", 0x24, FF_FLOAT, -1, 4);
	}
	strID = AddStrucEx(-1, "MESH", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "MatID", 0x0, FF_BYTE|FF_0NUMD, -1, 1);
		AddStrucMember(strID, "PolyType", 0x1, FF_BYTE, -1, 1);
		AddStrucMember(strID, "PolyTotal", 0x2, FF_WORD|FF_0NUMD, -1, 2);
		AddStrucMember(strID, "Poly", 0x4, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "PAttr", 0x8, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "PNormal", 0xC, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "VColor", 0x10, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "UV", 0x14, FF_DWRD|FF_0OFF, 0, 4);
	}
	strID = AddStrucEx(-1, "MATERIAL", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "Diffuse", 0x0, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Specular", 0x4, FF_DWRD, -1, 4);
		AddStrucMember(strID, "field_8", 0x8, FF_FLOAT, -1, 4);
		AddStrucMember(strID, "TexID", 0xC, FF_DWRD|FF_0NUMD, -1, 4);
		AddStrucMember(strID, "field_10", 0x10, FF_WORD, -1, 2);
		AddStrucMember(strID, "Flags", 0x12, FF_BYTE, -1, 1);
		AddStrucMember(strID, "EndOfMat", 0x13, FF_BYTE, -1, 1);
	}
	strID = AddStrucEx(-1, "UV", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "U", 0x0, FF_WORD|FF_0NUMD, -1, 2);
		AddStrucMember(strID, "V", 0x2, FF_WORD|FF_0NUMD, -1, 2);
	}
	strID = AddStrucEx(-1, "GeoAnimData", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "anonymous_0", 0x0, FF_DWRD, -1, 4);
		AddStrucMember(strID, "anonymous_1", 0x4, FF_DWRD, -1, 4);
		AddStrucMember(strID, "anonymous_2", 0x8, FF_DWRD, -1, 4);
		AddStrucMember(strID, "Model", 0xC, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Animation", 0x10, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "anonymous_3", 0x14, FF_DWRD, -1, 4);
	}
	strID = AddStrucEx(-1, "AnimHead", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "Model", 0x0, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Head2", 0x4, FF_DWRD|FF_0OFF, 0, 4);
	}
	strID = AddStrucEx(-1, "AnimHead2", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "FrameData", 0x0, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "FrameCount", 0x4, FF_DWRD|FF_0NUMD, -1, 4);
		AddStrucMember(strID, "Flags", 0x8, FF_WORD, -1, 2);
		AddStrucMember(strID, "field_A", 0xA, FF_WORD, -1, 2);
	}
	strID = AddStrucEx(-1, "TexListHead", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "Texlist", 0x0, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Count", 0x4, FF_DWRD|FF_0NUMD, -1, 4);
	}
	strID = AddStrucEx(-1, "TexListEntry", 0);
	if(strID != -1)
	{
		AddStrucMember(strID, "TexName", 0x0, FF_DWRD|FF_0OFF, 0, 4);
		AddStrucMember(strID, "Unknown", 0x4, FF_DWRD, -1, 4);
		AddStrucMember(strID, "RAMAddr", 0x8, FF_DWRD|FF_0OFF, 0, 4);
	}
}

static MakeModelStructs(address)
{
	auto ptr;
	MakeUnknown(address, GetStrucSize(GetStrucIdByName("OBJECT")), 2);
	MakeStructEx(address, -1, "OBJECT");
	ptr=Dword(address+4);
	if(ptr != 0) MakeAttachStructs(ptr);
	ptr=Dword(address+0x2C);
	if(ptr != 0) MakeModelStructs(ptr);
	ptr=Dword(address+0x30);
	if(ptr != 0) MakeModelStructs(ptr);
	return 1;
}

static ModelPointers()
{
	auto startaddr;
	auto endaddr;
	auto addr;
	auto ptr;
	startaddr = SelStart();
	endaddr = SelEnd();
	for(addr = startaddr; addr < endaddr; addr = addr + 4)
	{
		ptr = Dword(addr);
		if(ptr != 0) MakeModelStructs(ptr);
	}
	return 1;
}

static MakeAttachStructs(address)
{
	auto ptr2;
	auto cnt;
	auto polytype;
	auto i;
	auto j;
	auto polycnt;
	auto ptr3;
	auto strcnt;
	auto uvcnt;
	MakeUnknown(address, GetStrucSize(GetStrucIdByName("ATTACH")), 2);
	MakeStructEx(address, -1, "ATTACH");
	cnt=Dword(address+8);
	ptr2=Dword(address);
	MakeUnknown(ptr2, GetStrucSize(GetStrucIdByName("Vector3")) * cnt, 2);
	MakeStructEx(ptr2, -1, "Vector3");
	MakeArray(ptr2, cnt);
	ptr2=Dword(address+4);
	MakeUnknown(ptr2, GetStrucSize(GetStrucIdByName("Vector3")) * cnt, 2);
	MakeStructEx(ptr2, -1, "Vector3");
	MakeArray(ptr2, cnt);
	cnt=Word(address+0x16);
	ptr2=Dword(address+0x10);
	MakeUnknown(ptr2, GetStrucSize(GetStrucIdByName("MATERIAL")) * cnt, 2);
	MakeStructEx(ptr2, -1, "MATERIAL");
	MakeArray(ptr2, cnt);
	cnt=Word(address+0x14);
	ptr2=Dword(address+0xC);
	MakeUnknown(ptr2, GetStrucSize(GetStrucIdByName("MESH")) * cnt, 2);
	MakeStructEx(ptr2, -1, "MESH");
	if(Dword(ptr2+0x8) != 0) Message("Non-0 PAttr at MESH_" + ltoa(ptr2,16) + "!\n");
	if(Dword(ptr2+0xC) != 0) Message("Non-0 PNormal at MESH_" + ltoa(ptr2,16) + "!\n");
	MakeArray(ptr2, cnt);
	for(i=0;i<cnt;i=i+1)
	{
		uvcnt=0;
		polytype=(Word(ptr2)>>0xC)&0xC;
		polycnt=Word(ptr2+2);
		ptr3=Dword(ptr2+4);
		if(polytype==0)
		{
			MakeUnknown(ptr3, polycnt*6, 2);
			MakeData(ptr3, FF_WORD, 2, 0);
			MakeArray(ptr3, polycnt*3);
			uvcnt=polycnt*3;
		}
		else if(polytype==4)
		{
			MakeUnknown(ptr3, polycnt*8, 2);
			MakeData(ptr3, FF_WORD, 2, 0);
			MakeArray(ptr3, polycnt*4);
			uvcnt=polycnt*4;
		}
		else
		{
			for(j=0;j<polycnt;j=j+1)
			{
				MakeUnknown(ptr3, 2, 2);
				MakeData(ptr3, FF_BYTE, 1, 0);
				MakeData(ptr3+1, FF_BYTE, 1, 0);
				strcnt=Byte(ptr3);
				MakeComm(ptr3, "Strip " + ltoa(j+1,10));
				ptr3=ptr3+2;
				MakeUnknown(ptr3, strcnt*2, 2);
				MakeData(ptr3, FF_WORD, 2, 0);
				MakeArray(ptr3, strcnt);
				ptr3=ptr3+(strcnt*2);
				uvcnt=uvcnt+strcnt;
			}
		}
		ptr3=Dword(ptr2+0x10);
		if(ptr3 != 0)
		{
			MakeUnknown(ptr3, uvcnt*4, 2);
			MakeDword(ptr3);
			MakeArray(ptr3, uvcnt);
		}
		ptr3=Dword(ptr2+0x14);
		if(ptr3 != 0)
		{
			MakeUnknown(ptr3, uvcnt*4, 2);
			MakeStructEx(ptr3, -1, "UV");
			MakeArray(ptr3, uvcnt);
		}
		ptr2=ptr2+GetStrucSize(GetStrucIdByName("MESH"));
	}
}

static MakeLandTableStructs(address)
{
	auto colcnt;
	auto coladdr;
	auto curcol;
	auto i;
	MakeUnknown(address, GetStrucSize(GetStrucIdByName("LandTable")), 2);
	MakeStructEx(address, -1, "LandTable");
	colcnt=Word(address);
	coladdr=Dword(address+0xC);
	if(coladdr != 0)
	{
		MakeUnknown(coladdr, GetStrucSize(GetStrucIdByName("COL")) * colcnt, 2);
		MakeStructEx(coladdr, -1, "COL");
		MakeArray(coladdr, colcnt);
		curcol = coladdr;
		for(i=0;i<colcnt;i=i+1)
		{
			curcol = coladdr + (i * 0x24);
			MakeModelStructs(Dword(curcol+0x18));
		}
	}
	colcnt=Word(address+2);
	coladdr=Dword(address+0x10);
	if(coladdr != 0)
	{
		MakeUnknown(coladdr, GetStrucSize(GetStrucIdByName("GeoAnimData")) * colcnt, 2);
		MakeStructEx(coladdr, -1, "GeoAnimData");
		MakeArray(coladdr, colcnt);
		curcol = coladdr;
		for(i=0;i<colcnt;i=i+1)
		{
			curcol = coladdr + (i * 0x18);
			MakeModelStructs(Dword(curcol+0xC));
			MakeAnimStructs(Dword(curcol+0x10));
		}
	}
	coladdr = Dword(address+0x18);
	if(coladdr != 0) MakeTexListStructs(coladdr);
	return 1;
}

static LandTablePointers()
{
	auto startaddr;
	auto endaddr;
	auto addr;
	auto ptr;
	startaddr = SelStart();
	endaddr = SelEnd();
	for(addr = startaddr; addr < endaddr; addr = addr + 4)
	{
		ptr = Dword(addr);
		if(ptr != 0) MakeLandTableStructs(ptr);
	}
	return 1;
}

static MakeAnimStructs(address)
{
	auto ptr;
	MakeUnknown(address, GetStrucSize(GetStrucIdByName("AnimHead")), 2);
	MakeStructEx(address, -1, "AnimHead");
	ptr=Dword(address);
	if(ptr != 0) MakeModelStructs(ptr);
	ptr=Dword(address+0x4);
	if(ptr != 0)
	{
		MakeUnknown(ptr, GetStrucSize(GetStrucIdByName("AnimHead2")), 2);
		MakeStructEx(ptr, -1, "AnimHead2");
	}
	return 1;
}

static AnimPointers()
{
	auto startaddr;
	auto endaddr;
	auto addr;
	auto ptr;
	startaddr = SelStart();
	endaddr = SelEnd();
	for(addr = startaddr; addr < endaddr; addr = addr + 4)
	{
		ptr = Dword(addr);
		if(ptr != 0) MakeAnimStructs(ptr);
	}
	return 1;
}

static MakeTexListStructs(address)
{
	auto ptr;
	MakeUnknown(address, GetStrucSize(GetStrucIdByName("TexListHead")), 2);
	MakeStructEx(address, -1, "TexListHead");
	ptr=Dword(address);
	if(ptr != 0)
	{
		MakeUnknown(ptr, GetStrucSize(GetStrucIdByName("TexListEntry")) * Dword(address+4), 2);
		MakeStructEx(ptr, -1, "TexListEntry");
		MakeArray(ptr, Dword(address+4));
	}
	return 1;
}

static TexListPointers()
{
	auto startaddr;
	auto endaddr;
	auto addr;
	auto ptr;
	startaddr = SelStart();
	endaddr = SelEnd();
	for(addr = startaddr; addr < endaddr; addr = addr + 4)
	{
		ptr = Dword(addr);
		if(ptr != 0) MakeTexListStructs(ptr);
	}
	return 1;
}