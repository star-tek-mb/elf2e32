// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// Copyright (c) 2017-2018 Strizhniou Fiodar
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors: Strizhniou Fiodar - fix build and runtime errors.
//
// Description:
// Implementation of the Class ExexpTarget for the elf2e32 tool
// @internalComponent
// @released
//
//

#include "export_type_rebuild_target.h"
#include "deffile.h"

/**
Constructor for the ExportTypeRebuildTarget Class

@internalComponent
@released
*/
ExportTypeRebuildTarget::ExportTypeRebuildTarget(ParameterManager* aParams) : DLLTarget(aParams) {
}

/**
Destructor for the ExportTypeRebuildTarget Class

@internalComponent
@released
*/
ExportTypeRebuildTarget::~ExportTypeRebuildTarget()
{
}

/**
Function to process the symbols to be exported. The symbols from the DEF File are read
and passed to ValidateDefExports() to validate against the symbols present in the ELF file.
The final list of exports symbols is created.

@internalComponent
@released
*/
void ExportTypeRebuildTarget::ProcessExports()
{
    DefFile *def = new DefFile();
  	iDefExports = def->ReadDefFile( iParameterManager->DefInput() );
	delete def;

	ValidateDefExports(*iDefExports);
	CreateExports();
}
