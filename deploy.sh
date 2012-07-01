rm packages/win32.zip
cp fluorescence_win32.exe packages/win32

zip -9 -r packages/win32.zip shards/example/ fonts/ gumps/ shader/ themes/default/ data/ LICENSE effects/
cd packages/win32
zip -9 ../win32.zip *

