#!/usr/bin/env bash
trap ctrl_c INT

function ctrl_c() {
    exit;
}

read -p "Please enter the converted titlename (example: NPEBxxxxx): " NAME

mkdir $NAME
mkdir $NAME/LICDIR

cp -r PS3_GAME/TROPDIR $NAME/TROPDIR
cp PS3_GAME/* $NAME/

source_dir=$(pwd)

cd PS3_GAME/USRDIR/ && find . -type d -exec mkdir -p $source_dir/$NAME/USRDIR/{} \;
cd $source_dir

cd PS3_GAME/ && find "USRDIR/" -not -iname "*.sdat" -not -iname "EBOOT.BIN" -type f > ../list.txt
cd $source_dir

while read line; do
    ./make_npdata -e "PS3_GAME/$line" "$NAME/$line" 0 1 3 0 16
done < list.txt

./make_npdata -e PS3_GAME/LICDIR/LIC.DAT $NAME/LICDIR/LIC.EDAT 1 1 3 0 16 3 00 EP9000-$NAME_00-0000000000000001 1

rm list.txt

echo ===============================================================================
echo                                    END 
echo ===============================================================================
