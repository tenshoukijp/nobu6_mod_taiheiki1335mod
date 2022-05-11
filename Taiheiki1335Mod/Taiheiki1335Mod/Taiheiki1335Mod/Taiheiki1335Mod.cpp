
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

武将情報型 *const p武将情報	= (武将情報型 *)情報メモリアドレス::武将情報;


//--------------------------------------------------------
void WINAPI On_初期化(HANDLE 呼び出し元ハンドル) {

    // パスを受け取るためのバッファ
    char szFullDLLName[MAX_PATH] = "";
 
    // DLLハンドル(実際にはメモリ上のアドレス)⇒そやつのフルネームを得る。
    GetModuleFileNameA((HMODULE)呼び出し元ハンドル, szFullDLLName, sizeof(szFullDLLName)-1);
	// 小文字
	_strlwr(szFullDLLName);

	// TSMod.dllによって呼ばれているようだ。
	if ( strstr( szFullDLLName, "tsmod.dll" ) ) {
		CommonGlobalHandle::isCallByTSMod = TRUE;
	} else {
		CommonGlobalHandle::isCallByTSMod = FALSE;
	}


	// プロセス保存
	CommonGlobalHandle::curProcess = ::GetCurrentProcess();

	// 天翔記元年の変更 ------------------------------------------------------------------------
	WORD BeginYear = 1254;

	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x466A8A), &BeginYear, 2, NULL);
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x477D67), &BeginYear, 2, NULL);
	WriteProcessMemory( CommonGlobalHandle::curProcess, (LPVOID)(0x477E74), &BeginYear, 2, NULL);

	// シナリオ名 ------------------------------------------------------------------------
	int pt[] = { 0x4d36E4, 0x4d36C4, 0x4d36A4, 0x4d3684, 0x4d3664, 0x4d3644 }; // シナリオタイトルが存在するアドレス
		
	char *新タイトル[] = {
		" 南北朝の動乱　（１３３５年）",
		" -なし-",
		" -なし-",
		" -なし-",
		" -なし-",
		" -なし-",
	};

	for (int i=0; i<sizeof(pt)/sizeof(pt[0]); i++) {
		if (新タイトル[i]) {
			strncpy((char *)pt[i], 新タイトル[i], 31);
		}
	}
}

void WINAPI On_トップメニュー時() {
}



char** WINAPI On_シナリオ説明表示直前(int シナリオ番号) {

	switch(シナリオ番号) {

		case 1: 
			{
				static char *sn[] = {
					"　鎌倉時代末期。日本全土を巻き込んだ大きな戦乱が\x0A"
					"起こった。戦は幾年も続き、幾人もの命を奪った。\x0A"
					"　戦の中で武士たちは己の思うまま忠義に生き、武勇\x0A"
					"を馳せ、智謀の限りを尽くした。\x0A"
					"　幕府は北条氏の専制政治であった。賄賂が横行し、\x0A"
					,

					"腐敗・堕落した幕府に対し武士の怒りが爆発した。\x0A"
					"　公家一党の治を目指す後醍醐帝の綸旨を受け、討幕\x0A"
					"の志を持つ武士たちが兵を挙げた。　その武士たちの\x0A"
					"中で、ひときわ異彩を放つ武将がいた。\x0A"
					"\x0A"
					,
					"　源家再興を志す足利尊氏。　同じく源家の血を引く\x0A"
					"新田義貞。幕府の悪政に苦しむ農民を見かねた河内の\x0A"
					"豪族、楠木正成。　この三人が戦乱のそして、時代の\x0A"
					"趨勢を決めることになる。\x0A"
				};
				return sn;
			}

		default: 
			{
				static char *sn[] = {
					"　　",
					"　　",
					"　　",
				};
				return sn;
			}

	}

	return NULL;
}

namespace 弓適正 {
	enum { Ｓ=5, Ａ=4, Ｂ=3, Ｃ=2, Ｄ=1, Ｅ=0 };
};

void Update_弓適正() {

	// TSModから呼ばれているので、弓を設定する。
	if ( CommonGlobalHandle::isCallByTSMod ) {

		// TSMod.dllのアドレス位置
		HMODULE hModuleTSMod=GetModuleHandle("TSMod.dll");

		typedef int (WINAPI *PFNSETBUSHOUEXAPTITUDE)(int 武将番号【配列用】, int 適正種類, int 適正値);
		PFNSETBUSHOUEXAPTITUDE p_SetBushouExAptitude = NULL;

		// メソッドのアドレス
		p_SetBushouExAptitude = (PFNSETBUSHOUEXAPTITUDE)GetProcAddress( hModuleTSMod, "Extern_setBushouExAptitude" );

		if ( p_SetBushouExAptitude ) {
			// 足軽の得意度はそのまま弓の得意度へと反映する。
			for ( int iBushouID = 0; iBushouID < 532; iBushouID++ ) {

				// 弓適正は足軽適正よりも１つ低い値をコピーする。
				int 弓適正値 = p武将情報[iBushouID].足軽適正;
				if ( 弓適正値 > 0 ) {
					弓適正値--;
				}

				// 範囲チェック
				if ( 弓適正::Ｅ <= 弓適正値 && 弓適正値 <= 弓適正::Ｓ ) { 

					p_SetBushouExAptitude( iBushouID, 5, 弓適正値); // 5は弓(TSMod内)
				}
			}
		}
	}
}

void WINAPI On_シナリオ大名選択直前() {
	Update_弓適正();
}
 
void WINAPI On_メインゲーム開始() {
}
 
void WINAPI On_相場変更直前() {
	Update_弓適正();
}
 
void WINAPI On_末期化() {
}
 
