;-------------------------------------------------------------------------------
;                           Silicon Laboratories, Inc.
;                            http://www.silabs.com
;                                Copyright 2010
;-------------------------------------------------------------------------------
;
;     FILE:       si4010_rom_all.a51
;     TARGET:     Si4010
;     TOOLCHAIN:  Keil only! <-- for trim 3 and below!
;     DATE:       June 14, 2010, Monday
;     RELEASE:    2.3 (MPech), ROM version 02.00, trim 3
;
;---------------------------------------------------------------------------
;
;     DESCRIPTION:
;       This file is must be compiled and linked into builds which intend
;       to user ROM functions for the ROM release specified in the header
;       above. This file is to be used with other then Keil toolchain.
;       It could be used with Keil toolchain as well, but it is recommended
;       to use Keil dedicated si4010_rom_keil.a51 file instead.
;
;---------------------------------------------------------------------------

PUBLIC  _VAES_CIPHER
PUBLIC  _VAES_INVCIPHER
PUBLIC  _VAES_INVGENKEY
PUBLIC   WBSR_GETCURRENTBUTTON
PUBLIC   BBSR_GETPTSITEMCNT
PUBLIC   BBSR_GETTIMESTAMP
PUBLIC  _VBSR_INITPTS
PUBLIC   WBSR_POP
PUBLIC   VBSR_SERVICE
PUBLIC  _VBSR_SETUP
PUBLIC   VDMDTS_CLEARDMD
PUBLIC   VDMDTS_CLEARDMDINTFLAG
PUBLIC  _VDMDTS_ENABLE
PUBLIC   IDMDTS_GETDATA
PUBLIC   IDMDTS_GETLATESTDMDSAMPLE
PUBLIC   IDMDTS_GETLATESTTEMP
PUBLIC   BDMDTS_GETSAMPLESTAKEN
PUBLIC   VDMDTS_ISRCALL
PUBLIC  _VDMDTS_RESETCOUNTS
PUBLIC  _VDMDTS_RUNFORTEMP
PUBLIC  _VDMDTS_SETUP
PUBLIC  _BENC_4B5BENCODE
PUBLIC  _BENC_MANCHESTERENCODE
PUBLIC  _VENC_SET4B5BLASTBIT
PUBLIC  _VFCAST_FSKADJ
PUBLIC   VFCAST_SETUP
PUBLIC  _VFCAST_FINETUNE
PUBLIC  _VFCAST_TUNE
PUBLIC  _VFCAST_XOSETUP
PUBLIC   LFC_GETCOUNT
PUBLIC   VFC_POLLDONE
PUBLIC  _VFC_SETUP
PUBLIC   VFC_STARTCOUNT
PUBLIC   LFC_STARTPOLLGETCOUNT
PUBLIC  _BHVRAM_READ
PUBLIC  _VHVRAM_WRITE
PUBLIC  _LMTP_GETDECCOUNT
PUBLIC  _VMTP_INCCOUNT
PUBLIC  PBMTP_READ
PUBLIC  _VMTP_SETDECCOUNT
PUBLIC   VMTP_STROBE
PUBLIC  _BMTP_WRITE
PUBLIC  _IMVDD_MEASURE
PUBLIC   BNVM_COPYBLOCK
PUBLIC   WNVM_GETADDR
PUBLIC   VNVM_MCDISABLEREAD
PUBLIC   VNVM_MCENABLEREAD
PUBLIC  _VNVM_SETADDR
PUBLIC  _VODS_ENABLE
PUBLIC  _VODS_SETUP
PUBLIC  _VODS_WRITEDATA
PUBLIC  _VPA_SETUP
PUBLIC  _VPA_TUNE
PUBLIC  _VSLEEPTIM_ADDTXTIMETOCOUNTER
PUBLIC   BSLEEPTIM_CHECKDUTYCYCLE
PUBLIC   LSLEEPTIM_GETONEHOURVALUE
PUBLIC   LSLEEPTIM_GETCOUNT
PUBLIC  _VSLEEPTIM_SETCOUNT
PUBLIC  _BSTL_ENCODEBYTE
PUBLIC  _VSTL_ENCODESETUP
PUBLIC   VSTL_POSTLOOP
PUBLIC   VSTL_PRELOOP
PUBLIC  _VSTL_SINGLETXLOOP
PUBLIC  _VSYS_16BITDECLOOP
PUBLIC  _VSYS_8BITDECLOOP
PUBLIC  _VSYS_BANDGAPLDO
PUBLIC   VSYS_FIRSTPOWERUP
PUBLIC   VSYS_FORCELC
PUBLIC   BSYS_GETBOOTSTATUS
PUBLIC   WSYS_GETCHIPID
PUBLIC   WSYS_GETKEILVER
PUBLIC   LSYS_GETMASTERTIME
PUBLIC   LSYS_GETPRODID
PUBLIC   BSYS_GETREVID
PUBLIC   WSYS_GETROMID
PUBLIC  _VSYS_INCMASTERTIME
PUBLIC  _VSYS_LEDINTENSITY
PUBLIC   VSYS_LPOSCADJ
PUBLIC  _VSYS_SETCLKSYS
PUBLIC  _VSYS_SETMASTERTIME
PUBLIC  _VSYS_SETUP
PUBLIC   VSYS_SHUTDOWN

