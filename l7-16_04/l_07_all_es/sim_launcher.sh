#!/bin/bash
OUTPUT_DIR="./NSL_SIMULATOR/OUTPUT/"
INPUT_DIR="./NSL_SIMULATOR/INPUT/"


tipo=""
delta=""
temp=""
case "$1" in
    0)
        tipo="MD"
        delta="0.001"
        temp="2.0"
        ;;
    1)
        tipo="MC"
        delta="0.1"
        temp="1.1"
        ;;
    *)
        tipo="error"
        ;;
esac

echo "Tipo $tipo "
echo "Blocchi $2 "
echo "Step $3" 

folder_name="OUTPUT_${tipo}"
mkdir $folder_name


#imposta il tipo di simulazione  e i parametri
sed -i "s/^SIMULATION_TYPE.*/SIMULATION_TYPE        $1/" "${INPUT_DIR}input.dat"
sed -i "s/^TEMP.*/TEMP                   ${temp}/" "${INPUT_DIR}input.dat"
sed -i "s/^DELTA.*/DELTA                  ${delta}/" "${INPUT_DIR}input.dat"
sed -i "s/^NBLOCKS.*/NBLOCKS                $2/" "${INPUT_DIR}input.dat"
sed -i "s/^NSTEPS.*/NSTEPS                 $3/" "${INPUT_DIR}input.dat"


time ./main.exe #esegue la simulazione 
cp  "${OUTPUT_DIR}"/*.dat "$folder_name" #salva gli output per non farli sovrascrivere




