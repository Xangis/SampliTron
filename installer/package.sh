#!/bin/bash
cp -R ../Samples/*.wav Samplitron.app/Contents/Resources/Samples/
cp -R ../Patches Samplitron.app/Contents/Resources/
codesign --deep -f -s "3rd Party Mac Developer Application: Jason Champion (G6PFF85X2R)" Samplitron.app --entitlements Samplitron.app/Contents/Resources/Entitlements.plist
productbuild --sign "3rd Party Mac Developer Installer: Jason Champion (G6PFF85X2R)" --component Samplitron.app /Applications Samplitron.pkg
