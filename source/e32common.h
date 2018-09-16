// Copyright (c) 2018 Strizhniou Fiodar
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Strizhniou Fiodar - initial contribution.
//
// Contributors:
//
// Description:
// Store for common description of e32image for the elf2e32 tool
// @internalComponent
// @released
//
//

#ifndef E32COMMON_H_INCLUDED
#define E32COMMON_H_INCLUDED

#define __portable__

#include <cstdint>
#include <cstddef>

struct E32EpocExpSymInfoHdr
{
    int32_t iSize=0;      // size of this Table
    int16_t iFlags=0;
    int16_t iSymCount=0;     // number of symbols
    int32_t iSymbolTblOffset=0;   // start of the symbol table - offset from byte 0 of this header
    int32_t iStringTableSz=0;    // size of the string table
    int32_t iStringTableOffset=0;   // start of the string table having names of the symbols - offset from byte 0 of this header
    int32_t iDllCount=0;     // Number of dependent DLLs
    int32_t iDepDllZeroOrdTableOffset=0; // offset of the DLL dependency table - offset from byte 0 of this header.
};

struct E32RelocSection
{
    int32_t iSize;                 // size of this relocation section
    int32_t iNumberOfRelocs;       // number of relocations in this section
};

/**
Header for the Import Section in an image, as referenced by E32ImageHeader::iImportOffset.
Immediately following this structure are an array of E32ImportBlock structures.
The number of these is given by E32ImageHeader::iDllRefTableCount.
*/
struct E32ImportSection
{
    int32_t iSize;     ///< Size of this section excluding 'this' structure
//  E32ImportBlock iImportBlock[iDllRefTableCount];
};

/**
A block of imports from a single executable.
These structures are conatined in a images Import Section (E32ImportSection).
*/
struct E32ImportBlock
{
        inline const E32ImportBlock* NextBlock(uint32_t aImpFmt) const;
        inline uint32_t Size(uint32_t aImpFmt) const;
        inline const uint32_t* Imports() const;    // import list if present

        uint32_t iOffsetOfDllName;           ///< Offset from start of import section for a NUL terminated executable (DLL or EXE) name.
        int32_t    iNumberOfImports;           ///< Number of imports from this executable.
//  uint32_t   iImport[iNumberOfImports];  ///< For ELF-derived executes: list of code section offsets. For PE, list of imported ordinals. Omitted in PE2 import format
};

/**
Return address of first import in this block.
For import format KImageImpFmt_ELF, imports are list of code section offsets.
For import format KImageImpFmt_PE, imports are a list of imported ordinals.
For import format KImageImpFmt_PE2, the import list is not present and should not be accessed.
*/
inline const uint32_t* E32ImportBlock::Imports() const {
    return (const uint32_t*)(this + 1);
}

enum TProcessPriority
{
    EPriorityLow=150,
    EPriorityBackground=250,
    EPriorityForeground=350,
    EPriorityHigh=450,
    EPriorityWindowServer=650,
    EPriorityFileServer=750,
    EPriorityRealTimeServer=850,
    EPrioritySupervisor=950
};

enum TargetType
{
    EXE = 0,
    DLL = 1,
    FIXEDADDRESEXE = 2
};

/**< Version of PETRAN/ELFTRAN which generated this file */
struct ToolVersion {
    int8_t iMajor; //The major version number.
    int8_t iMinor; //The minor version number.
    int16_t iBuild; //The build number.
};

struct SCapabilitySet
{
    uint32_t iSet[2];
};

struct SSecurityInfo
{
    uint32_t iSecureId;
    uint32_t iVendorId;
    SCapabilitySet iCaps;   // Capabilities for e32image
};

