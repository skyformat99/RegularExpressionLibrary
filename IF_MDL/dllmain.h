// dllmain.h : モジュール クラスの宣言

class CIF_MDLModule : public ATL::CAtlDllModuleT< CIF_MDLModule >
{
public :
	DECLARE_LIBID(LIBID_IF_MDLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_IF_MDL, "{0130ff5d-d217-43e6-a98a-9ea83dc8c225}"/*"{FBE814A3-30A6-4601-93E1-BFB93E2037F8}"*/)
};

extern class CIF_MDLModule _AtlModule;

