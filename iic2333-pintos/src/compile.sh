#!/bin/bash

function compile-dir {
  A=$(pwd)
  echo "Compiling $1"
  cd $1
  if make > /dev/null; then
    cd $A
  else
    cd $A
    echo "Compiling $1 failed"
    exit $2
  fi
}

function pintos-compile {
  compile-dir threads 101
  compile-dir filesys 102
  compile-dir userprog 103
  compile-dir examples 104
}

function pintos-cp {
  echo "    Copying $1 to the disk"
  ../../utils/pintos -p ../../examples/$1 -a $1 -- -q
}

function pintos-mkfs {
  echo "Preparing the disk"

  echo "  Creating disk (2MiB!)"
  # Create new disk image
  rm $1 2> /dev/null
  ../../utils/pintos-mkdisk $1 --filesys-size=2 &> /dev/null
  # Format
  echo "  Formating disk"
  ../../utils/pintos -- -f -q &> /dev/null

  # copy programs
  echo "  Copying programs"
  pintos-cp 'insult'
  pintos-cp 'pwd'
  pintos-cp 'recursor'
  pintos-cp 'cowsay'
}


echo "Compiling $(pwd)"
if pintos-compile; then
  echo "$(pwd) compiled!"

  if cd userprog/build; then
    pintos-mkfs 'filesys.dsk'

    cd ../..
  fi #cd userprog/build
else
  echo "$(pwd) does not compile!"
fi