struct E32ImageHeader
{
    uint32_t iUid1;
    uint32_t iUid2;
    uint32_t iUid3;
    uint32_t iUidChecksum;
    char iSignature[4] = {'E', 'P', 'O', 'C'};
    uint32_t iHeaderCrc;         // CRC-32 of entire header
    uint32_t iModuleVersion;     // Version number for this executable (used in link resolution)
    uint32_t iCompressionType;   // Type of compression used (UID or 0 for none)
    ToolVersion iVersion;     // Version of PETRAN/ELFTRAN which generated this file
    uint32_t iTimeLo;
    uint32_t iTimeHi;
    uint32_t iFlags;               // 0 = exe, 1 = dll, 2 = fixed address exe
    int32_t iCodeSize;             // size of code, import address table, constant data and export dir
    int32_t iDataSize;             // size of initialised data
    int32_t iHeapSizeMin;
    int32_t iHeapSizeMax;
    int32_t iStackSize;
    int32_t iBssSize;
    uint32_t iEntryPoint;          // offset into code of entry point
    uint32_t iCodeBase;            // where the code is linked for
    uint32_t iDataBase;            // where the data is linked for
    int32_t iDllRefTableCount;     // filling this in enables E32ROM to leave space for it
    uint32_t iExportDirOffset;     // offset into the file of the export address table
    uint32_t iExportDirCount;
    int32_t iTextSize;             // size of just the text section, also doubles as the offset for the iat w.r.t. the code section
    uint32_t iCodeOffset;          // file offset to code section, also doubles as header size
    uint32_t iDataOffset;          // file offset to data section
    uint32_t iImportOffset;        // file offset to import section
    uint32_t iCodeRelocOffset;     // relocations for code and const
    uint32_t iDataRelocOffset;     // relocations for data
    uint16_t iProcessPriority;   // executables priority
    uint16_t iCpuIdentifier;     // 0x1000 = X86, 0x2000 = ARM;
};

/**< In e32 binary take place after E32Header */
struct E32ImageHeaderJ
{
	uint32_t iUncompressedSize;	///< Uncompressed size of file data after the header, or zero if file not compressed.
};

/**< In e32 binary take place after E32ImageHeaderJ */
struct E32ImageHeaderV
{
    SSecurityInfo iS;
    // Use iSpare1 as offset to Exception Descriptor
    uint32_t iExceptionDescriptor;   // Offset in bytes from start of code section to Exception Descriptor, bit 0 set if valid
    uint32_t iSpare2;
    uint16_t iExportDescSize;    // size of bitmap section
    uint8_t  iExportDescType;    // type of description of holes in export table
    uint8_t  iExportDesc[1];     // description of holes in export table - extend
};

const uint32_t KImageHdrFmtMask    = 0x0f000000u;
const int32_t  KImageHdrFmtShift   = 24;
enum E32HdrFmt
{
    KImageHdrFmt_Original   = 0x00000000u,  // without compression support
    KImageHdrFmt_J          = 0x01000000u,  // with compression support
    KImageHdrFmt_V          = 0x02000000u,  // with versioning support
};

//Hardware floating point types.
enum TFloatingPointType {
    /** No hardware floating point. */
    EFpTypeNone=0,
    /** ARM VFPv2 */
    EFpTypeVFPv2=1,
    /** ARM VFPv3 */
    EFpTypeVFPv3=2
    };

const uint32_t KImageHWFloatMask       = 0x00f00000u;
const int32_t  KImageHWFloatShift      = 20;
const uint32_t KImageHWFloat_None      = EFpTypeNone << KImageHWFloatShift; // No hardware floating point used
const uint32_t KImageHWFloat_VFPv2     = EFpTypeVFPv2 << KImageHWFloatShift; // ARM VFPv2 floating point used

const uint32_t KImageCodeUnpaged   = 0x00000100u; ///< Executable image should not be demand paged. Exclusive with KImageCodePaged,
const uint32_t KImageCodePaged    = 0x00000200u; ///< Executable image should be demand paged. Exclusive with KImageCodeUnpaged
const uint32_t KImageNmdExpData  = 0x00000400u; ///< Flag to indicate when named symbol export data present in image

