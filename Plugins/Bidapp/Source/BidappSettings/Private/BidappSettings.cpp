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
} 
FString FileName = TEXT("Bidapp_UPL_Android.xml");

FString FilePath = FPaths::Combine(PluginBaseDir, FileName);

FString ProjectName = FApp::GetProjectName();

UBidappSettings* BidappSettings = GetMutableDefault<UBidappSettings>();

FString AdmobAppIdValue;
FString AppLovinAppIdValue;

if (BidappSettings)
{
    AdmobAppIdValue = BidappSettings->AdmobAppId;
}
else
{
return;
}
    FString AARImportsString = FString::Printf(TEXT(
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "\n"
    "<!--Bidapp Ads - Plugin for Unreal Engine\n"
	"	Copyright (C) 2024 BIDAPP <support@bidapp.io>\n"
    "			All Rights Reserved.-->\n"
    "\n"
    "<root xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
    " xmlns:tools=\"http://schemas.android.com/tools\">\n"
    "  <!-- init section is always evaluated once per architecture -->\n"
    "  <init>\n"
    "    <log text=\"Bidapp Android SDK init\"/>\n"
    "\n"
    "    <!-- check if using Gradle -->\n"
    "    <setBoolFromProperty result=\"bGradle\" ini=\"Engine\" section=\"/Script/AndroidRuntimeSettings.AndroidRuntimeSettings\" property=\"bEnableGradle\" default=\"false\"/>\n"
    "    <if condition=\"bGradle\">\n"
    "      <false>\n"
    "        <log text=\"Bidapp Android SDK requires Gradle\"/>\n"
    "      </false>\n"
    "    </if>\n"
    "\n"
    "    <!-- get packaging for Applovin from ini and reset it if architecture not supported -->\n"
    "    <setBoolFromProperty result=\"bEnabled\" ini=\"Engine\" section=\"Bidapp\" property=\"bEnabled\" default=\"true\"/>\n"
    "    <setStringFromProperty result=\"ClientId\" ini=\"Engine\" section=\"Bidapp\" property=\"ClientId\" default=\"0\"/>\n"
    "\n"
    "    <dumpvars/>\n"
    "  </init>\n"
    "\n"
    "  <!-- optional updates applied to AndroidManifest.xml -->\n"
    "  <androidManifestUpdates>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "   <addPermission android:name=\"android.permission.INTERNET\" />\n"
    "   <addPermission android:name=\"android.permission.ACCESS_NETWORK_STATE\" />\n"
    ));

    if (!AdmobAppIdValue.IsEmpty() && BidappSettings->bAdmob)
    {
        AARImportsString += "   <setElement result=\"AppName\" value=\"meta-data\"/>\n"
	"	<addAttribute tag=\"$AppName\" name=\"android:name\" value=\"com.google.android.gms.ads.APPLICATION_ID\"/>\n"
	"	<addAttribute tag=\"$AppName\" name=\"android:value\" value=\""+ AdmobAppIdValue +"\"/>\n"
    "	<addElement tag=\"application\" name=\"AppName\"/>\n"; 
    }
    else if (AdmobAppIdValue.IsEmpty() && BidappSettings->bAdmob){
    AARImportsString += "   <setElement result=\"AppName\" value=\"meta-data\"/>\n"
	"	<addAttribute tag=\"$AppName\" name=\"android:name\" value=\"com.google.android.gms.ads.APPLICATION_ID\"/>\n"
	"	<addAttribute tag=\"$AppName\" name=\"android:value\" value=\"ca-app-pub-3940256099942544~3347511713\"/>\n"
	"	<addElement tag=\"application\" name=\"AppName\"/>\n"; 
    }
    AARImportsString += "   </true>\n"
    "    </if>\n"
    "  </androidManifestUpdates>\n"
    "\n"
    "  <!-- optional additions to proguard -->\n"
    "  <proguardAdditions>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "        <insert>\n"
    "          -keep public class com.epicgames.unreal.GameActivity$BidappUnrealPluginListener { *; }\n"
    "        </insert>\n"
    "      </true>\n"
    "    </if>\n"
    "  </proguardAdditions>\n"
    "\n"
    "  <!-- optional AAR imports additions -->\n"
    "  <AARImports>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "        <insertValue value=\"io.bidapp,sdk,1.1.0\" />\n"
    "        <insertNewline/>\n"
    "        <insertValue value=\"org.jetbrains.kotlin,kotlin-stdlib,1.9.21\" />\n"
    "        <insertNewline/>\n"
    "        <insertValue value=\"org.jetbrains.kotlinx,kotlinx-coroutines-android,1.7.3\" />\n"
    "        <insertNewline/>\n";
    

if (BidappSettings->bLiftoff)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,liftoff,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}

if (BidappSettings->bApplovin)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,applovin,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}

if (BidappSettings->bApplovinMax)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,applovinmax,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}

if (BidappSettings->bUnity)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,unity,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}


if (BidappSettings->bChartboost)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,chartboost,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}

if (BidappSettings->bDigitalTurbine)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,digitalturbine,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}

