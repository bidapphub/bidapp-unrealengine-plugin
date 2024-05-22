/*
* Bidapp Ads - Plugin for Unreal Engine 
*
* Copyright (C) 2024 BIDAPP <support@bidapp.io> All Rights Reserved.
*/


#include "BidappDelegate.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Engine/World.h"
#include "UObject/UObjectIterator.h"
#include "UObject/WeakObjectPtrTemplates.h"

bool IsValidDelegate(UBidappDelegate *Delegate)
{
     if (!IsValid(Delegate)) return false;

    UWorld *World = Delegate->GetWorld();
    return IsValid(World) && (World->WorldType == EWorldType::Game || World->WorldType == EWorldType::PIE);
}

void UBidappDelegate::BroadcastSdkInitializedEvent(const FSdkConfiguration &SdkConfiguration)
{
    AsyncTask(ENamedThreads::GameThread, [=]()
    {
        for (TObjectIterator<UBidappDelegate> Itr; Itr; ++Itr)
        {
            if (IsValidDelegate(*Itr))
            {
                Itr->SdkInitializedDynamicDelegate.Broadcast(SdkConfiguration);
            }
        }
    });
}


void UBidappDelegate::BroadcastAdEvent(const FString &Name, const FAdInfo &AdInfo)
{
    AsyncTask(ENamedThreads::GameThread, [=]()
    {
        for (TObjectIterator<UBidappDelegate> Itr; Itr; ++Itr)
        {
            if (IsValidDelegate(*Itr))
            {
                if (Name == TEXT("BannerAdViewDidLoadAdEvent"))
                {
                    Itr->BannerAdViewDidLoadAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("BannerAdViewClickedEvent"))
                {
                    Itr->BannerAdViewClickedDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("BannerAdViewDidDisplayAdEvent"))
                {
                    Itr->BannerAdViewDidDisplayAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("MrecAdViewDidLoadAdEvent"))
                {
                    Itr->MrecAdViewDidLoadAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("MrecAdViewClickedEvent"))
                {
                    Itr->MrecAdViewDidLoadAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("MrecAdViewDidDisplayAdEvent"))
                {
                    Itr->MrecAdViewDidDisplayAdDynamicDelegate.Broadcast(AdInfo);
                }
                //Leaderboard
                else if (Name == TEXT("LeaderboardAdViewDidLoadAdEvent"))
                {
                    Itr->LeaderboardAdViewDidLoadAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("LeaderboardAdViewClickedEvent"))
                {
                    Itr->LeaderboardAdViewDidLoadAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("LeaderboardAdViewDidDisplayAdEvent"))
                {
                    Itr->LeaderboardAdViewDidDisplayAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("InterstitialDidLoadEvent"))
                {
                    Itr->InterstitialDidLoadDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("InterstitialDidDisplayAdEvent"))
                {
                    Itr->InterstitialDidDisplayAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("InterstitialDidHideAdEvent"))
                {
                    Itr->InterstitialDidHideAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("InterstitialDidClickAdEvent"))
                {
                    Itr->InterstitialDidClickAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("RewardedDidLoadEvent"))
                {
                    Itr->RewardedDidLoadDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("RewardedDidDisplayAdEvent"))
                {
                    Itr->RewardedDidDisplayAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("RewardedDidHideAdEvent"))
                {
                    Itr->RewardedDidHideAdDynamicDelegate.Broadcast(AdInfo);
                }
                else if (Name == TEXT("RewardedDidClickAdEvent"))
                {
                    Itr->RewardedDidClickAdDynamicDelegate.Broadcast(AdInfo);
                }
            }
        }
    });
}

void UBidappDelegate::BroadcastAdErrorEvent(const FString &Name, const FAdInfo &AdInfo, const FAdError &AdError)
{
    AsyncTask(ENamedThreads::GameThread, [=]()
    {
        for (TObjectIterator<UBidappDelegate> Itr; Itr; ++Itr)
        {
            if (IsValidDelegate(*Itr))
            {
                if (Name == TEXT("BannerAdViewDidFailToDisplayAdEvent"))
                {
                    Itr->BannerAdViewDidFailToDisplayAdDynamicDelegate.Broadcast(AdInfo, AdError);
                }
                else if (Name == TEXT("MrecAdViewDidFailToDisplayEvent"))
                {
                    Itr->MrecAdViewDidFailToDisplayAdDynamicDelegate.Broadcast(AdInfo, AdError);
                }
                //Leaderboard
                else if (Name == TEXT("LeaderboardAdViewDidFailToDisplayEvent"))
                {
                    Itr->LeaderboardAdViewDidFailToDisplayAdDynamicDelegate.Broadcast(AdInfo, AdError);
                }
                else if (Name == TEXT("InterstitialDidFailToLoadEvent"))
                {
                    Itr->InterstitialDidFailToLoadDynamicDelegate.Broadcast(AdInfo, AdError);
                }
                else if (Name == TEXT("InterstitialDidFailToDisplayAdEvent"))
                {
                    Itr->InterstitialDidFailToDisplayAdDynamicDelegate.Broadcast(AdInfo, AdError);
                }
                else if (Name == TEXT("RewardedDidFailToLoadEvent"))
                {
                    Itr->RewardedDidFailToLoadDynamicDelegate.Broadcast(AdInfo, AdError);
                }
                else if (Name == TEXT("RewardedDidFailToDisplayAdEvent"))
                {
                    Itr->RewardedDidFailToDisplayAdDynamicDelegate.Broadcast(AdInfo, AdError);
                }
            }
        }
    });
}


void UBidappDelegate::BroadcastAdErrorAllNetworkEvent(const FString &Name, const FAdError &AdError)
{
    AsyncTask(ENamedThreads::GameThread, [=]()
    {
        for (TObjectIterator<UBidappDelegate> Itr; Itr; ++Itr)
        {
            if (IsValidDelegate(*Itr))
            {
                if (Name == TEXT("BannerAllNetworksDidFailToDisplayAdEvent"))
                {
                    Itr->BannerAllNetworksDidFailToDisplayAdDynamicDelegate.Broadcast(AdError);
                }
                else if (Name == TEXT("MrecAllNetworksDidFailToDisplayAdEvent"))
                {
                    Itr->MrecAllNetworksDidFailToDisplayAdDynamicDelegate.Broadcast(AdError);
                }
                //Leaderboard
                else if (Name == TEXT("LeaderboardAllNetworksDidFailToDisplayAdEvent"))
                {
                    Itr->LeaderboardAllNetworksDidFailToDisplayAdDynamicDelegate.Broadcast(AdError);
                }
                else if (Name == TEXT("InterstitialAllNetworksDidFailToDisplayAdEvent"))
                {
                    Itr->InterstitialAllNetworksDidFailToDisplayAdDynamicDelegate.Broadcast(AdError);
                }
                else if (Name == TEXT("RewardedAllNetworksDidFailToDisplayAdEvent"))
                {
                    Itr->RewardedAllNetworksDidFailToDisplayAdDynamicDelegate.Broadcast(AdError);
                }
            }
        }
    });
}


void UBidappDelegate::BroadcastRewardedDidRewardUserEvent()
{
    AsyncTask(ENamedThreads::GameThread, [=]()
    {
        for (TObjectIterator<UBidappDelegate> Itr; Itr; ++Itr)
        {
            Itr->RewardedDidRewardUserDynamicDelegate.Broadcast();
        }
    });
}

