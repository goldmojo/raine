# Must run "make install" first
cp -Rf output/usr/share/games/raine/* opk_data
cp -f output/usr/games/raine opk_data
cp -f output/usr/share/pixmaps/raine.png opk_data

rm -f raine.opk
mksquashfs opk_data raine.opk
