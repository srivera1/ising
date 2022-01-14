# !/bin/sh

# examples:
rm output/* 
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m 1.0 -n 1.0 -r 0.0 -s 0.0 -t 293.15 -w 1.0
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e2.mp4 ; cd ..
rm output/* 
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m 1.0 -n 1.0 -r 1.0 -s 0.0 -t 293.15 -w 1.0
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e3.mp4 ; cd ..
rm output/* 
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 7 -V 200 -c 1.0 -j 1.0 -m -1.0 -n 1.0 -r 1.0 -s 0.01 -t 293.15 -w 1.0
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e4.mp4 ; cd ..
rm output/* 
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m 1.0 -n 1.0 -r 1.0 -s 0.10 -t 293.15 -w 1.0
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e5.mp4 ; cd ..
rm output/* 
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 1.0 -j 1.0 -m -1.0 -n 1.0 -r 1.0 -s 0.10 -t 293.15 -w 1.0
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e6.mp4 ; cd ..
rm output/* 
./ising -H 500 -J 1.0 -M 1.0 -S 1000 -T 20 -V 200 -c 0.9990 -j 1.0 -m -1.0 -n 1.0 -r 0.10 -s 0.10 -t 293.15 -w 1.00010
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e7.mp4 ; cd ..
rm output/* 
./ising -H 500 -S 1000 -T 20 -V 200
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e8.mp4 ; cd ..
rm output/* 
./ising -H 200 -S 1000 -T 30 -V 500 -t 100.0
cd output/ ; ffmpeg -framerate 30 -i ising_%d.png -vcodec libx265 -crf 29 out.mp4 ; cp out.mp4 ../examples/e9.mp4 ; cd ..

