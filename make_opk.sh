# Must run "make install" first
cp -Rf output/usr/share/games/raine/* opk_data
cp -f output/usr/games/raine opk_data
cp -f output/usr/share/pixmaps/raine.png opk_data

# Remove beta libs
rm -f opk_data/libGL.so.1_BETA
rm -f opk_data/libmuparser.so.2_BETA

rm -f raine.opk
mksquashfs opk_data raine.opk -all-root -noappend -no-exports -no-xattrs