const uint32_t KImageDataUnpaged  = 0x00001000u; ///< Flag to indicate the image should not be data paged. Exclusive with KImageDataPaged.
const uint32_t KImageDataPaged   = 0x00002000u; ///< Flag to indicate the image should be data paged. Exclusive with KImageDataUnpaged.

const uint32_t KImageDebuggable  = 0x00000800u; ///< Flag to indicate image is debuggable
const uint32_t KImageSMPSafe   = 0x00004000u; ///< Flag to indicate image is SMP safe

enum Cpu
{
    UnknownCpu=0,
    X86Cpu=0x1000,
    ArmV4Cpu=0x2000,
    ArmV5Cpu=0x2001,
    ArmV6Cpu=0x2002,
    MCoreCpu=0x4000
};

const uint32_t KImageDll               = 0x00000001u;
const uint32_t KImageNoCallEntryPoint  = 0x00000002u;
const uint32_t KImageFixedAddressExe   = 0x00000004u;

const uint32_t KFormatNotCompressed=0;
const uint32_t KUidCompressionDeflate=0x101F7AFC;
const uint32_t KUidCompressionBytePair=0x102822AA;

const int32_t KErrNone=0;
const int32_t KErrGeneral=(-2);
const int32_t KErrNoMemory=(-4);
const int32_t KErrNotSupported=(-5);
const int32_t KErrCorrupt=(-20);
const int32_t KErrTooBig=(-40);

// export description type E32ImageHeaderV::iExportDescType
const uint32_t KImageHdr_ExpD_NoHoles          =0x00;  ///< No holes, all exports present.
const uint32_t KImageHdr_ExpD_FullBitmap       =0x01;  ///< Full bitmap present at E32ImageHeaderV::iExportDesc
const uint32_t KImageHdr_ExpD_SparseBitmap8    =0x02;  ///< Sparse bitmap present at E32ImageHeaderV::iExportDesc, granularity 8
const uint32_t KImageHdr_ExpD_Xip              =0xff;  ///< XIP file

// flag values for E32ImageHeader::iFlags
const uint32_t KImageOldJFlag          = 0x00000008u;  // so we can run binaries built with pre 2.00 tools (hdrfmt=0)
const uint32_t KImageOldElfFlag        = 0x00000010u;  // so we can run binaries built with pre 2.00 tools (hdrfmt=0)

const uint32_t KImageImpFmtMask        = 0xf0000000u;
const int32_t  KImageImpFmtShift       = 28;
const uint32_t KImageImpFmt_PE         = 0x00000000u;  // PE-derived imports
const uint32_t KImageImpFmt_ELF        = 0x10000000u;  // ELF-derived imports
const uint32_t KImageImpFmt_PE2        = 0x20000000u;  // PE-derived imports without redundant copy of import ordinals

struct TExceptionDescriptor
{
    uint32_t iExIdxBase;
    uint32_t iExIdxLimit;
    uint32_t iROSegmentBase;
    uint32_t iROSegmentLimit;
};


/**
Return pointer to import block which immediately follows this one.
@param aImpFmt Import format as obtained from image header.
*/
inline const E32ImportBlock* E32ImportBlock::NextBlock(uint32_t aImpFmt) const
{
    const E32ImportBlock* next = this + 1;
    if(aImpFmt!=KImageImpFmt_PE2)
        next = (const E32ImportBlock*)( (uint8_t*)next + iNumberOfImports * sizeof(uint32_t) );
    return next;
}

/**
Return size of this import block.
@param aImpFmt Import format as obtained from image header.
*/
inline uint32_t E32ImportBlock::Size(uint32_t aImpFmt) const
{
	uint32_t r = sizeof(E32ImportBlock);
    if(aImpFmt!=KImageImpFmt_PE2)
        r += iNumberOfImports * sizeof(uint32_t);
    return r;
}

#endif // E32COMMON_H_INCLUDED
