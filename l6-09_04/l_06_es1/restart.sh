
OUTPUT_DIR="./NSL_SIMULATOR/OUTPUT/"
INPUT_DIR="./NSL_SIMULATOR/INPUT/"

#sposta i file per non sovrascriverli
mv "${OUTPUT_DIR}temperature.dat" "${OUTPUT_DIR}temperature_for.dat"
mv "${OUTPUT_DIR}potential_energy.dat" "${OUTPUT_DIR}potential_energy_for.dat"

#modifica il file di input,di modo che restart sia settato a true
sed -i 's/^RESTART.*/RESTART                1/' "${INPUT_DIR}input.dat"


tmp=$(mktemp)
#scambia le coordinate, di modo da invertire le velocità
CONFIG="${OUTPUT_DIR}/CONFIG/config.xyz" 
CONFIG_1="${OUTPUT_DIR}/CONFIG/conf-1.xyz" 

mv ${CONFIG} "$tmp"
mv ${CONFIG_1} ${CONFIG}
mv "$tmp" ${CONFIG_1}
