#make
main:main.o md5.o  md5_hash.o rb_tree.h
	g++ -o main main.o md5.o md5_hash.o -g 

main.o:test_consistent_hash.cpp consistent_hash.h 
	g++ -o main.o -c test_consistent_hash.cpp -g
md5.o:md5.cpp md5.h
	g++ -o md5.o -c md5.cpp -g 
md5_hash.o:md5_hash.cpp md5_hash.h
	g++ -o md5_hash.o -c md5_hash.cpp -g  


.PHONY:clean
clean:
	rm -rf *.o main
