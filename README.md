
<div align="center">
    <img alt="Logo" src="demo/shared/src/commonMain/composeResources/drawable/bidapp_logo.png" width="120"/>
</div>
 <br/><br/>
<div align="center">
    <p>
        <img src="https://img.shields.io/badge/Kotlin-_multiplatform-blue"/>
        <img src="https://img.shields.io/badge/Android-green"/>
        <img src="https://img.shields.io/badge/IOS-red"/>
    </p>
</div>

# Bidapp ads kotlin multiplatform demo

Bidapp ads kotlin multiplatform demo for Android and IOS

# Documentation

Complete integration instructions and usage guide for the Bidapp Ads Kotlin Multiplatform library can be found [here](https://docs.bidapp.io).

# Demo APP
<div align="center">
  <img alt="Logo" src="https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/2db4bcac-159e-475e-b199-1494962ba517" width="350"/>
  <img alt="Logo" src="https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/656a76bc-ceb3-4b06-99b2-7d89d560338f" width="350"/>
</div>
<br/><br/>
The demo directory contains a shared module and test applications for Android and iOS

# Setting up advertising display in the demo

* Go to the [Bidapp dashboard](https://dashboard-564.pages.dev), add a new application, and connect advertising networks.  
For this demo, a test App ID for AdMob is set in the AndroidManifest for Android and in the Info.plist for iOS.

* In the build.gradle.kts file of the shared module, you can configure the adapters 	according to the settings of the advertising networks on the Bidapp dashboard.


Example for Android:
```groovy
plugins {
 android {
    //...
    dependencies {
        implementation ("io.bidapp.networks:unity:latest")
        implementation ("io.bidapp.networks:liftoff:latest")
        implementation ("io.bidapp.networks:admob:latest")
    }
  }
}
```

Example for IOS:
```groovy
plugins {
 cocoapods {
        //...
        pod("bidapp/AdMob")
        pod("bidapp/Unity")
        pod("bidapp/Liftoff")
        //...
    }
}
```

* Set in the Settings file located in the shared module the pubId for Android and iOS. Additionally, you can enable or disable:

Interstitial, Rewarded, Banner, Test mode, Logging

```kotlin
const val pubId = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"

const val isEnableInterstitial = true
const val isEnableRewarded = true
const val isEnableBanner = true
const val isEnableTestMode = true
const val isEnableLogging = true
```


### Samples
![interstitialandroid](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/b0d2a14a-ccde-43e1-9bdc-741da1840add)
![rewardedandroid](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/ec900def-b450-407d-bd94-2437be8e0c44)
![bannerandroid](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/86b4bffe-dc80-4324-8fe7-87972fc4f0b9)
![mrecandroid](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/458bd42f-ee1d-4319-9a73-67112dba3954)



![interstitialios](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/e4369c24-e0e2-4b93-ad44-b313292c5f7c)
![rewardedios](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/af354710-3f97-49b6-9d23-9e4c899dfb7e)
![mrecios](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/a3786a04-9617-44f1-8465-36d7d90b3432)
![bannerios](https://github.com/bidapphub/bidapp-unrealengine-plugin/assets/148830475/2fd6424b-1395-48b2-9648-da474980fb42)



# Contact Us

Email: [support@bidapp.io](support@bidapp.io)
