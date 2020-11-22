#include<iostream>
#include<elf.h>
#include"elf64.h"

void asm_dumper(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh){
	// まずは.text セクションまでジャンプ
	bd->setDC(sh->Sh_offset(sh->Section_hash(".text")));

	return;
}
