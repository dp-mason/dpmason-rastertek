client: applicationclass.o inputclass.o main.o openglclass.o systemclass.o textureclass.o lightshaderclass.o lightclass.o modelclass.o cameraclass.o
	g++ -o client applicationclass.o inputclass.o main.o openglclass.o systemclass.o textureclass.o lightshaderclass.o lightclass.o modelclass.o cameraclass.o -l GL -l X11 -g

applicationclass.o: applicationclass.cpp
	g++ -c applicationclass.cpp -g

inputclass.o: inputclass.cpp
	g++ -c inputclass.cpp -g

main.o: main.cpp
	g++ -c main.cpp -g

openglclass.o: openglclass.cpp
	g++ -c openglclass.cpp -g

systemclass.o: systemclass.cpp
	g++ -c systemclass.cpp -g

cameraclass.o: cameraclass.cpp
	g++ -c cameraclass.cpp -g

textureclass.o: textureclass.cpp
	g++ -c textureclass.cpp -g

modelclass.o: modelclass.cpp
	g++ -c modelclass.cpp -g

lightshaderclass.o: lightshaderclass.cpp
	g++ -c lightshaderclass.cpp -g

lightclass.o: lightclass.cpp
	g++ -c lightclass.cpp -g

clean:
	rm *.o
	rm client