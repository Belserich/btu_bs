#!/bin/bash

set -e
cd bin
cat /dev/null > ../compile_commands.json
bear -o ../compile_commands.json make clean bootdisk
