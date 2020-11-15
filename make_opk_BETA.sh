# Must run "make install" first
cp -Rf output/usr/share/games/raine/* opk_data
cp -f output/usr/games/raine opk_data
cp -f output/usr/share/pixmaps/raine.png opk_data

# Set beta libs
mv -f opk_data/libGL.so.1_BETA opk_data/libGL.so.1
mv -f opk_data/libmuparser.so.2_BETA opk_data/libmuparser.so.2

rm -f raine.opk
/usr/bin/mksquashfs opk_data raine.opk -all-root -noappend -no-exports -no-xattrs -force-uid 1000 -force-gid 100
