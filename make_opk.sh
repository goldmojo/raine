# Must run "make install" first
cp -Rf output/usr/share/games/raine/* opk_data
cp -f output/usr/games/raine opk_data
cp -f output/usr/share/pixmaps/raine.png opk_data

# Copy libs
cp -f /home/philippe/src/gl4es/lib/libGL.so.1 opk_data
cp -f /home/philippe/src/muparser/lib/libmuparser.so.2 opk_data

rm -f raine.opk
/usr/bin/mksquashfs opk_data raine.opk -all-root -noappend -no-exports -no-xattrs -force-uid 1000 -force-gid 100