if (BidappSettings->bFacebook)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,facebook,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}

if (BidappSettings->bYandex)
{
    AARImportsString += "      <insertValue value=\"io.bidapp.networks,yandex,1.1.0\" />\n";
    AARImportsString += "      <insertNewline/>\n";
}


    AARImportsString += "<insertValue value=\"repository $S(PluginDir)/../ThirdParty/Android/repository\" />\n"
    "        <insertNewline/>\n"
    "        <insertValue value=\"io.bidapp,bidapp-unreal-plugin,release\" />\n"
    "        <insertNewline/>\n"
    "      </true>\n"
    "    </if>\n"
    "  </AARImports>\n"
    "\n"
    "  <!-- optional app build.gradle additions -->\n"
    "  <buildGradleAdditions>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "        <insert>\n"
    "          android {\n"
    "              compileOptions {\n"
    "                  sourceCompatibility = JavaVersion.VERSION_1_8\n"
    "                  targetCompatibility = JavaVersion.VERSION_1_8\n"
    "              }\n"
    "          }\n"
    "\n"
    "          allprojects {\n"
    "            repositories {\n"
    "            maven {\n"
    "            url \"https://cboost.jfrog.io/artifactory/chartboost-ads/\"\n"
    "             }\n"
    "             }\n"
    "          }\n"
    "         \n"
    "          // Prefer newer versions of Play Services, particularly for below UE 5.2\n"
    "          dependencies {\n";

    if (BidappSettings->bAdmob){
    AARImportsString +=  "      implementation('io.bidapp.networks:admob:1.1.0')\n"
    "       {\n"
    "          exclude group: 'com.google.android.gms', module: 'play-services-ads'\n"
    "       }\n"
    "       implementation('com.google.android.gms:play-services-ads:22.3.0')\n"; 
    }

    AARImportsString +=  "      implementation(\"com.google.android.gms:play-services-auth\").version {\n"
    "              strictly(\"[21,23[\")\n"
    "              prefer(\"21.0.0\")\n"
    "            }\n"
    "            implementation(\"com.google.android.gms:play-services-nearby\").version {\n"
    "              strictly(\"[19,21[\")\n"
    "              prefer(\"19.0.0\")\n"
    "            }\n"
    "            implementation(\"com.google.android.gms:play-services-plus\").version {\n"
    "              strictly(\"[17,19[\")\n"
    "              prefer(\"17.0.0\")\n"
    "            }\n"
    "          }\n"
    "        </insert>\n"
    "      </true>\n"
    "    </if>\n"
    "  </buildGradleAdditions>\n"
    "\n"
    "  <!--optional properties to add to gradle.properties -->\n"
    "  <gradleProperties>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "        <insert>\n"
    "          android.useAndroidX=true\n"
    "        </insert>\n"
    "      </true>\n"
    "    </if>\n"
    "  </gradleProperties>\n"
    "\n"
    "  <!--optional additions to the GameActivity imports in GameActivity.java -->\n"
    "  <gameActivityImportAdditions>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "        <insert>\n"
    "          import io.bidapp.unreal.BidappUnrealPlugin;\n"
    "        </insert>\n"
    "      </true>\n"
    "    </if>\n"
    "  </gameActivityImportAdditions>\n"
    "\n"
    "  <!--optional additions to the GameActivity class body in GameActivity.java -->\n"
    "  <gameActivityClassAdditions>\n"
    "    <if condition=\"bEnabled\">\n"
    "      <true>\n"
    "        <insert>\n"
    "        // Begin Bidapp gameActivityClassAdditions\n"
    "        public static class BidappUnrealPluginListener implements BidappUnrealPlugin.EventListener\n"
    "        {\n"
    "          public native void forwardEvent(String name, String params);\n"
    "\n"
    "          public BidappUnrealPluginListener() {}\n"
    "\n"
    "          @Override\n"
    "          public void onReceivedEvent(String name, String params)\n"
    "          {\n"
    "            forwardEvent(name, params);\n"
    "          }\n"
    "        }\n"
    "        // End Bidapp gameActivityClassAdditions\n"
    "        </insert>\n"
    "      </true>\n"
    "    </if>\n"
    "  </gameActivityClassAdditions>\n"
    "</root>";

    AARImportsString.ReplaceInline(TEXT("AppName"), *ProjectName, ESearchCase::CaseSensitive);
    if (!ProjectName.IsEmpty())
    {
         AARImportsString.ReplaceInline(TEXT("AppName"), *ProjectName, ESearchCase::CaseSensitive);
    }
    else
    {
        FString AppName = "Bidapp";
        AARImportsString.ReplaceInline(TEXT("AppName"), *AppName, ESearchCase::CaseSensitive);
    }     


    bool bSuccess = FFileHelper::SaveStringToFile(AARImportsString, *FilePath);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("File Bidapp_UPL_Android.xml create success"));
        UE_LOG(LogTemp, Warning, TEXT("Path to the file: %s"), *FilePath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("File Bidapp_UPL_Android.xml create failure"));
    }
}



#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FBidappSettings, BidappSettings)