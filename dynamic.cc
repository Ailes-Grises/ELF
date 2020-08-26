#include<iostream>
#include<string>
#include<string.h>
#include<regex>
#include<elf.h>
#include"elf64.h"

Dynamic::Dynamic(void){
}

Dynamic::Dynamic(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh){
	this->dynamic_seq = sh->Section_hash(".dynamic"); // .dynamic セクションのシーケンス番号
	this->dynamic_num = sh->Sh_size(dynamic_seq)/sh->Sh_entsize(dynamic_seq);
	Dynamic::dynamic = new Elf64_Dyn[Dynamic::dynamic_num];
	for(int i = 0; i<this->dynamic_num; i++) memset(&dynamic[i], 0, sizeof(dynamic[i]));
	this->dyn_parser(bd, sh);
}

Dynamic::~Dynamic(void){
	delete [] dynamic;
}

int64_t Dynamic::D_tag(unsigned int seq){
	return dynamic[seq].d_tag;
}

uint64_t Dynamic::D_val(unsigned int seq){
	return dynamic[seq].d_un.d_val;
}

uint64_t Dynamic::D_ptr(unsigned int seq){
	return dynamic[seq].d_un.d_ptr;
}

void Dynamic::dyn_parser(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh){
	// まずは該当アドレスまでジャンプ
	bd->setDC(sh->Sh_offset(dynamic_seq));

	// データの抽出
	for(int i = 0; i<dynamic_num; i++){
		// d_tag メンバの内容によってunion の解釈が決まる
		switch(dynamic[i].d_tag = bd->get64bit()){
			case DT_NULL:
				this->dynamic_num = i + 1; break; // DT_NULL が来たら終わり

			// d_tag の内容がアドレスの時は以下の通り: 
			case DT_NEEDED:
			case DT_PLTGOT:
			case DT_HASH:
			case DT_STRTAB:
			case DT_SYMTAB:
			case DT_RELA:
			case DT_INIT:
			case DT_FINI:
			case DT_SONAME:
			case DT_RPATH:
			case DT_REL:
			case DT_JMPREL:
			case DT_RUNPATH:
				dynamic[i].d_un.d_ptr = bd->get64bit(); break;

			// それ以外は全て値として認識する
			default:
				dynamic[i].d_un.d_val = bd->get64bit(); break;
		}
	}
}

