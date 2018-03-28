#!sh

# source: http://cplusadd.blogspot.com.br/2008/12/automatic-build-number-generation.html
# FILE: make_buildnum.sh
version="`sed  's/^ *//' major_version`"
old="`sed  's/^ *//' build.number` +1"
echo $old | bc > build.number.temp
mv build.number.temp build.number
#version..
echo "$version`sed  's/^ *//' build.number` - `date`" > version.number
#header
echo "#ifndef BUILD_NUMBER_STR" > src/classes/BuildNumber.h
echo "#define BUILD_NUMBER_STR \"`sed  's/^ *//' build.number`\"" >> src/classes/BuildNumber.h
echo "#endif" >> src/classes/BuildNumber.h

echo "#ifndef VERSION_STR" >> src/classes/BuildNumber.h
echo "#define VERSION_STR \"Version $version`sed  's/^ *//' build.number` - `date`\"" >> src/classes/BuildNumber.h
echo "#endif" >> src/classes/BuildNumber.h

echo "#ifndef VERSION_STR_SHORT" >> src/classes/BuildNumber.h
echo "#define VERSION_STR_SHORT \"$version`sed  's/^ *//' build.number`\"" >> src/classes/BuildNumber.h
echo "#endif" >> src/classes/BuildNumber.h