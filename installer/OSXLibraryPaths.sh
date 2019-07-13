#!/bin/bash
install_name_tool -change /usr/local/lib/libsndfile.1.dylib @executable_path/../Frameworks/libsndfile.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libportaudio.2.dylib @executable_path/../Frameworks/libportaudio.2.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_xrc-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_xrc-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_html-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_html-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_qa-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_qa-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_adv-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_adv-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_core-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_core-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_baseu_xml-3.1.dylib @executable_path/../Frameworks/libwx_baseu_xml-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_baseu_net-3.1.dylib @executable_path/../Frameworks/libwx_baseu_net-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/MacOS/SampliTron
# wxWidget library references to other wxWidgets libraries.
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_core-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_core-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_adv-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_adv-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_core-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_core-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_core-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_html-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_html-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_baseu_net-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_core-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_core-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_qa-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_qa-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu_xml-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu_xml-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_qa-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_baseu_xml-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_core-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_core-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_xrc-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_xrc-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_baseu_xml-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_baseu_xml-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_xrc-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_html-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_html-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_xrc-3.1.dylib
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_adv-3.1.2.0.0.dylib @executable_path/../Frameworks/libwx_osx_cocoau_adv-3.1.dylib Samplitron.app/Contents/Frameworks/libwx_osx_cocoau_xrc-3.1.dylib
install_name_tool -change /usr/local/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/libSDL2-2.0.0.dylib Samplitron.app/Contents/Frameworks/libSDL2_ttf-2.0.0.dylib
install_name_tool -change /usr/local/lib/libfreetype.6.dylib @executable_path/../Frameworks/libfreetype.6.dylib Samplitron.app/Contents/Frameworks/libSDL2_ttf-2.0.0.dylib
otool -L Samplitron.app/Contents/MacOS/SampliTron
