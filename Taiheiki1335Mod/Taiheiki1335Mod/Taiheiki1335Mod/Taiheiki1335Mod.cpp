
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>


#include "TBushouData.h"
 

 
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved ) {
	return TRUE;
}



namespace CommonGlobalHandle {
	HANDLE curProcess = NULL;

	BOOL isCallByTSMod = FALSE;
}

�������^ *const p�������	= (�������^ *)��񃁃����A�h���X::�������;


//--------------------------------------------------------
void WINAPI On_������(HANDLE �Ăяo�����n���h��) {

    // �p�X���󂯎�邽�߂̃o�b�t�@
    char szFullDLLName[MAX_PATH] = "";
 
    // DLL�n���h��(���ۂɂ̓�������̃A�h���X)�˂���̃t���l�[���𓾂�B
    GetModuleFileNameA((HMODULE)�Ăяo�����n���h��, szFullDLLName, sizeof(szFullDLLName)-1);
	// ������
	_strlwr(szFullDLLName);

	// TSMod.dll�ɂ���ČĂ΂�Ă���悤���B
	if ( strstr( szFullDLLName, "tsmod.dll" ) ) {
		CommonGlobalHandle::isCallByTSMod = TRUE;
	} else {
		CommonGlobalHandle::isCallByTSMod = FALSE;
	}


	// �v���Z�X�ۑ�
	CommonGlobalHandle::curProcess = ::GetCurrentProcess();

	// �V�ċL���N�̕ύX ------------------------------------------------------------------------
	WORD BeginYear = 1254;

	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x466A8A), &BeginYear, 2, NULL);
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x477D67), &BeginYear, 2, NULL);
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x477E74), &BeginYear, 2, NULL);

	// �V�i���I�� ------------------------------------------------------------------------
	int pt[] = { 0x4d36E4, 0x4d36C4, 0x4d36A4, 0x4d3684, 0x4d3664, 0x4d3644 }; // �V�i���I�^�C�g�������݂���A�h���X
		
	char *�V�^�C�g��[] = {
		" ��k���̓����@�i�P�R�R�T�N�j",
		" -�Ȃ�-",
		" -�Ȃ�-",
		" -�Ȃ�-",
		" -�Ȃ�-",
		" -�Ȃ�-",
	};

	for (int i=0; i<sizeof(pt)/sizeof(pt[0]); i++) {
		if (�V�^�C�g��[i]) {
			strncpy((char *)pt[i], �V�^�C�g��[i], 31);
		}
	}
}

void WINAPI On_�g�b�v���j���[��() {
}



char** WINAPI On_�V�i���I�����\�����O(int �V�i���I�ԍ�) {

	switch(�V�i���I�ԍ�) {

		case 1: 
			{
				static char *sn[] = {
					"�@���q���㖖���B���{�S�y���������񂾑傫�Ȑ헐��\x0A"
					"�N�������B��͊��N�������A���l���̖���D�����B\x0A"
					"�@��̒��ŕ��m�����͌Ȃ̎v���܂ܒ��`�ɐ����A���E\x0A"
					"��y���A�q�d�̌����s�������B\x0A"
					"�@���{�͖k�����̐ꐧ�����ł������B�d�G�����s���A\x0A"
					,

					"���s�E���������{�ɑ΂����m�̓{�肪���������B\x0A"
					"�@���ƈ�}�̎���ڎw���������d�|���󂯁A����\x0A"
					"�̎u�������m�����������������B�@���̕��m������\x0A"
					"���ŁA�ЂƂ���ٍʂ�������������B\x0A"
					"\x0A"
					,
					"�@���ƍċ����u�����������B�@���������Ƃ̌�������\x0A"
					"�V�c�`��B���{�̈����ɋꂵ�ޔ_���������˂��͓���\x0A"
					"�����A��ؐ����B�@���̎O�l���헐�̂����āA�����\x0A"
					"���������߂邱�ƂɂȂ�B\x0A"
				};
				return sn;
			}

		default: 
			{
				static char *sn[] = {
					"�@�@",
					"�@�@",
					"�@�@",
				};
				return sn;
			}

	}

	return NULL;
}

namespace �|�K�� {
	enum { �r=5, �`=4, �a=3, �b=2, �c=1, �d=0 };
};

void Update_�|�K��() {

	// TSMod����Ă΂�Ă���̂ŁA�|��ݒ肷��B
	if ( CommonGlobalHandle::isCallByTSMod ) {

		// TSMod.dll�̃A�h���X�ʒu
		HMODULE hModuleTSMod=GetModuleHandle("TSMod.dll");

		typedef int (WINAPI *PFNSETBUSHOUEXAPTITUDE)(int �����ԍ��y�z��p�z, int �K�����, int �K���l);
		PFNSETBUSHOUEXAPTITUDE p_SetBushouExAptitude = NULL;

		// ���\�b�h�̃A�h���X
		p_SetBushouExAptitude = (PFNSETBUSHOUEXAPTITUDE)GetProcAddress( hModuleTSMod, "Extern_setBushouExAptitude" );

		if ( p_SetBushouExAptitude ) {
			// ���y�̓��ӓx�͂��̂܂܋|�̓��ӓx�ւƔ��f����B
			for ( int iBushouID = 0; iBushouID < 532; iBushouID++ ) {

				// �|�K���͑��y�K�������P�Ⴂ�l���R�s�[����B
				int �|�K���l = p�������[iBushouID].���y�K��;
				if ( �|�K���l > 0 ) {
					�|�K���l--;
				}

				// �͈̓`�F�b�N
				if ( �|�K��::�d <= �|�K���l && �|�K���l <= �|�K��::�r ) { 

					p_SetBushouExAptitude( iBushouID, 5, �|�K���l); // 5�͋|(TSMod��)
				}
			}
		}
	}
}

void WINAPI On_�V�i���I�喼�I�𒼑O() {
	Update_�|�K��();
}
 
void WINAPI On_���C���Q�[���J�n() {
}
 
void WINAPI On_����ύX���O() {
	Update_�|�K��();
}
 
void WINAPI On_������() {
}
 