void Dynamic::show_dynamic(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh){

	cout << "Dynamic section at offset 0x" << hexformat(4) << sh->Sh_offset(dynamic_seq) << " contains " << std::dec << this->dynamic_num << " entries:" << endl;

	cout << "       Tag           Type             Name/Value" << endl;

	for(int i = 0; i<dynamic_num; i++){

		/* d_tag */
		cout << "0x" << hexformat(16) << dynamic[i].d_tag << "  ";

		/* Type */
		switch(dynamic[i].d_tag){
			case DT_NULL:
				cout << setw_left(18) << "(NULL)"; break;
			case DT_NEEDED:
				cout << setw_left(18) << "(NEEDED)"; break;
			case DT_PLTRELSZ:
				cout << setw_left(18) << "(PLTRELSZ)"; break;
			case DT_PLTGOT:
				cout << setw_left(18) << "(PLTGOT)"; break;
			case DT_HASH:
				cout << setw_left(18) << "(HASH)"; break;
			case DT_STRTAB:
				cout << setw_left(18) << "(STRTAB)"; break;
			case DT_SYMTAB:
				cout << setw_left(18) << "(SYMTAB)"; break;
			case DT_RELA:
				cout << setw_left(18) << "(RELA)"; break;
			case DT_RELASZ:
				cout << setw_left(18) << "(RELASZ)"; break;
			case DT_RELAENT:
				cout << setw_left(18) << "(RELAENT)"; break;
			case DT_STRSZ:
				cout << setw_left(18) << "(STRSZ)"; break;
			case DT_SYMENT:
				cout << setw_left(18) << "(SYMENT)"; break;
			case DT_INIT:
				cout << setw_left(18) << "(INIT)"; break;
			case DT_FINI:
				cout << setw_left(18) << "(FINI)"; break;
			case DT_SONAME:
				cout << setw_left(18) << "(SONAME)"; break;
			case DT_RPATH:
				cout << setw_left(18) << "(RPATH)"; break;
			case DT_SYMBOLIC:
				cout << setw_left(18) << "(SYMBOLIC)"; break;
			case DT_REL:
				cout << setw_left(18) << "(REL)"; break;
			case DT_RELSZ:
				cout << setw_left(18) << "(RELSZ)"; break;
			case DT_RELENT:
				cout << setw_left(18) << "(RELENT)"; break;
			case DT_PLTREL:
				cout << setw_left(18) << "(PLTREL)"; break;
			case DT_DEBUG:
				cout << setw_left(18) << "(DEBUG)"; break;
			case DT_TEXTREL:
				cout << setw_left(18) << "(TEXTREL)"; break;
			case DT_JMPREL:
				cout << setw_left(18) << "(JMPREL)"; break;
			case DT_BIND_NOW:
				cout << setw_left(18) << "(BIND_NOW)"; break;
			case DT_INIT_ARRAY:
				cout << setw_left(18) << "(INIT_ARRAY)"; break;
			case DT_FINI_ARRAY:
				cout << setw_left(18) << "(FINI_ARRAY)"; break;
			case DT_INIT_ARRAYSZ:
				cout << setw_left(18) << "(INIT_ARRAYSZ)"; break;
			case DT_FINI_ARRAYSZ:
				cout << setw_left(18) << "(FINI_ARRAYSZ)"; break;
			case DT_RUNPATH:
				cout << setw_left(18) << "(RUNPATH)"; break;
			case DT_FLAGS:
				cout << setw_left(18) << "(FLAGS)"; break;
			case DT_ENCODING:
				cout << setw_left(18) << "(ENCODING)"; break;
			/* case DT_PREINIT_ARRAY: // 衝突
				cout << setw_left(18) << "(PREINIT_ARRAY)"; break; */
			case DT_PREINIT_ARRAYSZ:
				cout << setw_left(18) << "(PREINIT_ARRAYSZ)"; break;
			case DT_NUM:
				cout << setw_left(18) << "(NUM)"; break;
			case DT_LOOS:
				cout << setw_left(18) << "(LOOS)"; break;
			case DT_HIOS:
				cout << setw_left(18) << "(HIOS)"; break;
			case DT_LOPROC:
				cout << setw_left(18) << "(LOPROC)"; break;
			case DT_HIPROC:
				cout << setw_left(18) << "(HIPROC)"; break;
			case DT_PROCNUM:
				cout << setw_left(18) << "(PROCNUM)"; break;
			case DT_VALRNGLO:
				cout << setw_left(18) << "(VALRNGLO)"; break;
			case DT_GNU_PRELINKED:
				cout << setw_left(18) << "(GNU_PRELINKED)"; break;
			case DT_GNU_CONFLICTSZ:
				cout << setw_left(18) << "(GNU_CONFLICTSZ)"; break;
			case DT_GNU_LIBLISTSZ:
				cout << setw_left(18) << "(GNU_LIBLISTSZ)"; break;
			case DT_CHECKSUM:
				cout << setw_left(18) << "(CHECKSUM)"; break;
			case DT_PLTPADSZ:
				cout << setw_left(18) << "(PLTPADSZ)"; break;
			case DT_MOVEENT:
				cout << setw_left(18) << "(MOVEENT)"; break;
			case DT_MOVESZ:
				cout << setw_left(18) << "(MOVESZ)"; break;
			case DT_FEATURE_1:
				cout << setw_left(18) << "(FEATURE_1)"; break;
			case DT_POSFLAG_1:
				cout << setw_left(18) << "(POSFLAG_1)"; break;
			case DT_SYMINSZ:
				cout << setw_left(18) << "(SYMINSZ)"; break;
			case DT_SYMINENT:
				cout << setw_left(18) << "(SYMINENT)"; break;
			/* case DT_VALRNGHI:
				cout << setw_left(18) << "(VALRNGHI)"; break; */
			/* case DT_VALNUM:
				cout << setw_left(18) << "(VALNUM)"; break; */
			case DT_ADDRRNGLO:
				cout << setw_left(18) << "(ADDRRNGLO)"; break;
			case DT_GNU_HASH:
				cout << setw_left(18) << "(GNU_HASH)"; break;
			case DT_TLSDESC_PLT:
				cout << setw_left(18) << "(TLSDESC_PLT)"; break;
			case DT_TLSDESC_GOT:
				cout << setw_left(18) << "(TLSDESC_GOT)"; break;
			case DT_GNU_CONFLICT:
				cout << setw_left(18) << "(GNU_CONFLICT)"; break;
			case DT_GNU_LIBLIST:
				cout << setw_left(18) << "(GNU_LIBLIST)"; break;
			case DT_CONFIG:
				cout << setw_left(18) << "(CONFIG)"; break;
			case DT_DEPAUDIT:
				cout << setw_left(18) << "(DEPAUDIT)"; break;
			case DT_AUDIT:
				cout << setw_left(18) << "(AUDIT)"; break;
			case DT_PLTPAD:
				cout << setw_left(18) << "(PLTPAD)"; break;
			case DT_MOVETAB:
				cout << setw_left(18) << "(MOVETAB)"; break;
			case DT_SYMINFO:
				cout << setw_left(18) << "(SYMINFO)"; break;
			/* case DT_ADDRRNGHI:
				cout << setw_left(18) << "(ADDRRNGHI)"; break; */
			/* case DT_ADDRNUM:
				cout << setw_left(18) << "(ADDRNUM)"; break; */
			case DT_VERSYM:
				cout << setw_left(18) << "(VERSYM)"; break;
			case DT_RELACOUNT:
				cout << setw_left(18) << "(RELACOUNT)"; break;
			case DT_RELCOUNT:
				cout << setw_left(18) << "(RELCOUNT)"; break;
			case DT_FLAGS_1:
				cout << setw_left(18) << "(FLAGS_1)"; break;
			case DT_VERDEF:
				cout << setw_left(18) << "(VERDEF)"; break;
			case DT_VERDEFNUM:
				cout << setw_left(18) << "(VERDEFNUM)"; break;
			case DT_VERNEED:
				cout << setw_left(18) << "(VERNEED)"; break;
			case DT_VERNEEDNUM:
				cout << setw_left(18) << "(VERNEEDNUM)"; break;
			/* case DT_VERSIONTAGNUM:
				cout << setw_left(18) << "(VERSIONTAGNUM)"; break; */
			case DT_AUXILIARY:
				cout << setw_left(18) << "(AUXILIARY)"; break;
			/* case DT_FILTER:
				cout << setw_left(18) << "(FILTER)"; break; */
			/* case DT_EXTRANUM:
				cout << setw_left(18) << "(EXTRANUM)"; break; */
			default:
				cout << setw_left(18) << "(UNKNOWN)"; break;
		}

		/* Name/Value */
		// 本当はd_tag の内容によっては文字列などを表示するべきだが，面倒なのでそのまま値を表示する(全て16進表記)
		switch(dynamic[i].d_tag){
			case DT_NEEDED:
			case DT_PLTGOT:
			case DT_HASH:
			case DT_STRTAB:
			case DT_SYMTAB:
			case DT_RELA:
			case DT_INIT:
			case DT_FINI:
			case DT_SONAME:
			case DT_RPATH:
			case DT_REL:
			case DT_JMPREL:
			case DT_RUNPATH:
				cout << "0x" << std::right << hexformat(8) << dynamic[i].d_un.d_ptr; break;
			default:
				cout << "0x" << std::right << hexformat(8) << dynamic[i].d_un.d_val; break;
		}

		cout << endl;
	}
}
