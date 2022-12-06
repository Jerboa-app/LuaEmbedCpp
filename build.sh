#!/bin/bash

Green='\033[0;32m'
NC='\033[0m'

POSIX=1
while [[ $# -gt 0 ]]; do
  case $1 in
    -p|--posix)
      POSIX=0
      shift # past argument
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
    *)
      POSITIONAL_ARGS+=("$1") # save positional arg
      shift # past argument
      ;;
  esac
done
for file in build CMakeFiles cmake_install.cmake CMakeCache.txt Makefile test
do
  if [ -d $file ];
  then
    rm -rf $file
  fi
  if [ -f $file ];
  then
    rm $file
  fi
done

if [[ $POSIX -eq 0 ]]
then
  cmake -E make_directory build
  cmake -E chdir build cmake .. -D USE_POSIX=ON && make -C build
else
  cmake -E make_directory build
  cmake -E chdir build cmake .. -D USE_POSIX=OFF && make -C build
  echo -e "${Green}Build with -p or --posix to use mkstemp${NC}"
fi
