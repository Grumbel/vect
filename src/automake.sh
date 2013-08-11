echo "CXXFLAGS = -d -Wall";
echo "";
echo "OBJS = \\";
for name in *.cc;
do \
	echo "       ${name%%.cc}.o  \\";
done;
echo "";
echo "";

echo "../vect.exe : \$(OBJS) ";
echo "	g++ \$(OBJS) -o ../vect.exe -d -g -Wall -lalleg";

echo "";

for name in *.cc;
do \
	echo ${name%%.cc}.o : $name \\;
	grep -e "^ *#include *\"" $name | sed "s/#include \"\(.*\)\".*/            \1 \\\/";
	echo "";
	echo "";
done 

for name in *.hh;
do \
	echo "$name : \\";
	grep -e "^ *#include *\"" $name | sed "s/#include \"\(.*\)\".*/            \1 \\\/";
	echo "";
	echo "";
done 


echo ".PHONY : clean"
echo "clean:"
echo "	rm ../vect.exe";
echo "	rm \$(OBJS)"

