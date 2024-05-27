#!/bin/sh

cd "$( dirname "${BASH_SOURCE[0]}" )"

rm -rf adapters
rm -rf networks
rm -rf CHANGELOG.md
rm -rf plugins
rm -rf *.xcframework
rm -rf ./*.zip
rm -rf __MACOSX

curl https://downloads.bidapp.io/private/unreal-engine-tools/bidappNetworks_1.zip -o bidappNetworks_1.zip
unzip bidappNetworks_1.zip
rm bidappNetworks_1.zip
rm -rf __MACOSX
