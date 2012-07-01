rm packages/win32.zip
cp fluorescence_win32.exe packages/win32
mkdir packages/win32/shards

zip -9 -r packages/win32.zip fonts/ gumps/ shader/ themes/default/ data/ LICENSE effects/ defaultConfig.xml credits.txt
cd packages/win32
zip -9 ../win32.zip *

