make clean
make
rmmod vga_ball
insmod vga_ball.ko
./hello
