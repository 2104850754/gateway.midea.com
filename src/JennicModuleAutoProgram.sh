#!/bin/sh
# ----------------------------------------------------------------------
# Script that controls the FTBI CBUS pins and detects the Jennic module
# and flashes it
# ----------------------------------------------------------------------
# Author:    nlv10677
# Copyright: NXP B.V. 2015. All rights reserved
# ----------------------------------------------------------------------

echo "========== AutoProgScript start ============" >> /tmp/su.log
echo "Step 1: Switch bootloader to programming state" >> /tmp/su.log

# Control CB[2-3] to put JN516x in bootloader mode
# Note: As a side-effect, it removes /dev/ttyUSB0
iot_jf prog

echo "Step 2: Re-install the ftdi_sio driver " >> /tmp/su.log
rmmod ftdi_sio
modprobe ftdi_sio

echo "Step 3: Call the detection program" >> /tmp/su.log
iot_jd # >> /tmp/su.log
result=$?

binfile="unknowndevicetype"

if [ $result -eq 68 ]; then
  echo "Step 3: Found the JN5168" >> /tmp/su.log
  binfile="/tmp/ZigbeeNodeControlBridge_JN5168.bin"
else
  if [ $result -eq 69 ]; then
    echo "Step 3: Found the JN5169" >> /tmp/su.log
    binfile="/tmp/ZigbeeNodeControlBridge_JN5169.bin"
  fi
fi

if [ -f $binfile ]; then
  echo "Step 4: Flash it with $binfile" >> /tmp/su.log  
  iot_jp -f $binfile # >> /tmp/su.log
  result=$?
else
  echo "Could not find $binfile"
fi


echo "Step 5: Switch bootloader back to normal state" >> /tmp/su.log
# Control CB[2-3] to put JN516x in normal mode
# Note: As a side-effect, it removes /dev/ttyUSB0
iot_jf normal

echo "Step 6: Re-install the ftdi_sio driver " >> /tmp/su.log
rmmod ftdi_sio
modprobe ftdi_sio

echo "Step 7: result = $result" >> /tmp/su.log
echo "========== AutoProgScript end ============" >> /tmp/su.log
return $result
