Pod::Spec.new do |spec|
    spec.name                     = 'plugin'
    spec.version                  = '1.0'
    spec.homepage                 = 'Link to the Shared Module homepage'
    spec.source                   = { :http=> ''}
    spec.authors                  = ''
    spec.license                  = ''
    spec.summary                  = 'Some description for the Shared Module'
    spec.vendored_frameworks      = 'build/cocoapods/framework/bidapp_kotlin_multiplatform.framework'
    spec.libraries                = 'c++'
    spec.ios.deployment_target = '16.0'
    spec.dependency 'bidapp/SDK'
                
    if !Dir.exist?('build/cocoapods/framework/bidapp_kotlin_multiplatform.framework') || Dir.empty?('build/cocoapods/framework/bidapp_kotlin_multiplatform.framework')
        raise "

        Kotlin framework 'bidapp_kotlin_multiplatform' doesn't exist yet, so a proper Xcode project can't be generated.
        'pod install' should be executed after running ':generateDummyFramework' Gradle task:

            ./gradlew :plugin:generateDummyFramework

        Alternatively, proper pod installation is performed during Gradle sync in the IDE (if Podfile location is set)"
    end
                
    spec.pod_target_xcconfig = {
        'KOTLIN_PROJECT_PATH' => ':plugin',
        'PRODUCT_MODULE_NAME' => 'bidapp_kotlin_multiplatform',
    }
                
    spec.script_phases = [
        {
            :name => 'Build plugin',
            :execution_position => :before_compile,
            :shell_path => '/bin/sh',
            :script => <<-SCRIPT
                if [ "YES" = "$OVERRIDE_KOTLIN_BUILD_IDE_SUPPORTED" ]; then
                  echo "Skipping Gradle build task invocation due to OVERRIDE_KOTLIN_BUILD_IDE_SUPPORTED environment variable set to \"YES\""
                  exit 0
                fi
                set -ev
                REPO_ROOT="$PODS_TARGET_SRCROOT"
                "$REPO_ROOT/../gradlew" -p "$REPO_ROOT" $KOTLIN_PROJECT_PATH:syncFramework \
                    -Pkotlin.native.cocoapods.platform=$PLATFORM_NAME \
                    -Pkotlin.native.cocoapods.archs="$ARCHS" \
                    -Pkotlin.native.cocoapods.configuration="$CONFIGURATION"
            SCRIPT
        }
    ]
                
end