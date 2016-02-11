imageview: imageview.cpp
	g++ -std=gnu++11 -o imageview imageview.cpp `wx-config --toolkit=gtk3 --cflags --libs`
