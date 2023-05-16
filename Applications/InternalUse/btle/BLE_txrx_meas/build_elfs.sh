#!/bin/sh

ELF_DIR=meas_elfs

mkdir -p $ELF_DIR

for TX_POWER in -10 0 4
do
  rm -rf build/main.*
  make TX_POWER=$TX_POWER MEAS_TX=1
  mv build/max32665.elf $ELF_DIR\/me14_$TX_POWER\_TX.elf

  rm -rf build/main.*
  make TX_POWER=$TX_POWER MEAS_RX=1
  mv build/max32665.elf $ELF_DIR\/me14_$TX_POWER\_RX.elf

  rm -rf build/main.*
  make TX_POWER=$TX_POWER MEAS_SLEEP=1
  mv build/max32665.elf $ELF_DIR\/me14_$TX_POWER\_SLEEP.elf

  rm -rf build/main.*
  make TX_POWER=$TX_POWER MEAS_BLE_IDLE=1
  mv build/max32665.elf $ELF_DIR\/me14_$TX_POWER\_BLE_IDLE.elf
done 