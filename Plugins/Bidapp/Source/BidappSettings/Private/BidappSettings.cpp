/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#include "BidappSettings.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
#include "BidappSettings.h"
#include "Engine.h"
#include "Misc/Paths.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/EngineBuildSettings.h"


#define LOCTEXT_NAMESPACE "FBidappSettings"

void FBidappSettings::StartupModule()
{    
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

    if (SettingsModule != nullptr)
    {        
        SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Bidapp"), TEXT("Bidapp"),
            LOCTEXT("BidappSettingsName", "Bidapp Settings"),
            LOCTEXT("BidappSettingsDescription", "Configure the Bidapp SDK."),
            GetMutableDefault<UBidappSettings>()
        );
    }
    SetupAARImports();
}

void FBidappSettings::ShutdownModule()
{
     UE_LOG(LogTemp, Warning, TEXT("ShutdownModule() called"));
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Bidapp", "Bidapp");
    }
}

void UBidappSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    SaveConfig(CPF_Config, *GetDefaultConfigFilename());
}
void FBidappSettings::SetupAARImports()
{
    FString PluginBaseDir;
    FString MarketplacePluginDir = FPaths::ConvertRelativePathToFull(FPaths::EnginePluginsDir()) / TEXT("Marketplace") / TEXT("Bidapp") / TEXT("Source") / TEXT("Bidapp");
    if (FPaths::DirectoryExists(MarketplacePluginDir))
    {
        PluginBaseDir = MarketplacePluginDir;
    }
    else
    {
        FString ProjectPluginDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()) / TEXT("Bidapp") / TEXT("Source") / TEXT("Bidapp");
        if (FPaths::DirectoryExists(ProjectPluginDir))
        {
            PluginBaseDir = ProjectPluginDir;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Plugin directory not found"));
            return;
        }
    }

    FString FileName = TEXT("Bidapp_UPL_Android.xml");
    FString FilePath = FPaths::Combine(PluginBaseDir, FileName);
    UBidappSettings* BidappSettings = GetMutableDefault<UBidappSettings>();
    FString FileContent;

    if (!BidappSettings)
    {
        UE_LOG(LogTemp, Error, TEXT("BidappSettings is null"));
        return;
    }

    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("File does not exist: %s"), *FilePath);
        return;
    }

    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load XML file: %s"), *FilePath);
        return;
    }

    // Process AdmobAppId section
    FString AdmobAppIdValue = BidappSettings->AdmobAppId;
    FRegexPattern PatternAppId(TEXT("<!-- start meta data -->[\\s\\S]*?<!-- stop meta data -->"));
    FRegexMatcher MatcherAppId(PatternAppId, FileContent);

    if (MatcherAppId.FindNext())
    {
        FString MatchedTextAppId = MatcherAppId.GetCaptureGroup(0);
        FString AppIdString;

        if (!AdmobAppIdValue.IsEmpty() && BidappSettings->bAdmob)
        {
            AppIdString = TEXT("<!-- start meta data -->\n")
                "<addAttribute tag=\"$Bidapp\" name=\"android:name\" value=\"com.google.android.gms.ads.APPLICATION_ID\"/>\n"
                "<addAttribute tag=\"$Bidapp\" name=\"android:value\" value=\"" + AdmobAppIdValue + "\"/>\n"
                "  <!-- stop meta data -->";
        }
        else
        {
            AppIdString = TEXT("<!-- start meta data -->\n")
                "<addAttribute tag=\"$Bidapp\" name=\"android:name\" value=\"com.google.android.gms.ads.APPLICATION_ID\"/>\n"
                "<addAttribute tag=\"$Bidapp\" name=\"android:value\" value=\"ca-app-pub-3940256099942544~3347511713\"/>\n"
                "  <!-- stop meta data -->";
        }

        FileContent = FileContent.Replace(*MatchedTextAppId, *AppIdString);
        UE_LOG(LogTemp, Warning, TEXT("Meta-data updated: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No meta-data section found in file: %s"), *FilePath);
    }

    // Process dynamic imports section
    FRegexPattern PatternImpl(TEXT("<!-- Start Dynamic imports -->[\\s\\S]*?<!-- Stop Dynamic imports -->"));
    FRegexMatcher MatcherImpl(PatternImpl, FileContent);

    if (MatcherImpl.FindNext())
    {
        FString MatchedTextImpl = MatcherImpl.GetCaptureGroup(0);
        FString ImplString = TEXT("<!-- Start Dynamic imports -->\n");
        if (BidappSettings->bLiftoff)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:liftoff:2.1.0')\n");
        }
        if (BidappSettings->bApplovin)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:applovin:2.1.0')\n");
        }
        if (BidappSettings->bApplovinMax)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:applovinmax:2.1.0')\n");
        }
        if (BidappSettings->bUnity)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:unity:2.1.0')\n");
        }
        if (BidappSettings->bChartboost)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:chartboost:2.1.0')\n");
        }
        if (BidappSettings->bDigitalTurbine)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:digitalturbine:2.1.0')\n");
        }
        if (BidappSettings->bFacebook)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:facebook:2.1.0')\n");
        }
        if (BidappSettings->bYandex)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:yandex:2.1.0')\n");
        }
        if (BidappSettings->bAdmob)
        {
            ImplString += TEXT("        implementation('io.bidapp.networks:admob:2.1.0')\n"
                "       {\n"
                "           exclude group: 'com.google.android.gms', module: 'play-services-ads'\n"
                "       }\n"
                "        implementation('com.google.android.gms:play-services-ads:23.0.0')\n");
        }

        ImplString += TEXT("        <!-- Stop Dynamic imports -->");
        FileContent = FileContent.Replace(*MatchedTextImpl, *ImplString);
        UE_LOG(LogTemp, Warning, TEXT("Dynamic imports updated: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No dynamic imports section found in file: %s"), *FilePath);
    }

    // Save the updated file content
    if (!FFileHelper::SaveStringToFile(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save updated XML file: %s"), *FilePath);
    }
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBidappSettings, BidappSettings)