cd "$(dirname "$0")"
set -e

mkdir -p gen

python3 src/gallium/auxiliary/util/u_format_table.py src/gallium/auxiliary/util/u_format.yaml --enums >gen/u_format_gen.h
python3 src/gallium/auxiliary/util/u_format_table.py src/gallium/auxiliary/util/u_format.yaml >gen/u_format_table.c

python3  src/venus/vkr_device_object.py -o gen src/venus/vkr_device_object.json