;
;---------------------------------------------------------------------------
;

_VAES_CIPHER                    CODE    08003H
_VAES_INVCIPHER                 CODE    08006H
_VAES_INVGENKEY                 CODE    08009H
 WBSR_GETCURRENTBUTTON          CODE    0801BH
 BBSR_GETPTSITEMCNT             CODE    08015H
 BBSR_GETTIMESTAMP              CODE    0801EH
_VBSR_INITPTS                   CODE    08012H
 WBSR_POP                       CODE    0800CH
 VBSR_SERVICE                   CODE    08018H
_VBSR_SETUP                     CODE    0800FH
 VDMDTS_CLEARDMD                CODE    0802DH
 VDMDTS_CLEARDMDINTFLAG         CODE    08030H
_VDMDTS_ENABLE                  CODE    08039H
 IDMDTS_GETDATA                 CODE    011EFH
 IDMDTS_GETLATESTDMDSAMPLE      CODE    08027H
 IDMDTS_GETLATESTTEMP           CODE    0802AH
 BDMDTS_GETSAMPLESTAKEN         CODE    08036H
 VDMDTS_ISRCALL                 CODE    011ECH
_VDMDTS_RESETCOUNTS             CODE    0803FH
_VDMDTS_RUNFORTEMP              CODE    0803CH
_VDMDTS_SETUP                   CODE    08021H
_BENC_4B5BENCODE                CODE    08045H
_BENC_MANCHESTERENCODE          CODE    08048H
_VENC_SET4B5BLASTBIT            CODE    08042H
_VFCAST_FSKADJ                  CODE    080FCH
 VFCAST_SETUP                   CODE    080F9H
_VFCAST_FINETUNE                CODE    0809CH
_VFCAST_TUNE                    CODE    080F6H
_VFCAST_XOSETUP                 CODE    080FFH
 LFC_GETCOUNT                   CODE    08057H
 VFC_POLLDONE                   CODE    08051H
_VFC_SETUP                      CODE    0804BH
 VFC_STARTCOUNT                 CODE    0804EH
 LFC_STARTPOLLGETCOUNT          CODE    08054H
_BHVRAM_READ                    CODE    0805AH
_VHVRAM_WRITE                   CODE    0805DH
_LMTP_GETDECCOUNT               CODE    08060H
_VMTP_INCCOUNT                  CODE    08063H
PBMTP_READ                      CODE    0806CH
_VMTP_SETDECCOUNT               CODE    08072H
 VMTP_STROBE                    CODE    0806FH
_BMTP_WRITE                     CODE    08069H
_IMVDD_MEASURE                  CODE    080F0H
 BNVM_COPYBLOCK                 CODE    0807BH
 WNVM_GETADDR                   CODE    08078H
 VNVM_MCDISABLEREAD             CODE    08081H
 VNVM_MCENABLEREAD              CODE    0807EH
_VNVM_SETADDR                   CODE    08075H
_VODS_ENABLE                    CODE    08087H
_VODS_SETUP                     CODE    08084H
_VODS_WRITEDATA                 CODE    0808AH
_VPA_SETUP                      CODE    011E9H
_VPA_TUNE                       CODE    08090H
_VSLEEPTIM_ADDTXTIMETOCOUNTER   CODE    080EDH
 BSLEEPTIM_CHECKDUTYCYCLE       CODE    080EAH
 LSLEEPTIM_GETONEHOURVALUE      CODE    011E0H
 LSLEEPTIM_GETCOUNT             CODE    080E4H
_VSLEEPTIM_SETCOUNT             CODE    080E7H
_BSTL_ENCODEBYTE                CODE    080DEH
_VSTL_ENCODESETUP               CODE    080E1H
 VSTL_POSTLOOP                  CODE    011E3H
 VSTL_PRELOOP                   CODE    080D5H
_VSTL_SINGLETXLOOP              CODE    080DBH
_VSYS_16BITDECLOOP              CODE    080C3H
_VSYS_8BITDECLOOP               CODE    080C6H
_VSYS_BANDGAPLDO                CODE    080A2H
 VSYS_FIRSTPOWERUP              CODE    080C9H
 VSYS_FORCELC                   CODE    08102H
 BSYS_GETBOOTSTATUS             CODE    080CCH
 WSYS_GETCHIPID                 CODE    080A8H
 WSYS_GETKEILVER                CODE    080CFH
 LSYS_GETMASTERTIME             CODE    080B4H
 LSYS_GETPRODID                 CODE    011E6H
 BSYS_GETREVID                  CODE    080ABH
 WSYS_GETROMID                  CODE    080A5H
_VSYS_INCMASTERTIME             CODE    080B7H
_VSYS_LEDINTENSITY              CODE    080BDH
 VSYS_LPOSCADJ                  CODE    080D2H
_VSYS_SETCLKSYS                 CODE    080AEH
_VSYS_SETMASTERTIME             CODE    080BAH
_VSYS_SETUP                     CODE    080B1H
 VSYS_SHUTDOWN                  CODE    080C0H

END

;
;-------------------------------------------------------------------------------
;