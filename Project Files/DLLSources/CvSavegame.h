#ifndef SAVEGAME_H
#define SAVEGAME_H

class FDataStreamBase;
enum SavegameVariableTypes;

#include "JustInTimeArray.h"
#include "BoolArray.h"

class CvSavegameReader
{
public:
	CvSavegameReader(FDataStreamBase* pStream);

	~CvSavegameReader();

	void Read(int& variable);
	void Read(short& variable);
	void Read(char& variable);

	void Read(unsigned int& variable);
	void Read(unsigned short& variable);
	void Read(byte& variable);
	
	void Read(CvString& szString);
	void Read(CvWString& szString);
	
	template<class T>
	void Read(JustInTimeArray<T>& jitArray);

	void Read(BoolArray& baArray);

	template<typename T>
	void ReadEnum(T& variable);

	SavegameVariableTypes ReadSwitch();

	// Add all enums used in savegames
	void Read(DirectionTypes        & variable) { Read((int&)variable); }

	// everything linked ot JITarrayTypes
	// TODO comment out all unused
	void Read(ArtStyleTypes         & variable) { ReadEnum(variable); }
	void Read(BonusTypes            & variable) { ReadEnum(variable); }
	void Read(BuildTypes            & variable) { ReadEnum(variable); }
	void Read(BuildingTypes         & variable) { ReadEnum(variable); }
	void Read(BuildingClassTypes    & variable) { ReadEnum(variable); }
	void Read(SpecialBuildingTypes  & variable) { ReadEnum(variable); }
	void Read(CivEffectTypes        & variable) { ReadEnum(variable); }
	void Read(CivicTypes            & variable) { ReadEnum(variable); }
	void Read(CivicOptionTypes      & variable) { ReadEnum(variable); }
	void Read(CivilizationTypes     & variable) { ReadEnum(variable); }
	void Read(ColorTypes            & variable) { ReadEnum(variable); }
	void Read(CultureLevelTypes     & variable) { ReadEnum(variable); }
	void Read(DiplomacyTypes        & variable) { ReadEnum(variable); }
	void Read(EmphasizeTypes        & variable) { ReadEnum(variable); }
	void Read(EraTypes              & variable) { ReadEnum(variable); }
	void Read(EuropeTypes           & variable) { ReadEnum(variable); }
	void Read(EventTypes            & variable) { ReadEnum(variable); }
	void Read(EventTriggerTypes     & variable) { ReadEnum(variable); }
	void Read(FatherTypes           & variable) { ReadEnum(variable); }
	void Read(FatherPointTypes      & variable) { ReadEnum(variable); }
	void Read(FeatureTypes          & variable) { ReadEnum(variable); }
	void Read(GameOptionTypes       & variable) { ReadEnum(variable); }
	void Read(GoodyTypes            & variable) { ReadEnum(variable); }
	void Read(HandicapTypes         & variable) { ReadEnum(variable); }
	void Read(HurryTypes            & variable) { ReadEnum(variable); }
	void Read(ImprovementTypes      & variable) { ReadEnum(variable); }
	void Read(LeaderHeadTypes       & variable) { ReadEnum(variable); }
	void Read(MemoryTypes           & variable) { ReadEnum(variable); }
	void Read(PlayerColorTypes      & variable) { ReadEnum(variable); }
	void Read(PlayerOptionTypes     & variable) { ReadEnum(variable); }
	void Read(ProfessionTypes       & variable) { ReadEnum(variable); }
	void Read(PromotionTypes        & variable) { ReadEnum(variable); }
	void Read(RouteTypes            & variable) { ReadEnum(variable); }
	void Read(TerrainTypes          & variable) { ReadEnum(variable); }
	void Read(TraitTypes            & variable) { ReadEnum(variable); }
	void Read(UnitTypes             & variable) { ReadEnum(variable); }
	void Read(UnitAITypes           & variable) { ReadEnum(variable); }
	void Read(UnitClassTypes        & variable) { ReadEnum(variable); }
	void Read(UnitCombatTypes       & variable) { ReadEnum(variable); }
	void Read(SpecialUnitTypes      & variable) { ReadEnum(variable); }
	void Read(VictoryTypes          & variable) { ReadEnum(variable); }
	void Read(YieldTypes            & variable) { ReadEnum(variable); }

	void ReadConversionTable();

	int ConvertIndex(JITarrayTypes eType, int iIndex) const;

	// call when the reader is done.
	// throws an error if read byte count differs from saved byte count.
	void VerifyReadComplete() const;

#ifndef MakefileCompilation
	// remove IntelliSense errors, which causes bogus red lines in the code
	// This isn't compiled and won't effect the game at runtime

	void Read(wchar& variable); // the makefile will use short instead, but IntelliSense fail to detect this
#endif

private:

	void Read(byte* var, unsigned int iSize);

	FDataStreamBase* m_pStream;
	byte* m_Memory;
	byte* m_MemoryAllocation;
	unsigned int m_iSize;
};

class CvSavegameWriter
{
public:
	CvSavegameWriter(FDataStreamBase* pStream);

	template<enum T>
	void Write(T variable);

	void Write(int iVar);
	void Write(short iVar);
	void Write(char iVar);
	void Write(unsigned int iVar);
	void Write(unsigned short iVar);
	void Write(byte iVar);

	void Write(CvString& szString);
	void Write(CvWString& szString);

	void Write(BoolArray& baArray);
	
	template<class T>
	void Write(JustInTimeArray<T>& jitArray);

	void Write(SavegameVariableTypes eType);
	void Write(SavegameVariableTypes eType, CvString& szString);
	void Write(SavegameVariableTypes eType, CvWString& szString);
	void Write(SavegameVariableTypes eType, BoolArray& baArray);

	template<class T>
	void Write(SavegameVariableTypes eType, T eVariable, T eDefault);

	template<class T>
	void Write(SavegameVariableTypes eType, JustInTimeArray<T>& jitArray);
	
	void WriteFile();

	// enum conversion
	void GenerateTranslationTable();
	void WriteTranslationTable();

private:

	void Write(byte*var, unsigned int iSize);

	FDataStreamBase* m_pStream;
	std::vector<byte> m_vector;
};

//
// Inline functions to allow easy handling of templates
//

//
// Reader
//

template<typename T>
void CvSavegameReader::ReadEnum(T& variable)
{
	Read((byte*)&variable, sizeof(T));
	variable = (T)ConvertIndex(getJITarrayType(variable), variable);
}

template<class T>
inline void CvSavegameReader::Read(JustInTimeArray<T>& jitArray)
{
	jitArray.Read(this);
}



//
// Writer
//

template<enum T>
inline void CvSavegameWriter::Write(T variable)
{
	Write((byte*)&variable, sizeof(T));
}

template<class T>
inline void CvSavegameWriter::Write(JustInTimeArray<T>& jitArray)
{
	jitArray.Write(this);
}

template<class T>
inline void CvSavegameWriter::Write(SavegameVariableTypes eType, T eVariable, T eDefault)
{
	if (eVariable != eDefault)
	{
		Write(eType);
		Write(eVariable);
	}
}

template<class T>
inline void Write(SavegameVariableTypes eType, JustInTimeArray<T>& jitArray)
{
	if (jitArray->hasContent())
	{
		Write(eType);
		Write(jitArray);
	}
}


#endif