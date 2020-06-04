#!/bin/bash

set -e
cd bin
make clean bootdisk
bochs
