#!/bin/bash
OUTPUT_DIR="./NSL_SIMULATOR/OUTPUT/"
INPUT_DIR="./NSL_SIMULATOR/INPUT/"

#parametri
T_min=0.5
T_max=2.0
delta=-0.1

count=0


tipo=""
case "$1" in
    2)
        tipo="METRO"
        ;;
    3)
        tipo="GIBBS"
        ;;
    *)
        tipo=""
        ;;
esac

echo "Tipo $tipo "
echo "valore campo h $2" 


folder_name="OUTPUT_${tipo}_h_$2"
mkdir $folder_name

#si assicura che la configurazione xyz sia quella di ising anche per il restart
cp "${INPUT_DIR}CONFIG/config.ising" "${OUTPUT_DIR}CONFIG/config.xyz"

#imposta il tipo di simulazione  e il campo
sed -i "s/^SIMULATION_TYPE.*/SIMULATION_TYPE        $1 1 $2/" "${INPUT_DIR}input.dat"



for temp in $(seq $T_max $delta $T_min); do
    echo "Temperatura: $temp"
    if (( $(echo "$temp < 2.0" | bc -l) ));then #modifica il file di input, cambiando la linea di restart
        sed -i "s/^RESTART.*/RESTART                1/" "${INPUT_DIR}input.dat" #se la temperaura è < 2.0 restart=False
    else
        sed -i "s/^RESTART.*/RESTART                0/" "${INPUT_DIR}input.dat" #se è 2.0 restart=True

    fi
    sed -i "s/^TEMP.*/TEMP                   $temp/" "${INPUT_DIR}input.dat" #modifica il file di input, cambiando la linea della temperatura
    mkdir "$folder_name/T_$temp"
    ./main.exe #esegue la simulazione
    cp -r  "${OUTPUT_DIR}." "$folder_name/T_$temp" #salva gli output per non farli sovrascrivere

  
done


