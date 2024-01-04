// Copyright Epic Games, Inc. All Rights Reserved.

#include "PSXEditor.h"

#include "UnrealEdGlobals.h"
#include "VertexSpotlightComponent.h"
#include "VertexSpotLightVisualizer.h"
#include "Editor/UnrealEdEngine.h"

DEFINE_LOG_CATEGORY(PSXLibraryLog)

#define LOCTEXT_NAMESPACE "FPSXEditorModule"

void FPSXEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	UE_LOG(PSXLibraryLog, Warning, TEXT("PSXEditor: Module Started"));

	TSharedPtr<FVertexSpotLightVisualizer> SpotlightVisualizer = MakeShareable(new FVertexSpotLightVisualizer());

	GUnrealEd->RegisterComponentVisualizer(UVertexSpotlightComponent::StaticClass()->GetFName(), SpotlightVisualizer);
}

void FPSXEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    UE_LOG(PSXLibraryLog, Warning, TEXT("PSXEditor: Module Shutdown"));
	if (GUnrealEd != nullptr)
	{
		GUnrealEd->UnregisterComponentVisualizer(UVertexSpotlightComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPSXEditorModule, PSXEditor)